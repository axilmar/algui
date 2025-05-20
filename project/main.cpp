#include <iostream>
#include <memory>
#include <string>

#pragma warning (disable: 4309)
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_color.h"
#include "allegro5/allegro_ttf.h"

#include "algui/Widget.hpp"
using namespace algui;

class TestWidget : public Widget {
public:
    bool dragTarget = false;
    bool dropTarget = false;

    void onPaint() const override {
        al_draw_filled_rectangle(screenX1, screenY1, screenX2, screenY2, al_map_rgb(255, 255, 255));
        al_draw_rectangle(screenX1, screenY1, screenX2, screenY2, al_map_rgb(0, 0, 0), hasFocus() ? 2 : 1);
    }

    void onGotFocus() override {
        std::cout << "onGotFocus: " << id << std::endl;
    }

    void onLostFocus() override {
        std::cout << "onLostFocus: " << id << std::endl;
    }

    void onDescentantGotFocus() override {
        std::cout << "onDescentantGotFocus: " << id << std::endl;
    }

    void onDescentantLostFocus() override {
        std::cout << "onDescentantLostFocus: " << id << std::endl;
    }

    bool onMouseEnter(const ALLEGRO_EVENT& event) override {
        std::cout << "onMouseEnter: " << id << std::endl;
        return Widget::onMouseEnter(event);
    }

    bool onMouseMove(const ALLEGRO_EVENT& event) override {
        std::cout << "onMouseMove: " << id << std::endl;
        return Widget::onMouseMove(event);
    }

    bool onMouseLeave(const ALLEGRO_EVENT& event) override {
        std::cout << "onMouseLeave: " << id << std::endl;
        return Widget::onMouseLeave(event);
    }

    bool onMouseButtonDown(const ALLEGRO_EVENT& event) override {
        std::cout << "onMouseButtonDown: " << id << std::endl;
        if (dragTarget) {
            Widget::dragAndDrop = true;
            Widget::draggedContent = "The quick brown fox jumps over the lazy dog.";
            return true;
        }
        return Widget::onMouseButtonDown(event);
    }

    bool onMouseButtonUp(const ALLEGRO_EVENT& event) override {
        std::cout << "onMouseButtonUp: " << id << std::endl;
        return Widget::onMouseButtonUp(event);
    }

    bool onMouseWheel(const ALLEGRO_EVENT& event) override {
        return Widget::onMouseWheel(event);
    }

    bool onKeyDown(const ALLEGRO_EVENT& event) override {
        std::cout << "onKeyDown: " << id << std::endl;
        return true;
    }

    bool onKeyUp(const ALLEGRO_EVENT& event) override {
        std::cout << "onKeyUp: " << id << std::endl;
        return true;
    }

    bool onKeyChar(const ALLEGRO_EVENT& event) override {
        std::cout << "onKeyChar: " << id << std::endl;
        return true;
    }

    bool onDragEnter(const ALLEGRO_EVENT& event) override {
        std::cout << "onDragEnter: " << id << std::endl;
        return Widget::onDragEnter(event);
    }

    bool onDrag(const ALLEGRO_EVENT& event) override {
        std::cout << "onDrag: " << id << std::endl;
        return Widget::onDrag(event);
    }

    bool onDragLeave(const ALLEGRO_EVENT& event) override {
        std::cout << "onDragLeave: " << id << std::endl;
        return Widget::onDragLeave(event);
    }

    bool onDrop(const ALLEGRO_EVENT& event) override {
        std::cout << "onDrop: " << id << std::endl;
        if (dropTarget) {
            const char* msg = std::any_cast<const char*>(Widget::draggedContent);
            std::cout << id << ": dropped message: " << msg << std::endl;
            return true;
        }
        return Widget::onDrop(event);
    }
};

int main(int argc, char** argv) {
    al_init();
    al_init_image_addon();
    al_init_primitives_addon();
    //al_init_ttf_addon();

    al_install_keyboard();
    al_install_mouse();
    al_install_joystick();

    ALLEGRO_DISPLAY* display = al_create_display(800, 600);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0/60.0);
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());
    al_register_event_source(eventQueue, al_get_joystick_event_source());
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));

    std::shared_ptr<Widget> root = std::make_shared<TestWidget>();
    root->id = "root";
    root->width = 800;
    root->height = 600;
    //root->xScaling = 0.5f;
    //root->yScaling = 0.5f;

    TestWidget* form1 = new TestWidget();
    root->addChild(form1);
    form1->id = "form1";
    form1->x = 100;
    form1->y = 50;
    form1->width = 200;
    form1->height = 150;

    TestWidget* form2 = new TestWidget();
    root->addChild(form2);
    form2->id = "form2";
    form2->x = 200;
    form2->y = 150;
    form2->width = 200;
    form2->height = 150;
    //form2->xScaling = 0.5f;
    //form2->yScaling = 0.5f;

    TestWidget* form3 = new TestWidget();
    root->addChild(form3);
    form3->id = "form3";
    form3->x = 300;
    form3->y = 250;
    form3->width = 200;
    form3->height = 150;
    form3->dropTarget = true;

    TestWidget* button1 = new TestWidget();
    form2->addChild(button1);
    button1->id = "button1";
    button1->x = 50;
    button1->y = 40;
    button1->width = 50;
    button1->height = 40;
    button1->dragTarget = true;

    TestWidget* button2 = new TestWidget();
    form2->addChild(button2);
    button2->id = "button2";
    button2->x = 70;
    button2->y = 60;
    button2->width = 50;
    button2->height = 40;

    TestWidget* button3 = new TestWidget();
    form2->addChild(button3);
    button3->id = "button3";
    button3->x = 90;
    button3->y = 80;
    button3->width = 50;
    button3->height = 40;

    Widget::focusFollowsMouse = true;

    al_start_timer(timer);

    for (;;) {
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                goto END;

            case ALLEGRO_EVENT_TIMER:
                root->draw();
                al_flip_display();
                break;

            default:
                root->dispatch(event);
                break;
        }
    }

    END:
    al_destroy_timer(timer);
    al_destroy_event_queue(eventQueue);
    al_destroy_display(display);

    return 0;
}
