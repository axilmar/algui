#include "algui/UILayoutNode.hpp"


namespace algui {


    UILayoutNode::~UILayoutNode() {
        if (m_layout) {
            m_layout->removeOwner(this);
        }
    }


    void UILayoutNode::setVisible(bool v) {
        if (v != UIPositionedNode::isVisible()) {
            UIPositionedNode::setVisible(v);
            if (getParent()) {
                getParent()->invalidateLayout();
            }
        }
    }


    bool UILayoutNode::isGeometryValid() const {
        return UIPositionedNode::isGeometryValid() && m_layoutValid;
    }


    void UILayoutNode::invalidateGeometry() {
        UIPositionedNode::invalidateGeometry();
        m_layoutValid = false;
    }


    UILayoutNode* UILayoutNode::getParent() const {
        for (UINode* node = UINode::getParent(); node; node = node->UINode::getParent()) {
            if (node->isLayoutNode()) {
                return static_cast<UILayoutNode*>(node);
            }
        }
        return nullptr;
    }


    UILayoutNode* UILayoutNode::getPrevSibling() const {
        for (UINode* prevSibling = UINode::getPrevSibling(); prevSibling; prevSibling = prevSibling->UINode::getPrevSibling()) {
            if (prevSibling->isLayoutNode()) {
                return static_cast<UILayoutNode*>(prevSibling);
            }
        }
        return nullptr;
    }


    UILayoutNode* UILayoutNode::getNextSibling() const {
        for (UINode* nextSibling = UINode::getNextSibling(); nextSibling; nextSibling = nextSibling->UINode::getNextSibling()) {
            if (nextSibling->isLayoutNode()) {
                return static_cast<UILayoutNode*>(nextSibling);
            }
        }
        return nullptr;
    }


    UILayoutNode* UILayoutNode::getFirstChild() const {
        for (UINode* child = UINode::getFirstChild(); child; child = child->UINode::getNextSibling()) {
            if (child->isLayoutNode()) {
                return static_cast<UILayoutNode*>(child);
            }
        }
        return nullptr;
    }


    UILayoutNode* UILayoutNode::getLastChild() const {
        for (UINode* child = UINode::getLastChild(); child; child = child->UINode::getPrevSibling()) {
            if (child->isLayoutNode()) {
                return static_cast<UILayoutNode*>(child);
            }
        }
        return nullptr;
    }


    UILayoutNode* UILayoutNode::getRoot() const {
        UINode* node = const_cast<UILayoutNode*>(this);
        UILayoutNode* result = nullptr;
        do {
            if (node->isLayoutNode()) {
                result = static_cast<UILayoutNode*>(node);
            }
            node = node->getParent();
        } while (node);
        return result;
    }


    void UILayoutNode::setLayout(const std::shared_ptr<Layout>& layout) {
        if (layout == m_layout) {
            return;
        }

        if (m_layout) {
            m_layout->removeOwner(this);
        }

        m_layout = layout;

        if (m_layout) {
            m_layout->addOwner(this);
        }

        invalidateLayout();
    }


    void UILayoutNode::setMinPreferredWidth(float v) {
        if (v != m_minPreferredWidth) {
            m_minPreferredWidth = v;
            setPreferredSizeChanged();
        }
    }


    void UILayoutNode::setMinPreferredHeight(float v) {
        if (v != m_minPreferredHeight) {
            m_minPreferredHeight = v;
            setPreferredSizeChanged();
        }
    }


    void UILayoutNode::setMinPreferredSize(float w, float h) {
        if (w != m_minPreferredWidth || h != m_minPreferredHeight) {
            m_minPreferredWidth = w;
            m_minPreferredHeight = h;
            setPreferredSizeChanged();
        }
    }


    void UILayoutNode::setMaxPreferredWidth(float v) {
        if (v != m_maxPreferredWidth) {
            m_maxPreferredWidth = v;
            setPreferredSizeChanged();
        }
    }


    void UILayoutNode::setMaxPreferredHeight(float v) {
        if (v != m_maxPreferredHeight) {
            m_maxPreferredHeight = v;
            setPreferredSizeChanged();
        }
    }


