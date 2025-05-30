#include <limits>
#include "algui/Widget.hpp"


namespace algui {


    static Widget* focusedWidget = nullptr;


    Widget::Widget() noexcept
        : m_left(0)
        , m_top(0)
        , m_width(0)
        , m_height(0)
        , m_minWidth(0)
        , m_minHeight(0)
        , m_maxWidth(FLT_MAX)
        , m_maxHeight(FLT_MAX)
        , m_x1(0)
        , m_y1(0)
        , m_x2(0)
        , m_y2(0)
        , m_scalingX(1)
        , m_scalingY(1)
        , m_screenScalingX(1)
        , m_screenScalingY(1)
        , m_geometryDirty(true)
        , m_geometryTreeDirty(true)
        , m_layoutDirty(true)
        , m_layoutTreeDirty(true)
        , m_visible(true)
        , m_enabled(true)
        , m_highlighted(false)
        , m_pressed(false)
        , m_selected(false)
        , m_focused(false)
        , m_focusable(false)
        , m_valid(true)
        , m_visibleTree(true)
        , m_enabledTree(true)
        , m_highlightedTree(false)
        , m_pressedTree(false)
        , m_selectedTree(false)
        , m_focusedTree(false)
        , m_validTree(true)
    {
    }


    Widget::~Widget() noexcept {
        if (this == focusedWidget) {
            focusedWidget = nullptr;
        }
    }


    float Widget::getLeft() const noexcept {
        return m_left;
    }


    void Widget::setLeft(float v) noexcept {
        if (v != m_left) {
            m_left = v;
            if (getParent()) {
                getParent()->invalidateLayout();
            }
            invalidateScreenGeometry();
        }
    }


    float Widget::getTop() const noexcept {
        return m_top;
    }


    void Widget::setTop(float v) noexcept {
        if (v != m_top) {
            m_top = v;
            if (getParent()) {
                getParent()->invalidateLayout();
            }
            invalidateScreenGeometry();
        }
    }


    float Widget::getRight() const noexcept {
        return m_left + m_width;
    }


    void Widget::setRight(float v) noexcept {
        setWidth(v - m_left);
    }


    float Widget::getBottom() const noexcept {
        return m_top + m_height;
    }


    void Widget::setBottom(float v) noexcept {
        setHeight(v - m_top);
    }


    float Widget::getWidth() const noexcept {
        return m_width;
    }


    void Widget::setWidth(float v) noexcept {
        //width can't be less than min width
        if (v < m_minWidth) {
            v = m_minWidth;
        }

        //width can't be greater than max width
        else if (v > m_maxWidth) {
            v = m_maxWidth;
        }

        //if changed
        if (v != m_width) {
            //save it
            m_width = v;

            //since width changed, the parent's width might change
            if (getParent()) {
                getParent()->invalidateGeometry();
            }

            //since width changed, the children should be laid out again
            invalidateLayout();

            //screen geometry will also need recalculation
            invalidateScreenGeometry();
        }
    }


    float Widget::getHeight() const noexcept {
        return m_height;
    }


    void Widget::setHeight(float v) noexcept {
        //height cannot be less than min height
        if (v < m_minHeight) {
            v = m_minHeight;
        }

        //height cannot be greater then max height
        else if (v > m_maxHeight) {
            v = m_maxHeight;
        }

        //if changed
        if (v != m_height) {
            //save it
            m_height = v;

            //since height changed, the parent's height might change
            if (getParent()) {
                getParent()->invalidateGeometry();
            }

            //since height changed, the children should be laid out again
            invalidateLayout();

            //screen geometry will also need recalculation
            invalidateScreenGeometry();
        }
    }


    float Widget::getMinWidth() const noexcept {
        return m_minWidth;
    }


    void Widget::setMinWidth(float v) noexcept {
        //min width cannot be negative
        if (v < 0) {
            v = 0;
        }

        //min width cannot be greater than max width
        else if (v > m_maxWidth) {
            v = m_maxWidth;
        }

        //if changed
        if (v != m_minWidth) {
            //save it
            m_minWidth = v;

            //clamp width to min width
            if (m_width < m_minWidth) {
                setWidth(m_minWidth);
            }

            //since min width changed, the parent's min width may change
            if (getParent()) {
                getParent()->invalidateGeometry();
            }
        }
    }


    float Widget::getMinHeight() const noexcept {
        return m_minHeight;
    }


    void Widget::setMinHeight(float v) noexcept {
        //min height cannot be negative
        if (v < 0) {
            v = 0;
        }

        //min height cannot be greater than max height
        else if (v > m_maxHeight) {
            v = m_maxHeight;
        }

        //if changed
        if (v != m_minHeight) {
            //save it
            m_minHeight = v;

            //clamp height to min height
            if (m_height < m_minHeight) {
                setHeight(m_height);
            }

            //since min height changed, the parent's min height may change
            if (getParent()) {
                getParent()->invalidateGeometry();
            }
        }
    }


    float Widget::getMaxWidth() const noexcept {
        return m_maxWidth;
    }


