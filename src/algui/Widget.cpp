#include <algorithm>
#include <cmath>
#include <limits>
#include <chrono>
#include "algui/Widget.hpp"


#ifdef max
#undef max
#undef min
#endif


namespace algui {


    static Widget* _focusedWidget = nullptr;
    static ALLEGRO_EVENT _lastMouseButtonDownEvent;
    static ALLEGRO_EVENT _lastMouseMoveEvent;
    static bool _dragAndDrop = false;
    static std::any _draggedData;
    static const float _dragAndDropStartDistance = 5;
    static ALLEGRO_BITMAP* _dragIcon = nullptr;
    static int _dragIconX = 0;
    static int _dragIconY = 0;
    static decltype(std::chrono::high_resolution_clock::now()) _startClickTime;
    static ALLEGRO_EVENT _clickButtonDownEvent;
    static std::chrono::milliseconds _clickDuration{ 500 };
    static int _clickCount = 0;


    static struct _Init {
        _Init() {
            _lastMouseButtonDownEvent.mouse.button = 0;
            _lastMouseMoveEvent.mouse.x = INT_MIN;
            _lastMouseMoveEvent.mouse.y = INT_MIN;
            _clickButtonDownEvent.mouse.button = 0;
        }
    } _init;


    static bool _enabledTree(const Widget* wgt) {
        for (; wgt; wgt = wgt->getParent()) {
            if (!wgt->getEnabled()) {
                return false;
            }
        }
        return true;
    }


    static float _distance(float x1, float y1, float x2, float y2) {
        const float dx = std::abs(x1 - x2);
        const float dy = std::abs(y1 - y2);
        return sqrt(dx*dx+dy*dy);
    }


    static bool _intersect(
        float ax1, float ay1, float ax2, float ay2, 
        float bx1, float by1, float bx2, float by2,
        float& rx1, float& ry1, float& rx2, float& ry2)
    {
        rx1 = std::max(ax1, bx1);
        ry1 = std::max(ay1, by1);
        rx2 = std::min(ax2, bx2);
        ry2 = std::min(ay2, by2);
        return rx1 > rx2 || ry1 > ry2;
    }


    Widget::Widget() 
        : m_parent(nullptr)
        , m_x(0)
        , m_y(0)
        , m_width(0)
        , m_height(0)
        , m_horizontalScaling(1.0f)
        , m_verticalScaling(1.0f)
        , m_x1(0)
        , m_y1(0)
        , m_x2(0)
        , m_y2(0)
        , m_treeHorizontalScaling(1.0f)
        , m_treeVerticalScaling(1.0f)
        , m_visible(true)
        , m_enabled(true)
        , m_highlighted(false)
        , m_pressed(false)
        , m_selected(false)
        , m_focused(false)
        , m_error(false)
        , m_treeVisible(true)
        , m_treeEnabled(true)
        , m_treeHighlighted(false)
        , m_treePressed(false)
        , m_treeSelected(false)
        , m_treeFocused(false)
        , m_treeError(false)
        , m_layout(true)
        , m_doingLayout(false)
        , m_managed(true)
        , m_flexible(true)
        , m_clipped(false)
    {
    }


    Widget::~Widget() {
        detach();
        if (_focusedWidget == this) {
            _focusedWidget = nullptr;
        }
    }


    const std::string& Widget::getId() const {
        return m_id;
    }


    Widget* Widget::getParent() const {
        return m_parent;
    }


    Widget* Widget::getPrevSibling() const {
        return m_parent && this != m_parent->m_children.front() ? *std::prev(m_it) : nullptr;
    }


    Widget* Widget::getNextSibling() const {
        return m_parent && this != m_parent->m_children.back() ? *std::next(m_it) : nullptr;
    }


    Widget* Widget::getFirstChild() const {
        return m_children.empty() ? nullptr : m_children.front();
    }


    Widget* Widget::getLastChild() const {
        return m_children.empty() ? nullptr : m_children.back();
    }


    Widget* Widget::getRoot() const {
        Widget* wgt = const_cast<Widget*>(this);
        for(; wgt->m_parent; wgt = wgt->m_parent) {}
        return wgt;
    }


    float Widget::getX() const {
        return m_x;
    }


