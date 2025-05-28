#include <stdexcept>
#pragma warning (disable: 4309)
#include "algui/UIInteractiveNode.hpp"
#include "algui/UIInteractionEvent.hpp"
#include "algui/EventType.hpp"


namespace algui {


    UIInteractiveNode::~UIInteractiveNode() {
        //invoked here instead of TreeNode::~TreeNode in order to call the virtual method for 'removeChild',
        //which has some side effects.
        removeAllChildren();

        //if this still has the focus, then force reset of focusedNode ptr
        if (focusedNode == this) {
            focusedNode = nullptr;
        }
    }


    void UIInteractiveNode::addChild(const std::shared_ptr<UINode>& child, const std::shared_ptr<UINode>& nextSibling) {
        UIVisualStateNode::addChild(child, nextSibling);
        dispatchEvent(EventType::childAdded, UIEvent(child));
    }


    void UIInteractiveNode::removeChild(const std::shared_ptr<UINode>& child) {
        //check the child
        if (!child || child->getParent() != this) {
            throw std::invalid_argument("UIInteractiveNode: removeChild: invalid child.");
        }

        //reset the focus before removing the child,
        //so as that focus events bubble up to ancestors
        if (child->isInteractiveNode()) {
            static_cast<UIInteractiveNode*>(child.get())->resetFocus();
        }

        //remove the child
        UIVisualStateNode::removeChild(child);

        //if the removed child was the child with the mouse, reset the child with the mouse
        if (m_childWithMouse == child.get()) {
            m_childWithMouse->resetMouseState();
            m_childWithMouse = nullptr;
        }

        //child removed event
        dispatchEvent(EventType::childRemoved, UIEvent(child));
    }


    void UIInteractiveNode::setEnabled(bool v) {
        //if no change, do nothing
        if (isEnabled() == v) {
            return;
        }

        //enable
        if (v) {
            UIVisualStateNode::setEnabled(true);
            dispatchEvent(EventType::enabled, UIInteractionEvent(sharedFromThis<UIInteractiveNode>()));
        }

        //disable
        else {
            resetFocus();
            UIVisualStateNode::setEnabled(false);
            resetMouseState();
            if (getParent() && getParent()->m_childWithMouse == this) {
                getParent()->m_childWithMouse = nullptr;
            }
            dispatchEvent(EventType::disabled, UIInteractionEvent(sharedFromThis<UIInteractiveNode>()));
        }
    }


    bool UIInteractiveNode::setFocused(bool v) {
        //if no change, do nothing
        if (isFocused() == v) {
            return true;
        }

        //set the focus
        if (v) {
            //can't give the focus to a disabled or non-rendered node
            if (!isEnabledTree() || !isRenderedTree()) {
                return false;
            }

            //remove the focus from node that has it
            if (focusedNode && !focusedNode->setFocused(false)) {
                return false;
            }

            //set the focus to this node
            UIVisualStateNode::setFocused(true);
            UIInteractionEvent focusEvent(sharedFromThis<UIInteractiveNode>());
            dispatchEvent(EventType::focus, focusEvent);
            dispatchEventUp(EventType::focusIn, focusEvent);
        }

        //else remove the focus from this node
        else {
            UIVisualStateNode::setFocused(false);
            focusedNode = nullptr;
            UIInteractionEvent focusEvent(sharedFromThis<UIInteractiveNode>());
            dispatchEvent(EventType::blur, focusEvent);
            dispatchEventUp(EventType::focusOut, focusEvent);
        }

        return true;
    }


    UIInteractiveNode* UIInteractiveNode::getParent() const {
        for (UINode* node = UINode::getParent(); node; node = node->UINode::getParent()) {
            if (node->isInteractiveNode()) {
                return static_cast<UIInteractiveNode*>(node);
            }
        }
        return nullptr;
    }


    UIInteractiveNode* UIInteractiveNode::getPrevSibling() const {
        for (UINode* prevSibling = UINode::getPrevSibling(); prevSibling; prevSibling = prevSibling->UINode::getPrevSibling()) {
            if (prevSibling->isInteractiveNode()) {
                return static_cast<UIInteractiveNode*>(prevSibling);
            }
        }
        return nullptr;
    }


    UIInteractiveNode* UIInteractiveNode::getNextSibling() const {
        for (UINode* nextSibling = UINode::getNextSibling(); nextSibling; nextSibling = nextSibling->UINode::getNextSibling()) {
            if (nextSibling->isInteractiveNode()) {
                return static_cast<UIInteractiveNode*>(nextSibling);
            }
        }
        return nullptr;
    }


    UIInteractiveNode* UIInteractiveNode::getFirstChild() const {
        for (UINode* child = UINode::getFirstChild(); child; child = child->UINode::getNextSibling()) {
            if (child->isInteractiveNode()) {
                return static_cast<UIInteractiveNode*>(child);
            }
        }
        return nullptr;
    }


    UIInteractiveNode* UIInteractiveNode::getLastChild() const {
        for (UINode* child = UINode::getLastChild(); child; child = child->UINode::getPrevSibling()) {
            if (child->isInteractiveNode()) {
                return static_cast<UIInteractiveNode*>(child);
            }
        }
        return nullptr;
    }


    UIInteractiveNode* UIInteractiveNode::getRoot() const {
        UINode* node = const_cast<UIInteractiveNode*>(this);
        UIInteractiveNode* result = nullptr;
        do {
            if (node->isInteractiveNode()) {
                result = static_cast<UIInteractiveNode*>(node);
            }
            node = node->getParent();
        } while (node);
        return result;
    }


    bool UIInteractiveNode::intersects(float screenX, float screenY) const {
        return screenX >= getScreenX1() && screenX < getScreenX2() && screenY >= getScreenY1() && screenY < getScreenY2();
    }


    UIInteractiveNode* UIInteractiveNode::getChildFromPoint(float screenX, float screenY) const {
        for (UIInteractiveNode* child = getLastChild(); child; child = child->getPrevSibling()) {
            if (child->isVisible() && child->intersects(screenX, screenY)) {
                return child;
            }
        }
        return nullptr;
    }


    UIInteractiveNode* UIInteractiveNode::focusedNode = nullptr;


    void UIInteractiveNode::resetFocus() {
        if (contains(focusedNode)) {
            focusedNode->setFocused(false);
            if (contains(focusedNode)) {
                focusedNode = nullptr;
            }
        }
    }


    void UIInteractiveNode::resetMouseState() {
        m_hasMouse = false;
        if (m_childWithMouse) {
            m_childWithMouse->resetMouseState();
            m_childWithMouse = nullptr;
        }
    }


    bool UIInteractiveNode::dispatchEventUp(const std::string& eventName, const Event& event) const {
        for (const UIInteractiveNode* node = this; node; node = node->getParent()) {
            if (node->dispatchEvent(eventName, event)) {
                return true;
            }
        }
        return false;
    }


} //namespace algui
