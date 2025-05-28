#include "algui/UINode.hpp"


namespace algui {


    void UINode::addChild(const std::shared_ptr<UINode>& child, const std::shared_ptr<UINode>& nextSibling) {
        TreeNode<UINode>::addChild(child, nextSibling);
        if (m_rendered) {
            child->setRendered(true);
        }
    }


    void UINode::removeChild(const std::shared_ptr<UINode>& child) {
        TreeNode<UINode>::removeChild(child);
        if (m_rendered) {
            child->setRendered(false);
        }
    }


    void UINode::renderTree() {
        if (m_visible) {
            m_rendered = true;
            getParent() ? onCalcChildState(getParent()) : onCalcRootState();
            onLayout();
            onPaint();
            for (UINode* child = getFirstChild(); child; child = child->getNextSibling()) {
                child->renderTree();
            }
        }
        else if (!m_rendered) {
            setRendered(true);
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


    void UINode::setRendered(bool rendered) {
        m_rendered = rendered;
        for (UINode* child = getFirstChild(); child; child = child->getNextSibling()) {
            child->setRendered(rendered);
        }
    }


} //namespace algui
