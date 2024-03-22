#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include <allegro5/allegro_ttf.h>
#include "algui/widget.h"


static ALLEGRO_FONT* test_font;


unsigned char random255() {
    return (unsigned char)(rand() * 255);
}


ALLEGRO_COLOR random_color() {
    return al_map_rgb(random255(), random255(), random255());
}


typedef struct TEST {
    ALG_WIDGET widget;
} TEST;


static void test_paint(ALG_WIDGET* wgt, ALG_DATA_PAINT* data) {
    al_draw_filled_rectangle(data->x1, data->y1, data->x2, data->y2, al_map_rgb(255, 255, 255));
    al_draw_rectangle(data->x1, data->y1, data->x2, data->y2, al_map_rgb(0, 0, 0), 1);
}


static void test_props_changed(ALG_WIDGET* wgt, ALG_DATA_PROPS_CHANGED* data) {
    const char* id = (const char*)wgt->id;
    printf("props changed for widget %s: ", id);
    for (int i = 1, c = 0; i < alg_get_bitvector_size(&data->props_changed_bits); ++i) {
        if (alg_get_bitvector_bit(&data->props_changed_bits, i)) {
            if (c > 0) {
                printf(", ");
            }
            printf(alg_get_property_name(i));
            ++c;
        }
    }
    printf("\n");
}


uintptr_t test_proc(ALG_WIDGET* wgt, int id, void* data) {
    switch (id) {
        case ALG_MSG_MALLOC:
            return (uintptr_t)malloc(sizeof(TEST));

        case ALG_MSG_PAINT:
            test_paint(wgt, (ALG_DATA_PAINT*)data);
            return 1;

        case ALG_MSG_PROPS_CHANGED:
            test_props_changed(wgt, (ALG_DATA_PROPS_CHANGED*)data);
            return 1;
    }

    return alg_widget_proc(wgt, id, data);
}


ALG_WIDGET* create_test(const char* id, ALG_WIDGET* parent, int x, int y, int w, int h) {
    return alg_create_child_widget(parent, test_proc, ALG_PROP_X, x, ALG_PROP_Y, y, ALG_PROP_WIDTH, w, ALG_PROP_HEIGHT, h, ALG_PROP_ID, id, 0);
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

    ALG_WIDGET* root = create_test("root", NULL, 0, 0, 800, 600);
    ALG_WIDGET* form1 = create_test("form1", root, 100, 50, 250, 200);
    ALG_WIDGET* form2 = create_test("form2", root, 200, 150, 250, 200);
    ALG_WIDGET* form3 = create_test("form3", root, 300, 250, 250, 200);
    ALG_WIDGET* btn1 = create_test("btn1", form2, 50, 40, 50, 40);
    ALG_WIDGET* btn2 = create_test("btn2", form2, 70, 60, 50, 40);
    ALG_WIDGET* btn3 = create_test("btn3", form2, 90, 80, 50, 40);

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
            alg_paint_widget(root);
            al_flip_display();
        }
    }

    alg_destroy_widget(root);

    return 0;
}
