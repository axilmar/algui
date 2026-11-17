#ifndef ALGUI_PAINTER_HPP
#define ALGUI_PAINTER_HPP


#include <memory>
#include "Color.hpp"


namespace algui {


    class Painter {
    public:
        virtual ~Painter() {
        }

        virtual Color createColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255) = 0;

        virtual void drawRect(float left, float top, float right, float bottom, const Color& color, float thickness) = 0;

        virtual void fillRect(float left, float top, float right, float bottom, const Color& color) = 0;

        virtual void getClipping(float& left, float& top, float& right, float& bottom) = 0;

        virtual void setClipping(float left, float top, float right, float bottom) = 0;
    };


    using PainterPtr = std::shared_ptr<Painter>;


} //namespace algui


#endif //ALGUI_PAINTER_HPP
