#ifndef ALGUI_WIDGET_HPP
#define ALGUI_WIDGET_HPP


#include <functional>
#include <memory>
#include <utility>
#include <string>
#include "Tree.hpp"
#include "Coord.hpp"
#include "EventTarget.hpp"


namespace algui {


    /**
     * Clipping mode.
     */
    enum class ClippingMode {
        /**
         * The output is not clipped.
         * This is the default.
         */
        None,

        /**
         * Clip widget only.
         */
        Widget,

        /**
         * Clip widget and descentants.
         */
        Tree
    };


    /**
     * Event types.
     */
    enum EventType {
        /** Widget got the focus. */
        Event_GotFocus,

        /** Widget lost the focus. */
        Event_LostFocus,

        /** Descentant widget got the focus. */
        Event_DescentantGotFocus,

        /** Descentant widget lost the focus. */
        Event_DescentantLostFocus,

        /**
         * Joystick move event.
         * Dispatched at the widget tree that contains the input focus,
         * then if unused to the whole UI tree.
         */
        Event_JoystickMove,

        /**
         * Joystick button down event.
         * Dispatched at the widget tree that contains the input focus,
         * then if unused to the UI tree under the mouse.
         */
        Event_JoystickButtonDown,

        /**
         * Joystick button up event.
         * Dispatched at the widget tree that contains the input focus,
         * then if unused to the UI tree under the mouse.
         */
        Event_JoystickButtonUp,

        /**
         * Key down event.
         * Delivered to the widget with the focus.
         * If there is no focus widget to get the input focus,
         * then it is sent to the whole UI tree.
         */
        Event_KeyDown,

        /**
         * Key up event.
         * Delivered to the widget with the focus.
         * If there is no focus widget to get the input focus,
         * then it is sent to the whole UI tree.
         */
        Event_KeyUp,

        /**
         * Character event.
         * Delivered to the widget with the focus.
         * If there is no focus widget to get the input focus,
         * then it is sent to the whole UI tree.
         */
        Event_KeyChar,

        /** Mouse button down event. */
        Event_MouseButtonDown,

        /** Mouse button up event. */
        Event_MouseButtonUp,

        /** Mouse enter event. */
        Event_MouseEnter,

        /** Mouse move event. */
        Event_MouseMove,

        /** Mouse leave event. */
        Event_MouseLeave,

        /** Mouse wheel event. */
        Event_MouseWheel,

        /** timer event. */
        Event_Timer,

        /** First value available for user events. */
        Event_User = 256
    };


    /**
     * Event phase types.
     */
    enum EventPhaseType {
        /**
         * Bubble phase: events are propagated from child to parent.
         * It is the default value for all events that do not have a capture phase.
         */
        EventPhase_Bubble,

        /**
         * Capture phase: events are propagated from parent to child.
         */
        EventPhase_Capture,

        /** First value available for user event phases. */
        EventPhase_User = 256
    };


    class Widget;


    /**
     * Base class for widget events.
     */
    class WidgetEvent : public Event {
    public:
        /**
         * The constructor.
         * @param target the target widget.
         */
        WidgetEvent(Widget* target) : m_target(target) {
        }

        /**
         * Returns the target widget.
         * @return the target widget.
         */
        Widget* getTarget() const {
            return m_target;
        }

    private:
        Widget* m_target;
    };


    /**
     * An allegro event.
     */
    class AllegroEvent : public WidgetEvent {
    public:
        /**
         * Constructor.
         * @param target target widget.
         * @param event allegro event.
         */
        AllegroEvent(Widget* target, const ALLEGRO_EVENT& event) 
            : WidgetEvent(target)
            , m_event(event)
        {
        }

        /**
         * Returns the allegro event.
         * @return the allegro event.
         */
        const ALLEGRO_EVENT& getEvent() const {
            return m_event;
        }

    private:
        ALLEGRO_EVENT m_event;
    };


