#ifndef ALGUI_UIPOSITIONEDNODE_HPP
#define ALGUI_UIPOSITIONEDNODE_HPP


#include "UINode.hpp"
#include "CoordType.hpp"


namespace algui {


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
         * Checks if the node's geometry and position are valid.
         * @return true if the geometry and the position are valid, false otherwise.
         */
        bool isGeometryValid() const override;

        /**
         * Sets the geometry and the position to invalid state.
         */
        void invalidateGeometry() override;

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
        const PositionValue& getLeft() const {
            return m_left;
        }

        /**
         * Sets the left coordinate.
         * The geometry of the node will be updated at the next `renderTree()` call.
         * The position type is automatically set to `Positioned`.
         * @param type the new position type.
         */
        virtual void setLeft(const PositionValue& coord);

        /**
         * Unsets the left coordinate.
         * If the left coordinate is not set, then the width of the node is used for calculation of the left coordinate.
         * If all position and size values are unset, and the position type is `Positioned`, the position type becomes `Managed`.
         */
        virtual void unsetLeft() {
            m_positionValid = false;
            m_leftSet = false;
            updatePositionType();
        }

        /**
         * Returns the top coordinate.
         * @return the top coordinate.
         */
        const PositionValue& getTop() const {
            return m_top;
        }

        /**
         * Sets the top coordinate.
         * The geometry of the node will be updated at the next `renderTree()` call.
         * The position type is automatically set to `Positioned`.
         * @param type the new position type.
         */
        virtual void setTop(const PositionValue& coord);

        /**
         * Unsets the top coordinate.
         * If the top coordinate is not set, then the height of the node is used for calculation of the top coordinate.
         * If all position and size values are unset, and the position type is `Positioned`, the position type becomes `Managed`.
         */
        virtual void unsetTop() {
            m_positionValid = false;
            m_topSet = false;
            updatePositionType();
        }

        /**
         * Returns the right coordinate.
         * @return the right coordinate.
         */
        const PositionValue& getRight() const {
            return m_right;
        }

        /**
         * Sets the right coordinate.
         * The geometry of the node will be updated at the next `renderTree()` call.
         * The position type is automatically set to `Positioned`.
         * @param type the new position type.
         */
        virtual void setRight(const PositionValue& coord);

        /**
         * Unsets the right coordinate.
         * If the right coordinate is not set, then the width of the node is used for calculation of the right coordinate.
         * If all position and size values are unset, and the position type is `Positioned`, the position type becomes `Managed`.
         */
        virtual void unsetRight() {
            m_positionValid = false;
            m_rightSet = false;
            updatePositionType();
        }

        /**
         * Returns the bottom coordinate.
         * @return the bottom coordinate.
         */
        const PositionValue& getBottom() const {
            return m_bottom;
        }

        /**
         * Sets the bottom coordinate.
         * The geometry of the node will be updated at the next `renderTree()` call.
         * The position type is automatically set to `Positioned`.
         * @param type the new position type.
         */
        virtual void setBottom(const PositionValue& coord);

        /**
         * Unsets the bottom coordinate.
         * If the bottom coordinate is not set, then the height of the node is used for calculation of the bottom coordinate.
         * If all position and size values are unset, and the position type is `Positioned`, the position type becomes `Managed`.
         */
        virtual void unsetBottom() {
            m_positionValid = false;
            m_bottomSet = false;
            updatePositionType();
        }

        /**
         * Returns the width coordinate of the node.
         * @return the width coordinate of the node.
         */
        const SizeValue& getWidth() const {
            return m_width;
        }

        /**
         * Sets the width coordinate of the node.
         * The position type is automatically set to `Positioned`.
         * @param width the new width.
         */
        virtual void setWidth(const SizeValue& width);

        /**
         * Unsets the width.
         * If the width is unset, then the width of UINode is used for calculations.
         * If all position and size values are unset, and the position type is `Positioned`, the position type becomes `Managed`.
         */
        virtual void unsetWidth() {
            m_widthSet = false;
            m_positionValid = false;
            updatePositionType();
        }