    void Widget::setMaxWidth(float v) noexcept {
        //max width cannot be less than min width
        if (v < m_minWidth) {
            v = m_minWidth;
        }

        //if changed
        if (v != m_maxWidth) {
            //save it
            m_maxWidth = v;

            //clamp width to max width
            if (m_width > m_maxWidth) {
                setWidth(m_maxWidth);
            }

            //since max width changed, the parent's max width may change
            if (getParent()) {
                getParent()->invalidateGeometry();
            }
        }
    }


    float Widget::getMaxHeight() const noexcept {
        return m_maxHeight;
    }


    void Widget::setMaxHeight(float v) noexcept {
        //max height cannot be less than min height
        if (v < m_minHeight) {
            v = m_minHeight;
        }

        //if changed
        if (v != m_maxHeight) {
            //save it
            m_maxHeight = v;

            //clamp height to max height
            if (m_height > m_maxHeight) {
                setHeight(m_maxHeight);
            }

            //since max height changed, the parent's max height may also change
            if (getParent()) {
                getParent()->invalidateGeometry();
            }
        }
    }


    float Widget::getScalingX() const noexcept {
        return m_scalingX;
    }


    void Widget::setScalingX(float v) noexcept {
        if (v < 0) {
            v = 0;
        }
        if (v != m_scalingX) {
            m_scalingX = v;
            invalidateScreenGeometry();
        }
    }


    float Widget::getScalingY() const noexcept {
        return m_scalingY;
    }


    void Widget::setScalingY(float v) noexcept {
        if (v < 0) {
            v = 0;
        }
        if (v != m_scalingY) {
            m_scalingY = v;
            invalidateScreenGeometry();
        }
    }


    float Widget::getX1() const noexcept {
        return m_x1;
    }


    float Widget::getY1() const noexcept {
        return m_y1;
    }


    float Widget::getX2() const noexcept {
        return m_x2;
    }


    float Widget::getY2() const noexcept {
        return m_y2;
    }


    bool Widget::isVisible() const noexcept {
        return m_visible;
    }


    bool Widget::isVisibleTree() const noexcept {
        return m_visibleTree;
    }


    void Widget::setVisible(bool v) noexcept {
        if (v != m_visible) {
            m_visible = v;
            updateVisibleTree();
            if (getParent()) {
                getParent()->invalidateLayout();
            }
        }
    }


    bool Widget::isEnabled() const noexcept {
        return m_enabled;
    }


    bool Widget::isEnabledTree() const noexcept {
        return m_enabledTree;
    }


    void Widget::setEnabled(bool v) noexcept {
        if (v == m_enabled) {
            return;
        }

        if (v) {
            m_enabled = true;
        }

        //if it is disabled and contains the focus,
        //then the focus is lost
        else {
            if (contains(focusedWidget)) {
                focusedWidget->setFocused(false);
            }
            m_enabled = false;
        }

        updateEnabledTree();
    }


    bool Widget::isHighlighted() const noexcept {
        return m_highlighted;
    }


    bool Widget::isHighlightedTree() const noexcept {
        return m_highlightedTree;
    }


    void Widget::setHighlighted(bool v) noexcept {
        if (v != m_highlighted) {
            m_highlighted = v;
            updateHighlightedTree();
        }
    }


    bool Widget::isPressed() const noexcept {
        return m_pressed;
    }


    bool Widget::isPressedTree() const noexcept {
        return m_pressedTree;
    }


    void Widget::setPressed(bool v) noexcept {
        if (v != m_pressed) {
            m_pressed = v;
            updatePressedTree();
        }
    }


    bool Widget::isSelected() const noexcept {
        return m_selected;
    }


    bool Widget::isSelectedTree() const noexcept {
        return m_selectedTree;
    }


    void Widget::setSelected(bool v) noexcept {
        if (v != m_selected) {
            m_selected = v;
            updateSelectedTree();
        }
    }


    bool Widget::isFocused() const noexcept {
        return m_focused;
    }


    bool Widget::isFocusedTree() const noexcept {
        return m_focusedTree;
    }


    bool Widget::setFocused(bool v) noexcept {
        if (v == m_focused) {
            return true;
        }

        //set the focus
        if (v) {
            //must be focusable and enabled
            if (!m_focusable || !m_enabledTree) {
                return false;
            }

            //remove the focus from previous widget
            if (focusedWidget) {
                focusedWidget->setFocused(false);
            }

            //set the focus to this widget
            m_focused = v;
            updateFocusedTree();
            focusedWidget = this;

            //TODO got focus events
        }

        //remove the focus
        else {
            m_focused = false;
            updateFocusedTree();
            focusedWidget = nullptr;

            //TODO lost focus events
        }

        return true;
    }


    Widget* Widget::getFocused() noexcept {
        return focusedWidget;
    }


    bool Widget::isFocusable() const noexcept {
        return m_focusable;
    }


    void Widget::setFocusable(bool v) noexcept {
        m_focusable = v;
    }


    bool Widget::isValid() const noexcept {
        return m_valid;
    }


