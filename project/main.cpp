#include <iostream>
#include <memory>
#include <string>

#pragma warning (disable: 4309)
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_color.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_audio.h"

#include "algui/Widget.hpp"
using namespace algui;

class Test : public Widget {
public:

protected:
    void onPaint() const override {
        al_draw_filled_rectangle(getScreenLeft(), getScreenTop(), getScreenRight(), getScreenBottom(), isEnabledTree() ? al_map_rgb(255, 255, 255) : al_map_rgb(192, 192, 192));
        al_draw_rectangle(getScreenLeft() + 0.5f, getScreenTop() + 0.5f, getScreenRight(), getScreenBottom(), al_map_rgb(0, 0, 0), isFocused() ? 2 : 0);
    }
};

int main(int argc, char** argv) {
    al_init();
    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    al_install_keyboard();
    al_install_mouse();
    al_install_joystick();
    al_install_audio();

    ALLEGRO_DISPLAY* display = al_create_display(800, 600);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0/60.0);
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());
    al_register_event_source(eventQueue, al_get_joystick_event_source());
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    std::shared_ptr<Test> root = std::make_shared<Test>();
    root->setWidth(al_get_display_width(display));
    root->setHeight(al_get_display_height(display));

    Test* form1 = new Test();
    form1->setLeft(100);
    form1->setTop(50);
    form1->setWidth(200);
    form1->setHeight(150);
    root->add(form1);

    Test* form2 = new Test();
    form2->setLeft(200);
    form2->setTop(150);
    form2->setWidth(200);
    form2->setHeight(150);
    //form2->setEnabled(false);
    root->add(form2);

    Test* form3 = new Test();
    form3->setLeft(300);
    form3->setTop(250);
    form3->setWidth(200);
    form3->setHeight(150);
    root->add(form3);

    Test* button1 = new Test();
    button1->setLeft(50);
    button1->setTop(40);
    button1->setWidth(50);
    button1->setHeight(40);
    form2->add(button1);

    Test* button2 = new Test();
    button2->setLeft(70);
    button2->setTop(60);
    button2->setWidth(50);
    button2->setHeight(40);
    form2->add(button2);

    Test* button3 = new Test();
    button3->setLeft(90);
    button3->setTop(80);
    button3->setWidth(50);
    button3->setHeight(40);
    form2->add(button3);

    //form2->setCalculateGeometryConstraintsCallback([](Widget* wgt) {});

    for (;;) {
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                goto END;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                //form2->setLeft(form2->getLeft() + 8);
                //form2->setWidth(form2->getWidth() + 8);
                if (!Widget::getFocusedWidget()) {
                    button1->setFocused(true);
                }
                else if (Widget::getFocusedWidget() == button1) {
                    button2->setFocused(true);
                }
                else if (Widget::getFocusedWidget() == button2) {
                    button3->setFocused(true);
                }
                else if (Widget::getFocusedWidget() == button3) {
                    button3->setFocused(false);
                }
                break;

            case ALLEGRO_EVENT_TIMER:
                root->render();
                al_flip_display();
                break;

            default:
                break;
        }
    }

    END:
    al_destroy_timer(timer);
    al_destroy_event_queue(eventQueue);
    al_destroy_display(display);

    return 0;
}
