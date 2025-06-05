#include <limits>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <chrono>
#include <deque>
#include <thread>
#include "algui/Widget.hpp"
#include "algui/TimerThread.hpp"


#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif


namespace algui {


    /**************************************************************************
        INTERNALS
     **************************************************************************/


    #define _ALGUI_EVENT_TYPE  ALLEGRO_GET_EVENT_TYPE('a', 'g', 'u', 'i')


    //internal event ids
    enum _InternalEventId {
        _Event_Click
    };



    //event source
    class _AllegroEventSource {
    public:
        _AllegroEventSource() {
            al_init_user_event_source(&m_eventSource);
        }

        ~_AllegroEventSource() {
            al_destroy_user_event_source(&m_eventSource);
        }

        ALLEGRO_EVENT_SOURCE* getEventSource() {
            return &m_eventSource;
        }

    private:
        ALLEGRO_EVENT_SOURCE m_eventSource;
    };


    enum _ClickType {
        _ClickType_None,
        _ClickType_Mouse,
        _ClickType_Joystick
    };


    //the currently focused widget
    static Widget* _focusedWidget = nullptr;


    //click/double click context
    static constexpr int _MAX_CLICK_COUNT = 3;
    static size_t _clickDelay = 250;
    static std::atomic_int _clickButton = 0;
    static std::atomic_int _clickCounter = 0;
    static std::atomic_int _clickType = _ClickType_None;


    //drag and drop context
    static bool _dragAndDrop = false;
    static std::any _draggedData;


    //calc rect intersection
    static bool _intersect(
        float x1, float y1, float x2, float y2,
        float l, float t, float r, float b,
        float& resultX1, float& resultY1, float& resultX2, float& resultY2)
    {
        resultX1 = std::max(x1, l);
        resultY1 = std::max(y1, t);
        resultX2 = std::min(x2, r);
        resultY2 = std::min(y2, b);
        return resultX1 <= resultX2 && resultY1 <= resultY2;
    }


    //calc distance between two points
    static float _distance(float x1, float y1, float x2, float y2) {
        return std::hypot(abs(x1 - x2), abs(y1 - y2));
    }


    //get the timer thread
    static TimerThread& _getTimerThread() {
        static TimerThread timerThread;
        return timerThread;
    }


    static void _resetClick() {
        _clickCounter = 0;
        _clickButton = 0;
        _clickType = _ClickType_None;
    }


