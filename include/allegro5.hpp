#ifndef ALGUI_ALLEGRO5_HPP
#define ALGUI_ALLEGRO5_HPP


#include "Painter.hpp"


namespace algui {


    class allegro5 {
    public:
        static PainterPtr getPainter();
    };


} //namespace algui


#endif //ALGUI_ALLEGRO5_HPP
