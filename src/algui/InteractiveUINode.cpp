#include "algui/InteractiveUINode.hpp"
#include "algui/InteractiveUINodeEnabledChangedEvent.hpp"
#include "algui/InteractiveUINodeHighlightedChangedEvent.hpp"
#include "algui/InteractiveUINodePressedChangedEvent.hpp"
#include "algui/InteractiveUINodeSelectedChangedEvent.hpp"
#include "algui/InteractiveUINodeErrorChangedEvent.hpp"
#include "algui/InteractiveUINodeGotFocusEvent.hpp"
#include "algui/InteractiveUINodeLostFocusEvent.hpp"


namespace algui {


    enum FLAGS {
        ENABLED     = 1 << 0,
        HIGHLIGHTED = 1 << 1, 
        PRESSED     = 1 << 2,
        SELECTED    = 1 << 3,
        ERROR       = 1 << 4
    };


    static InteractiveUINode* _focusedNode = nullptr;


    InteractiveUINode::~InteractiveUINode() {
        if (this == _focusedNode) {
            _focusedNode = nullptr;
        }
    }


    std::shared_ptr<InteractiveUINode> InteractiveUINode::getParent() const {
        InteractiveUINode* inode = getParentPtr();
        return inode ? inode->sharedFromThis<InteractiveUINode>() : nullptr;
    }


    InteractiveUINode* InteractiveUINode::getParentPtr() const {
        for (UINode* node = UINode::getParentPtr(); node; node = node->getParentPtr()) {
            InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
            if (inode) {
                return inode;
            }
        }
        return nullptr;
    }


    std::shared_ptr<InteractiveUINode> InteractiveUINode::getFirstChild() const {
        InteractiveUINode* inode = getFirstChildPtr();
        return inode ? inode->sharedFromThis<InteractiveUINode>() : nullptr;
    }


    InteractiveUINode* InteractiveUINode::getFirstChildPtr() const {
        for (UINode* node = UINode::getFirstChildPtr(); node; node = node->getNextSiblingPtr()) {
            InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
            if (inode) {
                return inode;
            }
        }
        return nullptr;
    }


    std::shared_ptr<InteractiveUINode> InteractiveUINode::getLastChild() const {
        InteractiveUINode* inode = getLastChildPtr();
        return inode ? inode->sharedFromThis<InteractiveUINode>() : nullptr;
    }


    InteractiveUINode* InteractiveUINode::getLastChildPtr() const {
        for (UINode* node = UINode::getLastChildPtr(); node; node = node->getPrevSiblingPtr()) {
            InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
            if (inode) {
                return inode;
            }
        }
        return nullptr;
    }


    std::shared_ptr<InteractiveUINode> InteractiveUINode::getPrevSibling() const {
        InteractiveUINode* inode = getPrevSiblingPtr();
        return inode ? inode->sharedFromThis<InteractiveUINode>() : nullptr;
    }


    InteractiveUINode* InteractiveUINode::getPrevSiblingPtr() const {
        for (UINode* node = UINode::getPrevSiblingPtr(); node; node = node->getPrevSiblingPtr()) {
            InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
            if (inode) {
                return inode;
            }
        }
        return nullptr;
    }


    std::shared_ptr<InteractiveUINode> InteractiveUINode::getNextSibling() const {
        InteractiveUINode* inode = getNextSiblingPtr();
        return inode ? inode->sharedFromThis<InteractiveUINode>() : nullptr;
    }


    InteractiveUINode* InteractiveUINode::getNextSiblingPtr() const {
        for (UINode* node = UINode::getNextSiblingPtr(); node; node = node->getNextSiblingPtr()) {
            InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
            if (inode) {
                return inode;
            }
        }
        return nullptr;
    }


    std::shared_ptr<InteractiveUINode> InteractiveUINode::getRoot() const {
        InteractiveUINode* inode = getRootPtr();
        return inode ? inode->sharedFromThis<InteractiveUINode>() : nullptr;
    }


