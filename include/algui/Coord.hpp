#ifndef ALGUI_COORD_HPP
#define ALGUI_COORD_HPP


namespace algui {


    /**
     * A coordinate.
     * It can be a pixel value, or percent.
     */
    class Coord {
    public:
        /**
         * Type of coordinate.
         */
        enum Type {
            /**
             * Pixel coordinate.
             */
            Pixel,

            /**
             * Percent coordinate.
             */
            Percent
        };

        /**
         * The constructor.
         * @param v initial value.
         * @param t initial type.
         */
        Coord(float v = 0.0f, Type t = Pixel) 
            : m_value(v)
            , m_type(t)
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
         * Returns the type.
         * @return the type.
         */
        Type getType() const {
            return m_type;
        }

        /**
         * Returns the value.
         * @return the value.
         */
        operator float() const {
            return m_value;
        }

        /**
         * Calculates the pixel value of this coordinate,
         * based on size and scaling of container.
         * @param size size of container.
         * @param scaling interior scaling of container; ignored for percentages.
         * @return pixel value, based on type.
         */
        float calcPixelValue(float size, float scaling = 1.0f) const {
            if (m_type == Percent) {
                return m_value / 100.0 * size;
            }
            return m_value * scaling;
        }

        /**
         * Equality comparison.
         * @param coord coord to compare to this.
         * @return true if they are equal, false otherwise.
         */
        bool operator == (const Coord& coord) const {
            return coord.m_value == m_value && coord.m_type == m_type;
        }

        /**
         * Difference comparison.
         * @param coord coord to compare to this.
         * @return true if they are different, false otherwise.
         */
        bool operator != (const Coord& coord) const {
            return coord.m_value != m_value || coord.m_type != m_type;
        }

    private:
        float m_value;
        Type m_type;

    };


    /**
     * Literal operator that defines a pixel value coordinate.
     * @param value the value; truncated to float.
     * @return a pixel coordinate.
     */
    inline Coord operator ""_px(unsigned long long value) {
        return Coord((float)value, Coord::Pixel);
    }


    /**
     * Literal operator that defines a pixel value coordinate.
     * @param value the value; truncated to float.
     * @return a pixel coordinate.
     */
    inline Coord operator ""_px(long double value) {
        return Coord((float)value, Coord::Pixel);
    }


    /**
     * Literal operator that defines a percent value coordinate.
     * @param value the value; truncated to float.
     * @return a percent coordinate.
     */
    inline Coord operator ""_pct(unsigned long long value) {
        return Coord((float)value, Coord::Percent);
    }


    /**
     * Literal operator that defines a percent value coordinate.
     * @param value the value; truncated to float.
     * @return a percent coordinate.
     */
    inline Coord operator ""_pct(long double value) {
        return Coord((float)value, Coord::Percent);
    }


} //namespace algui


#endif //ALGUI_COORD_HPP
