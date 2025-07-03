#ifndef ALGUI_INTERACTIVEUINODE_HPP
#define ALGUI_INTERACTIVEUINODE_HPP


#include "UINode.hpp"


union ALLEGRO_EVENT;


namespace algui {


    /**
     * Base class for interactive UI nodes.
     */
    class InteractiveUINode : public UINode {
    public:
        /**
         * The destructor.
         * If this is the focused node, then the internal focused node pointer is reset.
         */
        virtual ~InteractiveUINode();

        /**
         * Returns a pointer to the closest ancestor node that is an interactive UI node.
         * @return a pointer to the closest ancestor node that is an interactive UI node.
         */
        std::shared_ptr<InteractiveUINode> getParent() const;

        /**
         * Returns a raw pointer to the closest ancestor node that is an interactive UI node.
         * @return a raw pointer to the closest ancestor node that is an interactive UI node.
         */
        InteractiveUINode* getParentPtr() const;

        /**
         * Returns a pointer to the first child node that is interactive.
         * @return a pointer to the first child node that is interactive.
         */
        std::shared_ptr<InteractiveUINode> getFirstChild() const;

        /**
         * Returns a raw pointer to the first child node that is interactive.
         * @return a raw pointer to the first child node that is interactive.
         */
        InteractiveUINode* getFirstChildPtr() const;

        /**
         * Returns a pointer to the last child node that is interactive.
         * @return a pointer to the last child node that is interactive.
         */
        std::shared_ptr<InteractiveUINode> getLastChild() const;

        /**
         * Returns a raw pointer to the last child node that is interactive.
         * @return a raw pointer to the last child node that is interactive.
         */
        InteractiveUINode* getLastChildPtr() const;

        /**
         * Returns a pointer to the first previous sibling node that is interactive.
         * @return a pointer to the first previous sibling node that is interactive.
         */
        std::shared_ptr<InteractiveUINode> getPrevSibling() const;

        /**
         * Returns a raw pointer to the first previous sibling node that is interactive.
         * @return a raw pointer to the first previous sibling node that is interactive.
         */
        InteractiveUINode* getPrevSiblingPtr() const;

        /**
         * Returns a pointer to the first next sibling node that is interactive.
         * @return a pointer to the first next sibling node that is interactive.
         */
        std::shared_ptr<InteractiveUINode> getNextSibling() const;

        /**
         * Returns a raw pointer to the first next sibling node that is interactive.
         * @return a raw pointer to the first next sibling node that is interactive.
         */
        InteractiveUINode* getNextSiblingPtr() const;

        /**
         * Returns a pointer to the highest ancestor node that is interactive.
         * @return a pointer to the highest ancestor node that is interactive.
         */
        std::shared_ptr<InteractiveUINode> getRoot() const;

        /**
         * Returns a raw pointer to the highest ancestor node that is interactive.
         * @return a raw pointer to the highest ancestor node that is interactive.
         */
        InteractiveUINode* getRootPtr() const;

        /**
         * Checks if this node is enabled.
         * @return true if the node is enabled, false otherwise.
         */
        bool isEnabled() const;

        /**
         * Sets the enabled state.
         * If it is to be disabled and contains the focus, then the node with the focus loses the focus.
         * It emits an InteractiveUINodeEvent with type "enabledChanged".
         * @param v the new enabled state.
         */
        void setEnabled(bool v);

        /**
         * Enables this node.
         * Equal to `setEnabled(true)`.
         */
        void enable() {
            setEnabled(true);
        }

        /**
         * Disables this node.
         * Equal to `setEnabled(false)`.
         */
        void disable() {
            setEnabled(false);
        }

        /**
         * Checks if this node has the input focus.
         * Only one interactive node can have the input focus at a time.
         * @return true if this has the input focus, false otherwise.
         */
        bool isFocused() const;

        /**
         * Returns the node that has the focus.
         * @return the node that has the focus.
         */
        static InteractiveUINode* getFocusedNode();

