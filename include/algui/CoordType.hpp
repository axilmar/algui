#ifndef ALGUI_COORDTYPE_HPP
#define ALGUI_COORDTYPE_HPP


#include <stdexcept>


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
     * A size value.
     */
    class SizeValue {
    public:
        /**
         * Constructor.
         * @param value coordinate value.
         * @param type coordinate type.
         */
        SizeValue(float value = 0.0f, CoordType type = CoordType::Pixel)
            : m_value(value >= 0.0f ? value : 0.0f)
            , m_type(type)
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
         * Sets the value and the type.
         * @param value coordinate value.
         * @param type coordinate type.
         */
        void set(float value, CoordType type = CoordType::Pixel) {
            m_value = value >= 0.0f ? value : 0.0f;
            m_type = type;
        }

        /**
         * Compares two coordinates for equality.
         * @param other the other coordinate to compare to this.
         * @return true if the two coordinates are equal, false otherwise.
         */
        bool operator == (const SizeValue& other) const {
            return m_type == other.m_type && m_value == other.m_value;
        }

        /**
         * Compares two coordinates for imequality.
         * @param other the other coordinate to compare to this.
         * @return true if the two coordinates are not equal, false otherwise.
         */
        bool operator != (const SizeValue& other) const {
            return !operator == (other);
        }

    private:
        float m_value;
        CoordType m_type;
    };


    /**
     * Position anchor type.
     */
    enum class PositionAnchorType {
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
     * A position value.
     */
    class PositionValue {
    public:
        /**
         * Constructor.
         * @param value coordinate value.
         * @param type coordinate type.
         * @param anchorType anchor type.
         */
        PositionValue(float value = 0.0f, CoordType type = CoordType::Pixel, PositionAnchorType anchorType = PositionAnchorType::Start)
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
        PositionValue(float value, PositionAnchorType anchorType)
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
        PositionAnchorType getAnchorType() const {
            return m_anchorType;
        }

        /**
         * Sets the anchor type.
         * @param anchorType the anchor type.
         */
        void setAnchorType(PositionAnchorType anchorType) {
            m_anchorType = anchorType;
        }

        /**
         * Sets the value, the type and the anchor type.
         * @param value coordinate value.
         * @param type coordinate type.
         * @param anchorType anchor type.
         */
        void set(float value, CoordType type = CoordType::Pixel, PositionAnchorType anchorType = PositionAnchorType::Start) {
            m_value = value >= 0.0f ? value : 0.0f;
            m_type = type;
            m_anchorType = anchorType;
        }

        /**
         * Compares two coordinates for equality.
         * @param other the other coordinate to compare to this.
         * @return true if the two coordinates are equal, false otherwise.
         */
        bool operator == (const PositionValue& other) const {
            return m_type == other.m_type && m_value == other.m_value && m_anchorType == other.m_anchorType;
        }

        /**
         * Compares two coordinates for imequality.
         * @param other the other coordinate to compare to this.
         * @return true if the two coordinates are not equal, false otherwise.
         */
        bool operator != (const PositionValue& other) const {
            return !operator == (other);
        }

    private:
        float m_value;
        CoordType m_type;
        PositionAnchorType m_anchorType;
    };


    template <class T>
    float calcPixelValue(const T& coord, float containerSize, float containerScaling) {
        switch (coord.getType()) {
            case CoordType::Pixel:
                return coord.getValue();
            case CoordType::Percent:
                return (coord.getValue() / 100.0f) * containerSize / containerScaling;
            }
        throw std::invalid_argument("calcPixelValue: invalid coord type");
    }


} //namespace algui


#endif //ALGUI_COORDTYPE_HPP
