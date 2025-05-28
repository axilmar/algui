#ifndef ALGUI_UIPOSITIONEDNODE_HPP
#define ALGUI_UIPOSITIONEDNODE_HPP


#include "UINode.hpp"


namespace algui {


    /**
     * Position type.
     */
    enum class PositionType {
        /**
         * A node's geometry is managed by its parent; the default.
         */
        Managed,

        /**
         * A node's geometry is not managed by its parent; its geometry is defined
         * by its left, top, right and bottom coordinates.
         */
        Positioned,

        /**
         * a node's geometry is managed neither by its parent nor by its left, top, right and bottom coordinates.
         * Its geometry can be set via setX, setY, setWidth and setHeight methods.
         */
        Free
    };


    /**
     * Coordinate type.
     */
    enum class CoordType {
        /**
         * Coordinate measured in pixels; the default.
         */
        Pixel,

        /**
         * Coordinate measured in percent of parent node size, or percent of target bitmap size for root nodes.
         */
        Percent
    };


    /**
     * Coordinate anchor type.
     */
    enum class CoordAnchorType {
        /**
         * Calculate the coordinate from the start of the parent or target bitmap.
         */
        Start,

        /**
         * Calculate the coordinate from the end of the parent or target bitmap.
         */
        End
    };


    /**
     * A coordinate.
     */
    class Coord {
    public:
        /**
         * Constructor.
         * @param value coordinate value.
         * @param type coordinate type.
         * @param anchorType anchor type.
         */
        Coord(float value = 0.0f, CoordType type = CoordType::Pixel, CoordAnchorType anchorType = CoordAnchorType::Start)
            : m_value(value >= 0.0f ? value : 0.0f)
            , m_type(type)
            , m_anchorType(anchorType)
        {
        }

        /**
         * Returns the value.
         * @return the value.
         */
        float getValue() const {
            return m_value;
        }

        /**
         * Sets the value.
         * @param value the value; it is clamped to 0.
         */
        void setValue(float value) {
            m_value = value >= 0.0f ? value : 0.0f;
        }

        /**
         * Returns the coordinate type.
         * @return the coordinate type.
         */
        const CoordType getType() const {
            return m_type;
        }

        /**
         * Sets the coordinate type.
         * @param type the coordinate type.
         */
        void setType(CoordType type) {
            m_type = type;
        }

        /**
         * Returns the anchor type.
         * @return the anchor type.
         */
        CoordAnchorType getAnchorType() const {
            return m_anchorType;
        }

        /**
         * Sets the anchor type.
         * @param anchorType the anchor type.
         */
        void setAnchorType(CoordAnchorType anchorType) {
            m_anchorType = anchorType;
        }

        /**
         * Sets the value, the type and the anchor type.
         * @param value coordinate value.
         * @param type coordinate type.
         * @param anchorType anchor type.
         */
        void set(float value, CoordType type = CoordType::Pixel, CoordAnchorType anchorType = CoordAnchorType::Start) {
            m_value = value >= 0.0f ? value : 0.0f;
            m_type = type;
            m_anchorType = anchorType;
        }

        /**
         * Compares two coordinates for equality.
         * @param other the other coordinate to compare to this.
         * @return true if the two coordinates are equal, false otherwise.
         */
        bool operator == (const Coord& other) const {
            return m_type == other.m_type && m_value == other.m_value && m_anchorType == other.m_anchorType;
        }

        /**
         * Compares two coordinates for imequality.
         * @param other the other coordinate to compare to this.
         * @return true if the two coordinates are not equal, false otherwise.
         */
        bool operator != (const Coord& other) const {
            return !operator == (other);
        }

    private:
        float m_value;
        CoordType m_type;
        CoordAnchorType m_anchorType;
    };


