#ifndef ALGUI_WIDGET_HPP
#define ALGUI_WIDGET_HPP


#include <list>
#include <any>
#include <string>
#include <memory>
#include "Theme.hpp"


namespace algui {


    /**
     * Base class for widgets.
     */
    class Widget {
    public:
        /**
         * The default constructor.
         */
        Widget();

        Widget(const Widget&) = delete;
        Widget(Widget&&) = delete;

        /**
         * The destructor.
         * It removes the widget from its parent.
         */
        virtual ~Widget();

        Widget& operator =(const Widget&) = delete;
        Widget& operator =(Widget&&) = delete;

        /**
         * Returns the id of the widget.
         * @return the id of the widget.
         */
        const std::string& getId() const;

        /**
         * Returns the parent widget.
         * @return the parent widget.
         */
        Widget* getParent() const;

        /**
         * Returns the previous sibling widget.
         * @return the previous sibling widget.
         */
        Widget* getPrevSibling() const;

        /**
         * Returns the next sibling widget.
         * @return the next sibling widget.
         */
        Widget* getNextSibling() const;

        /**
         * Returns the first child widget.
         * @return the first child widget.
         */
        Widget* getFirstChild() const;

        /**
         * Returns the last child widget.
         * @return the last child widget.
         */
        Widget* getLastChild() const;

        /**
         * Returns the root widget of this tree.
         * @return the root widget of this tree.
         */
        Widget* getRoot() const;

        /**
         * Returns the left coordinate of the widget, relative to its parent or to the screen.
         * @return the left coordinate of the widget.
         */
        float getX() const;

        /**
         * Returns the top coordinate of the widget, relative to its parent or to the screen.
         * @return the top coordinate of the widget.
         */
        float getY() const;

        /**
         * Returns the width of the widget.
         * @return the width of the widget.
         */
        float getWidth() const;

        /**
         * Returns the height of the widget.
         * @return the height of the widget.
         */
        float getHeight() const;

        /**
         * Returns the horizontal scaling factor.
         * @return the horizontal scaling factor.
         */
        float getHorizontalScaling() const;

        /**
         * Returns the vertical scaling factor.
         * @return the vertical scaling factor.
         */
        float getVerticalScaling() const;

        /**
         * Returns the horizontal scaling factor for this and all ancestor widgets.
         * @return the horizontal scaling factor for this and all ancestor widgets.
         */
        float getTreeHorizontalScaling() const;

        /**
         * Returns the vertical scaling factor for this and all ancestor widgets.
         * @return the vertical scaling factor for this and all ancestor widgets.
         */
        float getTreeVerticalScaling() const;

        /**
         * Returns the left coordinate of the widget, relative to the screen.
         * @return the left coordinate of the widget.
         */
        float getX1() const;

        /**
         * Returns the top coordinate of the widget, relative to the screen.
         * @return the top coordinate of the widget.
         */
        float getY1() const;

        /**
         * Returns the right coordinate of the widget, relative to the screen.
         * @return the right coordinate of the widget.
         */
        float getX2() const;

        /**
         * Returns the bottom coordinate of the widget, relative to the screen.
         * @return the bottom coordinate of the widget.
         */
        float getY2() const;

        /**
         * Returns the visible state of the widget.
         * @return the visible state of the widget.
         */
        bool getVisible() const;

        /**
         * Returns the enabled state of the widget.
         * @return the enabled state of the widget.
         */
        bool getEnabled() const;

        /**
         * Returns the highlighted state of the widget.
         * @return the highlighted state of the widget.
         */
        bool getHighlighted() const;

        /**
         * Returns the pressed state of the widget.
         * @return the pressed state of the widget.
         */
        bool getPressed() const;

        /**
         * Returns the selected state of the widget.
         * @return the selected state of the widget.
         */
        bool getSelected() const;

        /**
         * Returns the focused state of the widget.
         * @return the focused state of the widget.
         */
        bool getFocused() const;

        /**
         * Returns the error state of the widget.
         * @return the error state of the widget.
         */
        bool getError() const;

        /**
         * Returns true if this and all its ancestors are visible, false otherwise.
         * @return true if this and all its ancestors are visible, false otherwise.
         */
        bool getTreeVisible() const;

