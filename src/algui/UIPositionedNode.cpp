#include <stdexcept>
#include "algui/UIPositionedNode.hpp"
#pragma warning (disable: 4309)
#include "allegro5/allegro.h"


namespace algui {


    template <class T>
    static float calcCoordValueFromStart(const T& coord, float containerSize, float scaling) {
        return calcPixelValue(coord, containerSize, scaling);
    }


    static float calcCoordValueFromEnd(const PositionValue& coord, float containerSize, float scaling) {
        switch (coord.getType()) {
        case CoordType::Pixel:
            return containerSize - coord.getValue();
        case CoordType::Percent:
            return containerSize / scaling - ((coord.getValue() / 100.0f) * containerSize / scaling);
        }
        throw std::invalid_argument("UIPositionedNode: calcCoordValue: invalid coord type");
    }


    bool UIPositionedNode::isGeometryValid() const {
        return UINode::isGeometryValid() && m_positionValid;
    }


    void UIPositionedNode::invalidateGeometry() {
        invalidatePosition();
    }


    static float calcCoordValue(PositionValue& coord, float containerSize, float scaling) {
        switch (coord.getAnchorType()) {
            case PositionAnchorType::Start:
                return calcCoordValueFromStart(coord, containerSize, scaling);
            case PositionAnchorType::End:
                return calcCoordValueFromEnd(coord, containerSize, scaling);
        }
        throw std::invalid_argument("UIPositionedNode: calcCoordValue: invalid coord anchor type");
    }


    UIPositionedNode* UIPositionedNode::getParent() const {
        for (UINode* node = UINode::getParent(); node; node = node->UINode::getParent()) {
            if (node->isPositionedNode()) {
                return static_cast<UIPositionedNode*>(node);
            }
        }
        return nullptr;
    }


    UIPositionedNode* UIPositionedNode::getPrevSibling() const {
        for (UINode* prevSibling = UINode::getPrevSibling(); prevSibling; prevSibling = prevSibling->UINode::getPrevSibling()) {
            if (prevSibling->isPositionedNode()) {
                return static_cast<UIPositionedNode*>(prevSibling);
            }
        }
        return nullptr;
    }


    UIPositionedNode* UIPositionedNode::getNextSibling() const {
        for (UINode* nextSibling = UINode::getNextSibling(); nextSibling; nextSibling = nextSibling->UINode::getNextSibling()) {
            if (nextSibling->isPositionedNode()) {
                return static_cast<UIPositionedNode*>(nextSibling);
            }
        }
        return nullptr;
    }


    UIPositionedNode* UIPositionedNode::getFirstChild() const {
        for (UINode* child = UINode::getFirstChild(); child; child = child->UINode::getNextSibling()) {
            if (child->isPositionedNode()) {
                return static_cast<UIPositionedNode*>(child);
            }
        }
        return nullptr;
    }


    UIPositionedNode* UIPositionedNode::getLastChild() const {
        for (UINode* child = UINode::getLastChild(); child; child = child->UINode::getPrevSibling()) {
            if (child->isPositionedNode()) {
                return static_cast<UIPositionedNode*>(child);
            }
        }
        return nullptr;
    }


    UIPositionedNode* UIPositionedNode::getRoot() const {
        UINode* node = const_cast<UIPositionedNode*>(this);
        UIPositionedNode* result = nullptr;
        do {
            if (node->isPositionedNode()) {
                result = static_cast<UIPositionedNode*>(node);
            }
            node = node->getParent();
        } while (node);
        return result;
    }


    void UIPositionedNode::setPositionType(PositionType type) {
        if (type != m_positionType) {
            m_positionType = type;
            invalidatePosition();
            if (getParent()) {
                getParent()->onChildPositionTypeChanged(this);
            }
        }
    }


    void UIPositionedNode::setLeft(const PositionValue& coord) {
        if (coord != m_left) {
            m_left = coord;
            m_leftSet = true;
            invalidatePosition();
            setPositionType(PositionType::Positioned);
        }
    }


    void UIPositionedNode::setTop(const PositionValue& coord) {
        if (coord != m_top) {
            m_top = coord;
            m_topSet = true;
            invalidatePosition();
            setPositionType(PositionType::Positioned);
        }
    }


    void UIPositionedNode::setRight(const PositionValue& coord) {
        if (coord != m_right) {
            m_right = coord;
            m_rightSet = true;
            invalidatePosition();
            setPositionType(PositionType::Positioned);
        }
    }


    void UIPositionedNode::setBottom(const PositionValue& coord) {
        if (coord != m_bottom) {
            m_bottom = coord;
            m_bottomSet = true;
            invalidatePosition();
            setPositionType(PositionType::Positioned);
        }
    }


    void UIPositionedNode::setWidth(const SizeValue& width) {
        if (width != m_width) {
            m_width = width;
            m_widthSet = true;
            invalidatePosition();
            setPositionType(PositionType::Positioned);
        }
    }


    void UIPositionedNode::setHeight(const SizeValue& height) {
        if (height != m_height) {
            m_height = height;
            m_heightSet = true;
            invalidatePosition();
            setPositionType(PositionType::Positioned);
        }
    }


    void UIPositionedNode::setPosition(const PositionValue& left, const PositionValue& top) {
        if (left != m_left || top != m_top) {
            m_left = left;
            m_top = top;
            m_leftSet = true;
            m_topSet = true;
            invalidatePosition();
            setPositionType(PositionType::Positioned);
        }
    }


