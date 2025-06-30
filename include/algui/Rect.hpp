#ifndef ALGUI_RECT_HPP
#define ALGUI_RECT_HPP


#ifdef max
#undef max
#undef min
#endif


#include <cmath>
#include <algorithm>


namespace algui {


    /**
     * A rectangle.
     */
    class Rect {
    public:
        /**
         * The constructor.
         * @param left left coordinate.
         * @param top top coordinate.
         * @param right right coordinate.
         * @param bottom bottom coordinate.
         */
        Rect(float left = 0.0f, float top = 0.0f, float right = 0.0f, float bottom = 0.0f)
            : m_left(left)
            , m_top(top)
            , m_right(right)
            , m_bottom(bottom)
        {
        }

        /**
         * Returns the left coordinate.
         * @return the left coordinate.
         */
        float getLeft() const {
            return m_left;
        }

        /**
         * Returns the top coordinate.
         * @return the top coordinate.
         */
        float getTop() const {
            return m_top;
        }

        /**
         * Returns the right coordinate.
         * @return the right coordinate.
         */
        float getRight() const {
            return m_right;
        }

        /**
         * Returns the bottom coordinate.
         * @return the bottom coordinate.
         */
        float getBottom() const {
            return m_bottom;
        }

        /**
         * Returns the width.
         * @return the width.
         */
        float getWidth() const {
            return m_right - m_left;
        }

        /**
         * Returns the height.
         * @return the height.
         */
        float getHeight() const {
            return m_bottom - m_top;
        }

        /**
         * Checks if a point lies within a rectangle.
         * @param x horizontal coordinate.
         * @param y vertical coordinate.
         * @return true if the point lies within the rectangle, false otherwise.
         */
        bool intersects(float x, float y) const {
            return x >= m_left && x < m_right && y >= m_top && y < m_bottom;
        }

        /**
         * Checks if this rectangle intersects another rectangle.
         * @param r the other rectangle to check.
         * @return true if this intersects the other rectangle, false otherwise.
         */
        bool intersects(const Rect& r) const {
            return r.m_left < m_right && r.m_right > m_left && r.m_top < m_bottom && r.m_bottom > m_top;
        }

        /**
         * Checks if the rectangle is valid, i.e. left < right and top < bottom.
         * @return true if the rectangle is valid, false otherwise.
         */
        bool isValid() const {
            return m_left < m_right && m_top < m_bottom;
        }

        /**
         * Sets the left coordinate.
         * @param left the left coordinate.
         */
        void setLeft(float left) {
            m_left = left;
        }

        /**
         * Sets the top coordinate.
         * @param top the top coordinate.
         */
        void setTop(float top) {
            m_top = top;
        }

        /**
         * Sets the right coordinate.
         * @param right the right coordinate.
         */
        void setRight(float right) {
            m_right = right;
        }

        /**
         * Sets the bottom coordinate.
         * @param bottom the bottom coordinate.
         */
        void setBottom(float bottom) {
            m_bottom = bottom;
        }

        /**
         * Sets the top-left coordinate.
         * @param top the top coordinate.
         * @param left the left coordinate.
         */
        void setTopLeft(float top, float left) {
            m_top = top;
            m_left = left;
        }

        /**
         * Sets the top-right coordinate.
         * @param top the top coordinate.
         * @param right the right coordinate.
         */
        void setTopRight(float top, float right) {
            m_top = top;
            m_right = right;
        }

        /**
         * Sets the bottom-left coordinate.
         * @param bottom the bottom coordinate.
         * @param left the left coordinate.
         */
        void setBottomLeft(float bottom, float left) {
            m_bottom = bottom;
            m_left = left;
        }

        /**
         * Sets the bottom-right coordinate.
         * @param bottom the bottom coordinate.
         * @param right the right coordinate.
         */
        void setBottomRight(float bottom, float right) {
            m_bottom = bottom;
            m_right = right;
        }

        /**
         * Sets the coordinates.
         * @param left left coordinate.
         * @param top top coordinate.
         * @param right right coordinate.
         * @param bottom bottom coordinate.
         */
        void set(float left, float top, float right, float bottom) {
            m_left = left;
            m_top = top;
            m_right = right;
            m_bottom = bottom;
        }

        /**
         * Moves the rectangle to the specified position.
         * @param x the new left coordinates.
         * @param y the new top coordinate.
         */
        void setPosition(float x, float y) {
            float w = getWidth();
            float h = getHeight();
            m_left = x;
            m_top = y;
            m_right = x + w;
            m_bottom = y + h;
        }

        /**
         * Sets the size of the rectangle.
         * @param width the width.
         * @param height the height.
         */
        void setSize(float width, float height) {
            m_right = m_left + width;
            m_bottom = m_top + height;
        }

        /**
         * Sets the position and size of the rectangle.
         * @param x left coordinate.
         * @param y top coordinate.
         * @param width the width.
         * @param height the height.
         */
        void setPositionAndSize(float x, float y, float width, float height) {
            m_left = x;
            m_top = y;
            m_right = x + width;
            m_bottom = y + height;
        }

        /**
         * Makes sure neither width nor height is less than 0.
         */
        void clampSizeTo0() {
            m_right = std::max(m_left, m_right);
            m_bottom = std::max(m_top, m_bottom);
        }

        /**
         * Calculates the intersection of the given rectangles into this rectangle.
         * @param a 1st rectangle.
         * @param b 2nd rectangle.
         */
        void intersectionOf(const Rect& a, const Rect& b) {
            m_left = std::max(a.m_left, b.m_left);
            m_top = std::max(a.m_top, b.m_top);
            m_right = std::min(a.m_right, b.m_right);
            m_bottom = std::min(a.m_bottom, b.m_bottom);
        }

        /**
         * Calculates the union of the given rectangles into this rectangle.
         * @param a 1st rectangle.
         * @param b 2nd rectangle.
         */
        void unionOf(const Rect& a, const Rect& b) {
            m_left = std::min(a.m_left, b.m_left);
            m_top = std::min(a.m_top, b.m_top);
            m_right = std::max(a.m_right, b.m_right);
            m_bottom = std::max(a.m_bottom, b.m_bottom);
        }

        /**
         * Stores the current bitmap clipping into this rectangle.
         */
        void getClippingRectangle();

        /**
         * Sets the current bitmap clipping from this rectangle.
         */
        void setClippingRectangle() const;

        /**
         * Checks if the two rectangles are equal.
         * @param r the other rectangle to compare to this.
         * @return true if they are equal, false otherwise.
         */
        bool operator == (const Rect& r) const {
            return m_left == r.m_left && m_top == r.m_top && m_right == r.m_right && m_bottom == r.m_bottom;
        }

        /**
         * Checks if the two rectangles are different.
         * @param r the other rectangle to compare to this.
         * @return true if they are different, false otherwise.
         */
        bool operator != (const Rect& r) const {
            return m_left != r.m_left || m_top != r.m_top || m_right != r.m_right || m_bottom != r.m_bottom;
        }

    private:
        float m_left;
        float m_top;
        float m_right;
        float m_bottom;
    };


} //namespace algui


#endif //ALGUI_RECT_HPP
