#include "algui/UINode.hpp"


namespace algui {



    void UINode::addChild(const std::shared_ptr<UINode>& child, const std::shared_ptr<UINode>& nextSibling) {
        TreeNode<UINode>::addChild(child, nextSibling);
        if (child->m_flags & (RECT_DIRTY | DESCENTANT_RECT_DIRTY)) {
            _setDescentantRectDirty();
        }
    }


    void UINode::setRect(Rect rect) {
        rect.clampSizeTo0();
        const bool positionDiffers = m_rect.positionDiffers(rect);
        const bool sizeDiffers = m_rect.sizeDiffers(rect);
        if (positionDiffers || sizeDiffers) {
            m_rect = rect;
            if (sizeDiffers) {
                invalidateLayout();
            }
            invalidateScreenRect();
            if (getParentPtr()) {
                getParentPtr()->invalidateRect();
            }
        }
    }


    void UINode::setScaling(Scaling scaling) {
        scaling.clampTo0();
        if (scaling != m_scaling) {
            m_scaling = scaling;
            invalidateScreenScaling();
        }
    }


    void UINode::setVisible(bool v) {
        if (v != m_visible) {
            m_visible = v;
            if (getParentPtr()) {
                getParentPtr()->invalidateRect();
                getParentPtr()->invalidateLayout();
            }
        }
    }


    void UINode::render() {
        _updateRect();
        _render(0);
    }


    void UINode::render(const Rect& clipping) {
        _updateRect();
        _render(0, clipping);
    }


    void UINode::invalidateRect() {
        if (m_flags & RECT_DIRTY) {
            return;
        }
        m_flags |= RECT_DIRTY;
        if (getParentPtr()) {
            getParentPtr()->_setDescentantRectDirty();
        }
    }


    void UINode::invalidateLayout() {
        m_flags |= LAYOUT_DIRTY;
    }


    void UINode::invalidateScreenRect() {
        m_flags |= SCREEN_RECT_DIRTY;
    }


    void UINode::invalidateScreenScaling() {
        m_flags |= SCREEN_SCALING_DIRTY;
    }


    void UINode::updateScreenRect() {
        UINode* parent = getParentPtr();
        if (parent) {
            m_screenRect.setPositionAndSize(
                m_rect.left * parent->m_screenScaling.horizontal + parent->m_screenRect.left,
                m_rect.top * parent->m_screenScaling.vertical + parent->m_screenRect.top,
                m_rect.getWidth() * parent->m_screenScaling.horizontal,
                m_rect.getHeight() * parent->m_screenScaling.vertical);
        }
        else {
            m_screenRect = m_rect;
        }
    }


    void UINode::updateScreenScaling() {
        UINode* parent = getParentPtr();
        if (parent) {
            m_screenScaling.horizontal = m_scaling.horizontal * parent->m_screenScaling.horizontal;
            m_screenScaling.vertical = m_scaling.vertical * parent->m_screenScaling.vertical;
        }
        else {
            m_screenScaling = m_scaling;
        }
    }


    void UINode::updateScreenProps() {
        if (m_flags & LAYOUT_DIRTY) {
            updateLayout();
            m_flags &= ~LAYOUT_DIRTY;
        }
        if (m_flags & SCREEN_RECT_DIRTY) {
            updateScreenRect();
            m_flags &= ~SCREEN_RECT_DIRTY;
        }
        if (m_flags & SCREEN_SCALING_DIRTY) {
            updateScreenScaling();
            m_flags &= ~SCREEN_SCALING_DIRTY;
        }
    }


    void UINode::_updateRect() {
        if (m_flags & DESCENTANT_RECT_DIRTY) {
            for (UINode* child = getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                child->_updateRect();
            }
            m_flags &= ~DESCENTANT_RECT_DIRTY;
        }
        if (m_flags & RECT_DIRTY) {
            updateRect();
            m_flags &= ~RECT_DIRTY;
        }
    }


    void UINode::_updateScreenProps(uint64_t flags) {
        flags = m_flags |= flags;
        updateScreenProps();
    }


    void UINode::_render(uint64_t flags) {
        if (m_visible) {
            _updateScreenProps(flags);
            if (m_unclipped) {
                paint();
                for (UINode* child = getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                    child->_render(flags);
                }
                paintOverlay();
            }
            else {
                const Rect prevClipping = Rect::getClippingRectangle();
                m_screenRect.setClippingRectangle();
                paint();
                for (UINode* child = getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                    child->_render(flags);
                }
                paintOverlay();
                prevClipping.setClippingRectangle();
            }
        }
    }


    void UINode::_render(uint64_t flags, const Rect& clipping) {
        if (m_visible) {
            if (m_unclipped) {
                Rect intersection = Rect::intersectionOf(m_screenRect, clipping);
                if (intersection.isValid()) {
                    _updateScreenProps(flags);
                    const Rect prevClipping = Rect::getClippingRectangle();
                    clipping.setClippingRectangle();
                    paint();
                    for (UINode* child = getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                        child->_render1(flags, intersection);
                    }
                    paintOverlay();
                    prevClipping.setClippingRectangle();
                }
            }
            else {
                Rect intersection = Rect::intersectionOf(m_screenRect, clipping);
                if (intersection.isValid()) {
                    _updateScreenProps(flags);
                    const Rect prevClipping = Rect::getClippingRectangle();
                    intersection.setClippingRectangle();
                    paint();
                    for (UINode* child = getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                        child->_render1(flags, intersection);
                    }
                    paintOverlay();
                    prevClipping.setClippingRectangle();
                }
            }
        }
    }


    void UINode::_render1(uint64_t flags, const Rect& clipping) {
        if (m_visible) {
            if (m_unclipped) {
                Rect intersection = Rect::intersectionOf(m_screenRect, clipping);
                if (intersection.isValid()) {
                    _updateScreenProps(flags);
                    paint();
                    for (UINode* child = getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                        child->_render1(flags, intersection);
                    }
                    paintOverlay();
                }
            }
            else {
                Rect intersection = Rect::intersectionOf(m_screenRect, clipping);
                if (intersection.isValid()) {
                    _updateScreenProps(flags);
                    const Rect prevClipping = Rect::getClippingRectangle();
                    intersection.setClippingRectangle();
                    paint();
                    for (UINode* child = getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                        child->_render1(flags, intersection);
                    }
                    paintOverlay();
                    prevClipping.setClippingRectangle();
                }
            }
        }
    }


    void UINode::_setDescentantRectDirty() {
        for (UINode* node = this; node; node = node->getParentPtr()) {
            if (node->m_flags & DESCENTANT_RECT_DIRTY) {
                break;
            }
            node->m_flags |= DESCENTANT_RECT_DIRTY;
        }
    }


} //namespace algui