    /**
     * A node that its geometry is defined by position type and coordinates.
     * 
     * It allows a node's geometry to be set from pixels or percentages of container (the parent node or the target bitmap).
     * 
     * The default position type is managed, i.e. the parent manages geometry of the child.
     * 
     * If position type is 'positioned', then the positioned node does not participate in layout management,
     * and its geometry is defined by its left, top, right and bottom coordinates.
     * 
     * If position type is 'free', then the positioned node does not participate in layout management,
     * and its left, top, right and bottom coordinates are ignored. Its geometry can be set 
     * via the methods setX, setY, setWidth and setHeight.
     */
    class UIPositionedNode : public UINode {
    public:
        /**
         * Interface for checking if the underlying node is a positioned node.
         * @return always true.
         */
        bool isPositionedNode() const override {
            return true;
        }

        /**
         * Returns the closest ancestor node that is a positioned node.
         * @return the closest ancestor node that is a positioned node.
         */
        UIPositionedNode* getParent() const;

        /**
         * Returns the closest previous sibling node that is a positioned node.
         * @return the closest previous sibling node that is a positioned node.
         */
        UIPositionedNode* getPrevSibling() const;

        /**
         * Returns the closest next sibling node that is a positioned node.
         * @return the closest next sibling node that is a positioned node.
         */
        UIPositionedNode* getNextSibling() const;

        /**
         * Returns the first child node that is a positioned node.
         * @return the first child node that is a positioned node.
         */
        UIPositionedNode* getFirstChild() const;

        /**
         * Returns the last child node that is a positioned node.
         * @return the last child node that is a positioned node.
         */
        UIPositionedNode* getLastChild() const;

        /**
         * Returns the furthest ancestor node that is a positioned node.
         * @return the furthest ancestor node that is a positioned node.
         */
        UIPositionedNode* getRoot() const;

        /**
         * Returns the position type.
         * @return the position type.
         */
        PositionType getPositionType() const {
            return m_positionType;
        }

        /**
         * Sets the position type.
         * The geometry of the node will be updated at the next `renderTree()` call.
         * @param type the new position type.
         */
        void setPositionType(PositionType type);

        /**
         * Returns the left coordinate.
         * @return the left coordinate.
         */
        const Coord& getLeft() const {
            return m_left;
        }

        /**
         * Sets the left coordinate.
         * The geometry of the node will be updated at the next `renderTree()` call.
         * @param type the new position type.
         */
        void setLeft(const Coord& coord);

        /**
         * Returns the top coordinate.
         * @return the top coordinate.
         */
        const Coord& getTop() const {
            return m_top;
        }

        /**
         * Sets the top coordinate.
         * The geometry of the node will be updated at the next `renderTree()` call.
         * @param type the new position type.
         */
        void setTop(const Coord& coord);

        /**
         * Returns the right coordinate.
         * @return the right coordinate.
         */
        const Coord& getRight() const {
            return m_right;
        }

        /**
         * Sets the right coordinate.
         * The geometry of the node will be updated at the next `renderTree()` call.
         * @param type the new position type.
         */
        void setRight(const Coord& coord);

        /**
         * Returns the bottom coordinate.
         * @return the bottom coordinate.
         */
        const Coord& getBottom() const {
            return m_bottom;
        }

        /**
         * Sets the bottom coordinate.
         * The geometry of the node will be updated at the next `renderTree()` call.
         * @param type the new position type.
         */
        void setBottom(const Coord& coord);

    protected:
        /**
         * Calculates the geometry of the node,
         * according to its position type, coordinates and parent size.
         * Then it invokes the UINode method to establish the screen coordinates of the node.
         */
        void onCalcChildState(UINode* parent) override;

        /**
         * Calculates the geometry of the node,
         * according to its position type, coordinates and target bitmap size.
         * Then it invokes the UINode method to establish the screen coordinates of the node.
         */
        void onCalcRootState() override;

    private:
        PositionType m_positionType{ PositionType::Managed };
        Coord m_left;
        Coord m_top;
        Coord m_right;
        Coord m_bottom;
        bool m_positionValid{ false };

        void updateChildPosition(UINode* parent);
        void updateRootPosition();
        void updateGeometry(float x1, float y1, float x2, float y2);
    };


} //namespace algui


#endif //ALGUI_UIPOSITIONEDNODE_HPP
