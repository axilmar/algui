#include "algui/UIInteractiveNode.hpp"


namespace algui {


    UIInteractiveNode* UIInteractiveNode::focusedNode = nullptr;


    UIInteractiveNode::~UIInteractiveNode() {
        if (contains(focusedNode)) {
            focusedNode = nullptr;
        }
    }


    void UIInteractiveNode::setEnabled(bool v) {
        if (isEnabled() != v) {
            UIVisualStateNode::setEnabled(v);
            if (!v && contains(focusedNode)) {
                focusedNode->setFocused(false);
            }
        }
    }


    void UIInteractiveNode::setFocused(bool v) {
        if (isFocused() != v) {
            if (v) {
                if (focusedNode) {
                    focusedNode->setFocused(false);
                }
                UIVisualStateNode::setFocused(true);
                focusedNode = this;
                onGotFocus();
            }
            else {
                UIVisualStateNode::setFocused(false);
                focusedNode = nullptr;
                onLostFocus();
            }
        }
    }


} //namespace algui
