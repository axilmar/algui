#include "algui/UINode.hpp"
#include "algui/UINodeRectChangedEvent.hpp"
#include "algui/UINodeScalingChangedEvent.hpp"
#include "algui/UINodeVisibleChangedEvent.hpp"


namespace algui {


    enum FLAGS {
        VISIBLE               = 1 << 0,
        ENABLED_TREE          = 1 << 1,
        CLIPPED               = 1 << 2,
        RECT_DIRTY            = 1 << 3,
        DESCENTANT_RECT_DIRTY = 1 << 4,
        LAYOUT_DIRTY          = 1 << 5,
        SCREEN_RECT_DIRTY     = 1 << 6,
        SCREEN_SCALING_DIRTY  = 1 << 7,
        FOCUSED_TREE          = 1 << 8
    };


    static constexpr int DIRTY_FLAGS = RECT_DIRTY | DESCENTANT_RECT_DIRTY | LAYOUT_DIRTY | SCREEN_RECT_DIRTY | SCREEN_SCALING_DIRTY;


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
            dispatchEvent(UINodeRectChangedEvent(sharedFromThis<UINode>()));
        }
    }


    void UINode::setScaling(Scaling scaling) {
        scaling.clampTo0();
        if (scaling != m_scaling) {
            m_scaling = scaling;
            invalidateScreenScaling();
            dispatchEvent(UINodeScalingChangedEvent(sharedFromThis<UINode>()));
        }
    }


    bool UINode::isVisible() const {
        return (m_flags & VISIBLE) == VISIBLE;
    }


    void UINode::setVisible(bool v) {
        if (v != ((m_flags & VISIBLE) == VISIBLE)) {
            m_flags = v ? m_flags | VISIBLE : m_flags & ~VISIBLE;
            if (getParentPtr()) {
                getParentPtr()->invalidateRect();
                getParentPtr()->invalidateLayout();
            }
            dispatchEvent(UINodeVisibleChangedEvent(sharedFromThis<UINode>()));
        }
    }


    bool UINode::isClipped() const {
        return (m_flags & CLIPPED) == CLIPPED;
    }


    void UINode::setClipped(bool v) {
        m_flags = v ? m_flags | CLIPPED : m_flags & ~CLIPPED;
    }
       
        
    bool UINode::isEnabledTree() const {
        return (m_flags & ENABLED_TREE) == ENABLED_TREE;
    }


    bool UINode::isFocusedTree() const {
        return (m_flags & FOCUSED_TREE) == FOCUSED_TREE;
    }


    void UINode::render() {
        _updateRect();
        _render(0);
    }


    void UINode::render(const Rect& clipping) {
        _updateRect();
        _render(0, clipping);
    }


    void UINode::setNewChildState(const std::shared_ptr<UINode>& child) {
        TreeNode<UINode>::setNewChildState(child);
        if (child->m_flags & (RECT_DIRTY | DESCENTANT_RECT_DIRTY)) {
            _setDescentantRectDirty();
        }
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


    void UINode::_updateScreenProps(int& flags) {
        m_flags |= flags;
        flags = m_flags & DIRTY_FLAGS;
        updateScreenProps();
    }


    void UINode::_render(int flags) {
        if (m_flags & VISIBLE) {
            _updateScreenProps(flags);
            if ((m_flags & CLIPPED) == 0) {
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


    void UINode::_render(int flags, const Rect& clipping) {
        if (m_flags & VISIBLE) {
            if (((m_flags & CLIPPED) == 0)) {
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


    void UINode::_render1(int flags, const Rect& clipping) {
        if (m_flags & VISIBLE) {
            if (((m_flags & CLIPPED) == 0)) {
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


    void UINode::_setEnabledTree(bool v) {
        m_flags = v ? m_flags | ENABLED_TREE : m_flags & ~ENABLED_TREE;
    }


    void UINode::_setFocusedTree(bool v) {
        m_flags = v ? m_flags | FOCUSED_TREE : m_flags & ~FOCUSED_TREE;
    }


} //namespace algui
