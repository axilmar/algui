#include <limits>
#include "algui/Widget.hpp"


namespace algui {


    Widget::Widget() 
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
        , m_visible(true)
        , m_geometryDirty(true)
        , m_geometryTreeDirty(true)
        , m_layoutDirty(true)
        , m_layoutTreeDirty(true)
    {
    }


    float Widget::getLeft() const {
        return m_left;
    }


    void Widget::setLeft(float v) {
        if (v != m_left) {
            m_left = v;
            if (getParent()) {
                getParent()->invalidateLayout();
            }
            invalidateScreenGeometry();
        }
    }


    float Widget::getTop() const {
        return m_top;
    }


    void Widget::setTop(float v) {
        if (v != m_top) {
            m_top = v;
            if (getParent()) {
                getParent()->invalidateLayout();
            }
            invalidateScreenGeometry();
        }
    }


    float Widget::getRight() const {
        return m_left + m_width;
    }


    void Widget::setRight(float v) {
        setWidth(v - m_left);
    }


    float Widget::getBottom() const {
        return m_top + m_height;
    }


    void Widget::setBottom(float v) {
        setHeight(v - m_top);
    }


    float Widget::getWidth() const {
        return m_width;
    }


    void Widget::setWidth(float v) {
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


    float Widget::getHeight() const {
        return m_height;
    }


    void Widget::setHeight(float v) {
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


    float Widget::getMinWidth() const {
        return m_minWidth;
    }


    void Widget::setMinWidth(float v) {
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


    float Widget::getMinHeight() const {
        return m_minHeight;
    }


    void Widget::setMinHeight(float v) {
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


    float Widget::getMaxWidth() const {
        return m_maxWidth;
    }


    void Widget::setMaxWidth(float v) {
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


    float Widget::getMaxHeight() const {
        return m_maxHeight;
    }


    void Widget::setMaxHeight(float v) {
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


    float Widget::getScalingX() const {
        return m_scalingX;
    }


    void Widget::setScalingX(float v) {
        if (v < 0) {
            v = 0;
        }
        if (v != m_scalingX) {
            m_scalingX = v;
            invalidateScreenGeometry();
        }
    }


    float Widget::getScalingY() const {
        return m_scalingY;
    }


    void Widget::setScalingY(float v) {
        if (v < 0) {
            v = 0;
        }
        if (v != m_scalingY) {
            m_scalingY = v;
            invalidateScreenGeometry();
        }
    }


    bool Widget::isVisible() const {
        return m_visible;
    }


    void Widget::setVisible(bool v) {
        if (v != m_visible) {
            m_visible = v;
            if (getParent()) {
                getParent()->invalidateLayout();
            }
        }
    }


    void Widget::invalidateGeometry() {
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


    void Widget::invalidateLayout() {
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


    void Widget::render() {
        updateGeometry();
        updateLayout();
        updateScreenGeometry(false);
        paint();
    }


    void Widget::updateGeometry() {
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


    void Widget::updateLayout() {
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


    void Widget::invalidateScreenGeometry() {
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
    
    
    void Widget::updateScreenGeometry(bool force) {
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


    void Widget::paint() const {
        if (m_visible) {
            onPaint();
            for (Widget* child = getFirstChild(); child; child = child->getNextSibling()) {
                child->paint();
            }
        }
    }


} //namespace algui
