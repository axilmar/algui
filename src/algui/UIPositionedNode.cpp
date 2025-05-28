#include <stdexcept>
#include "algui/UIPositionedNode.hpp"
#pragma warning (disable: 4309)
#include "allegro5/allegro.h"


namespace algui {


    static float calcCoordValueFromStart(Coord& coord, float containerSize) {
        switch (coord.getType()) {
            case CoordType::Pixel:
                return coord.getValue();
            case CoordType::Percent:
                return (coord.getValue()/100.0) * containerSize;
        }
        throw std::invalid_argument("UIPositionedNode: calcCoordValue: invalid coord type");
    }


    static float calcCoordValueFromEnd(Coord& coord, float containerSize) {
        switch (coord.getType()) {
        case CoordType::Pixel:
            return containerSize - coord.getValue();
        case CoordType::Percent:
            return containerSize - ((coord.getValue() / 100.0) * containerSize);
        }
        throw std::invalid_argument("UIPositionedNode: calcCoordValue: invalid coord type");
    }


    static float calcCoordValue(Coord& coord, float containerSize) {
        switch (coord.getAnchorType()) {
            case CoordAnchorType::Start:
                return calcCoordValueFromStart(coord, containerSize);
            case CoordAnchorType::End:
                return calcCoordValueFromEnd(coord, containerSize);
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
            m_left = coord;
            m_positionValid = false;
        }
    }


    void UIPositionedNode::setTop(const Coord& coord) {
        if (coord != m_top) {
            m_top = coord;
            m_positionValid = false;
        }
    }


    void UIPositionedNode::setRight(const Coord& coord) {
        if (coord != m_right) {
            m_right = coord;
            m_positionValid = false;
        }
    }


    void UIPositionedNode::setBottom(const Coord& coord) {
        if (coord != m_bottom) {
            m_bottom = coord;
            m_positionValid = false;
        }
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
                float x1 = calcCoordValue(m_left, parent->getWidth());
                float y1 = calcCoordValue(m_top, parent->getHeight());
                float x2 = calcCoordValue(m_right, parent->getWidth());
                float y2 = calcCoordValue(m_bottom, parent->getHeight());
                updateGeometry(x1, y1, x2, y2);
            }
        }
    }


    void UIPositionedNode::updateRootPosition() {
        if (!m_positionValid) {
            m_positionValid = true;
            if (m_positionType == PositionType::Positioned) {
                ALLEGRO_BITMAP* target = al_get_target_bitmap();
                float width = al_get_bitmap_width(target);
                float height = al_get_bitmap_height(target);
                float x1 = calcCoordValue(m_left, width);
                float y1 = calcCoordValue(m_top, height);
                float x2 = calcCoordValue(m_right, width);
                float y2 = calcCoordValue(m_bottom, height);
                updateGeometry(x1, y1, x2, y2);
            }
        }
    }


    void UIPositionedNode::updateGeometry(float x1, float y1, float x2, float y2) {
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
        setX(x1);
        setY(y1);
        setWidth(x2 - x1);
        setHeight(y2 - y1);
    }


} //namespace algui
