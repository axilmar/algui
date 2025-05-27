#pragma warning (disable: 4309)
#include "algui/UIInteractiveNode.hpp"


namespace algui {


    const std::string EventType::focus = "focus";
    const std::string EventType::focusIn = "focusin";
    const std::string EventType::blur = "blur";
    const std::string EventType::focusOut = "focusout";
    static const std::string mouseEnter = "mouseenter";
    static const std::string mouseMove = "mousemove";
    static const std::string mouseLeave = "mouseleave";
    static const std::string mouseButtonDown = "mousebuttondown";
    static const std::string mouseButtonHeldDown = "mousebuttonhelddown";
    static const std::string mouseButtonUp = "mousebuttonup";
    static const std::string mousewheel = "mousewheel";
    static const std::string click = "click";
    static const std::string doubleClick = "doubleclick";
    static const std::string dragEnter = "dragenter";
    static const std::string drag = "drag";
    static const std::string dragLeave = "dragleave";
    static const std::string drop = "drop";
    static const std::string keyDown = "keydown";
    static const std::string keyUp = "keyup";
    static const std::string keyChar = "keychar";
    static const std::string unusedKeyDown = "unusedkeydown";
    static const std::string unusedKeyUp = "unusedkeyup";
    static const std::string unusedKeyChar = "unusedkeychar";
    static const std::string timer = "timer";


    UIInteractiveNode::~UIInteractiveNode() {
        if (contains(focusedNode)) {
            focusedNode = nullptr;
        }
    }


    void UIInteractiveNode::setEnabled(bool v) {
        if (isEnabled() != v) {
            UIVisualStateNode::setEnabled(v);
            if (!v) {
                if (contains(focusedNode)) {
                    focusedNode->setFocused(false);
                }
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


    UIInteractiveNode* UIInteractiveNode::getParent() const {
        UINode* node = getParentPtr();
        for (; node; node = node->getParentPtr()) {
            if (node->IsInteractiveNode()) {
                return static_cast<UIInteractiveNode*>(node);
            }
        }
        return nullptr;
    }


    UIInteractiveNode* UIInteractiveNode::getPrevSibling() const {
        for (UINode* prevSibling = UINode::getPrevSibling().get(); prevSibling; prevSibling = prevSibling->UINode::getPrevSibling().get()) {
            if (prevSibling->IsInteractiveNode()) {
                return static_cast<UIInteractiveNode*>(prevSibling);
            }
        }
        return nullptr;
    }


    UIInteractiveNode* UIInteractiveNode::getNextSibling() const {
        for (UINode* nextSibling = UINode::getNextSibling().get(); nextSibling; nextSibling = nextSibling->UINode::getNextSibling().get()) {
            if (nextSibling->IsInteractiveNode()) {
                return static_cast<UIInteractiveNode*>(nextSibling);
            }
        }
        return nullptr;
    }


    UIInteractiveNode* UIInteractiveNode::getFirstChild() const {
        for (UINode* child = UINode::getFirstChild().get(); child; child = child->UINode::getNextSibling().get()) {
            if (child->IsInteractiveNode()) {
                return static_cast<UIInteractiveNode*>(child);
            }
        }
        return nullptr;
    }


    UIInteractiveNode* UIInteractiveNode::getLastChild() const {
        for (UINode* child = UINode::getLastChild().get(); child; child = child->UINode::getPrevSibling().get()) {
            if (child->IsInteractiveNode()) {
                return static_cast<UIInteractiveNode*>(child);
            }
        }
        return nullptr;
    }


    UIInteractiveNode* UIInteractiveNode::getRoot() const {
        UINode* node = const_cast<UIInteractiveNode*>(this);
        UIInteractiveNode* result = nullptr;
        do {
            if (node->IsInteractiveNode()) {
                result = static_cast<UIInteractiveNode*>(node);
            }
            node = node->getParentPtr();
        } while (node);
        return result;
    }


    bool UIInteractiveNode::intersects(float screenX, float screenY) const {
        return screenX >= getScreenX1() && screenX < getScreenX2() && screenY >= getScreenY1() && screenY < getScreenY2();
    }


    UIInteractiveNode* UIInteractiveNode::focusedNode = nullptr;


    bool UIInteractiveNode::dispatchEventUp(const std::string& eventName, const void* event) const {
        for (const UIInteractiveNode* node = this; node; node = node->getParent()) {
            if (node->dispatchEvent(eventName, event)) {
                return true;
            }
        }
        return false;
    }


} //namespace algui
