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


    void UIVisualStateNode::setFocused(bool v) {
        if (isEnabled()) {
            if (v) {
                m_visualState = (VisualState)((unsigned)m_visualState | (unsigned)VisualState::Focused);
            }
            else {
                m_visualState = (VisualState)((unsigned)m_visualState & ~(unsigned)VisualState::Focused);
            }
        }
    }


    void UIVisualStateNode::onCalcChildState(UINode* parent) {
        UINode::onCalcChildState(parent);

        do {
            if (parent->IsVisualStateNode()) {
                const unsigned s = (unsigned)m_visualState;
                const unsigned e = s & (unsigned)VisualState::Enabled;

                UIVisualStateNode* p = static_cast<UIVisualStateNode*>(parent);
                const unsigned ps = (unsigned)p->m_treeVisualState;
                const unsigned pe = ps & (unsigned)VisualState::Enabled;

                m_treeVisualState = (VisualState)((s | ps) * e * pe);
                return;
            }
            parent = parent->getParentPtr();
        } while (parent);

        m_treeVisualState = m_visualState;
    }


    void UIVisualStateNode::onCalcRootState() {
        UINode::onCalcRootState();
        m_treeVisualState = m_visualState;
    }


} //namespace algui