        /**
         * Returns the height coordinate of the node.
         * @return the height coordinate of the node.
         */
        const SizeValue& getHeight() const {
            return m_height;
        }

        /**
         * Sets the height coordinate of the node.
         * The position type is automatically set to `Positioned`.
         * @param height the new height.
         */
        virtual void setHeight(const SizeValue& height);

        /**
         * Unsets the height.
         * If the height is unset, then the height of UINode is used for calculations.
         * If all position and size values are unset, and the position type is `Positioned`, the position type becomes `Managed`.
         */
        virtual void unsetHeight() {
            m_heightSet = false;
            m_positionValid = false;
            updatePositionType();
        }

        /*
         * Sets the position of the node by invoking the individual set functions for each coordinate.
         * @param left left coordinate.
         * @param top top coordinate.
         */
        virtual void setPosition(const PositionValue& left, const PositionValue& top);

        /*
         * Sets the position of the node by invoking the individual set functions for each coordinate.
         * @param left left coordinate.
         * @param top top coordinate.
         * @param right right coordinate.
         * @param bottom bottom coordinate.
         */
        virtual void setPosition(const PositionValue& left, const PositionValue& top, const PositionValue& right, const PositionValue& bottom);

        /*
         * Sets the position of the node by invoking the individual set functions for each coordinate.
         * @param left left coordinate.
         * @param top top coordinate.
         */
        virtual void setTopLeftPosition(const PositionValue& left, const PositionValue& top) {
            setPosition(left, top);
        }

        /*
         * Sets the position of the node by invoking the individual set functions for each coordinate.
         * @param right right coordinate.
         * @param bottom bottom coordinate.
         */
        virtual void setBottomRightPosition(const PositionValue& right, const PositionValue& bottom);

        /**
         * Sets the size of the node by invoking the individual set functions for each coordinate.
         * @param width width.
         * @param height height.
         */
        virtual void setSize(const SizeValue& width, const SizeValue& height);

        /**
         * Sets the geometry of the node by invoking the individual set functions for each coordinate.
         * @param left left coordinate.
         * @param top top coordinate.
         * @param width width.
         * @param height height.
         */
        virtual void setGeometry(const PositionValue& left, const PositionValue& top, const SizeValue& width, const SizeValue& height);

    protected:
        /**
         * Calculates the geometry of the node,
         * according to its position type, coordinates and parent size.
         */
        void onCalcGeometry(const UINode* parent) override;

        /**
         * Calculates the geometry of the node,
         * according to its position type, coordinates and target bitmap size.
         */
        void onCalcGeometry() override;

        /**
         * In addition to updating the geometry validity of the child,
         * it also sets the position validity of the child to false,
         * if this has an invalid geometry.
         */
        void setChildGeometryInvalid(UINode* child) const override;

        /**
         * Sets the geometry and position to be in valid state.
         */
        void setGeometryValid() override;

        /**
         * Invoked when the position type of a child is changed.
         * @param child the child that the position changed of.
         */
        virtual void onChildPositionTypeChanged(const UIPositionedNode* child) {
        }

    private:
        PositionType m_positionType{ PositionType::Managed };
        PositionValue m_left;
        PositionValue m_top;
        PositionValue m_right;
        PositionValue m_bottom;
        SizeValue m_width;
        SizeValue m_height;
        bool m_positionValid{ false };
        bool m_leftSet{ false };
        bool m_topSet{ false };
        bool m_rightSet{ false };
        bool m_bottomSet{ false };
        bool m_widthSet{ false };
        bool m_heightSet{ false };

        void invalidatePosition();
        void calcGeometry(float width, float scalingX, float height, float scalingY);
        void updatePositionType();
    };


} //namespace algui


#endif //ALGUI_UIPOSITIONEDNODE_HPP