        /**
         * Returns true if this and all its ancestors are enabled, false otherwise.
         * @return true if this and all its ancestors are enabled, false otherwise.
         */
        bool getTreeEnabled() const;

        /**
         * Returns true if this or one if its ancestors are in highlighted state, false otherwise.
         * @return true if this or one if its ancestors are in highlighted state, false otherwise.
         */
        bool getTreeHighlighted() const;

        /**
         * Returns true if this or one if its ancestors are in pressed state, false otherwise.
         * @return true if this or one if its ancestors are in pressed state, false otherwise.
         */
        bool getTreePressed() const;

        /**
         * Returns true if this or one if its ancestors are in selected state, false otherwise.
         * @return true if this or one if its ancestors are in selected state, false otherwise.
         */
        bool getTreeSelected() const;

        /**
         * Returns true if this or one if its ancestors have the input focus, false otherwise.
         * @return true if this or one if its ancestors have the input focus, false otherwise.
         */
        bool getTreeFocused() const;

        /**
         * Returns true if this or one if its ancestors are in error state, false otherwise.
         * @return true if this or one if its ancestors are in error state, false otherwise.
         */
        bool getTreeError() const;

        /**
         * Returns true if this widget participates in layout management of the parent, false otherwise.
         * @return true if this widget participates in layout management of the parent, false otherwise.
         */
        bool getManaged() const;

        /**
         * Returns true if this widget allows resizing it while doing layout management, false otherwise.
         * @return true if this widget allows resizing it while doing layout management, false otherwise.
         */
        bool getFlexible() const;

        /**
         * Returns the tree depth of this widget.
         * @return the tree depth of this widget; 0 if it is root, 1 if it is a child of root etc.
         */
        size_t getDepth() const;

        /**
         * Returns the theme of the widget.
         * @return the theme of the widget.
         */
        const std::shared_ptr<Theme>& getTheme() const;

        /**
         * Returns the current palette name.
         * @return the current palette name.
         */
        const std::string& getPaletteName() const;

        /**
         * Checks if the given widget is within this widget tree.
         * @param wgt widget to check if within this tree.
         * @return true if the given widget is within this tree, false otherwise.
         */
        bool contains(const Widget* wgt) const;

        /**
         * Returns the widget under the given screen coordinates.
         * The widget's `intersects(x, y)` function is used to test if coordinates lie within the widget.
         * @param x horizontal coordinate.
         * @param y vertical coordinate.
         * @return pointer to child under the given coordinates, null otherwise.
         */
        Widget* get(float x, float y) const;

        /**
         * Adds a widget as a child of this.
         * The child widget gets the theme of the parent, if the child does not have a theme attached to it.
         * @param child child to add.
         * @param nextSibling next sibling widget; if null, then the child is added on top of its siblings.
         * @return true if addition succeeded, false if the child pointer is null, or if the child already
         *  has a parent, or the child is ancestor of this, or the next sibling is not a child of this.
         */
        bool add(Widget* child, Widget* nextSibling = nullptr);

        /**
         * Removes a child widget.
         * @param child child to remove.
         * @return true if the removal succeeded, false if the given widget is not a child of this.
         */
        bool remove(Widget* child);

        /**
         * Removes this widget from its parent, if there is a parent.
         */
        void detach();

        /**
         * Removes all children.
         */
        void removeAll();

        /**
         * Sets the id of the widget.
         * @param id id of the widget.
         */
        void setId(const std::string& id);

        /**
         * Sets the left coordinate of the widget.
         * If this widget is in managed state, then the parent's layout is invalidated.
         * @param x the left coordinate of the widget, relative to its parent,
         *  or to the screen if there is no parent.
         */
        void setX(float x);

        /**
         * Sets the top coordinate of the widget.
         * If this widget is in managed state, then the parent's layout is invalidated.
         * @param y the top coordinate of the widget, relative to its parent,
         *  or to the screen if there is no parent.
         */
        void setY(float y);

        /**
         * Sets the width of the widget.
         * If this widget is in managed state, then the parent's layout is invalidated.
         * The widget's layout is also invalidated.
         * @param w the widget's width; clamped to 0.
         */
        void setWidth(float w);

        /**
         * Sets the height of the widget.
         * If this widget is in managed state, then the parent's layout is invalidated.
         * The widget's layout is also invalidated.
         * @param h the widget's height; clamped to 0.
         */
        void setHeight(float h);

