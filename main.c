#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include <allegro5/allegro_ttf.h>
#include <algui/widget.h>


static ALLEGRO_FONT* test_font;


unsigned char random255() {
    return (unsigned char)(rand() * 255);
}


ALLEGRO_COLOR random_color() {
    return al_map_rgb(random255(), random255(), random255());
}


typedef struct TEST {
    ALGUI_WIDGET widget;
} TEST;


int test_proc(ALGUI_WIDGET* wgt, int msgId, void* msgData) {
    switch (msgId) {
        case ALGUI_MESSAGE_PAINT:
            al_draw_filled_rectangle(wgt->x1, wgt->y1, wgt->x2, wgt->y2, al_map_rgb(255, 255, 255));
            al_draw_rectangle(wgt->x1, wgt->y1, wgt->x2, wgt->y2, al_map_rgb(0, 0, 0), 2);
            return 1;
    }
    return algui_widget_proc(wgt, msgId, msgData);
}


TEST* create_test(ALGUI_WIDGET* parent, float x, float y, float w, float h) {
    TEST* test = (TEST*)malloc(sizeof(TEST));
    algui_init_widget(&test->widget, test_proc, NULL, 1);
    algui_set_widget_geometry(&test->widget, x, y, w, h);
    if (parent) {
        algui_add_widget(parent, &test->widget);
    }
    return test;
}


int main(int argc, const char* argv[])
{
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_TIMER* timer;
    bool redraw = true;

    if (!al_init()) {
        return -1;
    }

    al_install_mouse();
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    display = al_create_display(800, 600);
    timer = al_create_timer(1.0 / 30);

    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    test_font = al_load_ttf_font("SourceSansPro-Regular.ttf", -12, 0);

    TEST* root = create_test(NULL, 0, 0, 800, 600);
    TEST* form1 = create_test(&root->widget, 100, 50, 250, 200);
    TEST* form2 = create_test(&root->widget, 200, 150, 250, 200);
    TEST* form3 = create_test(&root->widget, 300, 250, 250, 200);
    TEST* btn1 = create_test(&form2->widget, 50, 40, 50, 40);
    TEST* btn2 = create_test(&form2->widget, 70, 60, 50, 40);
    TEST* btn3 = create_test(&form2->widget, 90, 80, 50, 40);

    while (1) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        if (event.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
        }

        else if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                break;
            }
        }

        if (redraw) {
            redraw = false;
            //al_clear_to_color(al_map_rgb_f(0, 0, 0));
            algui_paint_widget(&root->widget);
            al_flip_display();
        }
    }

    algui_cleanup_widget(&root->widget);

    return 0;
}
