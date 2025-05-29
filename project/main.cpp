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

#include "algui/UIInteractiveNode.hpp"
using namespace algui;

class Test : public UIInteractiveNode {
public:

protected:
    void onPaint() const override {
        al_draw_filled_rectangle(getScreenX1(), getScreenY1(), getScreenX2(), getScreenY2(), al_map_rgb(255, 255, 255));
        al_draw_rectangle(getScreenX1(), getScreenY1(), getScreenX2(), getScreenY2(), al_map_rgb(0, 0, 0), 1);
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
    root->setSize(al_get_display_width(display), al_get_display_height(display));

    std::shared_ptr<Test> form1 = std::make_shared<Test>();
    form1->setGeometry(100, 50, 200, 150);
    root->addChild(form1);

    std::shared_ptr<Test> form2 = std::make_shared<Test>();
    form2->setGeometry(200, 150, 200, 150);
    //form2->setScaling(0.5f, 0.5f);
    root->addChild(form2);

    std::shared_ptr<Test> form3 = std::make_shared<Test>();
    form3->setGeometry(300, 250, 200, 150);
    root->addChild(form3);

    std::shared_ptr<Test> button1 = std::make_shared<Test>();
    button1->setGeometry(50, 40, 50, 40);
    form2->addChild(button1);

    std::shared_ptr<Test> button2 = std::make_shared<Test>();
    button2->setPosition(10, 60, { 10, CoordAnchorType::End }, 100);
    form2->addChild(button2);

    std::shared_ptr<Test> button3 = std::make_shared<Test>();
    button3->setGeometry(90, 80, 50, 40);
    form2->addChild(button3);

    for (;;) {
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                goto END;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                form2->setLeft(form2->UINode::getLeft() + 8);
                form2->setWidth(form2->UINode::getWidth() + 8);
                break;

            case ALLEGRO_EVENT_TIMER:
                root->renderTree();
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
