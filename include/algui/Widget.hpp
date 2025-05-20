#ifndef ALGUI_WIDGET_HPP
#define ALGUI_WIDGET_HPP


#include <any>
#include "allegro5/allegro.h"


namespace algui {


    /**
     * Base class for widgets.
     */
    class Widget {
    public:
        /** id of widget; mainly used for debugging. */
        const char* id;

        /** parent widget. */
        Widget* parent;

        /** previous sibling widget; drawn before this. */
        Widget* prevSibling;

        /** next sibling widget; drawn after this. */
        Widget* nextSibling;

        /** first child widget; drawn before all other children. */
        Widget* firstChild;

        /** last child widget; drawn after all other children. */
        Widget* lastChild;

        /** left coordinate of the widget relative to its parent. */
        float x;

        /** top coordinate of the widget relative to its parent. */
        float y;

        /** width of widget. */
        float width;

        /** height of widget. */
        float height;

        /**
         * Scaling factor of children along the X axis.
         * By default, it is 1.
         */
        float xScaling;

        /**
         * Scaling factor of children along the Y axis.
         * By default, it is 1.
         */
        float yScaling;

        /** if true, the widget can get the focus, otherwise it does not get the focus. */
        bool focusable : 1;

        /** 
         * If true, then the widget is drawn, otherwise it is not. 
         *  If not drawn, then its children are not drawn as well.
         */
        bool visible : 1;

        /**
         * If true, the widget accepts events.
         * Otherwise, it does not.
         * If it does not accept events, then children also do not accept events.
         */
        bool enabled : 1;

        /**
         * If true, then a widget and its children are drawn in highlighted style.
         */
        bool highlighted : 1;

        /**
         * If true, then a widget and its children are drawn in pressed style.
         */
        bool pressed : 1;

        /**
         * If true, then a widget and its children are drawn in selected style.
         */
        bool selected : 1;

        /** left coordinate of the widget relative to the screen. */
        float screenX1;

        /** top coordinate of the widget relative to the screen. */
        float screenY1;

        /** right coordinate of the widget relative to the screen. */
        float screenX2;

        /** bottom coordinate of the widget relative to the screen. */
        float screenY2;

        /**
         * Screen width of the widget.
         * The screen width of a widget might be different than the set width,
         * if scaling is activated.
         */
        float screenWidth;

        /**
         * Screen height of the widget.
         * The screen height of a widget might be different than the set height,
         * if scaling is activated.
         */
        float screenHeight;

        /**
         * Scaling of children along the X axis,
         * which also includes the scaling of ancestors.
         */
        float screenXScaling;

        /**
         * Scaling of children along the X axis,
         * which also includes the scaling of ancestors.
         */
        float screenYScaling;

        /**
         * Set to true if this and all its ancestors are enabled.
         * It is provided for drawing only purposes.
         */
        bool screenEnabled : 1;

        /**
         * Set to true if this or one of its ancestors is highlighted.
         * It is provided for drawing only purposes.
         */
        bool screenHighlighted : 1;

        /**
         * Set to true if this or one of its ancestors is pressed.
         * It is provided for drawing only purposes.
         */
        bool screenPressed : 1;

        /**
         * Set to true if this or one of its ancestors is selected.
         * It is provided for drawing only purposes.
         */
        bool screenSelected : 1;

        /** If the widget has the mouse flag. */
        bool hasMouse : 1;

        /**
         * The widget that currently has the focus.
         * If that widget is destroyed, then this is set to null.
         */
        static Widget* focusWidget;

        /**
         * Global flag that, if true, a widget that gets the mouse also gets the focus,
         * and a widget that loses the mouse also loses the focus.
         */
        static bool focusFollowsMouse;

        /**
         * Global flag that, if true, mouse events are translated to drag and drop events.
         * By default, it is false.
         */
        static bool dragAndDrop;