        /**
         * Sets the left and top coordinate of the widget.
         * If this widget is in managed state, then the parent's layout is invalidated.
         * @param x the left coordinate of the widget, relative to its parent,
         *  or to the screen if there is no parent.
         * @param y the top coordinate of the widget, relative to its parent,
         *  or to the screen if there is no parent.
         */
        void setPosition(float x, float y);

        /**
         * Sets the width and height of the widget.
         * If this widget is in managed state, then the parent's layout is invalidated.
         * The widget's layout is also invalidated.
         * @param width the widget's width; clamped to 0.
         * @param height the widget's height; clamped to 0.
         */
        void setSize(float width, float height);

        /**
         * Sets the position and size of the widget.
         * If this widget is in managed state, then the parent's layout is invalidated.
         * The widget's layout is also invalidated, if its size changes.
         * @param x the left coordinate of the widget, relative to its parent,
         *  or to the screen if there is no parent.
         * @param y the top coordinate of the widget, relative to its parent,
         *  or to the screen if there is no parent.
         * @param width the widget's width; clamped to 0.
         * @param height the widget's height; clamped to 0.
         */
        void setGeometry(float x, float y, float width, float height);

        /**
         * Sets the horizontal scaling factor.
         * @param f the horizontal scaling factor.
         */
        void setHorizontalScaling(float f);

        /**
         * Sets the vertical scaling factor.
         * @param f the vertical scaling factor.
         */
        void setVerticalScaling(float f);

        /**
         * Sets the scaling factors.
         * @param h the horizontal scaling factor.
         * @param v the vertical scaling factor.
         */
        void setScaling(float h, float v);

        /**
         * Sets the visible state of a widget.
         * An invisible widget is not drawn, and its children are not drawn as well.
         * By default, a widget is visible.
         * @param v the new visible state.
         */
        void setVisible(bool v);

        /**
         * Sets the enabled state of a widget.
         * A disabled widget and its children do not accept events.
         * By default, a widget is enabled .
         * @param v the new enabled state.
         */
        void setEnabled(bool v);

        /**
         * Sets the highlighted state of a widget.
         * @param v the new state.
         */
        void setHighlighted(bool v);

        /**
         * Sets the pressed state of a widget.
         * @param v the new state.
         */
        void setPressed(bool v);

        /**
         * Sets the selected state of a widget.
         * @param v the new state.
         */
        void setSelected(bool v);

        /**
         * Sets the focused state of a widget.
         * @param v the new state.
         */
        void setFocused(bool v);

        /**
         * Returns the widget that currently has the input focus.
         * @return the widget that currently has the input focus.
         */
        static Widget* getFocusedWidget();

        /**
         * Sets the error state of a widget.
         * @param v the new state.
         */
        void setError(bool v);

        /**
         * Sets the managed state of a widget.
         * @param v the new state.
         */
        void setManaged(bool v);

        /**
         * Sets the flexible state of a widget.
         * @param v the new state.
         */
        void setFlexible(bool v);

        /**
         * Invalidates the widget's layout.
         * The new layout will be computed before the next render.
         */
        virtual void invalidateLayout();

        /**
         * Invalidates the parent's layout, if there is one.
         */
        void invalidateParentLayout() const;

        /**
         * Calculates a widget's layout and screen coordinates,
         * then invokes the `paint()` function to allow the widget to paint itself with ALLEGRO,
         * then invokes the render method on its children.
         */
        void render();

        /**
         * Begins drag-n-drop.
         * @param data the dragged data.
         * @return true if drag-n-drop started, false if there is another drag-n-drop in progress,
         *  or there is no mouse button pressed, or the dragged data are empty, or if the mouse
         *  has not moved far enough from the last mouse button down position.
         */
        bool beginDragAndDrop(const std::any& data);

        /**
         * Sets the drag icon.
         * Valid only while a drag-n-drop is in progress.
         * The drag icon is automatically reset on drop.
         * @param bitmap bitmap to show below the mouse cursor; it can be null.
         * @param hotPointX mouse cursor horizontal position relative to left side of the bitmap.
         * @param hotPointY mouse cursor vertical position relative to top side of the bitmap.
         */
        static void setDragIcon(ALLEGRO_BITMAP* bitmap, int hotPointX = 0, int hotPointY = 0);

