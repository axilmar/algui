#include "algui/UIInteractiveNode.hpp"


namespace algui {


    const std::string EventType::focus = "focus";
    const std::string EventType::focusIn = "focusin";
    const std::string EventType::blur = "blur";
    const std::string EventType::focusOut = "focusout";


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
                dispatchEvent(EventType::focus);
                dispatchEventUp(EventType::focusIn);
            }
            else {
                UIVisualStateNode::setFocused(false);
                focusedNode = nullptr;
                dispatchEvent(EventType::blur);
                dispatchEventUp(EventType::focusOut);
            }
        }
    }


    UIInteractiveNode* UIInteractiveNode::getInteractiveParent() const {
        UINode* node = getParentPtr();
        for (; node; node = node->getParentPtr()) {
            if (node->IsInteractiveNode()) {
                return static_cast<UIInteractiveNode*>(node);
            }
        }
        return nullptr;
    }


    bool UIInteractiveNode::dispatchEventUp(const std::string& eventName, void* event) const {
        for (const UIInteractiveNode* node = this; node; node = node->getInteractiveParent()) {
            if (node->dispatchEvent(eventName, event)) {
                return true;
            }
        }
        return false;
    }


    UIInteractiveNode* UIInteractiveNode::focusedNode = nullptr;


} //namespace algui
