#include "algui/InteractiveUINode.hpp"
#include "algui/InteractiveUINodeEnabledChangedEvent.hpp"


namespace algui {


    enum FLAGS {
        ENABLED = 1 << 0,
    };


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
        if (v != ((m_flags & ENABLED) == ENABLED)) {
            m_flags = v ? m_flags | ENABLED : m_flags & ~ENABLED;
            _setEnabledTree(this, _isEnabledAncestorTree());
            dispatchEvent(InteractiveUINodeEnabledChangedEvent(sharedFromThis<InteractiveUINode>()));
        }
    }

    bool InteractiveUINode::_isEnabledAncestorTree() const {
        InteractiveUINode* parent = getParentPtr();
        return !parent || parent->isEnabledTree();
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
        node->UINode::_setEnabledTree(enabledTree);
        for (UINode* child = node->getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
            _setEnabledTree(child, enabledTree);
        }
    }


} //namespace algui