        /**
         * Returns the click timeout, in milliseconds.
         * @return the click timeout, in milliseconds.
         */
        static size_t getClickTimeout();

        /**
         * Sets the click timeout.
         * @param milliseconds click timeout, in milliseconds.
         */
        static void setClickTimeout(size_t milliseconds);

        /**
         * Sets the widget's theme.
         * It invokes the `themed()` virtual method for each widget in the tree that has a theme attached to it,
         * or the 'unthemed()' method if the widget does not have a theme.
         * @param theme pointer to theme; it can be null, in order to detach the theme from the widget tree.
         */
        void setTheme(const std::shared_ptr<Theme>& theme);

        /**
         * Resets the theme pointer for each widget in the tree.
         * It invokes the 'unthemed()' method.
         */
        void resetTheme();

        /**
         * Refreshes the widget's resources from its theme.
         * It invokes the `themed()` virtual method for each widget in the tree that has a theme attached to it,
         * or the 'unthemed()' method if the widget does not have a theme.
         */
        void refreshTheme();

        /**
         * Sets the palette name for this widget and its children.
         * Widgets may use different palettes, depending on usage.
         * By default, the palette name is empty, meaning 'default palette'.
         * If the palette name changes, then the method `themed()` is called if the widget has a theme,
         * or the 'unthemed()' method is called if the widget does not have a theme.
         * to allow the widget to get its resources from the theme, according to the new palette.
         * @param paletteName the new palette name.
         */
        void setPaletteName(const std::string& paletteName);

        /**
         * Processes an allegro event and invokes the relevant virtual methods of this widget.
         * @param event allegro event to process.
         * @return true if the event was successfully processed, false otherwise.
         */
        virtual bool doEvent(const ALLEGRO_EVENT& event);

    protected:
        /**
         * Invoked to position the children to appropriate places.
         * By default, it does nothing.
         */
        virtual void layout() const {}

        /**
         * Invoked to paint the widget.
         * By default, it does nothing.
         */
        virtual void paint() const = 0;

        /**
         * Invoked when the widget gets the focus.
         * By default, it does nothing.
         */
        virtual void gotFocus() {}

        /**
         * Invoked when the widget lsoes the focus.
         * By default, it does nothing.
         */
        virtual void lostFocus() {}

        /**
         * Invoked to check if a point falls within a widget.
         * By default, it checks the given coordinates against the screen rectangle of the widget.
         * Subclasses may provide customizations in order to provide specific shapes.
         * @param x horizontal coordinate relative to the screen.
         * @param y vertical coordinate relative to the screen.
         * @return true if the given coordinates fall within the widget, false otherwise.
         */
        virtual bool intersects(float x, float y) const;

