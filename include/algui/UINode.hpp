#ifndef ALGUI_UINODE_HPP
#define ALGUI_UINODE_HPP


#include <cstdint>
#include "TreeNode.hpp"
#include "Rect.hpp"


namespace algui {


    class InteractiveUINode;


    /**
     * Scaling.
     */
    struct Scaling {
        ///horizontal.
        float horizontal{ 1.0f };

        ///vertical.
        float vertical{ 1.0f };

        /**
         * Clamps the values to 0.
         */
        void clampTo0() {
            horizontal = std::max(horizontal, 0.0f);
            vertical = std::max(vertical, 0.0f);
        }

        /**
         * Checks if this equals the given value.
         * @param s the other scaling to compare to this.
         * @return true if they are equal, false otherwise.
         */
        bool operator == (const Scaling& s) const {
            return horizontal == s.horizontal && vertical == s.vertical;
        }

        /**
         * Checks if this differs from the given value.
         * @param s the other scaling to compare to this.
         * @return true if they are different, false otherwise.
         */
        bool operator != (const Scaling& s) const {
            return horizontal != s.horizontal || vertical != s.vertical;
        }
    };


    /**
     * Base class for UI nodes.
     */
    class UINode : public TreeNode<UINode> {
    public:
        /**
         * Returns the rectangle of this node, relative to its parent, or to the screen.
         * @return the rectangle of this node
         */
        const Rect& getRect() const {
            return m_rect;
        }

        /**
         * Returns the screen rectangle of this node.
         * @return the screen rectangle of this node.
         */
        const Rect& getScreenRect() const {
            return m_screenRect;
        }

        /**
         * Sets the local rectangle for this node.
         * It emits an ObjectEvent with type "rectChanged".
         * @param rect the new local rectangle for this node.
         */
        void setRect(Rect rect);

        /**
         * Returns the local scaling.
         * @return the local scaling.
         */
        const Scaling& getScaling() const {
            return m_scaling;
        }

        /**
         * Returns the screen scaling.
         * @return the screen scaling.
         */
        const Scaling& getScreenScaling() const {
            return m_screenScaling;
        }

        /**
         * Sets the scaling.
         * It emits an ObjectEvent with type "scalingChanged".
         * @param scaling the new scaling.
         */
        void setScaling(Scaling scaling);

        /**
         * Checks if this node is visible.
         * @return true if visible, false otherwise.
         */
        bool isVisible() const;

        /**
         * Hides/shows this node.
         * It emits an ObjectEvent with type "visibleChanged".
         * @param v the new visible state.
         */
        void setVisible(bool v);

        /**
         * Returns true if the node is painted clipped.
         * @return true if the node is painted clipped, false otherwise.
         */
        bool isClipped() const;

        /**
         * Sets the clipped state.
         * It emits an ObjectEvent with type "clippedChanged".
         * @param v if true, the node is painted clipped, otherwise it is not.
         */
        void setClipped(bool v);

        /**
         * Checks if this UI node belongs in an enabled tree.
         * @return true if this UI node belongs in an an enabled tree, false otherwise.
         */
        bool isEnabledTree() const;

        /**
         * Checks if this UI node belongs in a focused tree.
         * @return true if this UI node belongs in an a focused tree, false otherwise.
         */
        bool isFocusedTree() const;

        /**
         * Checks if this UI node belongs in a highlighted tree.
         * @return true if this UI node belongs in an a highlighted tree, false otherwise.
         */
        bool isHighlightedTree() const;

        /**
         * Checks if this UI node belongs in a pressed tree.
         * @return true if this UI node belongs in an a pressed tree, false otherwise.
         */
        bool isPressedTree() const;

        /**
         * Checks if this UI node belongs in a selected tree.
         * @return true if this UI node belongs in an a selected tree, false otherwise.
         */
        bool isSelectedTree() const;

        /**
         * Checks if this UI node belongs in a error tree.
         * @return true if this UI node belongs in an a error tree, false otherwise.
         */
        bool isErrorTree() const;

        /**
         * Updates and paints the node tree.
         */
        virtual void render();

