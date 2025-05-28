#include "algui/UIVisualStateNode.hpp"


namespace algui {


    UIVisualStateNode::UIVisualStateNode()
        : m_visualState(VisualState::Enabled)
        , m_treeVisualState(VisualState::Enabled)
    {
    }


    void UIVisualStateNode::setEnabled(bool v) {
        if (v) {
            m_visualState = (VisualState)((unsigned)m_visualState | (unsigned)VisualState::Enabled);
        }
        else {
            m_visualState = VisualState::Disabled;
        }
    }


    void UIVisualStateNode::setHighlighted(bool v) {
        if (isEnabled()) {
            if (v) {
                m_visualState = (VisualState)((unsigned)m_visualState | (unsigned)VisualState::Highlighted);
            }
            else {
                m_visualState = (VisualState)((unsigned)m_visualState & ~(unsigned)VisualState::Highlighted);
            }
        }
    }


    void UIVisualStateNode::setPressed(bool v) {
        if (isEnabled()) {
            if (v) {
                m_visualState = (VisualState)((unsigned)m_visualState | (unsigned)VisualState::Pressed);
            }
            else {
                m_visualState = (VisualState)((unsigned)m_visualState & ~(unsigned)VisualState::Pressed);
            }
        }
    }


    void UIVisualStateNode::setSelected(bool v) {
        if (isEnabled()) {
            if (v) {
                m_visualState = (VisualState)((unsigned)m_visualState | (unsigned)VisualState::Selected);
            }
            else {
                m_visualState = (VisualState)((unsigned)m_visualState & ~(unsigned)VisualState::Selected);
            }
        }
    }


    bool UIVisualStateNode::setFocused(bool v) {
        //do nothing if no change
        if (isFocused() == v) {
            return true;
        }

        //set the focus
        if (v) {
            //if disabled or part of disabled tree, it cannot get the focus
            if (!isEnabled() || getTreeVisualState() == VisualState::Disabled) {
                return false;
            }

            //set the visual state
            m_visualState = (VisualState)((unsigned)m_visualState | (unsigned)VisualState::Focused);
        }

        //else reset the focus
        else {
            m_visualState = (VisualState)((unsigned)m_visualState & ~(unsigned)VisualState::Focused);
        }

        return true;
    }


    UIVisualStateNode* UIVisualStateNode::getParent() const {
        for (UINode* node = UINode::getParent(); node; node = node->UINode::getParent()) {
            if (node->isVisualStateNode()) {
                return static_cast<UIVisualStateNode*>(node);
            }
        }
        return nullptr;
    }


    UIVisualStateNode* UIVisualStateNode::getPrevSibling() const {
        for (UINode* prevSibling = UINode::getPrevSibling(); prevSibling; prevSibling = prevSibling->UINode::getPrevSibling()) {
            if (prevSibling->isVisualStateNode()) {
                return static_cast<UIVisualStateNode*>(prevSibling);
            }
        }
        return nullptr;
    }


    UIVisualStateNode* UIVisualStateNode::getNextSibling() const {
        for (UINode* nextSibling = UINode::getNextSibling(); nextSibling; nextSibling = nextSibling->UINode::getNextSibling()) {
            if (nextSibling->isVisualStateNode()) {
                return static_cast<UIVisualStateNode*>(nextSibling);
            }
        }
        return nullptr;
    }


    UIVisualStateNode* UIVisualStateNode::getFirstChild() const {
        for (UINode* child = UINode::getFirstChild(); child; child = child->UINode::getNextSibling()) {
            if (child->isVisualStateNode()) {
                return static_cast<UIVisualStateNode*>(child);
            }
        }
        return nullptr;
    }


    UIVisualStateNode* UIVisualStateNode::getLastChild() const {
        for (UINode* child = UINode::getLastChild(); child; child = child->UINode::getPrevSibling()) {
            if (child->isVisualStateNode()) {
                return static_cast<UIVisualStateNode*>(child);
            }
        }
        return nullptr;
    }


    UIVisualStateNode* UIVisualStateNode::getRoot() const {
        UINode* node = const_cast<UIVisualStateNode*>(this);
        UIVisualStateNode* result = nullptr;
        do {
            if (node->isVisualStateNode()) {
                result = static_cast<UIVisualStateNode*>(node);
            }
            node = node->getParent();
        } while (node);
        return result;
    }


    void UIVisualStateNode::onCalcChildState(UINode* parent) {
        UIPositionedNode::onCalcChildState(parent);

        do {
            if (parent->isVisualStateNode()) {
                const unsigned s = (unsigned)m_visualState;
                const unsigned e = s & (unsigned)VisualState::Enabled;

                UIVisualStateNode* p = static_cast<UIVisualStateNode*>(parent);
                const unsigned ps = (unsigned)p->m_treeVisualState;
                const unsigned pe = ps & (unsigned)VisualState::Enabled;

                m_treeVisualState = (VisualState)((s | ps) * e * pe);
                return;
            }
            parent = parent->getParent();
        } while (parent);

        m_treeVisualState = m_visualState;
    }


    void UIVisualStateNode::onCalcRootState() {
        UIPositionedNode::onCalcRootState();
        m_treeVisualState = m_visualState;
    }


} //namespace algui
