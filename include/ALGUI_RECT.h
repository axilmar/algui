#ifndef ALGUI_RECT_H
#define ALGUI_RECT_H


/**
 * Rectangle.
 */
typedef struct ALGUI_RECT {
    ///left coordinate.
    float left;

    ///top coordinate.
    float top;

    ///right coordinate.
    float right;

    ///bottom coordinate.
    float bottom;
} ALGUI_RECT;


/**
 * Checks if the given point is within the given rectangle.
 * @param rect target rect.
 * @param x x coordinage.
 * @param y y coordinate.
 * @return non-zero if point is within rectangle, false otherwise.
 */
unsigned algui_rect_intersects_with_point(ALGUI_RECT* rect, float x, float y);


#endif //ALGUI_RECT_H
