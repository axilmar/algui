#include <stdexcept>
#include "algui/UIPositionedNode.hpp"
#pragma warning (disable: 4309)
#include "allegro5/allegro.h"


namespace algui {


    static float calcCoordValueFromStart(Coord& coord, float containerSize, float scaling) {
        switch (coord.getType()) {
            case CoordType::Pixel:
                return coord.getValue();
            case CoordType::Percent:
                return (coord.getValue()/100.0) * containerSize / scaling;
        }
        throw std::invalid_argument("UIPositionedNode: calcCoordValue: invalid coord type");
    }


    static float calcCoordValueFromEnd(Coord& coord, float containerSize, float scaling) {
        switch (coord.getType()) {
        case CoordType::Pixel:
            return containerSize - coord.getValue();
        case CoordType::Percent:
            return containerSize / scaling - ((coord.getValue() / 100.0) * containerSize / scaling);
        }
        throw std::invalid_argument("UIPositionedNode: calcCoordValue: invalid coord type");
    }


    static float calcCoordValue(Coord& coord, float containerSize, float scaling) {
        switch (coord.getAnchorType()) {
            case CoordAnchorType::Start:
                return calcCoordValueFromStart(coord, containerSize, scaling);
            case CoordAnchorType::End:
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
            m_positionValid = false;
        }
    }


    void UIPositionedNode::setLeft(const Coord& coord) {
        if (coord != m_left) {
            m_positionType = PositionType::Positioned;
            m_left = coord;
            m_positionValid = false;
            m_leftSet = true;
        }
    }


    void UIPositionedNode::setTop(const Coord& coord) {
        if (coord != m_top) {
            m_positionType = PositionType::Positioned;
            m_top = coord;
            m_positionValid = false;
            m_topSet = true;
        }
    }


    void UIPositionedNode::setRight(const Coord& coord) {
        if (coord != m_right) {
            m_positionType = PositionType::Positioned;
            m_right = coord;
            m_positionValid = false;
            m_rightSet = true;
        }
    }


    void UIPositionedNode::setBottom(const Coord& coord) {
        if (coord != m_bottom) {
            m_positionType = PositionType::Positioned;
            m_bottom = coord;
            m_positionValid = false;
            m_bottomSet = true;
        }
    }


    void UIPositionedNode::setWidth(const Coord& width) {
        if (width != m_width) {
            m_positionType = PositionType::Positioned;
            m_width = width;
            m_positionValid = false;
            m_widthSet = true;
        }
    }


    void UIPositionedNode::setHeight(const Coord& height) {
        if (height != m_height) {
            m_positionType = PositionType::Positioned;
            m_height = height;
            m_positionValid = false;
            m_heightSet = true;
        }
    }


    void UIPositionedNode::setPosition(const Coord& left, const Coord& top) {
        setLeft(left);
        setTop(top);
    }


    void UIPositionedNode::setPosition(const Coord& left, const Coord& top, const Coord& right, const Coord& bottom) {
        setPosition(left, top);
        setBottomRightPosition(right, bottom);
    }


    void UIPositionedNode::setBottomRightPosition(const Coord& right, const Coord& bottom) {
        setRight(right);
        setBottom(bottom);
    }


    void UIPositionedNode::setSize(const Coord& width, const Coord& height) {
        setWidth(width);
        setHeight(height);
    }


    void UIPositionedNode::setGeometry(const Coord& left, const Coord& top, const Coord& width, const Coord& height) {
        setPosition(left, top);
        setSize(width, height);
    }


    void UIPositionedNode::onCalcChildState(UINode* parent) {
        updateChildPosition(parent);
        UINode::onCalcChildState(parent);
    }


    void UIPositionedNode::onCalcRootState() {
        updateRootPosition();
        UINode::onCalcRootState();
    }


    void UIPositionedNode::updateChildPosition(UINode* parent) {
        if (!m_positionValid) {
            m_positionValid = true;
            if (m_positionType == PositionType::Positioned) {
                calcGeometry(parent->getWidth(), parent->getScreenScalingX(), parent->getHeight(), parent->getScreenScalingY());
            }
        }
    }


    void UIPositionedNode::updateRootPosition() {
        if (!m_positionValid) {
            m_positionValid = true;
            if (m_positionType == PositionType::Positioned) {
                ALLEGRO_BITMAP* target = al_get_target_bitmap();
                const float width = al_get_bitmap_width(target);
                const float height = al_get_bitmap_height(target);
                calcGeometry(width, 1.0f, height, 1.0f);
            }
        }
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

        const bool sizeChanged = updateGeometry(x1, y1, x2, y2);

        //required because a child's position might depend on the parent's size
        if (sizeChanged) {
            for (UIPositionedNode* child = getFirstChild(); child; child = child->getNextSibling()) {
                child->m_positionValid = false;
            }
        }
    }


    bool UIPositionedNode::updateGeometry(float x1, float y1, float x2, float y2) {
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
        const float width = x2 - x1;
        const float height = y2 - y1;
        const bool sizeChanged = width != UINode::getWidth() || height != UINode::getHeight();
        UINode::setGeometry(x1, y1, width, height);
        return sizeChanged;
    }


} //namespace algui