    InteractiveUINode* InteractiveUINode::getRootPtr() const {
        InteractiveUINode* result = const_cast<InteractiveUINode*>(this);
        for (UINode* node = UINode::getParentPtr(); node; node = node->getParentPtr()) {
            InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
            if (inode) {
                result = inode;
            }
        }
        return result;
    }


    bool InteractiveUINode::isEnabled() const {
        return (m_flags & ENABLED) == ENABLED;
    }


    void InteractiveUINode::setEnabled(bool v) {
        if (v != isEnabled()) {
            if (!v && contains(_focusedNode)) {
                _focusedNode->blur();
            }
            m_flags = v ? m_flags | ENABLED : m_flags & ~ENABLED;
            _setEnabledTree(this, !UINode::getParentPtr() || UINode::getParentPtr()->isEnabledTree());
            dispatchEvent(InteractiveUINodeEnabledChangedEvent(sharedFromThis<InteractiveUINode>()));
        }
    }


    bool InteractiveUINode::isFocused() const {
        return _focusedNode == this;
    }


    InteractiveUINode* InteractiveUINode::getFocusedNode() {
        return _focusedNode;
    }


    bool InteractiveUINode::setFocused(bool v) {
        if (v == isFocused()) {
            return true;
        }

        if (v) {
            if (!isEnabledTree()) {
                return false;
            }
            if (_focusedNode) {
                _focusedNode->blur();
            }
            _focusedNode = this;
            _setFocusedTree(this);
            InteractiveUINodeGotFocusEvent event(sharedFromThis<InteractiveUINode>());
            for (InteractiveUINode* inode = this; inode; inode = inode->getParentPtr()) {
                inode->dispatchEvent(event);
            }
        }

        else {
            _focusedNode = nullptr;
            _setFocusedTree(this);
            InteractiveUINodeLostFocusEvent event(sharedFromThis<InteractiveUINode>());
            for (InteractiveUINode* inode = this; inode; inode = inode->getParentPtr()) {
                inode->dispatchEvent(event);
            }
        }

        return true;
    }


    bool InteractiveUINode::isHighlighted() const {
        return (m_flags & HIGHLIGHTED) == HIGHLIGHTED;
    }


    void InteractiveUINode::setHighlighted(bool v) {
        if (v != isHighlighted()) {
            m_flags = v ? m_flags | HIGHLIGHTED : m_flags & ~HIGHLIGHTED;
            _setHighlightedTree(this, UINode::getParentPtr() && UINode::getParentPtr()->isHighlightedTree());
            dispatchEvent(InteractiveUINodeHighlightedChangedEvent(sharedFromThis<InteractiveUINode>()));
        }
    }


    bool InteractiveUINode::isPressed() const {
        return (m_flags & PRESSED) == PRESSED;
    }


    void InteractiveUINode::setPressed(bool v) {
        if (v != isPressed()) {
            m_flags = v ? m_flags | PRESSED : m_flags & ~PRESSED;
            _setPressedTree(this, UINode::getParentPtr() && UINode::getParentPtr()->isPressedTree());
            dispatchEvent(InteractiveUINodePressedChangedEvent(sharedFromThis<InteractiveUINode>()));
        }
    }


    bool InteractiveUINode::isSelected() const {
        return (m_flags & SELECTED) == SELECTED;
    }


    void InteractiveUINode::setSelected(bool v) {
        if (v != isSelected()) {
            m_flags = v ? m_flags | SELECTED : m_flags & ~SELECTED;
            _setSelectedTree(this, UINode::getParentPtr() && UINode::getParentPtr()->isSelectedTree());
            dispatchEvent(InteractiveUINodeSelectedChangedEvent(sharedFromThis<InteractiveUINode>()));
        }
    }


    bool InteractiveUINode::isError() const {
        return (m_flags & ERROR) == ERROR;
    }


