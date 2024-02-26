#include "ALGUI_RECT.h"


//test intersection with point
unsigned algui_rect_intersects_with_point(ALGUI_RECT* rect, float x, float y) {
    return x >= rect->left && y >= rect->top && x < rect->right&& y < rect->bottom;
}