    float Widget::getY() const {
        return m_y;
    }


    float Widget::getWidth() const {
        return m_width;
    }


    float Widget::getHeight() const {
        return m_height;
    }


    float Widget::getHorizontalScaling() const {
        return m_horizontalScaling;
    }


    float Widget::getVerticalScaling() const {
        return m_verticalScaling;
    }


    float Widget::getTreeHorizontalScaling() const {
        return m_treeHorizontalScaling;
    }


    float Widget::getTreeVerticalScaling() const {
        return m_treeVerticalScaling;
    }


    float Widget::getX1() const {
        return m_x1;
    }


    float Widget::getY1() const {
        return m_y1;
    }

    
    float Widget::getX2() const {
        return m_x2;
    }


    float Widget::getY2() const {
        return m_y2;
    }


    bool Widget::getVisible() const {
        return m_visible;
    }


    bool Widget::getEnabled() const {
        return m_enabled;
    }


    bool Widget::getHighlighted() const {
        return m_highlighted;
    }


    bool Widget::getPressed() const {
        return m_pressed;
    }


    bool Widget::getSelected() const {
        return m_selected;
    }


    bool Widget::getFocused() const {
        return m_focused;
    }


    bool Widget::getError() const {
        return m_error;
    }


    bool Widget::getTreeVisible() const {
        return m_treeVisible;
    }


    bool Widget::getTreeEnabled() const {
        return m_treeEnabled;
    }


    bool Widget::getTreeHighlighted() const {
        return m_treeHighlighted;
    }


    bool Widget::getTreePressed() const {
        return m_treePressed;
    }


    bool Widget::getTreeSelected() const {
        return m_treeSelected;
    }


    bool Widget::getTreeFocused() const {
        return m_treeFocused;
    }


    bool Widget::getTreeError() const {
        return m_treeError;
    }


    bool Widget::getManaged() const {
        return m_managed;
    }


    bool Widget::getFlexible() const {
        return m_flexible;
    }


    bool Widget::getClipped() const {
        return m_clipped;
    }


    size_t Widget::getDepth() const {
        size_t result = 0;
        for (const Widget* wgt = this; wgt->m_parent; wgt = wgt->m_parent, ++result) {}
        return result;
    }


    const std::shared_ptr<Theme>& Widget::getTheme() const {
        return m_theme;
    }


    const std::string& Widget::getSurfaceType() const {
        return m_surfaceType;
    }


    bool Widget::contains(const Widget* wgt) const {
        for (; wgt; wgt = wgt->m_parent) {
            if (wgt == this) {
                return true;
            }
        }
        return false;
    }


    Widget* Widget::get(float x, float y) const {
        for (Widget* child = getLastChild(); child; child = child->getPrevSibling()) {
            if (child->m_treeVisible && child->intersects(x, y)) {
                return child;
            }
        }
        return nullptr;
    }


    bool Widget::add(Widget* child, Widget* nextSibling) {
        if (!child || child->m_parent || child->contains(this) || (nextSibling && nextSibling->m_parent != this)) {
            return false;
        }
        child->m_parent = this;
        child->m_it = m_children.insert(nextSibling ? nextSibling->m_it : m_children.end(), child);
        child->_initThemeAndSurfaceType(m_theme, m_surfaceType);
        return true;
    }


    bool Widget::remove(Widget* child) {
        if (!child || child->m_parent != this) {
            return false;
        }
        child->m_parent = nullptr;
        m_children.erase(child->m_it);
        return true;
    }


    void Widget::detach() {
        if (m_parent) {
            m_parent->remove(this);
        }
    }


    void Widget::removeAll() {
        for (auto it = m_children.begin(); it != m_children.end();) {
            auto nextIt = std::next(it);
            remove(*it);
            it = nextIt;
        }
    }


    void Widget::setId(const std::string& id) {
        m_id = id;
    }


    void Widget::setX(float x) {
        if (x == m_x) {
            return;
        }
        m_x = x;
        invalidateParentLayout();
    }


    void Widget::setY(float y) {
        if (y == m_y) {
            return;
        }
        m_y = y;
        invalidateParentLayout();
    }


