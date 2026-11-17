#ifndef ALGUI_ALLEGRO5_PAINTER_HPP
#define ALGUI_ALLEGRO5_PAINTER_HPP


#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "Painter.hpp"


namespace algui {


    class Allegro5Painter : public Painter {
    private:
        static ALLEGRO_COLOR color(const Color& color) {
            return ALLEGRO_COLOR{ color.red, color.green, color.blue, color.alpha };
        }


        static Color color(const ALLEGRO_COLOR& color) {
            return Color{ color.r, color.g, color.b, color.a };
        }

    public:
        Color createColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) override {
            return color(al_map_rgba(red, green, blue, alpha));
        }

        void drawRect(float left, float top, float right, float bottom, const Color& color, float thickness) override {
            al_draw_rectangle(left, top, right, bottom, Allegro5Painter::color(color), thickness);
        }

        void fillRect(float left, float top, float right, float bottom, const Color& color) override {
            al_draw_filled_rectangle(left, top, right, bottom, Allegro5Painter::color(color));
        }

        void getClipping(float& left, float& top, float& right, float& bottom) override {
            int x, y, width, height;
            al_get_clipping_rectangle(&x, &y, &width, &height);
            left = x;
            top = y;
            right = x + width;
            bottom = y + height;
        }

        void setClipping(float left, float top, float right, float bottom) override {
            int x = (int)ceilf(left);
            int y = (int)ceilf(top);
            int width = (int)floorf(right - left);
            int height = (int)floorf(bottom - top);
            al_set_clipping_rectangle(x, y, width, height);
        }
    };


} //namespace algui


#endif //ALGUI_ALLEGRO5_PAINTER_HPP
