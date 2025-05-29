#include "algui/UINode.hpp"


namespace algui {


    void UINode::addChild(const std::shared_ptr<UINode>& child, const std::shared_ptr<UINode>& nextSibling) {
        TreeNode<UINode>::addChild(child, nextSibling);
        if (m_rendered) {
            child->setRendered(true);
            setLayoutValid(false);
        }
    }


    void UINode::removeChild(std::shared_ptr<UINode> child) {
        TreeNode<UINode>::removeChild(child);
        if (m_rendered) {
            child->setRendered(false);
            setLayoutValid(false);
        }
    }


    void UINode::setLeft(float v) {
        if (v != m_left) {
            m_left = v;
            m_geometryValid = false;
        }
    }


    void UINode::setTop(float v) {
        if (v != m_top) {
            m_top = v;
            m_geometryValid = false;
        }
    }


    void UINode::setWidth(float v) {
        if (v < 0) {
            v = 0;
        }
        if (v != m_width) {
            m_width = v;
            m_geometryValid = false;
        }
    }


    void UINode::setHeight(float v) {
        if (v < 0) {
            v = 0;
        }
        if (v != m_height) {
            m_height = v;
            m_geometryValid = false;
        }
    }


    void UINode::setPosition(float x, float y) {
        setLeft(x);
        setTop(y);
    }


    void UINode::setSize(float width, float height) {
        setWidth(width);
        setHeight(height);
    }


    void UINode::setGeometry(float x, float y, float width, float height) {
        setPosition(x, y);
        setSize(width, height);
    }


    void UINode::setScalingX(float v) {
        if (v != m_scalingX) {
            m_scalingX = v;
            m_geometryValid = false;
        }
    }


    void UINode::setScalingY(float v) {
        if (v != m_scalingY) {
            m_scalingY = v;
            m_geometryValid = false;
        }
    }


    void UINode::setScaling(float scalingX, float scalingY) {
        setScalingX(scalingX);
        setScalingY(scalingY);
    }


    void UINode::setVisible(bool v) {
        if (v != m_visible) {
            m_visible = v;
            if (getParent()) {
                getParent()->setLayoutValid(false);
            }
        }
    }


    void UINode::renderTree() {
        if (m_visible) {
            m_rendered = true;
            if (!m_layoutValid) {
                m_layoutValid = true;
                onLayout();
            }
            getParent() ? onCalcChildState(getParent()) : onCalcRootState();
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
        if (!m_geometryValid) {
            m_geometryValid = true;
            m_screenX1 = m_left * parent->m_screenScalingX + parent->m_screenX1;
            m_screenY1 = m_top * parent->m_screenScalingY + parent->m_screenY1;
            m_screenX2 = m_screenX1 + m_width * parent->m_screenScalingX;
            m_screenY2 = m_screenY1 + m_height * parent->m_screenScalingY;
            m_screenScalingX = m_scalingX * parent->m_screenScalingX;
            m_screenScalingY = m_scalingY * parent->m_screenScalingY;
        }
    }


    void UINode::onCalcRootState() {
        if (!m_geometryValid) {
            m_geometryValid = true;
            m_screenX1 = m_left;
            m_screenY1 = m_top;
            m_screenX2 = m_screenX1 + m_width;
            m_screenY2 = m_screenY1 + m_height;
            m_screenScalingX = m_scalingX;
            m_screenScalingY = m_scalingY;
        }
    }


    void UINode::setRendered(bool rendered) {
        m_rendered = rendered;
        for (UINode* child = getFirstChild(); child; child = child->getNextSibling()) {
            child->setRendered(rendered);
        }
    }


} //namespace algui
