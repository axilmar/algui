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
    bool m_dragAndDrop = false;
    ALLEGRO_BITMAP* m_dragBitmap = nullptr;

    ~Test() {
        if (m_dragBitmap) {
            al_destroy_bitmap(m_dragBitmap);
        }
    }

protected:
    void paint() const override {
        al_draw_filled_rectangle(getX1(), getY1(), getX2(), getY2(), al_map_rgb(255, 255, 255));
        al_draw_rectangle(getX1(), getY1(), getX2(), getY2(), al_map_rgb(0, 0, 0), getFocused() ? 2 : 1);
    }

    bool mouseButtonDown(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": mouseButtonDown\n";
        setFocused(true);
        return Widget::mouseButtonDown(event);
    }

    bool mouseButtonUp(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": mouseButtonUp\n";
        return Widget::mouseButtonUp(event);
    }

    bool mouseEnter(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": mouseEnter\n";
        return Widget::mouseEnter(event);
    }

    bool mouseMove(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": mouseMove\n";
        if (m_dragAndDrop) {
            beginDragAndDrop(std::string("test data"));
        }
        return Widget::mouseMove(event);
    }

    bool mouseLeave(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": mouseLeave\n";
        return Widget::mouseLeave(event);
    }

    bool mouseWheel(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": mouseWheel\n";
        return Widget::mouseWheel(event);
    }

    bool keyDown(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": keyDown\n";
        return Widget::keyDown(event);
    }

    bool keyUp(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": keyUp\n";
        return Widget::keyUp(event);
    }

    bool keyChar(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": keyChar\n";
        return Widget::keyChar(event);
    }

    bool unusedKeyDown(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": unusedKeyDown\n";
        return Widget::unusedKeyDown(event);
    }

    bool unusedKeyUp(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": unusedKeyUp\n";
        return Widget::unusedKeyUp(event);
    }

    bool unusedKeyChar(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": unusedKeyChar\n";
        return Widget::unusedKeyChar(event);
    }

    bool drop(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": drop\n";
        return Widget::drop(event);
    }

    bool dragEnter(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": dragEnter\n";
        if (m_dragBitmap) {
            setDragIcon(m_dragBitmap, 8, 8);
        }
        return Widget::dragEnter(event);
    }

    bool drag(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": drag\n";
        return Widget::drag(event);
    }

    bool dragLeave(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": dragLeave\n";
        if (m_dragBitmap) {
            setDragIcon(nullptr);
        }
        return Widget::dragLeave(event);
    }

    bool dragWheel(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": dragWheel\n";
        return Widget::dragWheel(event);
    }

    bool dragKeyDown(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": dragKeyDown\n";
        return Widget::dragKeyDown(event);
    }

    bool dragKeyUp(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": dragKeyUp\n";
        return Widget::dragKeyUp(event);
    }

    bool dragKeyChar(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth()*4, ' ') << getId() << ": dragKeyChar\n";
        return Widget::dragKeyChar(event);
    }

    bool click(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth() * 4, ' ') << getId() << ": click\n";
        return Widget::click(event);
    }

    bool doubleClick(const ALLEGRO_EVENT& event) override {
        std::cout << std::string(getDepth() * 4, ' ') << getId() << ": double click\n";
        return Widget::doubleClick(event);
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
    al_register_event_source(eventQueue, al_get_joystick_event_source());

    al_start_timer(timer);

    std::shared_ptr<Test> root = std::make_shared<Test>();
    root->setId("root");
    root->setSize(al_get_display_width(display), al_get_display_height(display));

    std::shared_ptr<Test> form1 = std::make_shared<Test>();
    form1->setId("form1");
    form1->setGeometry(100, 50, 250, 200);
    root->add(form1.get());

    std::shared_ptr<Test> form2 = std::make_shared<Test>();
    form2->setId("form2");
    form2->setGeometry(200, 150, 250, 200);
    root->add(form2.get());

    std::shared_ptr<Test> form3 = std::make_shared<Test>();
    form3->setId("form3");
    form3->setGeometry(300, 250, 250, 200);
    root->add(form3.get());

    std::shared_ptr<Test> button1 = std::make_shared<Test>();
    button1->setId("button1");
    button1->setGeometry(50, 40, 50, 40);
    form2->add(button1.get());

    std::shared_ptr<Test> button2 = std::make_shared<Test>();
    button2->setId("button2");
    button2->setGeometry(70, 60, 50, 40);
    button2->m_dragAndDrop = true;
    form2->add(button2.get());

    std::shared_ptr<Test> button3 = std::make_shared<Test>();
    button3->setId("button3");
    button3->setGeometry(90, 80, 50, 40);
    button3->m_dragBitmap = al_create_bitmap(16, 16);
    ALLEGRO_BITMAP* orgTarget = al_get_target_bitmap();
    al_set_target_bitmap(button3->m_dragBitmap);
    al_draw_filled_rectangle(0, 0, 16, 16, al_map_rgb(0, 255, 0));
    al_set_target_bitmap(orgTarget);
    form2->add(button3.get());

    for (;;) {
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                goto END;

            case ALLEGRO_EVENT_TIMER:
                root->doEvent(event);
                root->render();
                al_flip_display();
                break;

            default:
                root->doEvent(event);
                break;
        }
    }

    END:
    al_destroy_timer(timer);
    al_destroy_event_queue(eventQueue);
    al_destroy_display(display);

    return 0;
}