        /**
         * Global variable for the dragged content.
         * A widget that wishes to have its content dragged, must set this variable,
         * so as that other widgets read this and respond accordingly to drag events.
         */
        static std::any draggedContent;

        /**
         * The default constructor.
         */
        Widget();

        /**
         * The copy constructor.
         * The only properties copied are: id, x, y, width, height, focusable, visible, enabled, xScaling, yScaling.
         * @param src the source widget.
         */
        Widget(const Widget& src);

        /**
         * Deleted because of pointers to other widgets in the tree.
         */
        Widget(Widget&& src) = delete;

        /**
         * The destructor.
         * If it has focus, then the focus widget pointer is set to null.
         * It deletes its children widgets.
         */
        virtual ~Widget();

        /**
         * The copy assignment operator.
         * The only properties copied are: x, y, width, height, focusable, visible, enabled, xScaling, yScaling.
         * @param src the source widget.
         * @return reference to this.
         */
        Widget& operator = (const Widget& src);

        /**
         * Deleted because of pointers to other widgets in the tree.
         */
        Widget& operator = (Widget&& src) = delete;

        /**
         * Returns the root widget.
         * The root widget is the ancestor widget that does not have any parent.
         * @return the root widget.
         */
        Widget* getRoot() const;

        /**
         * Adds a child widget.
         * The child is added at the top of its siblings.
         * @param child child widget to add.
         */
        virtual void addChild(Widget* child);

        /**
         * Removes a child widget.
         * @param child child widget to remove.
         */
        virtual void removeChild(Widget* child);

        /**
         * Removes this from its parent, if there is one.
         */
        void detach() {
            if (parent) {
                parent->removeChild(this);
            }
        }

        /**
         * Draws this widget and its children.
         * It computes the screen properties of the widget.
         * It invokes onPaint, then draws its children, then it invokes onPaintOverlay,
         * allowing widgets to draw overlays above their children.
         * No screen clipping is performed.
         */
        void draw();

        /**
         * Checks if the widget has the focus.
         * @return true if it has the focus, false otherwise.
         */
        bool hasFocus() const { return focusWidget == this; }

        /**
         * Gives the focus to this widget.
         */
        void focus();

        /**
         * Removes the focus from this widget.
         */
        void blur();
        
        /**
         * Checks if the given screen coordinates lie inside the widget.
         * @param screenX screen X coordinate.
         * @param screenY screen Y coordinate.
         */
        virtual bool intersects(float screenX, float screenY) const {
            return screenX >= this->screenX1 && screenY >= this->screenY1 && screenX < this->screenX2 && screenY < this->screenY2;
        }

        /**
         * Returns the child widget from given screen coordinates.
         * The children are searched in reverse drawing order.
         * A child must be visible in order to be returned.
         * @return the child at the given coordinates.
         */
        Widget* getChild(float screenX, float screenY) const;

        /**
         * Dispatches an allegro event to this widget and its descentants.
         * Events dispatched are joystick, keyboard, mouse and timer.
         * @param event allegro event to dispatch.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool dispatch(const ALLEGRO_EVENT& event);

        /**
         * Invoked just before a widget's children are drawn,
         * in order to allow a widget to arrange its children to a specific layout.
         */
        virtual void onLayout() const {}

        /**
         * Invoked to paint this widget.
         * The screen properties can be used to paint it.
         */
        virtual void onPaint() const = 0;

        /**
         * Invoked to paint this widget's overlay over its children.
         */
        virtual void onPaintOverlay() const {}

        /**
         * Invoked after a widget gets the focus.
         */
        virtual void onGotFocus() {}

        /**
         * Invoked after a widget loses the focus.
         */
        virtual void onLostFocus() {}

        /**
         * Invoked when a descentant widget receives the focus.
         * It can be used to make windows for example be activated,
         * when a descentant gets the focus.
         */
        virtual void onDescentantGotFocus() {}

        /**
         * Invoked when a descentant widget loses the focus.
         */
        virtual void onDescentantLostFocus() {}

