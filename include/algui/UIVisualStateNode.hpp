#ifndef ALGUI_UIVISUALSTATENODE_HPP
#define ALGUI_UIVISUALSTATENODE_HPP


#include "UIPositionedNode.hpp"


namespace algui {


    /**
     * Base class for nodes that have a visual state.
     * 
     * Nodes with visual states inherit their visual state from the tree:
     *  - if an ancestor node is disabled, all descentant nodes have the disabled visual state.
     *  - if an ancestor is highlighted/pressed/selected/focused, all descentant nodes have these visual states,
     *      even if they are not highlighted/pressed/selected/focused themselves.
     * 
     * This design allows individual UI nodes to be part of trees which act as a single unit visually.
     */
    class UIVisualStateNode : public UIPositionedNode {
    public:
        /**
         * Visual state enumeration.
         */
        enum class VisualState : unsigned {
            /** Disabled. */
            Disabled = 0,

            /** Enabled, i.e. accepts input. */
            Enabled = 1,

            /** Highlighted, i.e. part of a tree that the mouse is within. */
            Highlighted = 2,

            /** Pressed, i.e. a mouse button is pressed over the item. */
            Pressed = 4,

            /** Selected, i.e. if part of an item that represents a user selection. */
            Selected = 8,

            /** Focused, i.e. it contains or is part of a tree that contains the input focus. */
            Focused = 16,

            /** Highlighted/pressed/selected/focused. */
            HighlightedPressedSelectedFocused = Enabled | Highlighted | Pressed | Selected | Focused,

            /** Highlighted/selected/focused. */
            HighlightedSelectedFocused = Enabled | Highlighted | Selected | Focused,

            /** Highlighted/pressed/focused. */
            HighlightedPressedFocused = Enabled | Highlighted | Pressed | Focused,

            /** Highlighted/pressed/selected. */
            HighlightedPressedSelected = Enabled | Highlighted | Pressed | Selected,

            /** Pressed/selected/focused. */
            PressedSelectedFocused = Enabled | Pressed | Selected | Focused,

            /** Pressed/selected. */
            PressedSelected = Enabled | Pressed | Selected,

            /** Pressed/focused. */
            PressedFocused = Enabled | Pressed | Focused,

            /** Selected/focused. */
            SelectedFocused = Enabled | Selected | Focused
        };

        /**
         * The default constructor.
         * The node starts in the enabled visual state.
         */
        UIVisualStateNode();

        /**
         * Interface for checking if the underlying node is a visual state node.
         * @return always true.
         */
        bool isVisualStateNode() const override {
            return true;
        }

        /**
         * Returns true if the node is enabled, false otherwise.
         * @return true if the node is enabled, false otherwise.
         */
        bool isEnabled() const {
            return (unsigned)m_visualState & (unsigned)VisualState::Enabled;
        }

        /**
         * Checks if this node belongs to a tree where this and all ancestors are enabled.
         * @return true if this and all ancestors are enabled, false otherwise.
         */
        bool isEnabledTree() const {
            return isEnabled() && (!getParent() || getParent()->isEnabledTree());
        }

        /**
         * Sets the enabled state.
         * If the enabled state is removed, all the other states are removed as well.
         * @param v the new state.
         */
        virtual void setEnabled(bool v);

        /**
         * Returns true if the node is highlighted, false otherwise.
         * @return true if the node is highlighted, false otherwise.
         */
        bool isHighlighted() const {
            return (unsigned)m_visualState & (unsigned)VisualState::Highlighted;
        }

        /**
         * Sets the highlighted state.
         * If the node is disabled, this has no effect.
         * @param v the new state.
         */
        virtual void setHighlighted(bool v);

        /**
         * Returns true if the node is pressed, false otherwise.
         * @return true if the node is pressed, false otherwise.
         */
        bool isPressed() const {
            return (unsigned)m_visualState & (unsigned)VisualState::Pressed;
        }

        /**
         * Sets the pressed state.
         * If the node is disabled, this has no effect.
         * @param v the new state.
         */
        virtual void setPressed(bool v);

        /**
         * Returns true if the node is selected, false otherwise.
         * @return true if the node is selected, false otherwise.
         */
        bool isSelected() const {
            return (unsigned)m_visualState & (unsigned)VisualState::Selected;
        }

        /**
         * Sets the selected state.
         * If the node is disabled, this has no effect.
         * @param v the new state.
         */
        virtual void setSelected(bool v);

        /**
         * Returns true if the node is focused, false otherwise.
         * @return true if the node is focused, false otherwise.
         */
        bool isFocused() const {
            return (unsigned)m_visualState & (unsigned)VisualState::Focused;
        }

        /**
         * Sets the focused state.
         * If the node is disabled, this has no effect.
         * @param v the new state.
         * #return true if the focus set successfully, false otherwise.
         */
        virtual bool setFocused(bool v);

        /**
         * Returns the visual state of the tree, from root up to this.
         * @return the visual state of the tree.
         */
        VisualState getTreeVisualState() const {
            return m_treeVisualState;
        }

        /**
         * Returns the closest ancestor node that is a visual state node.
         * @return the closest ancestor node that is a visual state node.
         */
        UIVisualStateNode* getParent() const;

        /**
         * Returns the closest previous sibling node that is a visual state node.
         * @return the closest previous sibling node that is a visual state node.
         */
        UIVisualStateNode* getPrevSibling() const;

        /**
         * Returns the closest next sibling node that is a visual state node.
         * @return the closest next sibling node that is a visual state node.
         */
        UIVisualStateNode* getNextSibling() const;

        /**
         * Returns the first child node that is a visual state node.
         * @return the first child node that is a visual state node.
         */
        UIVisualStateNode* getFirstChild() const;

        /**
         * Returns the last child node that is a visual state node.
         * @return the last child node that is a visual state node.
         */
        UIVisualStateNode* getLastChild() const;

        /**
         * Returns the furthest ancestor node that is a visual state node.
         * @return the furthest ancestor node that is a visual state node.
         */
        UIVisualStateNode* getRoot() const;

    protected:
        /**
         * Overriden so as that the tree visual state for children is computed.
         * It also invokes the base class method, in order to compute the position and scaling parameters.
         * @param parent the parent node; if it is not a visual state node, then the closest visual state node is used.
         */
        void onCalcChildState(UINode* parent) override;

        /**
         * Overriden so as that the tree visual state for root nodes is computed.
         * It also invokes the base class method, in order to compute the position and scaling parameters.
         */
        void onCalcRootState() override;

    private:
        VisualState m_visualState:8;
        VisualState m_treeVisualState:8;
    };


} //namespace algui


#endif //ALGUI_UIVISUALSTATENODE_HPP
