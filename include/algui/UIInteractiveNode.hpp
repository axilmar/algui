#ifndef ALGUI_UIINTERACTIVENODE_HPP
#define ALGUI_UIINTERACTIVENODE_HPP


#include "UIVisualStateNode.hpp"


namespace algui {


    /**
     * Base class for interactive nodes.
     * 
     * Interactive nodes have event handlers, and can accept the input focus.
     */
    class UIInteractiveNode : public UIVisualStateNode {
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
         * If the node receives the input focus, then the method `onGotFocus()` is invoked;
         * if the node loses the input focus, then the method `onLostFocus()` is invoked.
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

    protected:
        /**
         * Invoked when the node gets the focus.
         * It does nothing by default.
         */
        virtual void onGotFocus() {
        }

        /**
         * Invoked when the node loses the focus.
         * It does nothing by default.
         */
        virtual void onLostFocus() {
        }

    private:
        static UIInteractiveNode* focusedNode;
    };


} //namespace algui


#endif //ALGUI_UIINTERACTIVENODE_HPP
