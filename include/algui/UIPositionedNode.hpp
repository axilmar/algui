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
         * by its left, top, right, bottom, width and height coordinates.
         */
        Positioned,

        /**
         * a node's geometry is managed neither by its parent nor by its coordinates.
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
         * Constructor.
         * @param value coordinate value.
         * @param type coordinate type.
         * @param anchorType anchor type.
         */
        Coord(float value, CoordAnchorType anchorType)
            : m_value(value >= 0.0f ? value : 0.0f)
            , m_type(CoordType::Pixel)
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
     * and its geometry is defined by its left, top, right, bottom, width and height coordinates.
     * In case a left/top/right/bottom coordinate is not set, then the node's width/height coordinates
     * are used for geometry calculations.
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
        virtual void setLeft(const Coord& coord);

        /**
         * Unsets the left coordinate.
         * If the left coordinate is not set, then the width of the node is used for calculation of the left coordinate.
         */
        virtual void unsetLeft() {
            m_positionValid = false;
            m_leftSet = false;
        }

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
        virtual void setTop(const Coord& coord);

        /**
         * Unsets the top coordinate.
         * If the top coordinate is not set, then the height of the node is used for calculation of the top coordinate.
         */
        virtual void unsetTop() {
            m_positionValid = false;
            m_topSet = false;
        }

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
        virtual void setRight(const Coord& coord);

        /**
         * Unsets the right coordinate.
         * If the right coordinate is not set, then the width of the node is used for calculation of the right coordinate.
         */
        virtual void unsetRight() {
            m_positionValid = false;
            m_rightSet = false;
        }

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
        virtual void setBottom(const Coord& coord);

        /**
         * Unsets the bottom coordinate.
         * If the bottom coordinate is not set, then the height of the node is used for calculation of the bottom coordinate.
         */
        virtual void unsetBottom() {
            m_positionValid = false;
            m_bottomSet = false;
        }

        /**
         * Returns the width coordinate of the node.
         * @return the width coordinate of the node.
         */
        const Coord& getWidth() const {
            return m_width;
        }

        /**
         * Sets the width coordinate of the node.
         * @param width the new width.
         */
        virtual void setWidth(const Coord& width);

        /**
         * Unsets the width.
         * If the width is unset, then the width of UINode is used for calculations.
         */
        virtual void unsetWidth() {
            m_widthSet = false;
            m_positionValid = false;
        }

        /**
         * Returns the height coordinate of the node.
         * @return the height coordinate of the node.
         */
        const Coord& getHeight() const {
            return m_height;
        }

        /**
         * Sets the height coordinate of the node.
         * @param height the new height.
         */
        virtual void setHeight(const Coord& height);

        /**
         * Unsets the height.
         * If the height is unset, then the height of UINode is used for calculations.
         */
        virtual void unsetHeight() {
            m_heightSet = false;
            m_positionValid = false;
        }

        /*
         * Sets the position of the node by invoking the individual set functions for each coordinate.
         * @param left left coordinate.
         * @param top top coordinate.
         */
        virtual void setPosition(const Coord& left, const Coord& top);

        /*
         * Sets the position of the node by invoking the individual set functions for each coordinate.
         * @param left left coordinate.
         * @param top top coordinate.
         * @param right right coordinate.
         * @param bottom bottom coordinate.
         */
        virtual void setPosition(const Coord& left, const Coord& top, const Coord& right, const Coord& bottom);

        /*
         * Sets the position of the node by invoking the individual set functions for each coordinate.
         * @param left left coordinate.
         * @param top top coordinate.
         */
        virtual void setTopLeftPosition(const Coord& left, const Coord& top) {
            setPosition(left, top);
        }

        /*
         * Sets the position of the node by invoking the individual set functions for each coordinate.
         * @param right right coordinate.
         * @param bottom bottom coordinate.
         */
        virtual void setBottomRightPosition(const Coord& right, const Coord& bottom);

        /**
         * Sets the size of the node by invoking the individual set functions for each coordinate.
         * @param width width.
         * @param height height.
         */
        virtual void setSize(const Coord& width, const Coord& height);

        /**
         * Sets the geometry of the node by invoking the individual set functions for each coordinate.
         * @param left left coordinate.
         * @param top top coordinate.
         * @param width width.
         * @param height height.
         */
        virtual void setGeometry(const Coord& left, const Coord& top, const Coord& width, const Coord& height);

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
        Coord m_width;
        Coord m_height;
        bool m_positionValid{ false };
        bool m_leftSet{ false };
        bool m_topSet{ false };
        bool m_rightSet{ false };
        bool m_bottomSet{ false };
        bool m_widthSet{ false };
        bool m_heightSet{ false };

        void updateChildPosition(UINode* parent);
        void updateRootPosition();
        void calcGeometry(float width, float scalingX, float height, float scalingY);
        bool updateGeometry(float x1, float y1, float x2, float y2);
    };


} //namespace algui


#endif //ALGUI_UIPOSITIONEDNODE_HPP