    void InteractiveUINode::setError(bool v) {
        if (v != isError()) {
            m_flags = v ? m_flags | ERROR : m_flags & ~ERROR;
             _setErrorTree(this, UINode::getParentPtr() && UINode::getParentPtr()->isErrorTree());
            dispatchEvent(InteractiveUINodeErrorChangedEvent(sharedFromThis<InteractiveUINode>()));
        }
    }


    void InteractiveUINode::setNewChildState(const std::shared_ptr<UINode>& child) {
        UINode::setNewChildState(child);
        _setEnabledTree(child.get(), isEnabledTree());
        _setFocusedTree(child.get(), isFocusedTree());
        _setHighlightedTree(child.get(), isHighlightedTree());
        _setPressedTree(child.get(), isPressedTree());
        _setSelectedTree(child.get(), isSelectedTree());
        _setErrorTree(child.get(), isErrorTree());
    }


    void InteractiveUINode::_setEnabledTree(UINode* node, bool parentEnabledTree) {
        bool enabledTree;
        InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
        if (inode) {
            enabledTree = ((inode->m_flags & ENABLED) == ENABLED) && parentEnabledTree;
        }
        else {
            enabledTree = parentEnabledTree;
        }
        if (enabledTree != node->isEnabledTree()) {
            node->UINode::_setEnabledTree(enabledTree);
            for (UINode* child = node->getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                _setEnabledTree(child, enabledTree);
            }
        }
    }


    void InteractiveUINode::_setFocusedTree(UINode* node, bool parentFocusedTree) {
        bool focusedTree;
        InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
        if (inode) {
            focusedTree = inode->isFocused() || parentFocusedTree;
        }
        else {
            focusedTree = parentFocusedTree;
        }
        if (focusedTree != node->isFocusedTree()) {
            node->UINode::_setFocusedTree(focusedTree);
            for (UINode* child = node->getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                _setFocusedTree(child, focusedTree);
            }
        }
    }


    void InteractiveUINode::_setHighlightedTree(UINode* node, bool parentHighlightedTree) {
        bool highlightedTree;
        InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
        if (inode) {
            highlightedTree = inode->isHighlighted() || parentHighlightedTree;
        }
        else {
            highlightedTree = parentHighlightedTree;
        }
        if (highlightedTree != node->isHighlightedTree()) {
            node->UINode::_setHighlightedTree(highlightedTree);
            for (UINode* child = node->getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                _setHighlightedTree(child, highlightedTree);
            }
        }
    }


    void InteractiveUINode::_setPressedTree(UINode* node, bool parentPressedTree) {
        bool pressedTree;
        InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
        if (inode) {
            pressedTree = inode->isPressed() || parentPressedTree;
        }
        else {
            pressedTree = parentPressedTree;
        }
        if (pressedTree != node->isPressedTree()) {
            node->UINode::_setPressedTree(pressedTree);
            for (UINode* child = node->getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                _setPressedTree(child, pressedTree);
            }
        }
    }


    void InteractiveUINode::_setSelectedTree(UINode* node, bool parentSelectedTree) {
        bool selectedTree;
        InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
        if (inode) {
            selectedTree = inode->isSelected() || parentSelectedTree;
        }
        else {
            selectedTree = parentSelectedTree;
        }
        if (selectedTree != node->isSelectedTree()) {
            node->UINode::_setSelectedTree(selectedTree);
            for (UINode* child = node->getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                _setSelectedTree(child, selectedTree);
            }
        }
    }


    void InteractiveUINode::_setErrorTree(UINode* node, bool parentErrorTree) {
        bool errorTree;
        InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
        if (inode) {
            errorTree = inode->isError() || parentErrorTree;
        }
        else {
            errorTree = parentErrorTree;
        }
        if (errorTree != node->isErrorTree()) {
            node->UINode::_setErrorTree(errorTree);
            for (UINode* child = node->getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                _setErrorTree(child, errorTree);
            }
        }
    }


} //namespace algui
