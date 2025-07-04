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

extern void run_tests();

#include "algui/InteractiveUINode.hpp"

using namespace algui;

class Test : public InteractiveUINode {
public:
    Test(const std::string& id) : m_id(id) {
        addEventListener("mouseEnter", [&](const Event& event) { std::cout << std::string(getDepth() * 4, ' ') << m_id << ": mouseEnter\n"; return false; });
        addEventListener("mouseMove", [&](const Event& event) { std::cout << std::string(getDepth() * 4, ' ') << m_id << ": mouseMove\n"; return false; });
        addEventListener("mouseLeave", [&](const Event& event) { std::cout << std::string(getDepth() * 4, ' ') << m_id << ": mouseLeave\n"; return false; });
        addEventListener("mouseWheel", [&](const Event& event) { std::cout << std::string(getDepth() * 4, ' ') << m_id << ": mouseWheel\n"; return false; });
        addEventListener("mouseButtonDown", [&](const Event& event) { std::cout << std::string(getDepth() * 4, ' ') << m_id << ": mouseButtonDown\n"; return false; });
        addEventListener("mouseButtonUp", [&](const Event& event) { std::cout << std::string(getDepth() * 4, ' ') << m_id << ": mouseButtonUp\n"; return false; });
        addEventListener("dragEnter", [&](const Event& event) { std::cout << std::string(getDepth() * 4, ' ') << m_id << ": dragEnter\n"; return false; });
        addEventListener("drag", [&](const Event& event) { std::cout << std::string(getDepth() * 4, ' ') << m_id << ": dragMove\n"; return false; });
        addEventListener("dragLeave", [&](const Event& event) { std::cout << std::string(getDepth() * 4, ' ') << m_id << ": dragLeave\n"; return false; });
        addEventListener("dragWheel", [&](const Event& event) { std::cout << std::string(getDepth() * 4, ' ') << m_id << ": dragWheel\n"; return false; });
        addEventListener("drop", [&](const Event& event) { std::cout << std::string(getDepth() * 4, ' ') << m_id << ": drop\n"; return false; });
    }

protected:
    void paint() const override {
        const Rect& r = getScreenRect();
        al_draw_filled_rectangle(r.left, r.top, r.right, r.bottom, isEnabledTree() ? al_map_rgb(255, 255, 255) : al_map_rgb(192, 192, 192));
        al_draw_rectangle(r.left + 0.5f, r.top + 0.5f, r.right, r.bottom, isErrorTree() ? al_map_rgb(255, 0, 0) : al_map_rgb(0, 0, 0), 0);
    }

private:
    std::string m_id;
};

int main(int argc, char** argv) {
    //run_tests();
    //return 0;

    al_init();
    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    al_install_keyboard();
    al_install_mouse();
    al_install_joystick();
    al_install_audio();

    const Scaling scaling{1, 1};

    //al_set_new_display_flags(ALLEGRO_FULLSCREEN | ALLEGRO_OPENGL);
    ALLEGRO_DISPLAY* display = al_create_display(800*scaling.horizontal, 600*scaling.vertical);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0/60.0);
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());
    al_register_event_source(eventQueue, al_get_joystick_event_source());
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_joystick_event_source());

    al_start_timer(timer);

    std::shared_ptr<Test> root = std::make_shared<Test>("root");
    root->setRect(Rect::rect(0, 0, al_get_display_width(display), al_get_display_height(display)));
    root->setScaling(scaling);

    std::shared_ptr<Test> form1 = std::make_shared<Test>("form1");
    form1->setRect(Rect::rect(100, 50, 250, 200));
    root->addChild(form1);

    std::shared_ptr<Test> form2 = std::make_shared<Test>("form2");
    form2->setRect(Rect::rect(200, 150, 250, 200));
    root->addChild(form2);

    std::shared_ptr<Test> form3 = std::make_shared<Test>("form3");
    form3->setRect(Rect::rect(300, 250, 250, 200));
    root->addChild(form3);

    std::shared_ptr<Test> button1 = std::make_shared<Test>("button1");
    button1->setRect(Rect::rect(50, 40, 50, 40));
    form2->addChild(button1);

    std::shared_ptr<Test> button2 = std::make_shared<Test>("button2");
    button2->setRect(Rect::rect(70, 60, 50, 40));
    form2->addChild(button2);

    std::shared_ptr<Test> button3 = std::make_shared<Test>("button3");
    button3->setRect(Rect::rect(230, 80, 50, 40));
    form2->addChild(button3);

    button2->addEventListener("mouseMove", [&](const Event& event) { 
        return button2->beginDragAndDrop(static_cast<const MouseEvent&>(event), std::string("data"));
    });

    ALLEGRO_BITMAP* target = al_get_target_bitmap();

    ALLEGRO_BITMAP* bmp = al_create_bitmap(32, 32);
    al_set_target_bitmap(bmp);
    al_draw_filled_rectangle(0, 0, 32, 32, al_map_rgb(128, 64, 192));

    ALLEGRO_BITMAP* bmp1 = al_create_bitmap(8, 8);
    al_set_target_bitmap(bmp1);
    al_draw_filled_rectangle(0, 0, 8, 8, al_map_rgb(255, 0, 0));

    al_set_target_bitmap(target);

    std::vector<DraggedImage> draggedImages({ DraggedImage{bmp, 16, 16, true}, DraggedImage{bmp1, 16, -8, false} });

    button3->addEventListener("dragEnter", [&](const Event& event) { 
        InteractiveUINode::setDraggedImages(&draggedImages);
        return false;
    });

    button3->addEventListener("dragLeave", [&](const Event& event) { 
        InteractiveUINode::setDraggedImages(nullptr);
        return false;
    });

    root->addEventListener("dragKeyDown", [&](const KeyboardEvent& event) {
        if (event.getKeyCode() == ALLEGRO_KEY_LSHIFT) {
            draggedImages[1].enabled = true;
        }
        return false;
    });

    root->addEventListener("dragKeyUp", [&](const KeyboardEvent& event) { 
        if (event.getKeyCode() == ALLEGRO_KEY_LSHIFT) {
            draggedImages[1].enabled = false;
        }
        return false;
    });

    for (;;) {
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                goto END;

            case ALLEGRO_EVENT_TIMER:
                root->render();
                al_flip_display();
                break;

            case ALLEGRO_EVENT_MOUSE_AXES:
                std::cout << "mouse x = " << event.mouse.x << ", mouse y = " << event.mouse.y << std::endl;
                root->doEvent(event);
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    goto END;
                }
                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    /*
                    Rect r = form2->getRect();
                    r.setPosition(r.left + 8, r.top);
                    form2->setRect(r);
                    */
                    //form2->setClipped(!form2->isClipped());
                    //form2->setEnabled(!form2->isEnabled());
                    form2->setError(!form2->isError());
                    break;
                }
                [[fallthrough]];

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