    void UIPositionedNode::setPosition(const PositionValue& left, const PositionValue& top, const PositionValue& right, const PositionValue& bottom) {
        if (left != m_left || top != m_top || right != m_right || bottom != m_bottom) {
            m_left = left;
            m_top = top;
            m_right = right;
            m_bottom = bottom;
            m_leftSet = true;
            m_topSet = true;
            m_rightSet = true;
            m_bottomSet = true;
            invalidatePosition();
            setPositionType(PositionType::Positioned);
        }
    }


    void UIPositionedNode::setBottomRightPosition(const PositionValue& right, const PositionValue& bottom) {
        if (right != m_right || bottom != m_bottom) {
            m_right = right;
            m_bottom = bottom;
            m_rightSet = true;
            m_bottomSet = true;
            invalidatePosition();
            setPositionType(PositionType::Positioned);
        }
    }


    void UIPositionedNode::setSize(const SizeValue& width, const SizeValue& height) {
        if (width != m_width || height != m_height) {
            m_width = width;
            m_height = height;
            m_widthSet = true;
            m_heightSet = true;
            invalidatePosition();
            setPositionType(PositionType::Positioned);
        }
    }


    void UIPositionedNode::setGeometry(const PositionValue& left, const PositionValue& top, const SizeValue& width, const SizeValue& height) {
        if (left != m_left || top != m_top || width != m_width || height != m_height) {
            m_left = left;
            m_top = top;
            m_width = width;
            m_height = height;
            m_leftSet = true;
            m_topSet = true;
            m_widthSet = true;
            m_heightSet = true;
            invalidatePosition();
            setPositionType(PositionType::Positioned);
        }
    }


    void UIPositionedNode::onCalcGeometry(const UINode* parent) {
        UINode::onCalcGeometry();
        if (!m_positionValid && m_positionType == PositionType::Positioned) {
            calcGeometry(parent->getWidth(), parent->getScreenScalingX(), parent->getHeight(), parent->getScreenScalingY());
        }
    }


    void UIPositionedNode::onCalcGeometry() {
        UINode::onCalcGeometry();
        if (!m_positionValid && m_positionType == PositionType::Positioned) {
            ALLEGRO_BITMAP* target = al_get_target_bitmap();
            const float width = al_get_bitmap_width(target);
            const float height = al_get_bitmap_height(target);
            calcGeometry(width, 1.0f, height, 1.0f);
        }
    }


    void UIPositionedNode::setChildGeometryInvalid(UINode* child) const {
        UINode::setChildGeometryInvalid(child);
        if (!UINode::isGeometryValid() && child->isPositionedNode()) {
            UIPositionedNode* positionedChild = static_cast<UIPositionedNode*>(child);
            if (positionedChild->m_positionType == PositionType::Positioned) {
                positionedChild->m_positionValid = false;
            }
        }
    }


    void UIPositionedNode::setGeometryValid() {
        UINode::setGeometryValid();
        m_positionValid = true;
    }


    void UIPositionedNode::invalidatePosition() {
        UINode::invalidateGeometry();
        m_positionValid = false;
    }


    void UIPositionedNode::calcGeometry(float width, float scalingX, float height, float scalingY) {
        float x1 = 0.0f, y1 = 0.0f, x2 = 0.0f, y2 = 0.0f;

        if (m_leftSet && m_rightSet) {
            x1 = calcCoordValue(m_left, width, scalingX);
            x2 = calcCoordValue(m_right, width, scalingX);
        }
        else if (!m_leftSet && m_rightSet) {
            x2 = calcCoordValue(m_right, width, scalingX);
            const float w = m_widthSet ? calcCoordValueFromStart(m_width, width, scalingX) : UINode::getWidth();
            x1 = x2 - w;
        }
        else if (m_leftSet && !m_rightSet) {
            x1 = calcCoordValue(m_left, width, scalingX);
            const float w = m_widthSet ? calcCoordValueFromStart(m_width, width, scalingX) : UINode::getWidth();
            x2 = x1 + w;
        }
        else {
            x1 = UINode::getLeft();
            const float w = m_widthSet ? calcCoordValueFromStart(m_width, width, scalingX) : UINode::getWidth();
            x2 = x1 + w;
        }

        if (m_topSet && m_bottomSet) {
            y1 = calcCoordValue(m_top, height, scalingY);
            y2 = calcCoordValue(m_bottom, height, scalingY);
        }
        else if (!m_topSet && m_bottomSet) {
            y2 = calcCoordValue(m_bottom, height, scalingY);
            const float h = m_heightSet ? calcCoordValueFromStart(m_height, height, scalingY) : UINode::getHeight();
            y1 = y2 - h;
        }
        else if (m_topSet && !m_bottomSet) {
            y1 = calcCoordValue(m_top, height, scalingY);
            const float h = m_heightSet ? calcCoordValueFromStart(m_height, height, scalingY) : UINode::getHeight();
            y2 = y1 + h;
        }
        else {
            y1 = UINode::getTop();
            const float h = m_heightSet ? calcCoordValueFromStart(m_height, height, scalingY) : UINode::getHeight();
            y2 = y1 + h;
        }

        if (x1 > x2) {
            const float t = x1;
            x1 = x2;
            x2 = t;
        }
        if (y1 > y2) {
            const float t = y1;
            y1 = y2;
            y2 = t;
        }
        const float w = x2 - x1;
        const float h = y2 - y1;
        UINode::setGeometry(x1, y1, w, h);
    }


    void UIPositionedNode::updatePositionType() {
        if (m_positionType == PositionType::Positioned && !m_leftSet && !m_topSet && !m_rightSet && !m_bottomSet && !m_widthSet && !m_heightSet) {
            setPositionType(PositionType::Managed);
        }
    }


} //namespace algui