        /**
         * Invoked when the mouse cursor enters the widget area.
         * By default, it dispatches events to children.
         * @param event the allegro event to process.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool onMouseEnter(const ALLEGRO_EVENT& event);

        /**
         * Invoked when the mouse cursor moves over the widget area, including its children.
         * By default, it dispatches either a mouse move event to one of its children,
         * or mouse enter/leave events if the mouse moved over to another child.
         * @param event the allegro event to process.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool onMouseMove(const ALLEGRO_EVENT& event);

        /**
         * Invoked when the mouse cursor leaves the widget area.
         * By default, it dispatches events to children.
         * @param event the allegro event to process.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool onMouseLeave(const ALLEGRO_EVENT& event);

        /**
         * Invoked when a mouse button is pressed over the widget.
         * By default, it dispatches events to children.
         * @param event the allegro event to process.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool onMouseButtonDown(const ALLEGRO_EVENT& event);
        
        /**
         * Invoked when a mouse button is released over the widget.
         * The widget for release might be different than the widget for press,
         * depending on mouse position.
         * By default, it dispatches events to children.
         * @param event the allegro event to process.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool onMouseButtonUp(const ALLEGRO_EVENT& event);

        /**
         * Invoked for an allegro timer event.
         * It can be used to implement UI animations.
         * By default, it is dispatched to all enabled widgets of the tree.
         * @param event the allegro event to process.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool onTimer(const ALLEGRO_EVENT& event);

        /**
         * Invoked when the mouse wheel is rolled and the widget has the input focus.
         * @param event the allegro event to process.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool onMouseWheel(const ALLEGRO_EVENT& event) { return false; }

        /**
         * Invoked when a key is pressed and the widget has the input focus.
         * @param event the allegro event to process.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool onKeyDown(const ALLEGRO_EVENT& event) { return false; }

        /**
         * Invoked when a key is released and the widget has the input focus.
         * @param event the allegro event to process.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool onKeyUp(const ALLEGRO_EVENT& event) { return false; }

        /**
         * Invoked when a a character is available from a keypress and the widget has the input focus.
         * @param event the allegro event to process.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool onKeyChar(const ALLEGRO_EVENT& event) { return false; }

        /**
         * Invoked when a joystick axis value changed and the widget has the input focus.
         * @param event the allegro event to process.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool onJoystickAxis(const ALLEGRO_EVENT& event) { return false; }

        /**
         * Invoked when a joystick button is pressed and the widget has the input focus.
         * @param event the allegro event to process.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool onJoystickButtonDown(const ALLEGRO_EVENT& event) { return false; }

        /**
         * Invoked when a joystick button is released and the widget has the input focus.
         * @param event the allegro event to process.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool onJoystickButtonUp(const ALLEGRO_EVENT& event) { return false; }

        /**
         * Invoked when the mouse cursor enters the widget area, while drag and drop is active.
         * By default, it dispatches events to children.
         * @param event the allegro event to process.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool onDragEnter(const ALLEGRO_EVENT& event);

        /**
         * Invoked when the mouse cursor moves over the widget area, including its children, while drag and drop is active.
         * By default, it dispatches either a mouse move event to one of its children,
         * or mouse enter/leave events if the mouse moved over to another child.
         * @param event the allegro event to process.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool onDrag(const ALLEGRO_EVENT& event);

        /**
         * Invoked when the mouse cursor leaves the widget area, while drag and drop is active.
         * By default, it dispatches events to children.
         * @param event the allegro event to process.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool onDragLeave(const ALLEGRO_EVENT& event);

        /**
         * Invoked when a mouse button is released over the widget, while drag and drop is active.
         * The widget for release might be different than the widget for press,
         * depending on mouse position.
         * By default, it dispatches events to children.
         * @param event the allegro event to process.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool onDrop(const ALLEGRO_EVENT& event);
    };


} //namespace algui


#endif //ALGUI_WIDGET_HPP
