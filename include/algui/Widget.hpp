#ifndef ALGUI_WIDGET_HPP
#define ALGUI_WIDGET_HPP


#include "Tree.hpp"
#include "Coord.hpp"


namespace algui {


    /**
     * Base class for UI elements. 
     */
    class Widget : public Tree<Widget> {
    public:
        /**
         * The default constructor.
         */
        Widget();

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
         * Renders the tree into the target bitmap.
         */
        void render();

    protected:
        /**
         * Invoked to allow a widget to compute its geometry constraints
         * according to the geometry constraints of its children.
         * By default, it does nothing.
         */
        virtual void onUpdateGeometryConstraints() {
        }

        /**
         * Invoked to allow a widget to position its children
         * according to its geometry constraints and the preferred layout algorithm.
         * By default, it does nothing.
         */
        virtual void onLayout() const {
        }

        /**
         * Invoked to paint the widget.
         */
        virtual void onPaint() const = 0;

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
        bool m_visible : 1;
        bool m_screenGeometryDirty : 1;
        bool m_geometryConstraintsDirty : 1;
        bool m_descentantGeometryConstraintsDirty : 1;
        bool m_layoutDirty:1;

        //internal functions
        void _invalidateScreenGeometry();
        void _invalidateGeometryConstraints();
        void _invalidateParentGeometryConstraints();
        void _updateGeometryConstraints();
        void _invalidateLayout();
        void _invalidateParentLayout();
        void _calcScreenGeometry();
        void _paint(bool calcScreenGeometry);
    };


} //namespace algui


#endif //ALGUI_WIDGET_HPP