    void UILayoutNode::setMaxPreferredSize(float w, float h) {
        if (w != m_maxPreferredWidth || h != m_maxPreferredHeight) {
            m_maxPreferredWidth = w;
            m_maxPreferredHeight = h;
            setPreferredSizeChanged();
        }
    }


    void UILayoutNode::setPreferredWidth(float v) {
        if (v != m_preferredWidth) {
            m_preferredWidth = v;
            setPreferredSizeChanged();
        }
    }


    void UILayoutNode::setPreferredHeight(float v) {
        if (v != m_preferredHeight) {
            m_preferredHeight = v;
            setPreferredSizeChanged();
        }
    }


    void UILayoutNode::setPreferredSize(float w, float h) {
        if (w != m_preferredWidth || h != m_preferredHeight) {
            m_preferredWidth = w;
            m_preferredHeight = h;
            setPreferredSizeChanged();
        }
    }


    void UILayoutNode::setPreferredSize(float minWidth, float minHeight, float width, float height, float maxWidth, float maxHeight) {
        if (minWidth != m_minPreferredWidth || 
            minHeight != m_minPreferredHeight ||
            width != m_preferredWidth || 
            height != m_preferredHeight ||
            maxWidth != m_maxPreferredWidth || 
            maxHeight != m_maxPreferredHeight)
        {
            m_minPreferredWidth = minWidth;
            m_minPreferredHeight = minHeight;
            m_preferredWidth = width;
            m_preferredHeight = height;
            m_maxPreferredWidth = maxWidth;
            m_maxPreferredHeight = maxHeight;
            setPreferredSizeChanged();
        }
    }


    void UILayoutNode::onChildPositionTypeChanged(const UIPositionedNode* child) {
        if (child->isLayoutNode()) {
            onChildPreferredSizeChanged(static_cast<const UILayoutNode*>(child));
        }
    }


    void UILayoutNode::onChildPreferredSizeChanged(const UILayoutNode* child) {
        if (m_layout) {
            m_calculatingPreferredSize = true;
            m_preferredSizeChanged = false;
            m_layout->calcPreferredSize(this);
            m_calculatingPreferredSize = false;
            if (m_preferredSizeChanged) {
                m_preferredSizeChanged = false;
                if (getParent()) {
                    getParent()->onChildPreferredSizeChanged(child);
                }
            }
        }
    }


    void UILayoutNode::onCalcGeometry(const UINode* parent) {
        UIPositionedNode::onCalcGeometry(parent);
        if (!m_layoutValid || (parent->isLayoutNode() && static_cast<const UILayoutNode*>(parent)->m_layoutValid)) {
            m_layoutValid = false;
            if (m_layout) {
                m_layout->doLayout(this);
            }
        }
    }

    void UILayoutNode::onCalcGeometry() {
        UIPositionedNode::onCalcGeometry();
        if (!m_layoutValid && m_layout) {
            m_layout->doLayout(this);
        }
    }


    void UILayoutNode::setChildGeometryInvalid(UINode* child) const {
        UIPositionedNode::setChildGeometryInvalid(child);
        if (!UINode::isGeometryValid() && child->isLayoutNode()) {
            UILayoutNode* layoutChild = static_cast<UILayoutNode*>(child);
            if (layoutChild->getPositionType() == PositionType::Managed) {
                layoutChild->m_layoutValid = false;
            }
        }
    }


    void UILayoutNode::setGeometryValid() {
        UIPositionedNode::setGeometryValid();
        m_layoutValid = true;
    }


    void UILayoutNode::onResourceChanged(const Layout& layout) {
        invalidateLayout();
    }


    void UILayoutNode::invalidateLayout() {
        UINode::invalidateGeometry();
        m_layoutValid = false;
    }


    void UILayoutNode::setPreferredSizeChanged() {
        if (m_calculatingPreferredSize) {
            m_preferredSizeChanged = true;
        }
        else if (getParent()) {
            getParent()->onChildPreferredSizeChanged(this);
        }
    }


} //namespace algui
