#include <allegro5/allegro.h>
#include "algui/Rect.hpp"


namespace algui {


    void Rect::getClippingRectangle() {
        int x, y, w, h;
        al_get_clipping_rectangle(&x, &y, &w, &h);
        setPositionAndSize(x, y, w, h);
    }


    void Rect::setClippingRectangle() const {
        al_set_clipping_rectangle(
            (int)std::floor(m_left),
            (int)std::floor(m_top),
            std::max((int)std::ceil(m_right) - (int)std::floor(m_left), 0),
            std::max((int)std::ceil(m_bottom) - (int)std::floor(m_top), 0));
    }


} //namespace algui
