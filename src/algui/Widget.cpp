#pragma warning (disable: 4309)
#include <limits>
#include <algorithm>
#include <cmath>
#include "allegro5/allegro.h"
#include "algui/Widget.hpp"


#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif


namespace algui {


    /**************************************************************************
        INTERNALS
     **************************************************************************/


    static Widget* _focusedWidget = nullptr;


    /**************************************************************************
        PUBLIC
     **************************************************************************/ 


    //constructor
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
        , m_clippingMode(ClippingMode::None)
        , m_visible(true)
        , m_screenGeometryDirty(false)
        , m_geometryConstraintsDirty(false)
        , m_descentantGeometryConstraintsDirty(false)
        , m_layoutDirty(false)
        , m_enabled(true)
        , m_highlighted(false)
        , m_pressed(false)
        , m_selected(false)
        , m_focused(false)
        , m_validContent(true)
        , m_enabledTree(true)
        , m_highlightedTree(false)
        , m_pressedTree(false)
        , m_selectedTree(false)
        , m_focusedTree(false)
        , m_validContentTree(false)
        , m_treeVisualStateDirty(false)
        , m_focusable(true)
    {
    }


    //the destructor
    Widget::~Widget() {
        deleteAll();
        if (this == _focusedWidget) {
            _focusedWidget = nullptr;
        }
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

            //invalidate tree visual state of child,
            //so as that it is later recomputed
            child->_invalidateTreeVisualState();

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

            //if the removed child contains the focus, lose the focus
            if (child->contains(_focusedWidget)) {
                _focusedWidget->setFocused(false);
            }

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


    //Sets the enabled state of the widget.
    void Widget::setEnabled(bool enabled) {
        if (enabled != m_enabled) {
            if (!enabled && contains(_focusedWidget)) {
                _focusedWidget->setFocused(false);
            }
            m_enabled = enabled;
            _invalidateTreeVisualState();
        }
    }


    //Sets the highlighted state of the widget.
    void Widget::setHighlighted(bool highlighted) {
        if (highlighted != m_highlighted) {
            m_highlighted = highlighted;
            _invalidateTreeVisualState();
        }
    }


    //Sets the pressed state of the widget.
    void Widget::setPressed(bool pressed) {
        if (pressed != m_pressed) {
            m_pressed = pressed;
            _invalidateTreeVisualState();
        }
    }


    //Sets the selected state of the widget.
    void Widget::setSelected(bool selected) {
        if (selected != m_selected) {
            m_selected = selected;
            _invalidateTreeVisualState();
        }
    }


    //Sets the focused state of the widget.
    void Widget::setFocused(bool focused) {
        if (focused == m_focused) {
            return;
        }

        //get focus
        if (focused) {
            //check if it can get the focus
            if (!_canGetFocus()) {
                return;
            }

            //remove the focus from current owner
            if (_focusedWidget) {
                _focusedWidget->setFocused(false);
            }

            //set this as focused
            m_focused = true;
            _invalidateTreeVisualState();
            _focusedWidget = this;
            WidgetEvent focusEvent(this);
            dispatchEvent(EventType::Event_GotFocus, focusEvent, EventPhaseType::EventPhase_Bubble);
            for (Widget* ancestor = getParent(); ancestor; ancestor = ancestor->getParent()) {
                ancestor->dispatchEvent(EventType::Event_DescentantGotFocus, focusEvent, EventPhaseType::EventPhase_Bubble);
            }
        }

        //else lose focus
        else {
            m_focused = false;
            _invalidateTreeVisualState();
            _focusedWidget = nullptr;
            WidgetEvent focusEvent(this);
            dispatchEvent(EventType::Event_LostFocus, focusEvent, EventPhaseType::EventPhase_Bubble);
            for (Widget* ancestor = getParent(); ancestor; ancestor = ancestor->getParent()) {
                ancestor->dispatchEvent(EventType::Event_DescentantLostFocus, focusEvent, EventPhaseType::EventPhase_Bubble);
            }
        }
    }


    //Sets the valid content state of the widget.
    void Widget::setValidContent(bool validContent) {
        if (validContent != m_validContent) {
            m_validContent = validContent;
            _invalidateTreeVisualState();
        }
    }


    //Returns the widget with the focus.
    Widget* Widget::getFocusedWidget() {
        return _focusedWidget;
    }


    //Sets the focusable state.
    void Widget::setFocusable(bool focusable) {
        if (focusable != m_focusable) {
            if (!m_focusable && contains(_focusedWidget)) {
                _focusedWidget->setFocused(false);
            }
            m_focusable = focusable;
        }
    }


    //Renders the tree into the target bitmap.
    void Widget::render() {
        _updateGeometryConstraints();
        _paint(false, false);
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
            onCalculateGeometryConstraints();
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


    //invalidates the tree visual state
    void Widget::_invalidateTreeVisualState() {
        m_treeVisualStateDirty = true;
    }


    //calculate visual state of tree, depending on if the widget is a child or root
    void Widget::_calcTreeVisualState() {
        Widget* parent = getParent();

        if (parent) {
            m_enabledTree = m_enabled && parent->m_enabledTree;
            m_highlightedTree = m_highlighted || parent->m_highlightedTree;
            m_pressedTree = m_pressed || parent->m_pressedTree;
            m_selectedTree = m_selected || parent->m_selectedTree;
            m_focusedTree = m_focused || parent->m_focusedTree;
            m_validContentTree = m_validContent && parent->m_validContentTree;
        }

        else {
            m_enabledTree = m_enabled;
            m_highlightedTree = m_highlighted;
            m_pressedTree = m_pressed;
            m_selectedTree = m_selected;
            m_focusedTree = m_focused;
            m_validContentTree = m_validContent;
        }
    }

    //calc layout, screen geometry, screen visual state, paint widgets recursively
    void Widget::_paint(bool calcScreenGeometry, bool calcVisualState) {
        if (m_visible) {
            //recalculate screen geometry, if needed
            if (m_screenGeometryDirty || calcScreenGeometry) {
                _calcScreenGeometry();
                m_screenGeometryDirty = false;
                calcScreenGeometry = true;
            }

            //calc visual state of tree
            if (m_treeVisualStateDirty || calcVisualState) {
                _calcTreeVisualState();
                m_treeVisualStateDirty = false;
                calcVisualState = true;
            }

            //recalculate layout, if needed
            if (m_layoutDirty) {
                onLayout();
                m_layoutDirty = false;
            }

            //paint according to clipping
            switch (m_clippingMode) {
                //paint widget only if it overlaps clip rectangle;
                //children are tested separately
                case ClippingMode::None:
                {
                    int x1, y1, w, h, x2, y2;
                    al_get_clipping_rectangle(&x1, &y1, &w, &h);
                    x2 = x1 + w;
                    y2 = y1 + h;
                    const float clipX1 = std::max((float)x1, m_screenLeft);
                    const float clipY1 = std::max((float)y1, m_screenTop);
                    const float clipX2 = std::min((float)x2, m_screenRight);
                    const float clipY2 = std::min((float)y2, m_screenBottom);
                    const bool overlapsClipRect = clipX1 <= clipX2 && clipY1 <= clipY2;
                    if (overlapsClipRect) {
                        onPaint();
                    }
                    forEach([&](Widget* child) {
                        child->_paint(calcScreenGeometry, calcVisualState);
                    });
                    if (overlapsClipRect) {
                        onPaintOverlay();
                    }
                    break;
                }

                //paint widget only if it overlaps the clip rectangle; widget is clipped;
                //children are tested separately
                case ClippingMode::Widget:
                {
                    int x1, y1, w, h, x2, y2;
                    al_get_clipping_rectangle(&x1, &y1, &w, &h);
                    x2 = x1 + w;
                    y2 = y1 + h;
                    const float clipX1 = std::max((float)x1, m_screenLeft);
                    const float clipY1 = std::max((float)y1, m_screenTop);
                    const float clipX2 = std::min((float)x2, m_screenRight);
                    const float clipY2 = std::min((float)y2, m_screenBottom);
                    const bool overlapsClipRect = clipX1 <= clipX2 && clipY1 <= clipY2;
                    if (overlapsClipRect) {
                        al_set_clipping_rectangle((int)std::floor(clipX1), (int)std::floor(clipY1), (int)std::ceil(clipX2 - clipX1), (int)std::ceil(clipY2 - clipY1));
                        onPaint();
                        al_set_clipping_rectangle(x1, y1, w, h);
                    }
                    forEach([&](Widget* child) {
                        child->_paint(calcScreenGeometry, calcVisualState);
                        });
                    if (overlapsClipRect) {
                        al_set_clipping_rectangle((int)std::floor(clipX1), (int)std::floor(clipY1), (int)std::ceil(clipX2 - clipX1), (int)std::ceil(clipY2 - clipY1));
                        onPaintOverlay();
                        al_set_clipping_rectangle(x1, y1, w, h);
                    }
                    break;
                }

                //paint widget tree clipped
                case ClippingMode::Tree:
                {
                    int x1, y1, w, h, x2, y2;
                    al_get_clipping_rectangle(&x1, &y1, &w, &h);
                    x2 = x1 + w;
                    y2 = y1 + h;
                    const float clipX1 = std::max((float)x1, m_screenLeft);
                    const float clipY1 = std::max((float)y1, m_screenTop);
                    const float clipX2 = std::min((float)x2, m_screenRight);
                    const float clipY2 = std::min((float)y2, m_screenBottom);
                    if (clipX1 <= clipX2 && clipY1 <= clipY2) {
                        al_set_clipping_rectangle((int)std::floor(clipX1), (int)std::floor(clipY1), (int)std::ceil(clipX2 - clipX1), (int)std::ceil(clipY2 - clipY1));
                        onPaint();
                        forEach([&](Widget* child) {
                            child->_paint(calcScreenGeometry, calcVisualState);
                        });
                        onPaintOverlay();
                        al_set_clipping_rectangle(x1, y1, w, h);
                    }
                    break;
                }
            }
        }
    }


    bool Widget::_canGetFocus() const {
        for (const Widget* wgt = this; wgt; wgt = wgt->getParent()) {
            if (!wgt->m_enabled || !wgt->m_focusable) {
                return false;
            }
        }
        return true;
    }


} //namespace algui
