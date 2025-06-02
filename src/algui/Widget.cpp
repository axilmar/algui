#pragma warning (disable: 4309)
#include "allegro5/allegro.h"
#include "algui/Widget.hpp"


namespace algui {


    /**************************************************************************
        PUBLIC
     **************************************************************************/ 


    Widget::Widget()
        : m_scalingX(1)
        , m_scalingY(1)
        , m_screenLeft(0)
        , m_screenTop(0)
        , m_screenRight(0)
        , m_screenBottom(0)
        , m_screenScalingX(1)
        , m_screenScalingY(1)
        , m_visible(true)
        , m_screenGeometryDirty(false)
    {
    }


    //Sets the left coordinate.
    void Widget::setLeft(const Coord& left) {
        if (left != m_left) {
            m_left = left;
            _invalidateScreenGeometry();
        }
    }


    //Sets the top coordinate.
    void Widget::setTop(const Coord& top) {
        if (top != m_top) {
            m_top = top;
            _invalidateScreenGeometry();
        }
    }


    //Sets the width.
    void Widget::setWidth(const Coord& width) {
        if (width != m_width) {
            m_width = width;
            _invalidateScreenGeometry();
        }
    }


    //Sets the height.
    void Widget::setHeight(const Coord& height) {
        if (height != m_height) {
            m_height = height;
            _invalidateScreenGeometry();
        }
    }


    //Sets the interior scaling factor of the component along the X axis.
    void Widget::setScalingX(const float scalingX) {
        if (scalingX != m_scalingX) {
            m_scalingX = scalingX;
            _invalidateScreenGeometry();
        }
    }


    //Sets the interior scaling factor of the component along the Y axis.
    void Widget::setScalingY(const float scalingY) {
        if (scalingY != m_scalingY) {
            m_scalingY = scalingY;
            _invalidateScreenGeometry();
        }
    }


    //Sets the visible state of the widget.
    void Widget::setVisible(bool visible) {
        if (visible != m_visible) {
            m_visible = visible;
        }
    }


    //Renders the tree into the target bitmap.
    void Widget::render() {
        _paint(false);
    }


    /**************************************************************************
        PRIVATE
    **************************************************************************/


    //make the screen geometry invalid
    void Widget::_invalidateScreenGeometry() {
        m_screenGeometryDirty = true;
    }


    //calculate screen geometry
    void Widget::_calcScreenGeometry() {
        Widget* parent = getParent();

        if (parent) {
            const float width = parent->m_screenRight - parent->m_screenLeft;
            const float height = parent->m_screenBottom - parent->m_screenTop;
            m_screenLeft = parent->m_screenLeft + m_left.calcPixelValue(width, parent->m_screenScalingX);
            m_screenTop = parent->m_screenTop + m_top.calcPixelValue(height, parent->m_screenScalingY);
            m_screenRight = m_screenLeft + m_width.calcPixelValue(width, parent->m_screenScalingX);
            m_screenBottom = m_screenTop + m_height.calcPixelValue(height, parent->m_screenScalingY);
            m_screenScalingX = parent->m_screenScalingX * m_scalingX;
            m_screenScalingY = parent->m_screenScalingY * m_scalingY;
        }

        else {
            ALLEGRO_BITMAP* const target = al_get_target_bitmap();
            const float width = al_get_bitmap_width(target);
            const float height = al_get_bitmap_height(target);
            m_screenLeft = m_left.calcPixelValue(width);
            m_screenTop = m_top.calcPixelValue(height);
            m_screenRight = m_screenLeft + m_width.calcPixelValue(width);
            m_screenBottom = m_screenTop + m_height.calcPixelValue(height);
            m_screenScalingX = m_scalingX;
            m_screenScalingY = m_scalingY;
        }
    }


    //calc screen geometry, paint widgets recursively
    void Widget::_paint(bool calcScreenGeometry) {
        if (m_visible) {
            if (m_screenGeometryDirty || calcScreenGeometry) {
                _calcScreenGeometry();
                m_screenGeometryDirty = false;
                onPaint();
                forEach([&](Widget* child) {
                    child->_paint(true);
                });
            }
            else {
                onPaint();
                forEach([&](Widget* child) {
                    child->_paint(false);
                });
            }
        }
    }


} //namespace algui
