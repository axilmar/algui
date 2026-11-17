#include "allegro5.hpp"
#include "allegro5/Allegro5Painter.hpp"


namespace algui {


    PainterPtr allegro5::getPainter() {
        return std::make_shared<Allegro5Painter>();
    }


} //namespace algui