        /**
         * Invoked on ALLEGRO_EVENT_MOUSE_BUTTON_DOWN.
         * The default implementation passes the event to the appropriate child.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool mouseButtonDown(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_MOUSE_BUTTON_UP.
         * The default implementation passes the event to the appropriate child.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool mouseButtonUp(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_MOUSE_AXES, ALLEGRO_EVENT_MOUSE_WARPED, ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY.
         * The default implementation passes the event to the appropriate child.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool mouseEnter(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_MOUSE_AXES, ALLEGRO_EVENT_MOUSE_WARPED, ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY.
         * The default implementation passes the event to the appropriate child, creating enter and leave events if needed.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool mouseMove(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_MOUSE_AXES, ALLEGRO_EVENT_MOUSE_WARPED, ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY.
         * The default implementation passes the event to the appropriate child.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool mouseLeave(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_MOUSE_AXES.
         * The default implementation passes the event to the appropriate child.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool mouseWheel(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_KEY_DOWN.
         * The default implementation passes the event to root, as an unused key event.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool keyDown(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_KEY_UP.
         * The default implementation passes the event to root, as an unused key event.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool keyUp(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_KEY_CHAR.
         * The default implementation passes the event to root, as an unused key event.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool keyChar(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_KEY_DOWN, when a key event is not processed by the widget that has the focus.
         * The default implementation passes the event to children.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool unusedKeyDown(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_KEY_UP, when a key event is not processed by the widget that has the focus.
         * The default implementation passes the event to children.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool unusedKeyUp(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_KEY_CHAR, when a key event is not processed by the widget that has the focus.
         * The default implementation passes the event to children.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool unusedKeyChar(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_MOUSE_BUTTON_UP, while in drag-n-drop.
         * After this event, the drag-n-drop session ends automatically,
         * and a new one can be stated with the function `beginDragAndDrop()`.
         * The default implementation passes the event to children.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool drop(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_MOUSE_AXES, ALLEGRO_EVENT_MOUSE_WARPED, ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY, while in drag-n-drop.
         * The default implementation passes the event to the appropriate child.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool dragEnter(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_MOUSE_AXES, ALLEGRO_EVENT_MOUSE_WARPED, ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY, while in drag-n-drop.
         * The default implementation passes the event to the appropriate child, creating dragenter and drag leave events if needed.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool drag(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_MOUSE_AXES, ALLEGRO_EVENT_MOUSE_WARPED, ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY, while in drag-n-drop.
         * The default implementation passes the event to the appropriate child.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool dragLeave(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_MOUSE_AXES, while in drag-n-drop.
         * The default implementation passes the event to the appropriate child.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool dragWheel(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_KEY_DOWN, on the widgets that are under the mouse.
         * The default implementation passes the event to the appropriate child.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool dragKeyDown(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_KEY_UP, on the widgets that are under the mouse.
         * The default implementation passes the event to the appropriate child.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool dragKeyUp(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_KEY_CHAR, on the widgets that are under the mouse.
         * The default implementation passes the event to the appropriate child.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool dragKeyChar(const ALLEGRO_EVENT& event);

        /**
         * Invoked on ALLEGRO_EVENT_TIMER.
         * The default implementation passes the event to all children.
         * @param event the allegro event.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool timer(const ALLEGRO_EVENT& event);

        /**
         * Invoked when the user clicks the widget.
         * The default implementation passes the event to the appropriate child.
         * For this to work, a valid ALLEGRO_EVENT_TIMER must be passed to `doEvent()`.
         * @param event the allegro mouse event that caused the click.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool click(const ALLEGRO_EVENT& event);

        /**
         * Invoked when the user double-clicks the widget.
         * The default implementation passes the event to the appropriate child.
         * For this to work, a valid ALLEGRO_EVENT_TIMER must be passed to `doEvent()`.
         * @param event the allegro mouse event that caused the double-click.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool doubleClick(const ALLEGRO_EVENT& event);

        /**
         * Invoked when the theme changes or it is refreshed and there is a theme.
         * Subclasses can use this call to customize their resources.
         * The default implementation does nothing.
         */
        virtual void themed() {}

        /**
         * Invoked when the theme changes or it is refreshed and there is no theme.
         * Subclasses can use this call to customize their resources based on the lack of a theme.
         * The default implementation does nothing.
         */
        virtual void unthemed() {}

        /**
         * Invoked when the tree scaling of the widget changes.
         * Subclasses can use this call to load the appropriate resources based on the tree scaling of the widget.
         * The default implementation does nothing.
         */
        virtual void scaled() {}

    private:
        std::string m_id;
        Widget* m_parent;
        std::list<Widget*>::iterator m_it;
        std::list<Widget*> m_children;
        std::shared_ptr<Theme> m_theme;
        std::string m_paletteName;
        float m_x;
        float m_y;
        float m_width;
        float m_height;
        float m_horizontalScaling;
        float m_verticalScaling;
        float m_x1;
        float m_y1;
        float m_x2;
        float m_y2;
        float m_treeHorizontalScaling;
        float m_treeVerticalScaling;
        bool m_visible:1;
        bool m_enabled:1;
        bool m_highlighted:1;
        bool m_pressed:1;
        bool m_selected:1;
        bool m_focused:1;
        bool m_error:1;
        bool m_treeVisible:1;
        bool m_treeEnabled:1;
        bool m_treeHighlighted:1;
        bool m_treePressed:1;
        bool m_treeSelected:1;
        bool m_treeFocused:1;
        bool m_treeError:1;
        bool m_layout:1;
        bool m_doingLayout:1;
        bool m_managed:1;
        bool m_flexible:1;

        void _render();
        void _callThemed();
        void _initThemeAndPaletteName(const std::shared_ptr<Theme>& theme, const std::string& paletteName);
    };


} //namespace algui


#endif //ALGUI_WIDGET_HPP