    /**
     * Base class for UI elements. 
     */
    class Widget 
        : public Tree<Widget>
        , public EventTarget<EventType, EventPhaseType>
        , public EventHandlerOwner<EventType, EventPhaseType>
    {
    public:
        /**
         * The default constructor.
         */
        Widget();

        /**
         * The destructor.
         * Deletes all children.
         * Resets the focus widget pointer to null if it is this widget.
         * Removes itself from the set of widgets that receive timer events, if needed.
         */
        ~Widget();

        /**
         * Inserts a child widget.
         * It causes recomputation of the parent's geometry constraints and layout.
         * @param child child widget to add.
         * @param nextSibling next sibling.
         * @return true on success, false on failure.
         */
        bool add(Widget* child, Widget* nextSibling = nullptr) override;

        /**
         * Removes a child widget.
         * It causes recomputation of the parent's geometry constraints and layout.
         * @param child the child to remove.
         * @return true on success, false on failure.
         */
        bool remove(Widget* child) override;

        /**
         * Returns the left coordinate.
         * @return the left coordinate.
         */
        const Coord& getLeft() const {
            return m_left;
        }

        /**
         * Returns the top coordinate.
         * @return the top coordinate.
         */
        const Coord& getTop() const {
            return m_top;
        }

        /**
         * Returns the width.
         * @return the width.
         */
        const Coord& getWidth() const {
            return m_width;
        }

        /**
         * Returns the height.
         * @return the height.
         */
        const Coord& getHeight() const {
            return m_height;
        }

        /**
         * Returns the scaling factor along the X axis.
         * @return the scaling factor along the X axis.
         */
        float getScalingX() const {
            return m_scalingX;
        }

        /**
         * Returns the scaling factor along the Y axis.
         * @return the scaling factor along the Y axis.
         */
        float getScalingY() const {
            return m_scalingY;
        }

        /**
         * Returns the minimum width geometry constraint of the widget.
         * @return the minimum width geometry constraint of the widget.
         */
        const Coord& getMinimumWidth() const {
            return m_minimumWidth;
        }

        /**
         * Returns the minimum height geometry constraint of the widget.
         * @return the minimum height geometry constraint of the widget.
         */
        const Coord& getMinimumHeight() const {
            return m_minimumHeight;
        }

        /**
         * Returns the optimal width geometry constraint of the widget.
         * @return the optimal width geometry constraint of the widget.
         */
        const Coord& getOptimalWidth() const {
            return m_optimalWidth;
        }

        /**
         * Returns the optimal height geometry constraint of the widget.
         * @return the optimal height geometry constraint of the widget.
         */
        const Coord& getOptimalHeight() const {
            return m_optimalHeight;
        }

        /**
         * Returns the maximum width geometry constraint of the widget.
         * @return the maximum width geometry constraint of the widget.
         */
        const Coord& getMaximumWidth() const {
            return m_maximumWidth;
        }

        /**
         * Returns the maximum height geometry constraint of the widget.
         * @return the maximum height geometry constraint of the widget.
         */
        const Coord& getMaximumHeight() const {
            return m_maximumHeight;
        }

        /**
         * Returns the visible state of the widget.
         * @return the visible state of the widget.
         */
        bool isVisible() const {
            return m_visible;
        }

        /**
         * Returns the left pixel coordinate relative to the screen.
         * Screen geometry is valid only after the widget tree is rendered.
         * @return the left pixel coordinate relative to the screen.
         */
        float getScreenLeft() const {
            return m_screenLeft;
        }

        /**
         * Returns the top pixel coordinate relative to the screen.
         * Screen geometry is valid only after the widget tree is rendered.
         * @return the top pixel coordinate relative to the screen.
         */
        float getScreenTop() const {
            return m_screenTop;
        }

        /**
         * Returns the right pixel coordinate relative to the screen.
         * Screen geometry is valid only after the widget tree is rendered.
         * @return the right pixel coordinate relative to the screen.
         */
        float getScreenRight() const {
            return m_screenRight;
        }

        /**
         * Returns the bottom pixel coordinate relative to the screen.
         * Screen geometry is valid only after the widget tree is rendered.
         * @return the bottom pixel coordinate relative to the screen.
         */
        float getScreenBottom() const {
            return m_screenBottom;
        }

        /**
         * Returns the width of the widget on the screen.
         * It might be different than the one set due to scaling.
         * @return the width of the widget on the screen.
         */
        float getScreenWidth() const {
            return m_screenRight - m_screenLeft;
        }

        /**
         * Returns the height of the widget on the screen.
         * It might be different than the one set due to scaling.
         * @return the height of the widget on the screen.
         */
        float getScreenHeight() const {
            return m_screenBottom - m_screenTop;
        }

        /**
         * Returns the scaling factor for the X axis 
         * that is the product of all scaling factors from root to this.
         * Screen geometry is valid only after the widget tree is rendered.
         * @return the screen scaling factor for the X axis.
         */
        float getScreenScalingX() const {
            return m_screenScalingX;
        }

        /**
         * Returns the scaling factor for the Y axis
         * that is the product of all scaling factors from root to this.
         * Screen geometry is valid only after the widget tree is rendered.
         * @return the screen scaling factor for the Y axis.
         */
        float getScreenScalingY() const {
            return m_screenScalingY;
        }

        /**
         * Returns the tab index of the widget.
         * @return the tab index of the widget.
         */
        int getTabIndex() const {
            return m_tabIndex;
        }

        /**
         * Returns the focus container state.
         * A widget that is a focus container does not allow moving of focus with the keyboard outside of itself.
         * By default, no widget is a focus container, allowing moving of focus with the keyboard on the whole UI.
         * @return the focus container state.
         */
        bool isFocusContainer() const {
            return m_focusContainer;
        }

        /**
         * Returns the clipping mode.
         * @return the clipping mode.
         */
        ClippingMode getClippingMode() const {
            return m_clippingMode;
        }

        /**
         * Returns the enabled state of the widget.
         * @return the enabled state of the widget.
         */
        bool isEnabled() const {
            return m_enabled;
        }

        /**
         * Returns the enabled state of the widget tree the widget belongs to.
         * @return the enabled state of the widget tree the widget belongs to.
         */
        bool isEnabledTree() const {
            return m_enabledTree;
        }

        /**
         * Returns the highlighted state of the widget.
         * @return the highlighted state of the widget.
         */
        bool isHighlighted() const {
            return m_highlighted;
        }

        /**
         * Returns the highlighted state of the widget tree the widget belongs to.
         * @return the highlighted state of the widget tree the widget belongs to.
         */
        bool isHighlightedTree() const {
            return m_highlightedTree;
        }

        /**
         * Returns the pressed state of the widget.
         * @return the pressed state of the widget.
         */
        bool isPressed() const {
            return m_pressed;
        }

        /**
         * Returns the pressed state of the widget tree the widget belongs to.
         * @return the pressed state of the widget tree the widget belongs to.
         */
        bool isPressedTree() const {
            return m_pressedTree;
        }

        /**
         * Returns the selected state of the widget.
         * @return the selected state of the widget.
         */
        bool isSelected() const {
            return m_selected;
        }

        /**
         * Returns the selected state of the widget tree the widget belongs to.
         * @return the selected state of the widget tree the widget belongs to.
         */
        bool isSelectedTree() const {
            return m_selectedTree;
        }

        /**
         * Returns the focused state of the widget.
         * @return the focused state of the widget.
         */
        bool isFocused() const {
            return m_focused;
        }

        /**
         * Returns the focused state of the widget tree the widget belongs to.
         * @return the focused state of the widget tree the widget belongs to.
         */
        bool isFocusedTree() const {
            return m_focusedTree;
        }

        /**
         * Returns the valid content state of the widget.
         * @return the valid content state of the widget.
         */
        bool hasValidContent() const {
            return m_validContent;
        }

        /**
         * Returns the valid content state of the widget tree the widget belongs to.
         * @return the valid content state of the widget tree the widget belongs to.
         */
        bool treeHasValidContent() const {
            return m_validContentTree;
        }

        /**
         * Returns the focusable state of the widget.
         * @return the focusable state of the widget.
         */
        bool isFocusable() const {
            return m_focusable;
        }

        /**
         * Returns the id of the widget.
         * @return the id of the widget.
         */
        const std::string& getId() const {
            return m_id;
        }

        /**
         * Sets the left coordinate.
         * It causes recomputation of screen geometry from the `render()` method.
         * It also causes recomputation of the parent's layout, if there is a parent.
         * @param left the new left coordinate.
         */
        void setLeft(const Coord& left);

        /**
         * Sets the top coordinate.
         * It causes recomputation of screen geometry from the `render()` method.
         * It also causes recomputation of the parent's layout, if there is a parent.
         * @param top the new top coordinate.
         */
        void setTop(const Coord& top);

        /**
         * Sets the width.
         * It causes recomputation of screen geometry from the `render()` method.
         * It causes recomputation of the widget's layout.
         * It also causes recomputation of the parent's layout, if there is a parent.
         * @param width the new width.
         */
        void setWidth(const Coord& width);

        /**
         * Sets the height.
         * It causes recomputation of screen geometry from the `render()` method.
         * It causes recomputation of the widget's layout.
         * It also causes recomputation of the parent's layout, if there is a parent.
         * @param height the new height.
         */
        void setHeight(const Coord& height);

        /**
         * Sets the interior scaling factor of the component along the X axis.
         * It causes recomputation of screen geometry from the `render()` method.
         * @param scalingX the new value of the X axis scaling factor.
         */
        void setScalingX(const float scalingX);

        /**
         * Sets the interior scaling factor of the component along the Y axis.
         * It causes recomputation of screen geometry from the `render()` method.
         * @param scalingY the new value of the Y axis scaling factor.
         */
        void setScalingY(const float scalingY);

        /**
         * Sets the visible state of the widget.
         * It causes recomputation of the parent's geometry constraints and layout, if there is a parent.
         * @param visible the new visible state of the widget.
         */
        void setVisible(bool visible);

        /**
         * Sets the minimum width geometry constraint of the widget.
         * It causes recomputation of the parent's geometry constraints and layout, if there is a parent.
         * @param minimumWidth the new minimum width.
         */
        void setMinimumWidth(const Coord& minimumWidth);

        /**
         * Sets the minimum height geometry constraint of the widget.
         * It causes recomputation of the parent's geometry constraints and layout, if there is a parent.
         * @param minimumHeight the new minimum height.
         */
        void setMinimumHeight(const Coord& minimumHeight);

        /**
         * Sets the optimal width geometry constraint of the widget.
         * It causes recomputation of the parent's geometry constraints and layout, if there is a parent.
         * @param optimalWidth the new optimal width.
         */
        void setOptimalWidth(const Coord& optimalWidth);

        /**
         * Sets the optimal height geometry constraint of the widget.
         * It causes recomputation of the parent's geometry constraints and layout, if there is a parent.
         * @param optimalHeight the new optimal height.
         */
        void setOptimalHeight(const Coord& optimalHeight);

        /**
         * Sets the maximum width geometry constraint of the widget.
         * It causes recomputation of the parent's geometry constraints and layout, if there is a parent.
         * @param maximumWidth the new maximum width.
         */
        void setMaximumWidth(const Coord& maximumWidth);

        /**
         * Sets the maximum height geometry constraint of the widget.
         * It causes recomputation of the parent's geometry constraints and layout, if there is a parent.
         * @param maximumHeight the new maximum height.
         */
        void setMaximumHeight(const Coord& maximumHeight);

        /**
         * Sets the tab index of the widget.
         * @param tabIndex the new tab index.
         */
        void setTabIndex(int tabIndex) {
            m_tabIndex = tabIndex;
        }

        /**
         * Sets the focus container flag.
         * A widget that is a focus container does not allow moving of focus with the keyboard outside of itself.
         * By default, no widget is a focus container, allowing moving of focus with the keyboard on the whole UI.
         * If the widget gets the focus container state, it loses the focusable state,
         * since a widget cannot have the focus and also be a focus container.
         * @param focusContainer the new focus container state.
         */
        void setFocusContainer(bool focusContainer);

        /**
         * Sets the clipping mode.
         * @@param the clipping mode.
         */
        void setClippingMode(ClippingMode mode) {
            m_clippingMode = mode;
        }

        /**
         * Sets the enabled state of the widget.
         * If disabled and contains the focus, then the focus is lost.
         * @param enabled the enabled state of the widget.
         */
        void setEnabled(bool enabled);

        /**
         * Sets the highlighted state of the widget.
         * @param highlighted the highlighted state of the widget.
         */
        void setHighlighted(bool highlighted);

        /**
         * Sets the pressed state of the widget.
         * @param pressed the pressed state of the widget.
         */
        void setPressed(bool pressed);

        /**
         * Sets the selected state of the widget.
         * @param selected the selected state of the widget.
         */
        void setSelected(bool selected);

        /**
         * Sets the focused state of the widget.
         * It cannot get the focus if it is disabled or within a disabled tree or not focusable.
         * The previous focused widget loses the focus.
         * If the widget gets the focus, then it gets the got focus event, and ancestors get the descentant got focus event.
         * If the widget loses the focus, then it gets the lost focus event, and ancestors get the descentant lost focus event.
         * Focus events are dispatched in the bubble phase, since they bubble up.
         * @param focused the focused state of the widget.
         */
        void setFocused(bool focused);

        /**
         * Returns the widget with the focus.
         * @return the widget with the focus.
         */
        static Widget* getFocusedWidget();

        /**
         * Sets the focusable state.
         * If the widget becomes focusable, it loses the focus container status,
         * since a widget can either be focusable or contain the focus, but not both.
         * @param focusable the new focusable state.
         */
        void setFocusable(bool focusable);

        /**
         * Sets the valid content state of the widget.
         * @param valid content the valid content state of the widget.
         */
        void setValidContent(bool validcontent);

        /**
         * Sets the id of the widget.
         * @param id id of the widget.
         */
        void setId(const std::string& id) {
            m_id = id;
        }

        /**
         * Renders the tree into the target bitmap.
         * The current clipping is respected: if a widget falls outside of the current clipping rectangle,
         * it is not repainted.
         */
        void render();

        /**
         * Checks if the given screen point lies within the widget.
         * The widget must have been rendered at least once in order for the result to be valid.
         * The clipping mode affects the outcome:
         *  - if 'none' or 'widget', if the coordinate is outside of the widget area, then children are also tested.
         *  - if 'tree', only the test for inside the widget area is done.
         * @param screenX screen coordinate along the X axis.
         * @param screenY screen coordinate along the Y axis.
         * @return true if the given point is within the widget, false otherwise.
         */
        virtual bool intersects(float screenX, float screenY) const;

        /**
         * Returns visible child under the given coordinates.
         * The clipping mode also affects the outcome: 
         * If clipping mode is 'tree', then children only visible through the parent are considered.
         * @param screenX screen coordinate along the X axis.
         * @param screenY screen coordinate along the Y axis.
         * @return the child under the given coordinates.
         */
        Widget* getChild(float screenX, float screenY) const;

        /**
         * creates and dispatches events out of an allegro event.
         * @param event allegro event to dispatch.
         * @return true if the event was processed, false otherwise.
         */
        bool processAllegroEvent(const ALLEGRO_EVENT& event);

        /**
         * Moves the focus backward, according to tab index and widget order.
         * @return true if the focus was moved, false otherwise.
         */
        bool moveFocusBackward();

        /**
         * Moves the focus forward, according to tab index and widget order.
         * @return true if the focus was moved, false otherwise.
         */
        bool moveFocusForward();

        /**
         * Moves the focus to the left of the currently focused widget, depending on widget position.
         * @return true if the focus was moved, false otherwise.
         */
        bool moveFocusLeft();

        /**
         * Moves the focus above the currently focused widget, depending on widget position.
         * @return true if the focus was moved, false otherwise.
         */
        bool moveFocusUp();

        /**
         * Moves the focus to the right of the currently focused widget, depending on widget position.
         * @return true if the focus was moved, false otherwise.
         */
        bool moveFocusRight();

        /**
         * Moves the focus under the currently focused widget, depending on widget position.
         * @return true if the focus was moved, false otherwise.
         */
        bool moveFocusDown();

        /**
         * Adds this widget to the set of the widgets that receive timer events.
         * If the widget is already added, the call is ignored.
         * By default, a widget does not receive timer events.
         */
        void enableTimerEvents();

        /**
         * Removes this widget from the set of widgets that receive timer events.
         * If the widget is already removed, the call is ignored.
         * By default, a widget does not receive timer events.
         * It is called automatically from the widget's destructor.
         */
        void disableTimerEvents();

    protected:
        /**
         * Invoked to allow a widget to compute its geometry constraints
         * according to the geometry constraints of its children.
         * By default, it does nothing.
         */
        virtual void onCalculateGeometryConstraints() {
        }

        /**
         * Invoked to allow a widget to position its children
         * according to its geometry constraints and the preferred layout algorithm.
         * The default implementation does nothing.
         */
        virtual void onLayout() const {
        }

        /**
         * Invoked to paint the widget.
         * The default implementation does nothing.
         */
        virtual void onPaint() const {
        }

        /**
         * Invoked to paint the widget overlay over its children.
         * The default implementation does nothing.
         */
        virtual void onPaintOverlay() const {
        }

    private:
        //geometry
        Coord m_left;
        Coord m_top;
        Coord m_width;
        Coord m_height;
        float m_scalingX;
        float m_scalingY;

        //geometry constraints
        Coord m_minimumWidth;
        Coord m_minimumHeight;
        Coord m_optimalWidth;
        Coord m_optimalHeight;
        Coord m_maximumWidth;
        Coord m_maximumHeight;

        //screen geometry
        float m_screenLeft;
        float m_screenTop;
        float m_screenRight;
        float m_screenBottom;
        float m_screenScalingX;
        float m_screenScalingY;

        //state
        std::string m_id;
        Widget* m_childWithMouse;
        int m_tabIndex;
        ClippingMode m_clippingMode : 4;
        bool m_visible : 1;
        bool m_screenGeometryDirty : 1;
        bool m_geometryConstraintsDirty : 1;
        bool m_descentantGeometryConstraintsDirty : 1;
        bool m_layoutDirty:1;
        bool m_enabled : 1;
        bool m_highlighted : 1;
        bool m_pressed : 1;
        bool m_selected : 1;
        bool m_focused : 1;
        bool m_validContent : 1;
        bool m_enabledTree : 1;
        bool m_highlightedTree : 1;
        bool m_pressedTree : 1;
        bool m_selectedTree : 1;
        bool m_focusedTree : 1;
        bool m_validContentTree : 1;
        bool m_treeVisualStateDirty : 1;
        bool m_focusable : 1;
        bool m_hasMouse : 1;
        bool m_focusContainer : 1;
        bool m_hasTimer : 1;

        //various internal functions
        void _invalidateScreenGeometry();
        void _invalidateGeometryConstraints();
        void _invalidateParentGeometryConstraints();
        void _updateGeometryConstraints();
        void _invalidateLayout();
        void _invalidateParentLayout();
        void _calcScreenGeometry();
        void _invalidateTreeVisualState();
        void _calcTreeVisualState();
        void _paint(bool calcScreenGeometry, bool calcVisualState);
        bool _canGetFocus() const;
        bool _enabledTree() const;
        void _resetMouseState();
        void _resetChildWithMouseState();
        Widget* _getChildFromCoords(float screenX, float screenY) const;
        Widget* _getEnabledChild(float screenX, float screenY) const;
        Widget* _getClosestFocusContainerAncestor() const;
        Widget* _getDescentantWithLowerTabIndex(int tabIndex) const;
        Widget* _getDescentantWithHigherTabIndex(int tabIndex) const;
        Widget* _getPrevFocusDescentant(Widget* focusedWidget) const;
        Widget* _getNextFocusDescentant(Widget* focusedWidget) const;
        bool _isValidFocusContainer() const;
        Widget* _getPrevInnermostFocusContainer() const;
        Widget* _getNextInnermostFocusContainer() const;
        float _getScreenCenterX() const;
        float _getScreenCenterY() const;

        //joystick events
        bool _joystickButtonEventCapture(const ALLEGRO_EVENT& event, EventType eventType);
        bool _joystickButtonEventBubble(const ALLEGRO_EVENT& event, EventType eventType);
        bool _unusedJoystickButtonEvent(const ALLEGRO_EVENT& event, EventType eventType);
        bool _joystickButtonEvent(const ALLEGRO_EVENT& event, EventType eventType);
        bool _joystickMoveEventCapture(const ALLEGRO_EVENT& event);
        bool _joystickMoveEventBubble(const ALLEGRO_EVENT& event);
        bool _unusedJoystickMoveEvent(const ALLEGRO_EVENT& event);
        bool _moveFocusByJoystick(const ALLEGRO_EVENT& event);
        bool _joystickMoveEvent(const ALLEGRO_EVENT& event);

        //mouse events
        bool _mouseButtonEvent(const ALLEGRO_EVENT& event, EventType eventType);
        bool _mouseMove(const ALLEGRO_EVENT& event, EventType eventType);
        bool _mouseEnter(const ALLEGRO_EVENT& event);
        bool _mouseLeave(const ALLEGRO_EVENT& event);
        bool _mouseWheel(const ALLEGRO_EVENT& event);

        //keyboard events
        bool _keyEventCapture(const ALLEGRO_EVENT& event, EventType eventType);
        bool _keyEventBubble(const ALLEGRO_EVENT& event, EventType eventType);
        bool _unusedKeyEvent(const ALLEGRO_EVENT& event, EventType eventType);
        bool _moveFocusByKey(const ALLEGRO_EVENT& event);
        bool _keyEvent(const ALLEGRO_EVENT& event, EventType eventType);

        //other events
        bool _timerEvent(const ALLEGRO_EVENT& event);
        bool _exposeEvent(const ALLEGRO_EVENT& event);
    };


} //namespace algui


#endif //ALGUI_WIDGET_HPP
