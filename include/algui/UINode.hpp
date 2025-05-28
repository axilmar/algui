#ifndef ALGUI_UINODE_HPP
#define ALGUI_UINODE_HPP


#include "TreeNode.hpp"


namespace algui {


    /**
     * Base class for UI nodes.
     * 
     * It handles calculation of screen position and rendering of the UI tree.
     * 
     * Coordinates are measured in pixels, but each UI node contains separate scaling factors per axis,
     * and therefore the UI can be made to have the same proportions in various resolutions.
     */
    class UINode : public TreeNode<UINode> {
    public:
        /**
         * Removes the given child.
         * Invokes the `onResetState()` method for every node of the tree that starts with the removed child.
         * @param child child to remove.
         */
        void removeChild(const std::shared_ptr<UINode>& child) override;

            /**
         * Returns the x (left) coordinate of the node, relative to its parent, or to the screen.
         * @return the x coordinate of the node.
         */
        float getX() const {
            return m_x;
        }

        /**
         * Sets the x (left) coordinate of the node, relative to its parent, or to the screen.
         * @param v the new value.
         */
        virtual void setX(float v) {
            m_x = v;
        }

        /**
         * Returns the y (top) coordinate of the node, relative to its parent, or to the screen.
         * @return the y coordinate of the node.
         */
        float getY() const {
            return m_y;
        }

        /**
         * Sets the y (top) coordinate of the node, relative to its parent, or to the screen.
         * @param v the new value.
         */
        virtual void setY(float v) {
            m_y = v;
        }

        /**
         * Returns the width of the ui node.
         * @return the width of the ui node.
         */
        float getWidth() const {
            return m_width;
        }

        /**
         * Sets the width of the ui node.
         * @param v the new value.
         */
        virtual void setWidth(float v) {
            m_width = v >= 0 ? v : 0;
        }

        /**
         * Returns the height of the ui node.
         * @return the height of the ui node.
         */
        float getHeight() const {
            return m_height;
        }

        /**
         * Sets the height of the ui node.
         * @param v the new value.
         */
        virtual void setHeight(float v) {
            m_height = v >= 0 ? v : 0;
        }

        /**
         * Returns the scaling factor for the X axis.
         * @return the scaling factor for the X axis.
         */
        float getScalingX() const {
            return m_scalingX;
        }

        /**
         * Sets the scaling factor for the X axis.
         * @param v the new value.
         */
        virtual void setScalingX(float v) {
            m_scalingX = v;
        }

        /**
         * Returns the scaling factor for the Y axis.
         * @return the scaling factor for the Y axis.
         */
        float getScalingY() const {
            return m_scalingY;
        }

        /**
         * Sets the scaling factor for the Y axis.
         * @param v the new value.
         */
        virtual void setScalingY(float v) {
            m_scalingY = v;
        }

        /**
         * Returns true if the object is visible.
         * @return true if the object is visible.
         */
        bool isVisible() const {
            return m_visible;
        }

        /**
         * Sets the visible state of the object.
         * @param v the new visible state.
         */
        virtual void setVisible(bool v) {
            m_visible = v;
        }

        /**
         * Returns the left coordinate of the object, relative to the screen.
         * It can be used to paint an object using Allegro functions.
         * It is valid only after the object has been rendered at least once.
         * @return the left coordinate of the object, relative to the screen.
         */
        float getScreenX1() const {
            return m_screenX1;
        }

        /**
         * Returns the top coordinate of the object, relative to the screen.
         * It can be used to paint an object using Allegro functions.
         * It is valid only after the object has been rendered at least once.
         * @return the top coordinate of the object, relative to the screen.
         */
        float getScreenY1() const {
            return m_screenY1;
        }

        /**
         * Returns the right coordinate of the object, relative to the screen.
         * It can be used to paint an object using Allegro functions.
         * It is valid only after the object has been rendered at least once.
         * @return the right coordinate of the object, relative to the screen.
         */
        float getScreenX2() const {
            return m_screenX2;
        }

        /**
         * Returns the bottom coordinate of the object, relative to the screen.
         * It can be used to paint an object using Allegro functions.
         * It is valid only after the object has been rendered at least once.
         * @return the bottom coordinate of the object, relative to the screen.
         */
        float getScreenY2() const {
            return m_screenY2;
        }

        /**
         * Returns the final scaling factor for the X axis,
         * as computed by multiplying all the scaling factors of this node and up to the root node.
         * It can be used to paint an object using Allegro functions,
         * for example to select the appropriate font size, in order to draw text scaled in the appropriate size.
         * @return the final scaling factor for the X axis.
         */
        float getScreenScalingX() const {
            return m_screenScalingX;
        }

        /**
         * Returns the final scaling factor for the Y axis,
         * as computed by multiplying all the scaling factors of this node and up to the root node.
         * It can be used to paint an object using Allegro functions,
         * for example to select the appropriate font size, in order to draw text scaled in the appropriate size.
         * @return the final scaling factor for the Y axis.
         */
        float getScreenScalingY() const {
            return m_screenScalingY;
        }

        /**
         * Renders this node and its children, recursively.
         * If the node is not visible, it does nothing.
         * It performs the following tasks:
         *      - it invokes the method `onCalcChildState(parent)` to allow a UI node to compute its state based on its parent,
         *        or it invokes the medoth `onCalcRootState()` to allow a UI node to compute its state without a parent.
         *      - it invokes the `onLayout()` method to allow the node to position its children according to some layout algorithm.
         *      - it invokes the `onPaint()` method to allow the node to paint itself.
         *      - it renders the children of this.
         * Nodes are rendered from first to last child, and only if they are visible.
         */
        void renderTree();

        /**
         * Interface for checking if the underlying node is a visual state node.
         * @return true if this node is a visual state node, false otherwise.
         */
        virtual bool isVisualStateNode() const {
            return false;
        }

        /**
         * Interface for checking if the underlying node is an interactive node.
         * @return true if this node is an interactive node, false otherwise.
         */
        virtual bool isInteractiveNode() const {
            return false;
        }

    protected:
        /**
         * Invoked from `renderTree()` to allow a UI node to compute its state, based on the state of parent.
         * The default implementation computes the screen coordinates and screen scaling of the node, 
         * based on the parent's screen coordinates and screen scaling.
         * @param parent parent node.
         */
        virtual void onCalcChildState(UINode* parent);

        /**
         * Invoked from `renderTree()` to allow a UI node to compute its state when it has no parent.
         * The default implementation computes the screen coordinates and screen scaling of the node.
         * @param parent parent node.
         */
        virtual void onCalcRootState();

        /**
         * Invoked from removeChild, on removed children and their descentants,
         * in order to reset state after removal.
         */
        virtual void onResetState() {
        }

        /**
         * Invoked from `renderTree()` to allow a node to layout its children,
         * according to some layout algorihm.
         * The default implementation does nothing.
         */
        virtual void onLayout() const {
        }

        /**
         * Invoked from `renderTree()` to allow a node to paint itself on the screen.
         * The default implementation does nothing.
         */
        virtual void onPaint() const {
        }

    private:
        float m_x{ 0 };
        float m_y{ 0 };
        float m_width{ 0 };
        float m_height{ 0 };
        float m_scalingX{ 1 };
        float m_scalingY{ 1 };
        bool m_visible{ true };
        float m_screenX1{ 0 };
        float m_screenY1{ 0 };
        float m_screenX2{ 0 };
        float m_screenY2{ 0 };
        float m_screenScalingX{ 1 };
        float m_screenScalingY{ 1 };

        void resetState();
    };


} //namespace algui


#endif //ALGUI_UINODE_HPP
