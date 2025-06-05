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

static std::string spaces(size_t times) {
    return std::string(times, ' ');
}

class Test : public Widget {
public:
    Test(const std::string& id, bool l = false) : m_layout(l) {
        setId(id);
        addEventHandler(Event_MouseEnter, [&](EventType type, const Event& event, EventPhaseType phase) { std::cout << spaces(getDepth()*4) << getId() << ": mouse enter (capture)\n"; return false; }, EventPhase_Capture);
        addEventHandler(Event_MouseEnter, [&](EventType type, const Event& event, EventPhaseType phase) { std::cout << spaces(getDepth() * 4) << getId() << ": mouse enter (bubble)\n"; return false; }, EventPhase_Bubble);
        addEventHandler(Event_MouseMove, [&](EventType type, const Event& event, EventPhaseType phase) { std::cout << spaces(getDepth() * 4) << getId() << ": mouse move (capture)\n"; return false; }, EventPhase_Capture);
        addEventHandler(Event_MouseMove, [&](EventType type, const Event& event, EventPhaseType phase) { std::cout << spaces(getDepth() * 4) << getId() << ": mouse move (bubble)\n"; return false; }, EventPhase_Bubble);
        addEventHandler(Event_MouseLeave, [&](EventType type, const Event& event, EventPhaseType phase) { std::cout << spaces(getDepth() * 4) << getId() << ": mouse leave (capture)\n"; return false; }, EventPhase_Capture);
        addEventHandler(Event_MouseLeave, [&](EventType type, const Event& event, EventPhaseType phase) { std::cout << spaces(getDepth() * 4) << getId() << ": mouse leave (bubble)\n"; return false; }, EventPhase_Bubble);
        addEventHandler(Event_MouseWheel, [&](EventType type, const Event& event, EventPhaseType phase) { std::cout << spaces(getDepth() * 4) << getId() << ": mouse wheel (capture)\n"; return false; }, EventPhase_Capture);
        addEventHandler(Event_MouseWheel, [&](EventType type, const Event& event, EventPhaseType phase) { std::cout << spaces(getDepth() * 4) << getId() << ": mouse wheel (bubble)\n"; return false; }, EventPhase_Bubble);
        addEventHandler(Event_MouseButtonDown, [&](EventType type, const Event& event, EventPhaseType phase) { std::cout << spaces(getDepth() * 4) << getId() << ": mouse button down (capture)\n"; return false; }, EventPhase_Capture);
        addEventHandler(Event_MouseButtonDown, [&](EventType type, const Event& event, EventPhaseType phase) { std::cout << spaces(getDepth() * 4) << getId() << ": mouse button down (bubble)\n"; return false; }, EventPhase_Bubble);
        addEventHandler(Event_MouseButtonUp, [&](EventType type, const Event& event, EventPhaseType phase) { std::cout << spaces(getDepth() * 4) << getId() << ": mouse button up (capture)\n"; return false; }, EventPhase_Capture);
        addEventHandler(Event_MouseButtonUp, [&](EventType type, const Event& event, EventPhaseType phase) { std::cout << spaces(getDepth() * 4) << getId() << ": mouse button up (bubble)\n"; return false; }, EventPhase_Bubble);
        addEventHandler(Event_Click, [&](EventType type, const Event& event, EventPhaseType phase) { std::cout << spaces(getDepth() * 4) << getId() << ": click (bubble)\n"; return false; }, EventPhase_Bubble);
        addEventHandler(Event_DoubleClick, [&](EventType type, const Event& event, EventPhaseType phase) { std::cout << spaces(getDepth() * 4) << getId() << ": double click (bubble)\n"; return false; }, EventPhase_Bubble);
        addEventHandler(Event_TripleClick, [&](EventType type, const Event& event, EventPhaseType phase) { std::cout << spaces(getDepth() * 4) << getId() << ": triple click (bubble)\n"; return false; }, EventPhase_Bubble);
    }

protected:
    void onLayout() const override {
        if (m_layout) {
            float x = 0;
            forEach([&](Widget* child) {
                child->setLeft(x);
                child->setTop(0);
                child->setHeight(50_pct);
                x += child->getWidth();
            });
        }
    }

    void onPaint() const override {
        al_draw_filled_rectangle(getScreenLeft(), getScreenTop(), getScreenRight(), getScreenBottom(), isEnabledTree() ? al_map_rgb(255, 255, 255) : al_map_rgb(192, 192, 192));
        al_draw_rectangle(getScreenLeft() + 0.5f, getScreenTop() + 0.5f, getScreenRight(), getScreenBottom(), al_map_rgb(0, 0, 0), isFocused() ? 4 : 0);
    }

private:
    bool m_layout;
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

    std::shared_ptr<Test> root = std::make_shared<Test>("root");
    root->setWidth(al_get_display_width(display));
    root->setHeight(al_get_display_height(display));

    Test* form1 = new Test("form1");
    form1->setLeft(100);
    form1->setTop(50);
    form1->setWidth(200);
    form1->setHeight(150);
    root->add(form1);

    Test* form2 = new Test("form2", false);
    form2->setLeft(200);
    form2->setTop(150);
    form2->setWidth(200);
    form2->setHeight(150);
    //form2->setEnabled(false);
    form2->setFocusContainer(true);
    root->add(form2);

    Test* form3 = new Test("form3");
    form3->setLeft(300);
    form3->setTop(250);
    form3->setWidth(200);
    form3->setHeight(150);
    root->add(form3);

    Test* button1 = new Test("button1");
    button1->setLeft(-20);
    button1->setTop(-10);
    button1->setWidth(50);
    button1->setHeight(40);
    form2->add(button1);

    Test* button2 = new Test("button2");
    button2->setLeft(70);
    button2->setTop(60);
    button2->setWidth(50_pct);
    button2->setHeight(40);
    //button2->setFocusContainer(true);
    button2->setFocusable(false);
    form2->add(button2);

    Test* button3 = new Test("button3");
    button3->setLeft(180);
    button3->setTop(80);
    button3->setWidth(50);
    button3->setHeight(40);
    form2->add(button3);
    button3->addEventHandler(Event_GotFocus, [&](EventType eventType, const Event& event, EventPhaseType phaseType) {
        std::cout << "Button 3 got the focus\n";
        return false;
    }, EventPhase_Bubble);

    Test* buttonChild1 = new Test("buttonChild1");
    buttonChild1->setLeft(10);
    buttonChild1->setTop(5);
    buttonChild1->setWidth(30);
    buttonChild1->setHeight(20);
    button2->add(buttonChild1);

    Test* buttonChild2 = new Test("buttonChild2");
    buttonChild2->setLeft(20);
    buttonChild2->setTop(15);
    buttonChild2->setWidth(30);
    buttonChild2->setHeight(20);
    button2->add(buttonChild2);

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

            default:
                std::cout << "-------------------------------------------------------------------------------\n";
                root->processAllegroEvent(event);
                break;
        }
    }

    END:
    al_destroy_timer(timer);
    al_destroy_event_queue(eventQueue);
    al_destroy_display(display);

    return 0;
}