        /**
         * Sets or removes the focus from this node.
         * If the node is disabled, it cannot get the focus.
         * If the node gets the focus, then it emits an InteractiveUINodeEvent with type "gotFocus".
         * If the node loses the focus, then it emits the InteractiveUINodeEvent with type "lostFocus".
         * Both events bubble up to root.
         * @param v the new focused state.
         * @return true if the state was changed successfully, false otherwise.
         */
        bool setFocused(bool v);

        /**
         * Sets the focus to this.
         * Equal to `setFocused(true)`.
         */
        bool focus() {
            return setFocused(true);
        }

        /**
         * Removes the focus from this.
         * Equal to `setFocused(false)`.
         */
        bool blur() {
            return setFocused(false);
        }

        /**
         * Checks if this node is highlighted.
         * @return true if this node is highlighted, false otherwise.
         */
        bool isHighlighted() const;

        /**
         * Sets the highlighted state.
         * It emits an InteractiveUINodeEvent with type "highlightedChanged".
         * @param v the new highlighted state.
         */
        void setHighlighted(bool v);

        /**
         * Checks if this node is pressed.
         * @return true if this node is pressed, false otherwise.
         */
        bool isPressed() const;

        /**
         * Sets the pressed state.
         * It emits an InteractiveUINodeEvent with type "pressedChanged".
         * @param v the new pressed state.
         */
        void setPressed(bool v);

        /**
         * Checks if this node is selected.
         * @return true if this node is selected, false otherwise.
         */
        bool isSelected() const;

        /**
         * Sets the selected state.
         * It emits an InteractiveUINodeEvent with type "selectedChanged".
         * @param v the new selected state.
         */
        void setSelected(bool v);

        /**
         * Checks if this node is in error state.
         * @return true if this node is in error state, false otherwise.
         */
        bool isError() const;

        /**
         * Sets the error state.
         * It emits an InteractiveUINodeEvent with type "errorChanged".
         * @param v the new error state.
         */
        void setError(bool v);

        /**
         * Handles the given allegro event and creates events for this UI tree.
         * The node must be enabled in order to handle events.
         * @param event allegro event to handle.
         * @return true if the event was handled, false otherwise.
         */
        virtual bool doEvent(const ALLEGRO_EVENT& event);

    protected:
        /**
         * Sets the new child state.
         * @param child the new child.
         */
        void setNewChildState(const std::shared_ptr<UINode>& child) override;

    private:
        int m_flags{1};

        static void _setEnabledTree(UINode* node, bool parentEnabledTree);
        static void _setFocusedTree(UINode* node, bool parentFocusedTree = false);
        static void _setHighlightedTree(UINode* node, bool parentHighlightedTree = false);
        static void _setPressedTree(UINode* node, bool parentPressedTree = false);
        static void _setSelectedTree(UINode* node, bool parentSelectedTree = false);
        static void _setErrorTree(UINode* node, bool parentErrorTree = false);
        static bool _doRootMouseMoveEvent(UINode* node, const ALLEGRO_EVENT& event);
        static bool _doMouseEnterEvent(UINode* node, const ALLEGRO_EVENT& event);
        static bool _doMouseMoveEvent(UINode* node, const ALLEGRO_EVENT& event);
        static bool _doMouseLeaveEvent(UINode* node, const ALLEGRO_EVENT& event);
        static bool _doMouseWheelEvent(UINode* node, const ALLEGRO_EVENT& event);
        static bool _doMouseButtonDownEvent(UINode* node, const ALLEGRO_EVENT& event);
        static bool _doMouseButtonUpEvent(UINode* node, const ALLEGRO_EVENT& event);
        static bool _doKeyboardEvent(const std::string_view& type, InteractiveUINode* node, const ALLEGRO_EVENT& event);
        static bool _doUnusedKeyboardEvent(const std::string_view& type, UINode* node, const ALLEGRO_EVENT& event);
        static bool _doTimerEvent(UINode* node, const Event& event);
};


} //namespace algui


#endif //ALGUI_INTERACTIVEUINODE_HPP
