#ifndef ALGUI_UIINTERACTIVENODE_HPP
#define ALGUI_UIINTERACTIVENODE_HPP


#include "UIVisualStateNode.hpp"
#include "EventTarget.hpp"


namespace algui {


    /**
     * Event types.
     */
    class EventType {
    public:
        /** focus; received by node that gets the focus; does not bubble. */
        static const std::string focus;

        /** focus in; received by node that gets the focus; bubbles up to root. */
        static const std::string focusIn;

        /** blur; received by node that loses the focus; does not bubble. */
        static const std::string blur;

        /** focus out; received by node that loses the focus; bubbles up to root. */
        static const std::string focusOut;
    };


    /**
     * Base class for interactive nodes.
     * 
     * Interactive nodes have event handlers, and can accept the input focus.
     */
    class UIInteractiveNode : public UIVisualStateNode, public EventTarget {
    public:
        /**
         * Takes care of resetting the global focused node pointer,
         * if that is contained within the tree specified by this node.
         */
        ~UIInteractiveNode();

        /**
         * Interface for checking if the underlying node is an interactive node.
         * @return always true.
         */
        bool IsInteractiveNode() const override {
            return true;
        }

        /**
         * Sets the enabled state.
         * If the node is disabled and it contains the input focus,
         * then the input focus is lost.
         * @param v the new enabled state.
         */
        void setEnabled(bool v) override;

        /**
         * Sets or resets the input focus.
         * If the node receives the input focus, then the event `focus` is fired,
         * followed by a `focusin` event which bubbles up to root.
         * If the node loses the input focus, then the event `blur` is fired, followed
         * by a `focusout` event which bubbles up to root.
         * @param v the new focused state.
         */
        void setFocused(bool v) override;

        /**
         * Returns the currently focused node.
         * @return the currently focused node.
         */
        static UIInteractiveNode* getFocusedNode() {
            return focusedNode;
        }

        /**
         * Returns the closest ancestor node that is an interactive node.
         * @return the closest ancestor node that is an interactive node.
         */
        UIInteractiveNode* getInteractiveParent() const;

        /**
         * Dispatches an event to this and all its ancestors,
         * stopping when the event is succesfully dispatched.
         * @param eventName event name.
         * @param event event data.
         * @return true if the event was processed, false otherwise.
         */
        bool dispatchEventUp(const std::string& eventName, void* event = nullptr) const;

    private:
        static UIInteractiveNode* focusedNode;
    };


} //namespace algui


#endif //ALGUI_UIINTERACTIVENODE_HPP
