#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "ALGUI_NODE.h"


typedef struct TEST {
    ALGUI_NODE node;
} TEST;


static ALGUI_RESULT test_paint(TEST* test, ALGUI_MESSAGE_DATA_PAINT* data) {
    ALLEGRO_COLOR bg, fg;

    if (!data->enabled) {
        bg = al_map_rgb(0xAA, 0xAA, 0xAA);
        fg = al_map_rgb(0x88, 0x88, 0x88);
    }
    else if (data->highlighted) {
        bg = al_map_rgb(255, 255, 255);
        fg = al_map_rgb(0, 0, 255);
    }
    else if (data->focused) {
        bg = al_map_rgb(255, 255, 255);
        fg = al_map_rgb(0, 255, 0);
    }
    else if (data->pressed) {
        bg = al_map_rgb(0, 0, 255);
        fg = al_map_rgb(0, 0, 0);
    }
    else if (data->selected) {
        bg = al_map_rgb(0, 255, 255);
        fg = al_map_rgb(0, 0, 0);
    }
    else if (data->active) {
        bg = al_map_rgb(255, 128, 128);
        fg = al_map_rgb(0, 0, 0);
    }
    else {
        bg = al_map_rgb(255, 255, 255);
        fg = al_map_rgb(0, 0, 0);
    }

    al_draw_filled_rectangle(data->position.left, data->position.top, data->position.right, data->position.bottom, bg);
    al_draw_rectangle(data->position.left, data->position.top, data->position.right, data->position.bottom, fg, 2);

    return ALGUI_RESULT_OK;
}


static ALGUI_RESULT test_proc(ALGUI_NODE* node, int id, void* data) {
    switch (id) {
        case ALGUI_MESSAGE_PAINT_BACKGROUND:
            return test_paint((TEST*)node, (ALGUI_MESSAGE_DATA_PAINT*)data);
    }
    return algui_node_proc(node, id, data);
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

    display = al_create_display(800, 600);
    timer = al_create_timer(1.0 / 30);

    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    TEST root;
    algui_init_node(&root.node, test_proc, false, NULL);
    root.node.position.right = 800;
    root.node.position.bottom = 600;

    TEST form1;
    algui_init_node(&form1.node, test_proc, false, NULL);
    form1.node.position.left = 100;
    form1.node.position.top = 50;
    form1.node.position.right = form1.node.position.left + 200;
    form1.node.position.bottom = form1.node.position.top + 150;
    algui_insert_child(&root.node, &form1.node, -1);

    TEST form2;
    algui_init_node(&form2.node, test_proc, false, NULL);
    form2.node.position.left = 200;
    form2.node.position.top = 150;
    form2.node.position.right = form2.node.position.left + 200;
    form2.node.position.bottom = form2.node.position.top + 150;
    algui_insert_child(&root.node, &form2.node, -1);

    TEST form3;
    algui_init_node(&form3.node, test_proc, false, NULL);
    form3.node.position.left = 300;
    form3.node.position.top = 250;
    form3.node.position.right = form3.node.position.left + 200;
    form3.node.position.bottom = form3.node.position.top + 150;
    algui_insert_child(&root.node, &form3.node, -1);

    TEST button1;
    algui_init_node(&button1.node, test_proc, false, NULL);
    button1.node.position.left = 50;
    button1.node.position.top = 40;
    button1.node.position.right = button1.node.position.left + 50;
    button1.node.position.bottom = button1.node.position.top + 40;
    algui_insert_child(&form2.node, &button1.node, -1);

    TEST button2;
    algui_init_node(&button2.node, test_proc, false, NULL);
    button2.node.position.left = 70;
    button2.node.position.top = 60;
    button2.node.position.right = button2.node.position.left + 50;
    button2.node.position.bottom = button2.node.position.top + 40;
    algui_insert_child(&form2.node, &button2.node, -1);

    TEST button3;
    algui_init_node(&button3.node, test_proc, false, NULL);
    button3.node.position.left = 90;
    button3.node.position.top = 80;
    button3.node.position.right = button3.node.position.left + 50;
    button3.node.position.bottom = button3.node.position.top + 40;
    algui_insert_child(&form2.node, &button3.node, -1);

    form2.node.active = 1;

    while (1) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        if (event.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
        }

        if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                break;
            }
        }

        if (redraw) {
            redraw = false;
            //al_clear_to_color(al_map_rgb_f(0, 0, 0));
            algui_paint_node(&root.node, NULL);
            al_flip_display();
        }
    }

    return 0;
}