    //the function that waits for click delay and then emits a user event for clicks
    static std::function<void()> _emitClickEventFunction = [&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(_clickDelay));
        ALLEGRO_EVENT event{};
        event.type = _ALGUI_EVENT_TYPE;
        event.user.data1 = _Event_Click;
        event.user.data2 = _clickCounter;
        al_emit_user_event(getUIEventSource(), &event, nullptr);
        _resetClick();
    };


    //adds a click, only if the currently initiated click session is for the given click type and button
    static void _addClick(_ClickType clickType, int button) {
        if (_clickType == clickType && _clickButton == button) {
            ++_clickCounter;
        }
    }


    /**************************************************************************
        PUBLIC
     **************************************************************************/ 


    //constructor
    Widget::Widget()
        : m_scalingX(1)
        , m_scalingY(1)
        , m_maximumWidth(FLT_MAX)
        , m_maximumHeight(FLT_MAX)
        , m_screenLeft(0)
        , m_screenTop(0)
        , m_screenRight(0)
        , m_screenBottom(0)
        , m_screenScalingX(1)
        , m_screenScalingY(1)
        , m_childWithMouse(nullptr)
        , m_childWithButton(nullptr)
        , m_tabIndex(0)
        , m_clippingMode(ClippingMode::None)
        , m_visible(true)
        , m_screenGeometryDirty(false)
        , m_geometryConstraintsDirty(false)
        , m_descentantGeometryConstraintsDirty(false)
        , m_layoutDirty(false)
        , m_enabled(true)
        , m_highlighted(false)
        , m_pressed(false)
        , m_selected(false)
        , m_focused(false)
        , m_validContent(true)
        , m_enabledTree(true)
        , m_highlightedTree(false)
        , m_pressedTree(false)
        , m_selectedTree(false)
        , m_focusedTree(false)
        , m_validContentTree(false)
        , m_treeVisualStateDirty(false)
        , m_focusable(true)
        , m_hasMouse(false)
        , m_focusContainer(false)
    {
    }


    //the destructor
    Widget::~Widget() {
        //delete children
        deleteAll();

        //manage focus widget
        if (this == _focusedWidget) {
            _focusedWidget = nullptr;
        }
    }


    //Inserts a child widget.
    bool Widget::add(Widget* child, Widget* nextSibling) {
        //if child is added successfully
        if (Tree<Widget>::add(child, nextSibling)) {

            //if geometry constraints in the child tree are invalid,
            //invalidate the constraints of this and its ancestors
            if (child->m_geometryConstraintsDirty || child->m_descentantGeometryConstraintsDirty) {
                for (Widget* ancestor = this; ancestor; ancestor = ancestor->getParent()) {
                    if (ancestor->m_descentantGeometryConstraintsDirty) {
                        break;
                        ancestor->m_descentantGeometryConstraintsDirty = true;
                    }
                }
            }

            //invalidate geometry constraints and layout of this,
            //since a child was added
            _invalidateGeometryConstraints();
            _invalidateLayout();

            //invalidate tree visual state of child,
            //so as that it is later recomputed
            child->_invalidateTreeVisualState();

            //also reset child state
            child->_resetMouseAndButtonState();

            //successful addition
            return true;
        }

        //fail to add child
        return false;
    }


    //Removes a child widget.
    bool Widget::remove(Widget* child) {
        //remove widget from tree
        if (Tree<Widget>::remove(child)) {

            //invalidate geometry constraints and layout of this,
            //since a child was removed
            _invalidateGeometryConstraints();
            _invalidateLayout();

            //if the removed child contains the focus, lose the focus
            if (child->contains(_focusedWidget)) {
                _focusedWidget->setFocused(false);
            }

            //reset mouse/button state
            child->_resetMouseAndButtonState();
            _resetChildState(child);

            //success
            return true;
        }

        //failure
        return false;
    }


    //Sets the left coordinate.
    void Widget::setLeft(const Coord& left) {
        if (left != m_left) {
            m_left = left;
            _invalidateParentLayout();
            _invalidateScreenGeometry();
        }
    }


    //Sets the top coordinate.
    void Widget::setTop(const Coord& top) {
        if (top != m_top) {
            m_top = top;
            _invalidateParentLayout();
            _invalidateScreenGeometry();
        }
    }


    //Sets the width.
    void Widget::setWidth(const Coord& width) {
        if (width != m_width) {
            m_width = width;
            _invalidateLayout();
            _invalidateParentLayout();
            _invalidateScreenGeometry();
        }
    }


    //Sets the height.
    void Widget::setHeight(const Coord& height) {
        if (height != m_height) {
            m_height = height;
            _invalidateLayout();
            _invalidateParentLayout();
            _invalidateScreenGeometry();
        }
    }


    //Sets the interior scaling factor of the component along the X axis.
    void Widget::setScalingX(const float scalingX) {
        if (scalingX != m_scalingX) {
            m_scalingX = scalingX;
            _invalidateScreenGeometry();
        }
    }


    //Sets the interior scaling factor of the component along the Y axis.
    void Widget::setScalingY(const float scalingY) {
        if (scalingY != m_scalingY) {
            m_scalingY = scalingY;
            _invalidateScreenGeometry();
        }
    }


    //Sets the visible state of the widget.
    void Widget::setVisible(bool visible) {
        if (visible != m_visible) {
            m_visible = visible;
            _invalidateParentGeometryConstraints();
            _invalidateParentLayout();
            if (!visible) {
                _resetMouseAndButtonState();
                if (getParent()) {
                    getParent()->_resetChildState(this);
                }
            }
        }
    }


    //Sets the minimum width geometry constraint of the widget.
    void Widget::setMinimumWidth(const Coord& minimumWidth) {
        if (minimumWidth != m_minimumWidth) {
            m_minimumWidth = minimumWidth;
            _invalidateParentGeometryConstraints();
            _invalidateParentLayout();
        }
    }


    //Sets the minimum height geometry constraint of the widget.
    void Widget::setMinimumHeight(const Coord& minimumHeight) {
        if (minimumHeight != m_minimumHeight) {
            m_minimumHeight = minimumHeight;
            _invalidateParentGeometryConstraints();
            _invalidateParentLayout();
        }
    }


    //Sets the optimal width geometry constraint of the widget.
    void Widget::setOptimalWidth(const Coord& optimalWidth) {
        if (optimalWidth != m_optimalWidth) {
            m_optimalWidth = optimalWidth;
            _invalidateParentGeometryConstraints();
            _invalidateParentLayout();
        }
    }


    //Sets the optimal height geometry constraint of the widget.
    void Widget::setOptimalHeight(const Coord& optimalHeight) {
        if (optimalHeight != m_optimalHeight) {
            m_optimalHeight = optimalHeight;
            _invalidateParentGeometryConstraints();
            _invalidateParentLayout();
        }
    }


    //Sets the maximum width geometry constraint of the widget.
    void Widget::setMaximumWidth(const Coord& maximumWidth) {
        if (maximumWidth != m_maximumWidth) {
            m_maximumWidth = maximumWidth;
            _invalidateParentGeometryConstraints();
            _invalidateParentLayout();
        }
    }


    //Sets the maximum height geometry constraint of the widget.
    void Widget::setMaximumHeight(const Coord& maximumHeight) {
        if (maximumHeight != m_maximumHeight) {
            m_maximumHeight = maximumHeight;
            _invalidateParentGeometryConstraints();
            _invalidateParentLayout();
        }
    }


    //set the focus container flag.
    void Widget::setFocusContainer(bool focusContainer) {
        m_focusContainer = focusContainer;
        if (m_focusContainer) {
            m_focusable = false;
        }
    }


    //Sets the enabled state of the widget.
    void Widget::setEnabled(bool enabled) {
        if (enabled != m_enabled) {

            //if disabled
            if (!enabled) {
                //remove the focus from the disabled widget
                if (contains(_focusedWidget)) {
                    _focusedWidget->setFocused(false);
                }

                //reset the mouse/button state
                _resetMouseAndButtonState();
                if (getParent()) {
                    getParent()->_resetChildState(this);
                }
            }

            //set flags
            m_enabled = enabled;
            _invalidateTreeVisualState();
        }
    }


    //Sets the highlighted state of the widget.
    void Widget::setHighlighted(bool highlighted) {
        if (highlighted != m_highlighted) {
            m_highlighted = highlighted;
            _invalidateTreeVisualState();
        }
    }


    //Sets the pressed state of the widget.
    void Widget::setPressed(bool pressed) {
        if (pressed != m_pressed) {
            m_pressed = pressed;
            _invalidateTreeVisualState();
        }
    }


    //Sets the selected state of the widget.
    void Widget::setSelected(bool selected) {
        if (selected != m_selected) {
            m_selected = selected;
            _invalidateTreeVisualState();
        }
    }


    //Sets the focused state of the widget.
    void Widget::setFocused(bool focused) {
        if (focused == m_focused) {
            return;
        }

        //get focus
        if (focused) {
            //check if it can get the focus
            if (!_canGetFocus()) {
                return;
            }

            //remove the focus from current owner
            if (_focusedWidget) {
                _focusedWidget->setFocused(false);
            }

            //set this as focused
            m_focused = true;
            _invalidateTreeVisualState();
            _focusedWidget = this;
            WidgetEvent focusEvent(this);
            dispatchEvent(EventType::Event_GotFocus, focusEvent, EventPhaseType::EventPhase_Bubble);
            for (Widget* ancestor = getParent(); ancestor; ancestor = ancestor->getParent()) {
                ancestor->dispatchEvent(EventType::Event_DescentantGotFocus, focusEvent, EventPhaseType::EventPhase_Bubble);
            }
        }

        //else lose focus
        else {
            m_focused = false;
            _invalidateTreeVisualState();
            _focusedWidget = nullptr;
            WidgetEvent focusEvent(this);
            dispatchEvent(EventType::Event_LostFocus, focusEvent, EventPhaseType::EventPhase_Bubble);
            for (Widget* ancestor = getParent(); ancestor; ancestor = ancestor->getParent()) {
                ancestor->dispatchEvent(EventType::Event_DescentantLostFocus, focusEvent, EventPhaseType::EventPhase_Bubble);
            }
        }
    }


    //Sets the valid content state of the widget.
    void Widget::setValidContent(bool validContent) {
        if (validContent != m_validContent) {
            m_validContent = validContent;
            _invalidateTreeVisualState();
        }
    }


    //Sets the focusable state.
    void Widget::setFocusable(bool focusable) {
        m_focusable = focusable;
        if (focusable) {
            m_focusContainer = false;
        }
    }


    //Renders the tree into the target bitmap.
    void Widget::render() {
        _updateGeometryConstraints();
        _paint(false, false);
    }


    //intersection with point
    bool Widget::intersects(float screenX, float screenY) const {
        const bool pointInWidget = screenX >= m_screenLeft && screenX < m_screenRight && screenY >= m_screenTop && screenY < m_screenBottom;

        switch (m_clippingMode) {
            case ClippingMode::None:
            case ClippingMode::Widget:
                if (pointInWidget) {
                    return true;
                }
                for (Widget* child = getLastChild(); child; child = child->getPrevSibling()) {
                    if (child->m_visible && child->intersects(screenX, screenY)) {
                        return true;
                    }
                }
                return false;

            case ClippingMode::Tree:
                return pointInWidget;
        }

        throw std::logic_error("Widget::intersects: invalid clipping mode");
    }

    //get child from coordinates
    Widget* Widget::getChild(float screenX, float screenY) const {
        switch (m_clippingMode) {
            case ClippingMode::None:
            case ClippingMode::Widget:
                return _getChildFromCoords(screenX, screenY);

            case ClippingMode::Tree:
                if (intersects(screenX, screenY)) {
                    return _getChildFromCoords(screenX, screenY);
                }
                return nullptr;
        }

        throw std::logic_error("Widget::getChild: invalid clipping mode");
    }


    bool Widget::processAllegroEvent(const ALLEGRO_EVENT& event) {
        //if disabled, it cannot process events
        if (!_enabledTree()) {
            return false;
        }

        //handle events
        switch (event.type) {
            case ALLEGRO_EVENT_MOUSE_AXES:
            case ALLEGRO_EVENT_MOUSE_WARPED:
            case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
            case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
            {
                bool result = false;

                //mouse move
                if (event.mouse.dx || event.mouse.dy) {
                    const bool oldMouse = m_hasMouse;
                    const bool newMouse = intersects(event.mouse.x, event.mouse.y);
                    if (newMouse && oldMouse) {
                        result = _mouseMove(event, _dragAndDrop ? Event_Drag : Event_MouseMove);
                    }
                    else if (newMouse) {
                        result = _mouseEnter(event);
                    }
                    else if (oldMouse) {
                        result = _mouseLeave(event);
                    }
                }

                //mouse wheel
                if (event.mouse.dz || event.mouse.dw) {
                    result = _mouseWheel(event) || result;
                }

                return result;
            }

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            {
                if (_dragAndDrop) {
                    return false;
                }
                const bool result = _mouseButtonDownEvent(event);
                _initClick(_ClickType_Mouse, event.mouse.button, event);
                return result;
            }

            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            {
                bool result = _mouseButtonUpEvent(event);
                _addClick(_ClickType_Mouse, event.mouse.button);
                result = _endDragAndDrop(event) || result;
                return result;
            }

            case ALLEGRO_EVENT_KEY_DOWN:
                return _keyEvent(event, Event_KeyDown);

            case ALLEGRO_EVENT_KEY_UP:
                return _keyEvent(event, Event_KeyUp);

            case ALLEGRO_EVENT_KEY_CHAR:
                return _keyEvent(event, Event_KeyChar);

            case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
            {
                if (_dragAndDrop) {
                    return false;
                }
                bool result = _joystickButtonEvent(event, Event_JoystickButtonDown);
                _initClick(_ClickType_Joystick, event.joystick.button, event);
                return result;
            }

            case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
            {
                bool result = _joystickButtonEvent(event, Event_JoystickButtonUp);
                _addClick(_ClickType_Joystick, event.joystick.button);
                result = _endDragAndDrop(event) || result;
                return result;
            }

            case ALLEGRO_EVENT_JOYSTICK_AXIS:
                return _joystickMoveEvent(event);

            case ALLEGRO_EVENT_TIMER:
                return _timerEvent(event);

            case ALLEGRO_EVENT_DISPLAY_EXPOSE:
                return _exposeEvent(event);

            case _ALGUI_EVENT_TYPE:
                return _internalEvent(event);
        }

        //event not processed
        return false;
    }


    bool Widget::moveFocusBackward() {
        Widget* focusContainer = nullptr;
        int tabIndex = INT_MAX;

        //if there is a focused widget, limit the focus movement within the focus container
        //the focused widget belongs to
        if (_focusedWidget) {
            focusContainer = _focusedWidget->_getClosestFocusContainerAncestor();
            tabIndex = _focusedWidget->m_tabIndex;
        }

        //otherwise move the focus within the last innermost focus container
        if (!focusContainer) {
            focusContainer = _getPrevInnermostFocusContainer();
            if (!focusContainer) {
                focusContainer = getRoot();
            }
        }

        //find widget with lower tab index; if found, give it the focus
        Widget* widgetWithLowerTabIndex = focusContainer->_getDescentantWithLowerTabIndex(tabIndex);
        if (widgetWithLowerTabIndex) {
            widgetWithLowerTabIndex->setFocused(true);
            return true;
        }

        //since no lower tab index is found, find previous widget that can get the input focus
        Widget* prevWidgetToFocus = focusContainer->_getPrevFocusDescentant(_focusedWidget);
        if (prevWidgetToFocus) {
            prevWidgetToFocus->setFocused(true);
            return true;
        }

        return false;
    }


    bool Widget::moveFocusForward() {
        Widget* focusContainer = nullptr;
        int tabIndex = 0;

        //if there is a focused widget, limit the focus movement within the focus container
        //the focused widget belongs to
        if (_focusedWidget) {
            focusContainer = _focusedWidget->_getClosestFocusContainerAncestor();
            tabIndex = _focusedWidget->m_tabIndex;
        }

        //otherwise move the focus within the first innermost focus container
        if (!focusContainer) {
            focusContainer = _getNextInnermostFocusContainer();
            if (!focusContainer) {
                focusContainer = getRoot();
            }
        }

        //find widget with higher tab index; if found, give it the focus
        Widget* widgetWithHigherTabIndex = focusContainer->_getDescentantWithHigherTabIndex(tabIndex);
        if (widgetWithHigherTabIndex) {
            widgetWithHigherTabIndex->setFocused(true);
            return true;
        }

        //since no higher tab index is found, find next widget that can get the input focus
        Widget* nextWidgetToFocus = focusContainer->_getNextFocusDescentant(_focusedWidget);
        if (nextWidgetToFocus) {
            nextWidgetToFocus->setFocused(true);
            return true;
        }

        return false;
    }


    bool Widget::moveFocusLeft() {
        Widget* focusContainer = nullptr;

        if (_focusedWidget) {
            focusContainer = _focusedWidget->_getClosestFocusContainerAncestor();
        }

        if (!focusContainer) {
            focusContainer = _getPrevInnermostFocusContainer();
            if (!focusContainer) {
                focusContainer = getRoot();
            }
        }

        if (!_focusedWidget) {
            Widget* wgt = focusContainer->_getPrevFocusDescentant(nullptr);
            if (wgt) {
                wgt->setFocused(true);
                return true;
            }
        }

        Widget* bestWidget = nullptr;
        float bestDist = FLT_MAX;
        for (Widget* wgt = focusContainer->getFirstChild(); wgt; wgt = focusContainer->getNext(wgt)) {
            if (wgt != _focusedWidget && wgt->m_enabled && wgt->m_focusable && wgt->m_screenRight <= _focusedWidget->m_screenRight) {
                const float dist1 = _distance(wgt->m_screenRight, wgt->m_screenTop, _focusedWidget->m_screenLeft, _focusedWidget->m_screenTop);
                const float dist2 = _distance(wgt->m_screenRight, wgt->m_screenBottom, _focusedWidget->m_screenLeft, _focusedWidget->m_screenTop);
                const float dist3 = _distance(wgt->m_screenRight, wgt->m_screenTop, _focusedWidget->m_screenLeft, _focusedWidget->m_screenBottom);
                const float dist4 = _distance(wgt->m_screenRight, wgt->m_screenBottom, _focusedWidget->m_screenLeft, _focusedWidget->m_screenBottom);
                const float dist5 = _distance(wgt->m_screenLeft, wgt->m_screenTop, _focusedWidget->m_screenLeft, _focusedWidget->m_screenTop);
                const float dist6 = _distance(wgt->m_screenLeft, wgt->m_screenBottom, _focusedWidget->m_screenLeft, _focusedWidget->m_screenTop);
                const float dist7 = _distance(wgt->m_screenLeft, wgt->m_screenTop, _focusedWidget->m_screenLeft, _focusedWidget->m_screenBottom);
                const float dist8 = _distance(wgt->m_screenLeft, wgt->m_screenBottom, _focusedWidget->m_screenLeft, _focusedWidget->m_screenBottom);
                const float dist = std::min(dist1, std::min(dist2, std::min(dist3, std::min(dist4, std::min(dist5, std::min(dist6, std::min(dist7, dist8)))))));
                if (dist < bestDist) {
                    bestDist = dist;
                    bestWidget = wgt;
                }
            }
        }
        if (bestWidget) {
            bestWidget->setFocused(true);
            return true;
        }

        Widget* wgt = focusContainer->_getPrevFocusDescentant(nullptr);
        if (wgt) {
            wgt->setFocused(true);
            return true;
        }

        return false;
    }


    bool Widget::moveFocusUp() {
        Widget* focusContainer = nullptr;

        if (_focusedWidget) {
            focusContainer = _focusedWidget->_getClosestFocusContainerAncestor();
        }

        if (!focusContainer) {
            focusContainer = _getPrevInnermostFocusContainer();
            if (!focusContainer) {
                focusContainer = getRoot();
            }
        }

        if (!_focusedWidget) {
            Widget* wgt = focusContainer->_getPrevFocusDescentant(nullptr);
            if (wgt) {
                wgt->setFocused(true);
                return true;
            }
        }

        Widget* bestWidget = nullptr;
        float bestDist = FLT_MAX;
        for (Widget* wgt = focusContainer->getFirstChild(); wgt; wgt = focusContainer->getNext(wgt)) {
            if (wgt != _focusedWidget && wgt->m_enabled && wgt->m_focusable && wgt->m_screenBottom <= _focusedWidget->m_screenBottom) {
                const float dist1 = _distance(wgt->m_screenLeft, wgt->m_screenBottom, _focusedWidget->m_screenLeft, _focusedWidget->m_screenTop);
                const float dist2 = _distance(wgt->m_screenRight, wgt->m_screenBottom, _focusedWidget->m_screenLeft, _focusedWidget->m_screenTop);
                const float dist3 = _distance(wgt->m_screenLeft, wgt->m_screenBottom, _focusedWidget->m_screenRight, _focusedWidget->m_screenTop);
                const float dist4 = _distance(wgt->m_screenRight, wgt->m_screenBottom, _focusedWidget->m_screenRight, _focusedWidget->m_screenTop);
                const float dist5 = _distance(wgt->m_screenLeft, wgt->m_screenTop, _focusedWidget->m_screenLeft, _focusedWidget->m_screenTop);
                const float dist6 = _distance(wgt->m_screenRight, wgt->m_screenTop, _focusedWidget->m_screenLeft, _focusedWidget->m_screenTop);
                const float dist7 = _distance(wgt->m_screenLeft, wgt->m_screenTop, _focusedWidget->m_screenRight, _focusedWidget->m_screenTop);
                const float dist8 = _distance(wgt->m_screenRight, wgt->m_screenTop, _focusedWidget->m_screenRight, _focusedWidget->m_screenTop);
                const float dist = std::min(dist1, std::min(dist2, std::min(dist3, std::min(dist4, std::min(dist5, std::min(dist6, std::min(dist7, dist8)))))));
                if (dist < bestDist) {
                    bestDist = dist;
                    bestWidget = wgt;
                }
            }
        }
        if (bestWidget) {
            bestWidget->setFocused(true);
            return true;
        }

        Widget* wgt = focusContainer->_getPrevFocusDescentant(nullptr);
        if (wgt) {
            wgt->setFocused(true);
            return true;
        }

        return false;
    }


    bool Widget::moveFocusRight() {
        Widget* focusContainer = nullptr;

        if (_focusedWidget) {
            focusContainer = _focusedWidget->_getClosestFocusContainerAncestor();
        }

        if (!focusContainer) {
            focusContainer = _getNextInnermostFocusContainer();
            if (!focusContainer) {
                focusContainer = getRoot();
            }
        }

        if (!_focusedWidget) {
            Widget* wgt = focusContainer->_getNextFocusDescentant(nullptr);
            if (wgt) {
                wgt->setFocused(true);
                return true;
            }
        }

        Widget* bestWidget = nullptr;
        float bestDist = FLT_MAX;
        for (Widget* wgt = focusContainer->getFirstChild(); wgt; wgt = focusContainer->getNext(wgt)) {
            if (wgt != _focusedWidget && wgt->m_enabled && wgt->m_focusable && wgt->m_screenLeft >= _focusedWidget->m_screenLeft) {
                const float dist1 = _distance(wgt->m_screenRight, wgt->m_screenTop, _focusedWidget->m_screenRight, _focusedWidget->m_screenTop);
                const float dist2 = _distance(wgt->m_screenRight, wgt->m_screenBottom, _focusedWidget->m_screenRight, _focusedWidget->m_screenTop);
                const float dist3 = _distance(wgt->m_screenRight, wgt->m_screenTop, _focusedWidget->m_screenRight, _focusedWidget->m_screenBottom);
                const float dist4 = _distance(wgt->m_screenRight, wgt->m_screenBottom, _focusedWidget->m_screenRight, _focusedWidget->m_screenBottom);
                const float dist5 = _distance(wgt->m_screenLeft, wgt->m_screenTop, _focusedWidget->m_screenRight, _focusedWidget->m_screenTop);
                const float dist6 = _distance(wgt->m_screenLeft, wgt->m_screenBottom, _focusedWidget->m_screenRight, _focusedWidget->m_screenTop);
                const float dist7 = _distance(wgt->m_screenLeft, wgt->m_screenTop, _focusedWidget->m_screenRight, _focusedWidget->m_screenBottom);
                const float dist8 = _distance(wgt->m_screenLeft, wgt->m_screenBottom, _focusedWidget->m_screenRight, _focusedWidget->m_screenBottom);
                const float dist = std::min(dist1, std::min(dist2, std::min(dist3, std::min(dist4, std::min(dist5, std::min(dist6, std::min(dist7, dist8)))))));
                if (dist < bestDist) {
                    bestDist = dist;
                    bestWidget = wgt;
                }
            }
        }
        if (bestWidget) {
            bestWidget->setFocused(true);
            return true;
        }

        Widget* wgt = focusContainer->_getNextFocusDescentant(nullptr);
        if (wgt) {
            wgt->setFocused(true);
            return true;
        }

        return false;
    }


    bool Widget::moveFocusDown() {
        Widget* focusContainer = nullptr;

        if (_focusedWidget) {
            focusContainer = _focusedWidget->_getClosestFocusContainerAncestor();
        }

        if (!focusContainer) {
            focusContainer = _getNextInnermostFocusContainer();
            if (!focusContainer) {
                focusContainer = getRoot();
            }
        }

        if (!_focusedWidget) {
            Widget* wgt = focusContainer->_getNextFocusDescentant(nullptr);
            if (wgt) {
                wgt->setFocused(true);
                return true;
            }
        }

        Widget* bestWidget = nullptr;
        float bestDist = FLT_MAX;
        for (Widget* wgt = focusContainer->getFirstChild(); wgt; wgt = focusContainer->getNext(wgt)) {
            if (wgt != _focusedWidget && wgt->m_enabled && wgt->m_focusable && wgt->m_screenTop >= _focusedWidget->m_screenTop) {
                const float dist1 = _distance(wgt->m_screenLeft, wgt->m_screenBottom, _focusedWidget->m_screenLeft, _focusedWidget->m_screenBottom);
                const float dist2 = _distance(wgt->m_screenRight, wgt->m_screenBottom, _focusedWidget->m_screenLeft, _focusedWidget->m_screenBottom);
                const float dist3 = _distance(wgt->m_screenLeft, wgt->m_screenBottom, _focusedWidget->m_screenRight, _focusedWidget->m_screenBottom);
                const float dist4 = _distance(wgt->m_screenRight, wgt->m_screenBottom, _focusedWidget->m_screenRight, _focusedWidget->m_screenBottom);
                const float dist5 = _distance(wgt->m_screenLeft, wgt->m_screenTop, _focusedWidget->m_screenLeft, _focusedWidget->m_screenBottom);
                const float dist6 = _distance(wgt->m_screenRight, wgt->m_screenTop, _focusedWidget->m_screenLeft, _focusedWidget->m_screenBottom);
                const float dist7 = _distance(wgt->m_screenLeft, wgt->m_screenTop, _focusedWidget->m_screenRight, _focusedWidget->m_screenBottom);
                const float dist8 = _distance(wgt->m_screenRight, wgt->m_screenTop, _focusedWidget->m_screenRight, _focusedWidget->m_screenBottom);
                const float dist = std::min(dist1, std::min(dist2, std::min(dist3, std::min(dist4, std::min(dist5, std::min(dist6, std::min(dist7, dist8)))))));
                if (dist < bestDist) {
                    bestDist = dist;
                    bestWidget = wgt;
                }
            }
        }
        if (bestWidget) {
            bestWidget->setFocused(true);
            return true;
        }

        Widget* wgt = focusContainer->_getNextFocusDescentant(nullptr);
        if (wgt) {
            wgt->setFocused(true);
            return true;
        }

        return false;
    }


    //begin drag and drop
    bool Widget::beginDragAndDrop(const ALLEGRO_EVENT& event) {
        //only one drag-n-drop session can be active.
        if (_dragAndDrop) {
            return false;
        }

        //the widget must be enabled
        if (!_enabledTree()) {
            return false;
        }

        //the event type must be a button down event.
        if (event.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.type != ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
            return false;
        }

        //the widget must have dragged data
        _draggedData = onGetDraggedData();
        if (!_draggedData.has_value()) {
            return false;
        }

        //all requirements satisfied; begin drag-n-drop.
        _resetClick();
        _resetMouseAndButtonState();
        _dragAndDrop = true;
        _dragEvent(event, Event_DragStarted);
        return true;
    }


    /**************************************************************************
        PRIVATE
    **************************************************************************/


    //make the screen geometry invalid
    void Widget::_invalidateScreenGeometry() {
        m_screenGeometryDirty = true;
    }


    //invalidate geometry constraits of widget
    void Widget::_invalidateGeometryConstraints() {
        if (m_geometryConstraintsDirty) {
            return;
        }
        m_geometryConstraintsDirty = true;
        for (Widget* ancestor = getParent(); ancestor; ancestor = ancestor->getParent()) {
            if (ancestor->m_descentantGeometryConstraintsDirty) {
                return;
            }
            ancestor->m_descentantGeometryConstraintsDirty = true;
        }
    }


    //invalidates geometry constraints of parent
    void Widget::_invalidateParentGeometryConstraints() {
        if (getParent()) {
            getParent()->_invalidateGeometryConstraints();
        }
    }


    //reevaluate geometry constraints, based on constraints of children
    void Widget::_updateGeometryConstraints() {
        if (m_descentantGeometryConstraintsDirty) {
            forEach([&](Widget* child) {
                child->_updateGeometryConstraints();
            });
            m_descentantGeometryConstraintsDirty = false;
        }
        if (m_geometryConstraintsDirty) {
            onCalculateGeometryConstraints();
            m_geometryConstraintsDirty = false;
        }
    }


    //invalidate the layout of the widget
    void Widget::_invalidateLayout() {
        m_layoutDirty = true;
    }


    //invalidate the layout of the widget's parent
    void Widget::_invalidateParentLayout() {
        if (getParent()) {
            getParent()->_invalidateLayout();
        }
    }


    //calculate screen geometry
    void Widget::_calcScreenGeometry() {
        Widget* parent = getParent();

        if (parent) {
            const float width = parent->m_screenRight - parent->m_screenLeft;
            const float height = parent->m_screenBottom - parent->m_screenTop;
            m_screenLeft = parent->m_screenLeft + m_left.calcPixelValue(width, parent->m_screenScalingX);
            m_screenTop = parent->m_screenTop + m_top.calcPixelValue(height, parent->m_screenScalingY);
            m_screenRight = m_screenLeft + m_width.calcPixelValue(width, parent->m_screenScalingX);
            m_screenBottom = m_screenTop + m_height.calcPixelValue(height, parent->m_screenScalingY);
            m_screenScalingX = parent->m_screenScalingX * m_scalingX;
            m_screenScalingY = parent->m_screenScalingY * m_scalingY;
        }

        else {
            ALLEGRO_BITMAP* const target = al_get_target_bitmap();
            const float width = al_get_bitmap_width(target);
            const float height = al_get_bitmap_height(target);
            m_screenLeft = m_left.calcPixelValue(width);
            m_screenTop = m_top.calcPixelValue(height);
            m_screenRight = m_screenLeft + m_width.calcPixelValue(width);
            m_screenBottom = m_screenTop + m_height.calcPixelValue(height);
            m_screenScalingX = m_scalingX;
            m_screenScalingY = m_scalingY;
        }
    }


    //invalidates the tree visual state
    void Widget::_invalidateTreeVisualState() {
        m_treeVisualStateDirty = true;
    }


    //calculate visual state of tree, depending on if the widget is a child or root
    void Widget::_calcTreeVisualState() {
        Widget* parent = getParent();

        if (parent) {
            m_enabledTree = m_enabled && parent->m_enabledTree;
            m_highlightedTree = m_highlighted || parent->m_highlightedTree;
            m_pressedTree = m_pressed || parent->m_pressedTree;
            m_selectedTree = m_selected || parent->m_selectedTree;
            m_focusedTree = m_focused || parent->m_focusedTree;
            m_validContentTree = m_validContent && parent->m_validContentTree;
        }

        else {
            m_enabledTree = m_enabled;
            m_highlightedTree = m_highlighted;
            m_pressedTree = m_pressed;
            m_selectedTree = m_selected;
            m_focusedTree = m_focused;
            m_validContentTree = m_validContent;
        }
    }

    //calc layout, screen geometry, screen visual state, paint widgets recursively
    void Widget::_paint(bool calcScreenGeometry, bool calcVisualState) {
        if (m_visible) {
            //recalculate screen geometry, if needed
            if (m_screenGeometryDirty || calcScreenGeometry) {
                _calcScreenGeometry();
                m_screenGeometryDirty = false;
                calcScreenGeometry = true;
            }

            //calc visual state of tree
            if (m_treeVisualStateDirty || calcVisualState) {
                _calcTreeVisualState();
                m_treeVisualStateDirty = false;
                calcVisualState = true;
            }

            //recalculate layout, if needed
            if (m_layoutDirty) {
                onLayout();
                m_layoutDirty = false;
            }

            //paint according to clipping
            switch (m_clippingMode) {
                //paint widget only if it overlaps clip rectangle;
                //children are tested separately
                case ClippingMode::None:
                {
                    int x1, y1, w, h, x2, y2;
                    al_get_clipping_rectangle(&x1, &y1, &w, &h);
                    x2 = x1 + w;
                    y2 = y1 + h;
                    const float clipX1 = std::max((float)x1, m_screenLeft);
                    const float clipY1 = std::max((float)y1, m_screenTop);
                    const float clipX2 = std::min((float)x2, m_screenRight);
                    const float clipY2 = std::min((float)y2, m_screenBottom);
                    const bool overlapsClipRect = clipX1 <= clipX2 && clipY1 <= clipY2;
                    if (overlapsClipRect) {
                        onPaint();
                    }
                    forEach([&](Widget* child) {
                        child->_paint(calcScreenGeometry, calcVisualState);
                    });
                    if (overlapsClipRect) {
                        onPaintOverlay();
                    }
                    break;
                }

                //paint widget only if it overlaps the clip rectangle; widget is clipped;
                //children are tested separately
                case ClippingMode::Widget:
                {
                    int x1, y1, w, h, x2, y2;
                    al_get_clipping_rectangle(&x1, &y1, &w, &h);
                    x2 = x1 + w;
                    y2 = y1 + h;
                    float clipX1, clipY1, clipX2, clipY2;
                    const bool overlapsClipRect = _intersect(x1, y1, x2, y2, m_screenLeft, m_screenTop, m_screenRight, m_screenBottom, clipX1, clipY1, clipX2, clipY2);
                    if (overlapsClipRect) {
                        al_set_clipping_rectangle((int)std::floor(clipX1), (int)std::floor(clipY1), (int)std::ceil(clipX2 - clipX1), (int)std::ceil(clipY2 - clipY1));
                        onPaint();
                        al_set_clipping_rectangle(x1, y1, w, h);
                    }
                    forEach([&](Widget* child) {
                        child->_paint(calcScreenGeometry, calcVisualState);
                        });
                    if (overlapsClipRect) {
                        al_set_clipping_rectangle((int)std::floor(clipX1), (int)std::floor(clipY1), (int)std::ceil(clipX2 - clipX1), (int)std::ceil(clipY2 - clipY1));
                        onPaintOverlay();
                        al_set_clipping_rectangle(x1, y1, w, h);
                    }
                    break;
                }

                //paint widget tree clipped
                case ClippingMode::Tree:
                {
                    int x1, y1, w, h, x2, y2;
                    al_get_clipping_rectangle(&x1, &y1, &w, &h);
                    x2 = x1 + w;
                    y2 = y1 + h;
                    float clipX1, clipY1, clipX2, clipY2;
                    const bool overlapsClipRect = _intersect(x1, y1, x2, y2, m_screenLeft, m_screenTop, m_screenRight, m_screenBottom, clipX1, clipY1, clipX2, clipY2);
                    if (overlapsClipRect) {
                        al_set_clipping_rectangle((int)std::floor(clipX1), (int)std::floor(clipY1), (int)std::ceil(clipX2 - clipX1), (int)std::ceil(clipY2 - clipY1));
                        onPaint();
                        forEach([&](Widget* child) {
                            child->_paint(calcScreenGeometry, calcVisualState);
                        });
                        onPaintOverlay();
                        al_set_clipping_rectangle(x1, y1, w, h);
                    }
                    break;
                }
            }
        }
    }


    bool Widget::_canGetFocus() const {
        return m_focusable && _enabledTree();
    }


    bool Widget::_enabledTree() const {
        for (const Widget* wgt = this; wgt; wgt = wgt->getParent()) {
            if (!wgt->m_enabled) {
                return false;
            }
        }
        return true;
    }


    void Widget::_resetMouseState() {
        if (m_hasMouse || m_childWithMouse) {
            m_hasMouse = false;
            m_childWithMouse = nullptr;
            forEach([](Widget* child) {
                child->_resetMouseState();
            });
        }
    }


    void Widget::_resetButtonState() {
        if (m_childWithButton) {
            m_childWithButton->_resetButtonState();
            m_childWithButton = nullptr;
        }
    }


    void Widget::_resetMouseAndButtonState() {
        _resetMouseState();
        _resetButtonState();
    }


    void Widget::_resetChildState(Widget* child) {
        if (m_childWithMouse == child) {
            m_childWithMouse = nullptr;
        }
        if (m_childWithButton == child) {
            m_childWithButton = nullptr;
        }
    }


    //get child from coordinates
    Widget* Widget::_getChildFromCoords(float screenX, float screenY) const {
        for (Widget* wgt = getLastChild(); wgt; wgt = wgt->getPrevSibling()) {
            if (wgt->m_visible && wgt->intersects(screenX, screenY)) {
                return wgt;
            }
        }
        return nullptr;
    }


    //get child that is enabled
    Widget* Widget::_getEnabledChild(float screenX, float screenY) const {
        Widget* child = getChild(screenX, screenY);
        return child && child->m_enabled ? child : nullptr;
    }


    Widget* Widget::_getClosestFocusContainerAncestor() const {
        for (Widget* ancestor = getParent(); ancestor; ancestor = ancestor->getParent()) {
            if (ancestor->m_focusContainer) {
                return ancestor;
            }
        }
        return nullptr;
    }


    Widget* Widget::_getDescentantWithLowerTabIndex(int tabIndex) const {
        for (Widget* descentant = getFirstChild(); descentant; descentant = getNext(descentant)) {
            if (descentant->_canGetFocus() && descentant->m_tabIndex < tabIndex) {
                return descentant;
            }
        }
        return nullptr;
    }


    Widget* Widget::_getDescentantWithHigherTabIndex(int tabIndex) const {
        for (Widget* descentant = getFirstChild(); descentant; descentant = getNext(descentant)) {
            if (descentant->_canGetFocus() && descentant->m_tabIndex > tabIndex) {
                return descentant;
            }
        }
        return nullptr;
    }


    Widget* Widget::_getPrevFocusDescentant(Widget* focusedWidget) const {
        if (focusedWidget) {
            for (Widget* descentant = getPrev(focusedWidget); descentant; descentant = getPrev(descentant)) {
                if (descentant->_canGetFocus()) {
                    return descentant;
                }
            }
        }

        for (Widget* descentant = getInnermostLastChild(); descentant != focusedWidget; descentant = getPrev(descentant)) {
            if (descentant->_canGetFocus()) {
                return descentant;
            }
        }

        return nullptr;
    }


    Widget* Widget::_getNextFocusDescentant(Widget* focusedWidget) const {
        if (focusedWidget) {
            for (Widget* descentant = getNext(focusedWidget); descentant; descentant = getNext(descentant)) {
                if (descentant->_canGetFocus()) {
                    return descentant;
                }
            }
        }

        for (Widget* descentant = getFirstChild(); descentant != focusedWidget; descentant = getNext(descentant)) {
            if (descentant->_canGetFocus()) {
                return descentant;
            }
        }

        return nullptr;
    }


    bool Widget::_isValidFocusContainer() const {
        if (m_enabled && m_focusContainer) {
            for (Widget* descentant = getFirstChild(); descentant; descentant = getNext(descentant)) {
                if (descentant->_canGetFocus()) {
                    return true;
                }
            }
        }
        return false;
    }


    Widget* Widget::_getPrevInnermostFocusContainer() const {
        for (Widget* child = getLastChild(); child; child = child->getPrevSibling()) {
            if (child->_isValidFocusContainer()) {
                Widget* descentant = child->_getPrevInnermostFocusContainer();
                if (descentant) {
                    return descentant;
                }
                return child;
            }
        }
        return nullptr;
    }


    Widget* Widget::_getNextInnermostFocusContainer() const {
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            if (child->_isValidFocusContainer()) {
                Widget* descentant = child->_getNextInnermostFocusContainer();
                if (descentant) {
                    return descentant;
                }
                return child;
            }
        }
        return nullptr;
    }


    float Widget::_getScreenCenterX() const {
        return (m_screenLeft + m_screenRight) / 2.0f;
    }


    float Widget::_getScreenCenterY() const {
        return (m_screenTop + m_screenBottom) / 2.0f;
    }


    void Widget::_initClick(int clickType, int button, const ALLEGRO_EVENT& event) {
        if (!_dragAndDrop && _clickType == _ClickType_None) {
            _clickType = clickType;
            _clickButton = button;
            _beginClickEvent(event);
            _getTimerThread().add(_emitClickEventFunction, _clickDelay);
        }
    }


    bool Widget::_endDragAndDrop(const ALLEGRO_EVENT& event) {
        bool result = false;
        if (_dragAndDrop) {
            result = _dragEvent(event, Event_DragEnded);
            _dragAndDrop = false;
            _draggedData.reset();
        }
        return result;
    }


    bool Widget::_joystickButtonEventCapture(const ALLEGRO_EVENT& event, EventType eventType) {
        Widget* parent = getParent();
        if (parent && parent->_joystickButtonEventCapture(event, eventType)) {
            return true;
        }
        return dispatchEvent(eventType, AllegroEvent(this, event), EventPhaseType::EventPhase_Capture);
    }


    bool Widget::_joystickButtonEventBubble(const ALLEGRO_EVENT& event, EventType eventType) {
        if (dispatchEvent(eventType, AllegroEvent(this, event), EventPhaseType::EventPhase_Bubble)) {
            return true;
        }
        Widget* parent = getParent();
        if (parent && parent->_joystickButtonEventBubble(event, eventType)) {
            return true;
        }
        return false;
    }


    bool Widget::_unusedJoystickButtonEvent(const ALLEGRO_EVENT& event, EventType eventType) {
        if (dispatchEvent(eventType, AllegroEvent(this, event), EventPhaseType::EventPhase_Capture)) {
            return true;
        }
        if (m_childWithMouse && m_childWithMouse->_unusedJoystickButtonEvent(event, eventType)) {
            return true;
        }
        return dispatchEvent(eventType, AllegroEvent(this, event), EventPhaseType::EventPhase_Bubble);
    }


    bool Widget::_joystickButtonEvent(const ALLEGRO_EVENT& event, EventType eventType) {
        if (_focusedWidget) {
            if (_focusedWidget->_joystickButtonEventCapture(event, eventType)) {
                return true;
            }
            if (_focusedWidget->_joystickButtonEventBubble(event, eventType)) {
                return true;
            }
        }
        return _unusedJoystickButtonEvent(event, eventType);
    }


    bool Widget::_joystickMoveEventCapture(const ALLEGRO_EVENT& event) {
        Widget* parent = getParent();
        if (parent && parent->_joystickMoveEventCapture(event)) {
            return true;
        }
        return dispatchEvent(Event_JoystickMove, AllegroEvent(this, event), EventPhaseType::EventPhase_Capture);
    }


    bool Widget::_joystickMoveEventBubble(const ALLEGRO_EVENT& event) {
        if (dispatchEvent(Event_JoystickMove, AllegroEvent(this, event), EventPhaseType::EventPhase_Bubble)) {
            return true;
        }
        Widget* parent = getParent();
        if (parent && parent->_joystickMoveEventBubble(event)) {
            return true;
        }
        return false;
    }


    bool Widget::_unusedJoystickMoveEvent(const ALLEGRO_EVENT& event) {
        if (dispatchEvent(Event_JoystickMove, AllegroEvent(this, event), EventPhaseType::EventPhase_Capture)) {
            return true;
        }
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            if (child->m_enabled && child->_unusedJoystickMoveEvent(event)) {
                return true;
            }
        }
        return dispatchEvent(Event_JoystickMove, AllegroEvent(this, event), EventPhaseType::EventPhase_Bubble);
    }


    bool Widget::_moveFocusByJoystick(const ALLEGRO_EVENT& event) {
        //0 axis is the horizontal axis.
        if (event.joystick.axis == 0) {
            if (event.joystick.pos < 0) {
                return moveFocusLeft();
            }
            else if (event.joystick.pos > 0) {
                return moveFocusRight();
            }
        }

        //1 axis is the vertical axis.
        else if (event.joystick.axis == 1) {
            if (event.joystick.pos < 0) {
                return moveFocusUp();
            }
            else if (event.joystick.pos > 0) {
                return moveFocusDown();
            }
        }

        return false;
    }


    bool Widget::_joystickMoveEvent(const ALLEGRO_EVENT& event) {
        //send joystick move event to the focus tree
        if (_focusedWidget) {
            if (_focusedWidget->_joystickMoveEventCapture(event)) {
                return true;
            }
            if (_focusedWidget->_joystickMoveEventBubble(event)) {
                return true;
            }
        }

        //sent unused joystick move event to the tree
        if (_unusedJoystickMoveEvent(event)) {
            return true;
        }

        //move focus by joystick
        return _moveFocusByJoystick(event);
    }


    //mouse button down event
    bool Widget::_mouseButtonDownEvent(const ALLEGRO_EVENT& event) {
        if (dispatchEvent(Event_MouseButtonDown, AllegroEvent(this, event), EventPhaseType::EventPhase_Capture)) {
            return true;
        }
        if (m_childWithMouse && m_childWithMouse->_mouseButtonDownEvent(event)) {
            return true;
        }
        return dispatchEvent(Event_MouseButtonDown, AllegroEvent(this, event), EventPhaseType::EventPhase_Bubble);
    }


    //mouse button up event
    bool Widget::_mouseButtonUpEvent(const ALLEGRO_EVENT& event) {
        if (dispatchEvent(_dragAndDrop ? Event_Drop : Event_MouseButtonUp, AllegroEvent(this, event), EventPhaseType::EventPhase_Capture)) {
            return true;
        }
        if (m_childWithMouse && m_childWithMouse->_mouseButtonUpEvent(event)) {
            return true;
        }
        return dispatchEvent(_dragAndDrop ? Event_Drop : Event_MouseButtonUp, AllegroEvent(this, event), EventPhaseType::EventPhase_Bubble);
    }


    //click event
    bool Widget::_clickEvent(const ALLEGRO_EVENT& event, EventType eventType) {
        if (dispatchEvent(eventType, AllegroEvent(this, event), EventPhaseType::EventPhase_Capture)) {
            _resetButtonState();
            return true;
        }
        if (m_childWithButton) {
            if (m_childWithButton->m_enabled && m_childWithButton->_clickEvent(event, eventType)) {
                _resetButtonState();
                return true;
            }
        }
        _resetButtonState();
        return dispatchEvent(eventType, AllegroEvent(this, event), EventPhaseType::EventPhase_Bubble);
    }


    //begin click event
    void Widget::_beginClickEvent(const ALLEGRO_EVENT& event) {
        m_childWithButton = _getEnabledChild(event.mouse.x, event.mouse.y);
        if (m_childWithButton) {
            m_childWithButton->_beginClickEvent(event);
        }
    }


    //end click event
    bool Widget::_endClickEvent(const ALLEGRO_EVENT& event) {
        const int buttonClickCount = (int)event.user.data2;
        if (buttonClickCount > 0 && buttonClickCount <= _MAX_CLICK_COUNT) {
            const bool result = _clickEvent(event, (EventType)(Event_Click + buttonClickCount - 1));
            _resetButtonState();
            return result;
        }
        return false;
    }


    bool Widget::_mouseMove(const ALLEGRO_EVENT& event, EventType eventType) {
        //dispatch event in capture phase
        if (dispatchEvent(eventType, AllegroEvent(this, event), EventPhaseType::EventPhase_Capture)) {
            return true;
        }

        Widget* oldMouseChild = m_childWithMouse;
        m_childWithMouse = _getEnabledChild(event.mouse.x, event.mouse.y);

        //if moved over the same child
        if (oldMouseChild == m_childWithMouse) {
            if (m_childWithMouse && m_childWithMouse->_mouseMove(event, _dragAndDrop ? Event_Drag : Event_MouseMove)) {
                return true;
            }
        }

        //else mouse moved over a different child
        else {
            bool result = false;

            //child mouse leave
            if (oldMouseChild) {
                result = oldMouseChild->_mouseLeave(event) || result;
            }

            //child mouse enter
            if (m_childWithMouse) {
                result = m_childWithMouse->_mouseEnter(event) || result;
            }

            if (result) {
                return result;
            }
        }

        return dispatchEvent(eventType, AllegroEvent(this, event), EventPhaseType::EventPhase_Bubble);
    }


    //mouse enter
    bool Widget::_mouseEnter(const ALLEGRO_EVENT& event) {
        m_hasMouse = true;
        return _mouseMove(event, _dragAndDrop ? Event_DragEnter : Event_MouseEnter);
    }
    

    //mouse leave
    bool Widget::_mouseLeave(const ALLEGRO_EVENT& event) {
        m_hasMouse = false;

        //dispatch event in capture phase
        if (dispatchEvent(_dragAndDrop ? Event_DragLeave : Event_MouseLeave, AllegroEvent(this, event), EventPhaseType::EventPhase_Capture)) {
            _resetMouseAndButtonState();
            return true;
        }

        //pass event to child with mouse
        if (m_childWithMouse && m_childWithMouse->_mouseLeave(event)) {
            _resetMouseAndButtonState();
            return true;
        }

        _resetMouseAndButtonState();

        //dispatch event in bubble phase
        return dispatchEvent(_dragAndDrop ? Event_DragLeave : Event_MouseLeave, AllegroEvent(this, event), EventPhaseType::EventPhase_Bubble);
    }


    //mouse wheel
    bool Widget::_mouseWheel(const ALLEGRO_EVENT& event) {
        //dispatch event in capture phase
        if (dispatchEvent(_dragAndDrop ? Event_DragWheel : Event_MouseWheel, AllegroEvent(this, event), EventPhaseType::EventPhase_Capture)) {
            return true;
        }

        //dispatch event in child with mouse
        if (m_childWithMouse && m_childWithMouse->_mouseWheel(event)) {
            return true;
        }

        //dispatch event in bubble phase
        return dispatchEvent(_dragAndDrop ? Event_DragWheel : Event_MouseWheel, AllegroEvent(this, event), EventPhaseType::EventPhase_Bubble);
    }


    //do capture phase for key event
    bool Widget::_keyEventCapture(const ALLEGRO_EVENT& event, EventType eventType) {
        Widget* parent = getParent();
        if (parent && parent->_keyEventCapture(event, eventType)) {
            return true;
        }
        return dispatchEvent(eventType, AllegroEvent(this, event), EventPhaseType::EventPhase_Capture);
    }


    //do bubble phase for key event
    bool Widget::_keyEventBubble(const ALLEGRO_EVENT& event, EventType eventType) {
        if (dispatchEvent(eventType, AllegroEvent(this, event), EventPhaseType::EventPhase_Bubble)) {
            return true;
        }
        Widget* parent = getParent();
        if (parent && parent->_keyEventBubble(event, eventType)) {
            return true;
        }
        return false;
    }


    //unused key event
    bool Widget::_unusedKeyEvent(const ALLEGRO_EVENT& event, EventType eventType) {
        if (dispatchEvent(eventType, AllegroEvent(this, event), EventPhaseType::EventPhase_Capture)) {
            return true;
        }
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            if (child->m_enabled && child->_unusedKeyEvent(event, eventType)) {
                return true;
            }
        }
        return dispatchEvent(eventType, AllegroEvent(this, event), EventPhaseType::EventPhase_Bubble);
    }


    //moves the focus according to the key pressed;
    //the focus is moved either to the sibling with a higher tab index,
    //or if there is no sibling with a higher tab index, then to the closest widget
    //that can get the input focus.
    bool Widget::_moveFocusByKey(const ALLEGRO_EVENT& event) {
        switch (event.keyboard.keycode) {
        case ALLEGRO_KEY_LEFT:
            if (event.keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT) {
                return moveFocusRight();
            }
            return moveFocusLeft();

        case ALLEGRO_KEY_UP:
            if (event.keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT) {
                return moveFocusDown();
            }
            return moveFocusUp();

        case ALLEGRO_KEY_RIGHT:
            if (event.keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT) {
                return moveFocusLeft();
            }
            return moveFocusRight();

        case ALLEGRO_KEY_DOWN:
            if (event.keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT) {
                return moveFocusUp();
            }
            return moveFocusDown();

        case ALLEGRO_KEY_TAB:
            if (event.keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT) {
                return moveFocusBackward();
            }
            return moveFocusForward();
        }

        return false;
    }


    //key event
    bool Widget::_keyEvent(const ALLEGRO_EVENT& event, EventType eventType) {
        //dispatch to focus widget first
        if (_focusedWidget) {
            if (_focusedWidget->_keyEventCapture(event, eventType)) {
                return true;
            }
            if (_focusedWidget->_keyEventBubble(event, eventType)) {
                return true;
            }
        }

        //do unused key event
        if (_unusedKeyEvent(event, eventType)) {
            return true;
        }

        //move the focus in response to key pressed
        if (eventType == Event_KeyChar && _moveFocusByKey(event)) {
            return true;
        }

        //event not processed at all
        return false;
    }


    bool Widget::_timerEvent(const ALLEGRO_EVENT& event) {
        if (dispatchEvent(Event_Timer, AllegroEvent(this, event), EventPhaseType::EventPhase_Capture)) {
            return true;
        }
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            if (child->_timerEvent(event)) {
                return true;
            }
        }
        return dispatchEvent(Event_Timer, AllegroEvent(this, event), EventPhaseType::EventPhase_Bubble);
    }


    bool Widget::_exposeEvent(const ALLEGRO_EVENT& event) {
        int x, y, w, h;
        al_get_clipping_rectangle(&x, &y, &w, &h);
        al_set_clipping_rectangle(event.display.x, event.display.y, event.display.width, event.display.height);
        render();
        al_set_clipping_rectangle(x, y, w, h);
        return true;
    }


    bool Widget::_dragEventCapture(const ALLEGRO_EVENT& event, EventType eventType) {
        Widget* parent = getParent();
        if (parent && parent->_dragEventCapture(event, eventType)) {
            return true;
        }
        return dispatchEvent(eventType, AllegroEvent(this, event), EventPhaseType::EventPhase_Capture);
    }
    
    
    bool Widget::_dragEventBubble(const ALLEGRO_EVENT& event, EventType eventType) {
        if (dispatchEvent(eventType, AllegroEvent(this, event), EventPhaseType::EventPhase_Bubble)) {
            return true;
        }
        Widget* parent = getParent();
        if (parent && parent->_dragEventBubble(event, eventType)) {
            return true;
        }
        return false;
    }
    
    
    bool Widget::_dragEvent(const ALLEGRO_EVENT& event, EventType eventType) {
        if (_dragEventCapture(event, eventType)) {
            return true;
        }
        return _dragEventBubble(event, eventType);
    }


    bool Widget::_internalEvent(const ALLEGRO_EVENT& event) {
        switch (event.user.data1) {
            case _Event_Click:
                return _endClickEvent(event);
        }
        return false;
    }


    /**************************************************************************
        EXPORTED
    **************************************************************************/


    //Returns the widget with the focus.
    Widget* getFocusedWidget() {
        return _focusedWidget;
    }


    //Returns the maximum time, in milliseconds, that can pass in order to register a click.
    size_t getClickDelay() {
        return _clickDelay;
    }


    //Sets the click delay, i.e. the maximum time than can pass in order to register a click.
    void setClickDelay(size_t msecs) {
        _clickDelay = msecs;
    }


    //returns the allegro event source for the UI
    ALLEGRO_EVENT_SOURCE* getUIEventSource() {
        static _AllegroEventSource UIEventSource;
        return UIEventSource.getEventSource();
    }


    //return drag-n-drop status
    bool isDragAndDropActive() {
        return _dragAndDrop;
    }


    //Returns the dragged data
    const std::any& getDraggedData() {
        return _draggedData;
    }


} //namespace algui
