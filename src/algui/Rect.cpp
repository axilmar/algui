#include <allegro5/allegro.h>
#include "algui/Rect.hpp"


namespace algui {


    Rect Rect::getClippingRectangle() {
        int x, y, w, h;
        al_get_clipping_rectangle(&x, &y, &w, &h);
        return { (float)x, (float)y, (float)(x + w), (float)(y + h) };
    }


    void Rect::setClippingRectangle() const {
        al_set_clipping_rectangle(
            (int)std::floor(left),
            (int)std::floor(top),
            std::max((int)std::ceil(right) - (int)std::floor(left), 0),
            std::max((int)std::ceil(bottom) - (int)std::floor(top), 0));
    }


} //namespace algui