        /**
         * Updates and paints the node tree, for every node that falls within the given screen rectangle.
         * @param clipping screen clipping.
         */
        virtual void render(const Rect& clipping);

        /**
         * Checks intersection with coordinates, i.e. if coordinates lie within this node.
         * The comparison is done against the screen rectangle of the node, if the node is clipped,
         * otherwise the children are also taken into account.
         * @param x screen horizontal coordinate.
         * @param y screen vertical coordinate.
         */
        virtual bool intersects(float x, float y) const;

        /**
         * Returns the child under the given screen coordinates.
         * The method `intersects(x, y)` is called on visible children in order to 
         * check which child is under the given coordinates.
         * @param x screen horizontal coordinate.
         * @param y screen vertical coordinate.
         * @param enabled if true, then only enabled children (i.e. those with `isEnabledTree() == true`) are considered.
         * @return the child at the given screen coordinates.
         */
        UINode* getChildAt(float x, float y, bool enabled = false) const;

    protected:
        /**
         * Sets the new child state.
         * @param child the new child.
         */
        void setNewChildState(const std::shared_ptr<UINode>& child) override;

        /**
         * Sets the RECT_DIRTY flag on this UI node,
         * and the DESCENTANT_RECT_DIRTY flag to all the ancestor nodes,
         * allowing the updating of the rect of this node at the next `render()` call.
         */
        void invalidateRect();

        /**
         * Sets the LAYOUT_DIRTY flag on this UI node,
         * allowing the updating of the layout of this node at the next `render()` call.
         */
        void invalidateLayout();

        /**
         * Sets the SCREEN_RECT_DIRTY flag on this UI node,
         * allowing the updating of the screen rect of this node at the next `render()` call.
         */
        void invalidateScreenRect();

        /**
         * Sets the SCREEN_SCALING_DIRTY flag on this UI node,
         * allowing the updating of the screen scaling of this node at the next `render()` call.
         */
        void invalidateScreenScaling();

        /**
         * Interface for updating a node's position and size, based on its children or content.
         * A subclass can use the `setRect()` method to define a desired position and size.
         * This will be invoked before layout computations.
         * The default implementation does nothing.
         */
        virtual void updateRect() {}

        /**
         * Interface for updating a node's layout.
         * A subclass can use the `setRect()` method to define a rectangle for each of its children.
         * The default implementation does nothing.
         */
        virtual void updateLayout() const {}

        /**
         * Updates the screen rectangle of the node, based on its local rectangle and the screen rectangle of its parent.
         */
        void updateScreenRect();

        /**
         * Updates the screen scaling of the node, based on its own scaling and on the screen scaling of its parent.
         */
        void updateScreenScaling();

        /**
         * Interface for updating screen properties.
         * Invoked just before a UI node is to be painted onto the target bitmap.
         * The default implementation updates the layout, the screen rect and the screen scaling, if the relevant flags are set.
         * Subclasses can put more update functions here, for example to update resources based on screen scaling.
         */
        virtual void updateScreenProperties();

        /**
         * Interface for painting the node, using Allegro functions.
         * The default implementation is empty.
         * This is called after all screen props are updated, but before the children.
         */
        virtual void paint() const {}

        /**
         * Interface for painting above a node's children, using Allegro functions.
         * The default implementation is empty.
         * This is called after all children have been painted.
         */
        virtual void paintOverlay() const {}

    private:
        Rect m_rect;
        Rect m_screenRect;
        Scaling m_scaling;
        Scaling m_screenScaling;
        int m_flags{3};

        void _updateRect();
        void _updateScreenProps(int& flags);
        void _render(int flags);
        void _render(int flags, const Rect& clipping);
        void _render1(int flags, const Rect& clipping);
        void _setDescentantRectDirty();
        void _setEnabledTree(bool v);
        void _setFocusedTree(bool v);
        void _setHighlightedTree(bool v);
        void _setPressedTree(bool v);
        void _setSelectedTree(bool v);
        void _setErrorTree(bool v);

        friend class InteractiveUINode;
    };


} //namespace algui


#endif //ALGUI_UINODE_HPP
