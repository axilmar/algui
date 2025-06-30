#ifndef ALGUI_UINODE_HPP
#define ALGUI_UINODE_HPP


#include <cstdint>
#include "TreeNode.hpp"
#include "Rect.hpp"


namespace algui {


    /**
     * Scaling.
     */
    struct Scaling {
        ///horizontal.
        float horizontal{ 0 };

        ///vertical.
        float vertical{ 0 };

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


    class UINode : public TreeNode<UINode> {
    public:
        /**
         * Overrides the base class method in order to set the state of the added child.
         * @param child child to add; must not be null, must not already be a child, must not be an ancestor of this.
         * @param nextSibling optional; if not given, then the child is added as last child, otherwise it is added before this; must be a child of this.
         * @exception std::invalid_argument thrown if the constraints mentioned above are violated.
         */
        void addChild(const std::shared_ptr<UINode>& child, const std::shared_ptr<UINode>& nextSibling = nullptr) override;

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
         * @param scaling the new scaling.
         */
        void setScaling(Scaling scaling);

        /**
         * Checks if this node is visible.
         * @return true if visible, false otherwise.
         */
        bool isVisible() const {
            return m_visible;
        }

        /**
         * Hides/shows this node.
         * @param v the new visible state.
         */
        void setVisible(bool v);

        /**
         * Returns true if the node is painted clipped.
         * @return true if the node is painted clipped, false otherwise.
         */
        bool isClipped() const {
            return !m_unclipped;
        }

        /**
         * Sets the clipped state.
         * @param v if true, the node is painted clipped, otherwise it is not.
         */
        void setClipped(bool v) {
            m_unclipped = !v;
        }

        /**
         * Updates and paints the node tree.
         */
        void render();

        /**
         * Updates and paints the node tree, for every node that falls within the given screen rectangle.
         * @param clipping screen clipping.
         */
        void render(const Rect& clipping);

    protected:
        /**
         * Internal state flags.
         */
        enum FLAGS : uint64_t {
            ///rect needs updating.
            RECT_DIRTY            = 1 << 0,

            ///descentant rect needs updating.
            DESCENTANT_RECT_DIRTY = 1 << 1,

            ///layout needs updating.
            LAYOUT_DIRTY          = 1 << 2,

            ///screen rect needs updating.
            SCREEN_RECT_DIRTY     = 1 << 3,

            ///screen scaling needs updating.
            SCREEN_SCALING_DIRTY  = 1 << 4,

            ///first free flag.
            FREE_FLAG             = 1 << 8
        };

        /**
         * Returns the current flags.
         * @return the current flags.
         */
        uint64_t getFlags() const {
            return m_flags;
        }

        /**
         * Sets the flags.
         * @param flags new flags.
         */
        void setFlags(uint64_t flags) {
            m_flags = flags;
        }

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
         * Interface for updating screen props.
         * Invoked just before a UI node is to be painted onto the target bitmap.
         * The default implementation updates the layout, the screen rect and the screen scaling, if the relevant flags are set.
         * Subclasses can put more update functions here, for example to update resources based on screen scaling.
         */
        virtual void updateScreenProps();

        /**
         * Interface for painting the node, using Allegro functions.
         * The default implementation is empty.
         * This is called after all screen props are updated, but before the children.
         */
        virtual void paint() {}

        /**
         * Interface for painting above a node's children, using Allegro functions.
         * The default implementation is empty.
         * This is called after all children have been painted.
         */
        virtual void paintOverlay() {}

    private:
        Rect m_rect;
        Rect m_screenRect;
        Scaling m_scaling;
        Scaling m_screenScaling;
        uint64_t m_flags{ 0 };
        bool m_visible{ true };
        bool m_unclipped{ true };

        void _updateRect();
        void _updateScreenProps(uint64_t flags);
        void _render(uint64_t flags);
        void _render(uint64_t flags, const Rect& clipping);
        void _render1(uint64_t flags, const Rect& clipping);
        void _setDescentantRectDirty();
    };


} //namespace algui


#endif //ALGUI_UINODE_HPP