    bool Widget::isValidTree() const noexcept {
        return m_validTree;
    }


    void Widget::setValid(bool v) noexcept {
        if (v != m_valid) {
            m_valid = v;
            updateValidTree();
        }
    }


    void Widget::invalidateGeometry() noexcept {
        if (m_geometryDirty) {
            return;
        }

        m_geometryDirty = true;

        for (Widget* ancestor = getParent(); ancestor; ancestor = ancestor->getParent()) {
            if (ancestor->m_geometryTreeDirty) {
                break;
            }
            ancestor->m_geometryTreeDirty = true;
        }
    }


    void Widget::invalidateLayout() noexcept {
        if (m_layoutDirty) {
            return;
        }

        m_layoutDirty = true;

        for (Widget* ancestor = getParent(); ancestor; ancestor = ancestor->getParent()) {
            if (ancestor->m_layoutTreeDirty) {
                break;
            }
            ancestor->m_layoutTreeDirty = true;
        }
    }


    void Widget::render() noexcept {
        updateGeometry();
        updateLayout();
        updateScreenGeometry(false);
        paint();
    }


    void Widget::updateGeometry() noexcept {
        if (m_geometryTreeDirty) {
            for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
                child->updateGeometry();
            }
            m_geometryTreeDirty = false;
        }
        if (m_geometryDirty) {
            onGeometry();
            m_geometryDirty = false;
        }
    }


    void Widget::updateLayout() noexcept {
        if (m_layoutDirty) {
            onLayout();
            m_layoutDirty = false;
        }
        if (m_layoutTreeDirty) {
            for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
                child->updateLayout();
            }
            m_layoutTreeDirty = false;
        }
    }


    void Widget::invalidateScreenGeometry() noexcept {
        if (m_screenGeometryDirty) {
            return;
        }

        m_screenGeometryDirty = true;

        for (Widget* ancestor = getParent(); ancestor; ancestor = ancestor->getParent()) {
            if (ancestor->m_screenGeometryTreeDirty) {
                break;
            }
            ancestor->m_screenGeometryTreeDirty = true;
        }
    }
    
    
    void Widget::updateScreenGeometry(bool force) noexcept {
        //update screen geometry of widget
        if (m_screenGeometryDirty || force) {
            //calc screen geometry
            Widget* parent = getParent();
            if (parent) {
                m_x1 = m_left * parent->m_screenScalingX + parent->m_x1;
                m_y1 = m_top * parent->m_screenScalingY + parent->m_y1;
                m_x2 = m_x1 + m_width * parent->m_screenScalingX;
                m_y2 = m_y1 + m_height * parent->m_screenScalingY;
                m_screenScalingX = m_scalingX * parent->m_screenScalingX;
                m_screenScalingY = m_scalingY * parent->m_screenScalingY;
            }
            else {
                m_x1 = m_left;
                m_y1 = m_top;
                m_x2 = m_x1 + m_width;
                m_y2 = m_y1 + m_height;
                m_screenScalingX = m_scalingX;
                m_screenScalingY = m_scalingY;
            }

            //force update screen geometry of the tree
            for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
                child->updateScreenGeometry(true);
            }

            //reset flags
            m_screenGeometryDirty = false;
            m_screenGeometryTreeDirty = false;
        }

        //else update screen geometry down the tree
        else if (m_screenGeometryTreeDirty) {
            //update screen geometry of tree
            for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
                child->updateScreenGeometry(false);
            }

            //reset flags
            m_screenGeometryTreeDirty = false;
        }
    }


    void Widget::paint() const noexcept {
        if (m_visible) {
            onPaint();
            for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
                child->paint();
            }
        }
    }


    void Widget::updateVisibleTree() noexcept {
        m_visibleTree = m_visible && (!getParent() || getParent()->m_visibleTree);
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            child->updateVisibleTree();
        }
    }


    void Widget::updateEnabledTree() noexcept {
        m_enabledTree = m_enabled && (!getParent() || getParent()->m_enabledTree);
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            child->updateEnabledTree();
        }
    }


    void Widget::updateHighlightedTree() noexcept {
        m_highlightedTree = m_highlighted || (getParent() && getParent()->m_highlightedTree);
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            child->updateHighlightedTree();
        }
    }


    void Widget::updatePressedTree() noexcept {
        m_pressedTree = m_pressed || (getParent() && getParent()->m_pressedTree);
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            child->updatePressedTree();
        }
    }


    void Widget::updateSelectedTree() noexcept {
        m_selectedTree = m_selected || (getParent() && getParent()->m_selectedTree);
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            child->updateSelectedTree();
        }
    }


    void Widget::updateFocusedTree() noexcept {
        m_focusedTree = m_focused || (getParent() && getParent()->m_focusedTree);
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            child->updateFocusedTree();
        }
    }


    void Widget::updateValidTree() noexcept {
        m_validTree = m_valid && (!getParent() || getParent()->m_validTree);
        for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
            child->updateValidTree();
        }
    }


} //namespace algui
