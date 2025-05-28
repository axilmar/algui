#include "algui/UINode.hpp"


namespace algui {


    void UINode::removeChild(const std::shared_ptr<UINode>& child) {
        TreeNode<UINode>::removeChild(child);
        child->resetState();
    }


    void UINode::renderTree() {
        if (m_visible) {
            getParent() ? onCalcChildState(getParent()) : onCalcRootState();
            onLayout();
            onPaint();
        }
    }


    void UINode::onCalcChildState(UINode* parent) {
        m_screenX1 = m_x * parent->m_screenScalingX + parent->m_screenX1;
        m_screenY1 = m_y * parent->m_screenScalingY + parent->m_screenY1;
        m_screenX2 = m_screenX1 + m_width * parent->m_screenScalingX;
        m_screenY2 = m_screenY1 + m_height * parent->m_screenScalingY;
        m_screenScalingX = m_scalingX * parent->m_screenScalingX;
        m_screenScalingY = m_scalingY * parent->m_screenScalingY;
    }


    void UINode::onCalcRootState() {
        m_screenX1 = m_x;
        m_screenY1 = m_y;
        m_screenX2 = m_screenX1 + m_width;
        m_screenY2 = m_screenY1 + m_height;
        m_screenScalingX = m_scalingX;
        m_screenScalingY = m_scalingY;
    }


    void UINode::onPaint() const {
        for (UINode* child = getFirstChild(); child; child = child->getNextSibling()) {
            child->renderTree();
        }
    }


    void UINode::resetState() {
        onResetState();
        for (UINode* child = getFirstChild(); child; child = child->getNextSibling()) {
            child->resetState();
        }
    }


} //namespace algui