    void Widget::setWidth(float w) {
        if (w < 0) {
            w = 0;
        }
        if (w == m_width) {
            return;
        }
        m_width = w;
        Widget::invalidateLayout();
        invalidateParentLayout();
    }


    void Widget::setHeight(float h) {
        if (h < 0) {
            h = 0;
        }
        if (h == m_height) {
            return;
        }
        m_height = h;
        Widget::invalidateLayout();
        invalidateParentLayout();
    }


    void Widget::setPosition(float x, float y) {
        if (x == m_x && y == m_y) {
            return;
        }
        m_x = x;
        m_y = y;
        invalidateParentLayout();
    }


    void Widget::setSize(float width, float height) {
        if (width == m_width && height == m_height) {
            return;
        }
        m_width = width;
        m_height = height;
        Widget::invalidateLayout();
        invalidateParentLayout();
    }


    void Widget::setGeometry(float x, float y, float width, float height) {
        const bool samePosition = x == m_x && y == m_y;
        const bool sameSize = width == m_width && height == m_height;
        if (samePosition && sameSize) {
            return;
        }
        if (!samePosition) {
            m_x = x;
            m_y = y;
        }
        if (!sameSize) {
            m_width = width;
            m_height = height;
            Widget::invalidateLayout();
        }
        invalidateParentLayout();
    }


    void Widget::setHorizontalScaling(float f) {
        m_horizontalScaling = f;
    }


    void Widget::setVerticalScaling(float f) {
        m_verticalScaling = f;
    }


    void Widget::setScaling(float h, float v) {
        m_horizontalScaling = h;
        m_verticalScaling = v;
    }


    void Widget::setVisible(bool v) {
        if (v == m_visible) {
            return;
        }
        m_visible = v;
        invalidateParentLayout();
    }


    void Widget::setEnabled(bool v) {
        if (v == m_enabled) {
            return;
        }
        if (v && contains(_focusedWidget)) {
            _focusedWidget->setFocused(false);
        }
        m_enabled = v;
    }


    void Widget::setHighlighted(bool v) {
        m_highlighted = v;
    }


    void Widget::setPressed(bool v) {
        m_pressed = v;
    }


    void Widget::setSelected(bool v) {
        m_selected = v;
    }


    void Widget::setFocused(bool v) {
        if (v == m_focused) {
            return;
        }
        if (v) {
            if (!_enabledTree(this)) {
                return;
            }
            if (_focusedWidget) {
                _focusedWidget->setFocused(false);
            }
            m_focused = true;
            _focusedWidget = this;
            gotFocus();
        }
        else {
            m_focused = false;
            _focusedWidget = nullptr;
            lostFocus();
        }
    }


    Widget* Widget::getFocusedWidget() {
        return _focusedWidget;
    }
   
    
    void Widget::setError(bool v) {
        m_error = v;
    }


    void Widget::setManaged(bool v) {
        if (v != m_managed) {
            return;
        }
        m_managed = v;
        if (m_parent && !m_parent->m_doingLayout) {
            m_parent->invalidateLayout();
        }
    }
        
        
    void Widget::setFlexible(bool v) {
        if (v != m_flexible) {
            return;
        }
        m_flexible = v;
        if (m_parent && !m_parent->m_doingLayout) {
            m_parent->invalidateLayout();
        }
    }


    void Widget::setClipped(bool v) {
        m_clipped = v;
    }


    void Widget::invalidateLayout() {
        m_layout = true;
    }


    void Widget::invalidateParentLayout() const {
        if (m_parent && m_managed && !m_parent->m_doingLayout) {
            m_parent->invalidateLayout();
        }
    }


    void Widget::render() {
        _render();
        if (_dragAndDrop && _dragIcon) {
            int w = al_get_bitmap_width(_dragIcon);
            int h = al_get_bitmap_height(_dragIcon);
            int x = _lastMouseMoveEvent.mouse.x - _dragIconY * m_treeHorizontalScaling;
            int y = _lastMouseMoveEvent.mouse.y - _dragIconX * m_treeVerticalScaling;
            al_draw_scaled_bitmap(_dragIcon, 0, 0, w, h, x, y, w * m_treeHorizontalScaling, h * m_treeVerticalScaling, 0);
        }
    }


