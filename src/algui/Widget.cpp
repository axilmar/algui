#pragma warning (disable: 4309)
#include <limits>
#include "allegro5/allegro.h"
#include "algui/Widget.hpp"


namespace algui {


    /**************************************************************************
        PUBLIC
     **************************************************************************/ 


    Widget::Widget()
        : m_scalingX(1)
        , m_scalingY(1)
        , m_maximumWidth(FLT_MAX)
        , m_maximumHeight(FLT_MAX)
        , m_screenLeft(0)
        , m_screenTop(0)
        , m_screenRight(0)
        , m_screenBottom(0)
        , m_screenScalingX(1)
        , m_screenScalingY(1)
        , m_visible(true)
        , m_screenGeometryDirty(false)
        , m_geometryConstraintsDirty(false)
        , m_descentantGeometryConstraintsDirty(false)
        , m_layoutDirty(false)
    {
    }


    //Inserts a child widget.
    bool Widget::add(Widget* child, Widget* nextSibling) {
        //if child is added successfully
        if (Tree<Widget>::add(child, nextSibling)) {

            //if geometry constraints in the child tree are invalid,
            //invalidate the constraints of this and its ancestors
            if (child->m_geometryConstraintsDirty || child->m_descentantGeometryConstraintsDirty) {
                for (Widget* ancestor = this; ancestor; ancestor = ancestor->getParent()) {
                    if (ancestor->m_descentantGeometryConstraintsDirty) {
                        break;
                        ancestor->m_descentantGeometryConstraintsDirty = true;
                    }
                }
            }

            //invalidate geometry constraints and layout of this,
            //since a child was added
            _invalidateGeometryConstraints();
            _invalidateLayout();

            //successful addition
            return true;
        }

        //fail to add child
        return false;
    }


    //Removes a child widget.
    bool Widget::remove(Widget* child) {
        //remove widget from tree
        if (Tree<Widget>::remove(child)) {

            //invalidate geometry constraints and layout of this,
            //since a child was removed
            _invalidateGeometryConstraints();
            _invalidateLayout();

            //success
            return true;
        }

        //failure
        return false;
    }


    //Sets the left coordinate.
    void Widget::setLeft(const Coord& left) {
        if (left != m_left) {
            m_left = left;
            _invalidateParentLayout();
            _invalidateScreenGeometry();
        }
    }


    //Sets the top coordinate.
    void Widget::setTop(const Coord& top) {
        if (top != m_top) {
            m_top = top;
            _invalidateParentLayout();
            _invalidateScreenGeometry();
        }
    }


    //Sets the width.
    void Widget::setWidth(const Coord& width) {
        if (width != m_width) {
            m_width = width;
            _invalidateLayout();
            _invalidateParentLayout();
            _invalidateScreenGeometry();
        }
    }


    //Sets the height.
    void Widget::setHeight(const Coord& height) {
        if (height != m_height) {
            m_height = height;
            _invalidateLayout();
            _invalidateParentLayout();
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
            _invalidateParentGeometryConstraints();
            _invalidateParentLayout();
        }
    }


    //Sets the minimum width geometry constraint of the widget.
    void Widget::setMinimumWidth(const Coord& minimumWidth) {
        if (minimumWidth != m_minimumWidth) {
            m_minimumWidth = minimumWidth;
            _invalidateParentGeometryConstraints();
            _invalidateParentLayout();
        }
    }


    //Sets the minimum height geometry constraint of the widget.
    void Widget::setMinimumHeight(const Coord& minimumHeight) {
        if (minimumHeight != m_minimumHeight) {
            m_minimumHeight = minimumHeight;
            _invalidateParentGeometryConstraints();
            _invalidateParentLayout();
        }
    }


    //Sets the optimal width geometry constraint of the widget.
    void Widget::setOptimalWidth(const Coord& optimalWidth) {
        if (optimalWidth != m_optimalWidth) {
            m_optimalWidth = optimalWidth;
            _invalidateParentGeometryConstraints();
            _invalidateParentLayout();
        }
    }


    //Sets the optimal height geometry constraint of the widget.
    void Widget::setOptimalHeight(const Coord& optimalHeight) {
        if (optimalHeight != m_optimalHeight) {
            m_optimalHeight = optimalHeight;
            _invalidateParentGeometryConstraints();
            _invalidateParentLayout();
        }
    }


    //Sets the maximum width geometry constraint of the widget.
    void Widget::setMaximumWidth(const Coord& maximumWidth) {
        if (maximumWidth != m_maximumWidth) {
            m_maximumWidth = maximumWidth;
            _invalidateParentGeometryConstraints();
            _invalidateParentLayout();
        }
    }


    //Sets the maximum height geometry constraint of the widget.
    void Widget::setMaximumHeight(const Coord& maximumHeight) {
        if (maximumHeight != m_maximumHeight) {
            m_maximumHeight = maximumHeight;
            _invalidateParentGeometryConstraints();
            _invalidateParentLayout();
        }
    }


    //Renders the tree into the target bitmap.
    void Widget::render() {
        _updateGeometryConstraints();
        _paint(false);
    }


    /**************************************************************************
        PRIVATE
    **************************************************************************/


    //make the screen geometry invalid
    void Widget::_invalidateScreenGeometry() {
        m_screenGeometryDirty = true;
    }


    //invalidate geometry constraits of widget
    void Widget::_invalidateGeometryConstraints() {
        if (m_geometryConstraintsDirty) {
            return;
        }
        m_geometryConstraintsDirty = true;
        for (Widget* ancestor = getParent(); ancestor; ancestor = ancestor->getParent()) {
            if (ancestor->m_descentantGeometryConstraintsDirty) {
                return;
            }
            ancestor->m_descentantGeometryConstraintsDirty = true;
        }
    }


    //invalidates geometry constraints of parent
    void Widget::_invalidateParentGeometryConstraints() {
        if (getParent()) {
            getParent()->_invalidateGeometryConstraints();
        }
    }


    //reevaluate geometry constraints, based on constraints of children
    void Widget::_updateGeometryConstraints() {
        if (m_descentantGeometryConstraintsDirty) {
            forEach([&](Widget* child) {
                child->_updateGeometryConstraints();
            });
            m_descentantGeometryConstraintsDirty = false;
        }
        if (m_geometryConstraintsDirty) {
            onUpdateGeometryConstraints();
            m_geometryConstraintsDirty = false;
        }
    }


    //invalidate the layout of the widget
    void Widget::_invalidateLayout() {
        m_layoutDirty = true;
    }


    //invalidate the layout of the widget's parent
    void Widget::_invalidateParentLayout() {
        if (getParent()) {
            getParent()->_invalidateLayout();
        }
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
            //recalculate layout, if needed
            if (m_layoutDirty) {
                onLayout();
                m_layoutDirty = false;
            }

            //recalculate screen geometry, if needed
            if (m_screenGeometryDirty || calcScreenGeometry) {
                _calcScreenGeometry();
                m_screenGeometryDirty = false;
                calcScreenGeometry = true;
            }

            //paint widget
            onPaint();

            //paint children
            forEach([&](Widget* child) {
                child->_paint(calcScreenGeometry);
            });
        }
    }


} //namespace algui
