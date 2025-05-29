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
         * Adds a child node.
         * If this is rendered, then the child tree is set to be rendered.
         * @param child child node to add.
         * @param nextSibling the next sibling; if null, the child is added after the last child.
         * @exception std::invalid_argument thrown if child is null or belongs to another tree,
         *  or if nextSibling is not null and not a child of this.
         */
        void addChild(const std::shared_ptr<UINode>& child, const std::shared_ptr<UINode>& nextSibling = nullptr) override;

        /**
         * Removes a child node.
         * The child is set to unrendered state.
         * @param child child node to remove; the method `child->onRemoved()` is invoked.
         * @exception std::invalid_argument thrown if child is null or if not a child of this node.
         */
        void removeChild(std::shared_ptr<UINode> child) override;

        /**
         * Returns the left coordinate of the node, relative to its parent, or to the screen.
         * @return the left coordinate of the node.
         */
        float getLeft() const {
            return m_left;
        }

        /**
         * Sets the left coordinate of the node, relative to its parent, or to the screen.
         * @param v the new value.
         */
        virtual void setLeft(float v);

        /**
         * Returns the top coordinate of the node, relative to its parent, or to the screen.
         * @return the top coordinate of the node.
         */
        float getTop() const {
            return m_top;
        }

        /**
         * Sets the top coordinate of the node, relative to its parent, or to the screen.
         * @param v the new value.
         */
        virtual void setTop(float v);

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
        virtual void setWidth(float v);

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
        virtual void setHeight(float v);

        /**
         * Sets the position of the node by invoking the individual set functions for each coordinate.
         * @param left left coordinate.
         * @param top top coordinate.
         */
        virtual void setPosition(float left, float top);

        /**
         * Sets the size of the node by invoking the individual set functions for each coordinate.
         * @param width width.
         * @param height height.
         */
        virtual void setSize(float width, float height);

        /**
         * Sets the geometry of the node by invoking the individual set functions for each coordinate.
         * @param left left coordinate.
         * @param top top coordinate.
         * @param width width.
         * @param height height.
         */
        virtual void setGeometry(float left, float top, float width, float height);

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
        virtual void setScalingX(float v);

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
        virtual void setScalingY(float v);

        /**
         * Sets the scaling parameters by calling the individual set function for each parameter.
         * @param scalingX scaling along the X axis.
         * @param scalingY scaling along the Y axis.
         */
        virtual void setScaling(float scalingX, float scalingY);

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
        virtual void setVisible(bool v);

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
         *        or it invokes the method `onCalcRootState()` to allow a UI node to compute its state without a parent.
         *      - it invokes the `onPaint()` method to allow the node to paint itself.
         *      - it renders the children of this.
         * Nodes are rendered from first to last child, and only if they are visible.
         */
        void renderTree();

        /**
         * Interface for checking if the underlying node is a positioned node.
         * @return true if this node is a positioned node, false otherwise.
         */
        virtual bool isPositionedNode() const {
            return false;
        }

        /**
         * Interface for checking if the underlying node is a layout node.
         * @return true if this node is a layout node, false otherwise.
         */
        virtual bool isLayoutNode() const {
            return false;
        }

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

        /**
         * Checks if this is rendered currently on the screen.
         * @return true if it is rendered, false otherwise.
         */
        bool isRendered() const {
            return m_rendered;
        }

        /**
         * Checks if this and all ancestors are rendered.
         * @return true if this and all ancestors are rendered, false otherwise.
         */
        bool isRenderedTree() const {
            return m_rendered && (!getParent() || getParent()->isRenderedTree());
        }

        /**
         * Checks if the geometry of the object is valid or not.
         * @return the geometry valid flag.
         */
        virtual bool isGeometryValid() const {
            return m_geometryValid;
        }

        /**
         * Sets the geometry valid flag to false, causing a recomputation of the on screen geometry
         * on the next `renderTree()` call.
         */
        virtual void invalidateGeometry() {
            m_geometryValid = false;
        }

    protected:
        /**
         * Invoked when the node must compute its geometry based on its parent.
         * The default implementation does nothing.
         */
        virtual void onCalcGeometry(const UINode* parent) {
        }

        /**
         * Invoked when the node must compute its geometry when being root.
         * The default implementation does nothing.
         */
        virtual void onCalcGeometry() {
        }

        /**
         * Interface for copying the invalid geometry state of the parent to the child,
         * in order to set the child to an invalid state and let the child do the appropriate
         * geometry computations.
         * The default implementation sets the geometry validity of the UI node to false,
         * if the geometry validity of this is false.
         */
        virtual void setChildGeometryInvalid(UINode* child) const;

        /**
         * Sets the geometry to be in valid state.
         * Invoked after a parent with its children are rendered,
         * from within `renderTree()`.
         */
        virtual void setGeometryValid() {
            m_geometryValid = true;
        }

        /**
         * Invoked from `renderTree()` to allow a UI node to compute its state, based on the state of parent.
         * The default implementation computes the screen coordinates and screen scaling of the node, 
         * based on the parent's screen coordinates and screen scaling.
         * @param parent parent node.
         */
        virtual void onCalcState(const UINode* parent);

        /**
         * Invoked from `renderTree()` to allow a UI node to compute its state when it has no parent.
         * The default implementation computes the screen coordinates and screen scaling of the node.
         * @param parent parent node.
         */
        virtual void onCalcState();

        /**
         * Invoked from `renderTree()` to allow a node to paint itself on the screen.
         * The default implementation does nothing.
         */
        virtual void onPaint() const {
        }

    private:
        float m_left{ 0 };
        float m_top{ 0 };
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
        bool m_rendered{ false };
        bool m_geometryValid{ false };

        void setRendered(bool rendered);
    };


} //namespace algui


#endif //ALGUI_UINODE_HPP