    bool Widget::beginDragAndDrop(const std::any& data) {
        if (!_lastMouseButtonDownEvent.mouse.button || _dragAndDrop || !data.has_value()) {
            return false;
        }
        const float dist = _distance(_lastMouseButtonDownEvent.mouse.x, _lastMouseButtonDownEvent.mouse.y, _lastMouseMoveEvent.mouse.x, _lastMouseMoveEvent.mouse.y);
        if (dist < _dragAndDropStartDistance) {
            return false;
        }
        if (_focusedWidget) {
            _focusedWidget->setFocused(false);
        }
        _dragAndDrop = true;
        _draggedData = data;
        return true;
    }


    const std::any& getDraggedData() {
        return _draggedData;
    }


    void Widget::setDragIcon(ALLEGRO_BITMAP* bitmap, int hotPointX, int hotPointY) {
        _dragIcon = bitmap;
        _dragIconX = hotPointX;
        _dragIconY = hotPointY;
    }


    size_t Widget::getClickTimeout() {
        return static_cast<size_t>(_clickDuration.count());
    }


    void Widget::setClickTimeout(size_t milliseconds) {
        _clickDuration = std::chrono::milliseconds(milliseconds);
    }


    void Widget::setTheme(const std::shared_ptr<Theme>& theme) {
        m_theme = theme;
        _callThemed();
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            child->setTheme(theme);
        }
    }


    void Widget::resetTheme() {
        m_theme.reset();
        unthemed();
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            child->resetTheme();
        }
    }


    void Widget::refreshTheme() {
        _callThemed();
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            child->refreshTheme();
        }
    }


    void Widget::setSurfaceType(const std::string& surfaceType) {
        if (surfaceType != m_surfaceType) {
            m_surfaceType = surfaceType;
            _callThemed();
        }
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            child->setSurfaceType(surfaceType);
        }
    }


    bool Widget::doEvent(const ALLEGRO_EVENT& event) {
        switch (event.type) {
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            {
                bool result = false;
                if (!_lastMouseButtonDownEvent.mouse.button) {
                    _lastMouseButtonDownEvent = event;
                    result = m_treeEnabled && !_dragAndDrop ? mouseButtonDown(event) : false;
                }
                if (!_clickButtonDownEvent.mouse.button) {
                    _startClickTime = std::chrono::high_resolution_clock::now();
                    _clickCount = 0;
                    _clickButtonDownEvent = event;
                }
                return result;
            }

            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            {
                bool result = false;
                if (event.mouse.button == _lastMouseButtonDownEvent.mouse.button) {
                    if (_dragAndDrop) {
                        if (m_treeEnabled) {
                            result = drop(event);
                        }
                        _dragAndDrop = false;
                        _dragIcon = nullptr;
                        _draggedData.reset();
                    }
                    else {
                        if (m_treeEnabled) {
                            result = mouseButtonUp(event);
                        }
                    }
                    _lastMouseButtonDownEvent.mouse.button = 0;
                }
                if (event.mouse.button == _clickButtonDownEvent.mouse.button) {
                    ++_clickCount;
                }
                return result;
            }

            case ALLEGRO_EVENT_MOUSE_AXES:
            case ALLEGRO_EVENT_MOUSE_WARPED:
            case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
            {
                //mouse move
                bool result1 = false;
                if (event.mouse.dx || event.mouse.dy) {
                    if (m_treeEnabled) {
                        const bool oldMouse = intersects(_lastMouseMoveEvent.mouse.x, _lastMouseMoveEvent.mouse.y);
                        const bool newMouse = intersects(event.mouse.x, event.mouse.y);
                        if (oldMouse && newMouse) {
                            result1 = _dragAndDrop ? drag(event) : mouseMove(event);
                        }
                        else if (newMouse) {
                            result1 = _dragAndDrop ? dragEnter(event) : mouseEnter(event);
                        }
                        else if (oldMouse) {
                            result1 = _dragAndDrop ? dragLeave(event) :mouseLeave(_lastMouseMoveEvent);
                        }
                    }
                    _lastMouseMoveEvent = event;
                }

                //mouse wheel
                bool result2 = false;
                if (event.mouse.dz || event.mouse.dw) {
                    if (m_treeEnabled) {
                        result2 = _dragAndDrop ? dragWheel(event) : mouseWheel(event);
                    }
                }

                return result1 || result2;
            }

            case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
            {
                bool result = false;
                if (m_treeEnabled) {
                    result = _dragAndDrop ? dragLeave(_lastMouseMoveEvent) : mouseLeave(_lastMouseMoveEvent);
                }
                _lastMouseMoveEvent = event;
                return result;
            }

            case ALLEGRO_EVENT_KEY_DOWN:
                if (_focusedWidget) {
                    return _focusedWidget->keyDown(event);
                }
                if (m_treeEnabled) {
                    return _dragAndDrop ? dragKeyDown(event) : unusedKeyDown(event);
                }
                return false;

            case ALLEGRO_EVENT_KEY_UP:
                if (_focusedWidget) {
                    return _focusedWidget->keyUp(event);
                }
                if (m_treeEnabled) {
                    return _dragAndDrop ? dragKeyUp(event) : unusedKeyUp(event);
                }
                return false;

            case ALLEGRO_EVENT_KEY_CHAR:
                if (_focusedWidget) {
                    return _focusedWidget->keyChar(event);
                }
                if (m_treeEnabled) {
                    return _dragAndDrop ? dragKeyChar(event) : unusedKeyChar(event);
                }
                return false;

            case ALLEGRO_EVENT_TIMER:
                {
                    const bool result1 = m_treeEnabled ? timer(event) : false;
                    bool result2 = false;
                    if (_clickButtonDownEvent.mouse.button) {
                        const auto now = std::chrono::high_resolution_clock::now();
                        const auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - _startClickTime);
                        if (dur >= _clickDuration) {
                            if (_clickCount == 1) {
                                result2 = click(_clickButtonDownEvent);
                            }
                            else if (_clickCount == 2) {
                                result2 = doubleClick(_clickButtonDownEvent);
                            }
                            _clickButtonDownEvent.mouse.button = 0;
                        }
                    }
                    return result1 || result2;
                }

            case ALLEGRO_EVENT_DISPLAY_EXPOSE:
            {
                int cx, cy, cw, ch;
                al_get_clipping_rectangle(&cx, &cy, &cw, &ch);
                al_set_clipping_rectangle(event.display.x, event.display.y, event.display.width, event.display.height);
                render();
                al_set_clipping_rectangle(cx, cy, cw, ch);
                return true;
            }
        }
        return false;
    }


    bool Widget::intersects(float x, float y) const {
        return x >= m_x1 && x < m_x2 && y >= m_y1 && y < m_y2;
    }


    bool Widget::mouseButtonDown(const ALLEGRO_EVENT& event) {
        Widget* child = get(event.mouse.x, event.mouse.y);
        return child && child->m_treeEnabled ? child->mouseButtonDown(event) : false;
    }


    bool Widget::mouseButtonUp(const ALLEGRO_EVENT& event) {
        Widget* child = get(event.mouse.x, event.mouse.y);
        return child && child->m_treeEnabled ? child->mouseButtonUp(event) : false;
    }


    bool Widget::mouseEnter(const ALLEGRO_EVENT& event) {
        Widget* child = get(event.mouse.x, event.mouse.y);
        return child && child->m_treeEnabled ? child->mouseEnter(event) : false;
    }


    bool Widget::mouseMove(const ALLEGRO_EVENT& event) {
        Widget* oldChild = get(_lastMouseMoveEvent.mouse.x, _lastMouseMoveEvent.mouse.y);
        Widget* newChild = get(event.mouse.x, event.mouse.y);
        if (oldChild == newChild) {
            return newChild && newChild->m_treeEnabled ? newChild->mouseMove(event) : false;
        }
        bool result1 = false, result2 = false;
        if (oldChild) {
            result1 = oldChild->m_treeEnabled ? oldChild->mouseLeave(_lastMouseMoveEvent) : false;
        }
        if (newChild) {
            result1 = newChild->m_treeEnabled ? newChild->mouseEnter(event) : false;
        }
        return result1 || result2;
    }


    bool Widget::mouseLeave(const ALLEGRO_EVENT& event) {
        Widget* child = get(event.mouse.x, event.mouse.y);
        return child && child->m_treeEnabled ? child->mouseLeave(event) : false;
    }


    bool Widget::mouseWheel(const ALLEGRO_EVENT& event) {
        Widget* child = get(event.mouse.x, event.mouse.y);
        return child && child->m_treeEnabled ? child->mouseWheel(event) : false;
    }


    bool Widget::keyDown(const ALLEGRO_EVENT& event) {
        return getRoot()->unusedKeyDown(event);
    }


    bool Widget::keyUp(const ALLEGRO_EVENT& event) {
        return getRoot()->unusedKeyUp(event);
    }


    bool Widget::keyChar(const ALLEGRO_EVENT& event) {
        return getRoot()->unusedKeyChar(event);
    }


    bool Widget::unusedKeyDown(const ALLEGRO_EVENT& event) {
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            if (child->m_treeEnabled && child->unusedKeyDown(event)) {
                return true;
            }
        }
        return false;
    }


    bool Widget::unusedKeyUp(const ALLEGRO_EVENT& event) {
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            if (child->m_treeEnabled && child->unusedKeyUp(event)) {
                return true;
            }
        }
        return false;
    }


    bool Widget::unusedKeyChar(const ALLEGRO_EVENT& event) {
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            if (child->m_treeEnabled && child->unusedKeyChar(event)) {
                return true;
            }
        }
        return false;
    }


    bool Widget::drop(const ALLEGRO_EVENT& event) {
        Widget* child = get(event.mouse.x, event.mouse.y);
        return child && child->m_treeEnabled ? child->drop(event) : false;
    }


    bool Widget::dragEnter(const ALLEGRO_EVENT& event) {
        Widget* child = get(event.mouse.x, event.mouse.y);
        return child && child->m_treeEnabled ? child->dragEnter(event) : false;
    }


    bool Widget::drag(const ALLEGRO_EVENT& event) {
        Widget* oldChild = get(_lastMouseMoveEvent.mouse.x, _lastMouseMoveEvent.mouse.y);
        Widget* newChild = get(event.mouse.x, event.mouse.y);
        if (oldChild == newChild) {
            return newChild && newChild->m_treeEnabled ? newChild->drag(event) : false;
        }
        bool result1 = false, result2 = false;
        if (oldChild) {
            result1 = oldChild->m_treeEnabled ? oldChild->dragLeave(_lastMouseMoveEvent) : false;
        }
        if (newChild) {
            result1 = newChild->m_treeEnabled ? newChild->dragEnter(event) : false;
        }
        return result1 || result2;
    }


    bool Widget::dragLeave(const ALLEGRO_EVENT& event) {
        Widget* child = get(event.mouse.x, event.mouse.y);
        return child && child->m_treeEnabled ? child->dragLeave(event) : false;
    }


    bool Widget::dragWheel(const ALLEGRO_EVENT& event) {
        Widget* child = get(event.mouse.x, event.mouse.y);
        return child && child->m_treeEnabled ? child->dragWheel(event) : false;
    }


    bool Widget::dragKeyDown(const ALLEGRO_EVENT& event) {
        Widget* child = get(_lastMouseMoveEvent.mouse.x, _lastMouseMoveEvent.mouse.y);
        return child && child->m_treeEnabled ? child->dragKeyDown(event) : false;
    }


    bool Widget::dragKeyUp(const ALLEGRO_EVENT& event) {
        Widget* child = get(_lastMouseMoveEvent.mouse.x, _lastMouseMoveEvent.mouse.y);
        return child && child->m_treeEnabled ? child->dragKeyUp(event) : false;
    }


    bool Widget::dragKeyChar(const ALLEGRO_EVENT& event) {
        Widget* child = get(_lastMouseMoveEvent.mouse.x, _lastMouseMoveEvent.mouse.y);
        return child && child->m_treeEnabled ? child->dragKeyChar(event) : false;
    }


    bool Widget::timer(const ALLEGRO_EVENT& event) {
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            if (child->timer(event)) {
                return true;
            }
        }
        return false;
    }


    bool Widget::click(const ALLEGRO_EVENT& event) {
        Widget* child = get(event.mouse.x, event.mouse.y);
        return child && child->m_treeEnabled ? child->click(event) : false;
    }


    bool Widget::doubleClick(const ALLEGRO_EVENT& event) {
        Widget* child = get(event.mouse.x, event.mouse.y);
        return child && child->m_treeEnabled ? child->doubleClick(event) : false;
    }


    void Widget::_render() {
        if (m_visible) {
            const float prevTreeHorizontalScaling = m_treeHorizontalScaling;
            const float prevTreeVerticalScaling = m_treeVerticalScaling;

            //calculate screen state for child
            if (m_parent) {
                m_x1 = m_x * m_parent->m_treeHorizontalScaling + m_parent->m_x1;
                m_y1 = m_y * m_parent->m_treeVerticalScaling + m_parent->m_y1;
                m_x2 = m_x1 + m_width * m_parent->m_treeHorizontalScaling;
                m_y2 = m_y1 + m_height * m_parent->m_treeVerticalScaling;
                m_treeVisible = m_visible && m_parent->m_treeVisible;
                m_treeEnabled = m_enabled && m_parent->m_treeEnabled;
                m_treeHighlighted = m_highlighted || m_parent->m_treeHighlighted;
                m_treePressed = m_pressed || m_parent->m_treePressed;
                m_treeSelected = m_selected || m_parent->m_treeSelected;
                m_treeFocused = m_focused || m_parent->m_treeFocused;
                m_treeError = m_error || m_parent->m_treeError;
                m_treeHorizontalScaling = m_horizontalScaling * m_parent->m_treeHorizontalScaling;
                m_treeVerticalScaling = m_verticalScaling * m_parent->m_treeVerticalScaling;
            }

            //else calculate screen state for parent
            else {
                m_x1 = m_x;
                m_y1 = m_y;
                m_x2 = m_x1 + m_width;
                m_y2 = m_y1 + m_height;
                m_treeVisible = m_visible;
                m_treeEnabled = m_enabled;
                m_treeHighlighted = m_highlighted;
                m_treePressed = m_pressed;
                m_treeSelected = m_selected;
                m_treeFocused = m_focused;
                m_treeError = m_error;
                m_treeHorizontalScaling = m_horizontalScaling;
                m_treeVerticalScaling = m_verticalScaling;
            }
            
            //if tree scaling changed, allow the widget to pick the correct resources
            if (m_treeHorizontalScaling != prevTreeHorizontalScaling || m_treeVerticalScaling != prevTreeVerticalScaling) {
                scaled();
            }
            
            //paint clipped
            int cx, cy, cw, ch;
            if (m_clipped) {
                al_get_clipping_rectangle(&cx, &cy, &cw, &ch);

                //calculate intersection between widget rectangle and current clipping
                float rx1, ry1, rx2, ry2;
                const bool totallyClipped = _intersect(
                    cx, cy, cx + cw, cy + ch, 
                    m_x1, m_y1, m_x2, m_y2,
                    rx1, ry1, rx2, ry2);

                //if the widget is outside of the current clipping,
                //then do not draw it
                if (totallyClipped) {
                    return;
                }

                //clip the output
                al_set_clipping_rectangle(rx1, ry1, std::floor(rx2 - rx1), std::floor(ry2 - ry1));
            }

            //paint the widget
            paint();
            
            //before painting the children, do the layout
            if (m_layout) {
                m_doingLayout = true;
                layout();
                m_doingLayout = false;
                m_layout = false;
            }

            //render the children
            for (Widget* child : m_children) {
                child->_render();
            }

            //paint above the children
            paintOverlay();

            //restore clipping
            if (m_clipped) {
                al_set_clipping_rectangle(cx, cy, cw, ch);
            }
        }
    }


    void Widget::_callThemed() {
        if (m_theme) {
            themed();
        }
        else {
            unthemed();
        }
    }


    void Widget::_initThemeAndSurfaceType(const std::shared_ptr<Theme>& theme, const std::string& surfaceType) {
        if (theme != m_theme || surfaceType != m_surfaceType) {
            m_theme = theme;
            m_surfaceType = surfaceType;
            _callThemed();
        }
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            child->_initThemeAndSurfaceType(theme, surfaceType);
        }
    }


} //namespace algui
