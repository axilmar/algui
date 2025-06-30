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
    struct Rect {
        ///left coordinate
        float left{ 0 };

        ///top coordinate
        float top{ 0 };

        ///right coordinae
        float right{ 0 };

        ///bottom coordinate
        float bottom{ 0 };

        /**
         * Returns the width.
         * @return the width.
         */
        float getWidth() const {
            return right - left;
        }

        /**
         * Returns the height.
         * @return the height.
         */
        float getHeight() const {
            return bottom - top;
        }

        /**
         * Checks if a point lies within a rectangle.
         * @param x horizontal coordinate.
         * @param y vertical coordinate.
         * @return true if the point lies within the rectangle, false otherwise.
         */
        bool intersects(float x, float y) const {
            return x >= left && x < right && y >= top && y < bottom;
        }

        /**
         * Checks if this rectangle intersects another rectangle.
         * @param r the other rectangle to check.
         * @return true if this intersects the other rectangle, false otherwise.
         */
        bool intersects(const Rect& r) const {
            return r.left < right && r.right > left && r.top < bottom && r.bottom > top;
        }

        /**
         * Checks if the rectangle is valid, i.e. left < right and top < bottom.
         * @return true if the rectangle is valid, false otherwise.
         */
        bool isValid() const {
            return left < right && top < bottom;
        }

        /**
         * Sets the top-left coordinate.
         * @param top the top coordinate.
         * @param left the left coordinate.
         */
        void setTopLeft(float top, float left) {
            top = top;
            left = left;
        }

        /**
         * Sets the top-right coordinate.
         * @param top the top coordinate.
         * @param right the right coordinate.
         */
        void setTopRight(float top, float right) {
            top = top;
            right = right;
        }

        /**
         * Sets the bottom-left coordinate.
         * @param bottom the bottom coordinate.
         * @param left the left coordinate.
         */
        void setBottomLeft(float bottom, float left) {
            bottom = bottom;
            left = left;
        }

        /**
         * Sets the bottom-right coordinate.
         * @param bottom the bottom coordinate.
         * @param right the right coordinate.
         */
        void setBottomRight(float bottom, float right) {
            bottom = bottom;
            right = right;
        }

        /**
         * Sets the coordinates.
         * @param left left coordinate.
         * @param top top coordinate.
         * @param right right coordinate.
         * @param bottom bottom coordinate.
         */
        void set(float left, float top, float right, float bottom) {
            left = left;
            top = top;
            right = right;
            bottom = bottom;
        }

        /**
         * Moves the rectangle to the specified position.
         * @param x the new left coordinates.
         * @param y the new top coordinate.
         */
        void setPosition(float x, float y) {
            float w = getWidth();
            float h = getHeight();
            left = x;
            top = y;
            right = x + w;
            bottom = y + h;
        }

        /**
         * Sets the size of the rectangle.
         * @param width the width.
         * @param height the height.
         */
        void setSize(float width, float height) {
            right = left + width;
            bottom = top + height;
        }

        /**
         * Sets the position and size of the rectangle.
         * @param x left coordinate.
         * @param y top coordinate.
         * @param width the width.
         * @param height the height.
         */
        void setPositionAndSize(float x, float y, float width, float height) {
            left = x;
            top = y;
            right = x + width;
            bottom = y + height;
        }

        /**
         * Makes sure neither width nor height is less than 0.
         */
        void clampSizeTo0() {
            right = std::max(left, right);
            bottom = std::max(top, bottom);
        }

        /**
         * Calculates the intersection of the given rectangles into this rectangle.
         * @param a 1st rectangle.
         * @param b 2nd rectangle.
         * @return the intersection of the two rectangles.
         */
        static Rect intersectionOf(const Rect& a, const Rect& b) {
            return { std::max(a.left, b.left), std::max(a.top, b.top), std::min(a.right, b.right), std::min(a.bottom, b.bottom) };
        }

        /**
         * Calculates the union of the given rectangles into this rectangle.
         * @param a 1st rectangle.
         * @param b 2nd rectangle.
         * @return the union of the two rectangles.
         */
        static Rect unionOf(const Rect& a, const Rect& b) {
            return { std::min(a.left, b.left), std::min(a.top, b.top), std::max(a.right, b.right), std::max(a.bottom, b.bottom) };
        }

        /**
         * Stores the current bitmap clipping into a rectangle.
         * @return a rectangle with the current bitmap clipping.
         */
        static Rect getClippingRectangle();

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
            return left == r.left && top == r.top && right == r.right && bottom == r.bottom;
        }

        /**
         * Checks if the two rectangles are different.
         * @param r the other rectangle to compare to this.
         * @return true if they are different, false otherwise.
         */
        bool operator != (const Rect& r) const {
            return left != r.left || top != r.top || right != r.right || bottom != r.bottom;
        }

        /**
         * Checks if the position of the given rect differs from this one.
         * @return r the other node to compare to this.
         * @return true if position differs, false otherwise.
         */
        bool positionDiffers(const Rect& r) const {
            return left != r.left || top != r.top;
        }

        /**
         * Checks if the size of the given rect differs from this one.
         * @return r the other node to compare to this.
         * @return true if size differs, false otherwise.
         */
        bool sizeDiffers(const Rect& r) const {
            return getWidth() != r.getWidth() || getHeight() != r.getHeight();
        }

        /**
         * constructs a rectangle from position and size.
         * @param x left coordinate.
         * @param y top coordinate.
         * @param width width.
         * @param height height.
         */
        static Rect rect(float x, float y, float width, float height) {
            return { x , y, x + width, y + height };
        }
    };


} //namespace algui


#endif //ALGUI_RECT_HPP
