#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include <allegro5/allegro_ttf.h>
#include "ALGUI_NODE.h"


#define MAX_MESSAGES 15


typedef enum LAYOUT {
    LAYOUT_NONE,
    LAYOUT_HOR,
    LAYOUT_VER
} LAYOUT;


typedef struct TEST {
    ALGUI_NODE node;
    LAYOUT layout;
    const char* messages[MAX_MESSAGES];
    ALLEGRO_COLOR colors[MAX_MESSAGES];
} TEST;


static ALLEGRO_FONT* test_font;


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
        bg = al_map_rgb(255, 255, 255);
        fg = al_map_rgb(255, 128, 128);
    }
    else {
        bg = al_map_rgb(255, 255, 255);
        fg = al_map_rgb(0, 0, 0);
    }

    al_draw_filled_rectangle(data->position.left, data->position.top, data->position.right, data->position.bottom, bg);
    al_draw_rectangle(data->position.left, data->position.top, data->position.right, data->position.bottom, fg, 2);

    for (unsigned i = 0; i < MAX_MESSAGES; ++i) {
        if (test->messages[i]) {
            al_draw_text(test_font, test->colors[i], data->position.left, data->position.top + al_get_font_line_height(test_font) * i, 0, test->messages[i]);
        }
    }

    return algui_node_proc(&test->node, ALGUI_MESSAGE_PAINT, data);
}


static void test_format(TEST* test, float xf, float yf) {
    float x = 0, y = 0;

    for (ALGUI_NODE* child = test->node.first; child; child = child->next) {
        float w = child->position.right - child->position.left;
        float h = child->position.bottom - child->position.top;
        child->position.left = x;
        child->position.top = y;
        child->position.right = child->position.left + w;
        child->position.bottom = child->position.top + h;
        x += w * xf;
        y += h * yf;
    }
}


static ALGUI_RESULT test_layout(TEST* test) {
    algui_send_message_to_child_nodes(&test->node, ALGUI_MESSAGE_DO_LAYOUT, NULL, NULL);
    switch (test->layout) {
        case LAYOUT_HOR:
            test_format(test, 1, 0);
            break;

        case LAYOUT_VER:
            test_format(test, 0, 1);
            break;
    }
    return ALGUI_RESULT_OK;
}


unsigned char random255() {
    return (unsigned char)(rand() * 255);
}


ALLEGRO_COLOR random_color() {
    return al_map_rgb(random255(), random255(), random255());
}


static ALGUI_RESULT test_proc(ALGUI_NODE* node, int id, void* data) {
    switch (id) {
        case ALGUI_MESSAGE_INIT:
            algui_node_proc(node, id, data);
            ((TEST*)node)->layout = LAYOUT_NONE;
            memset((char*)&((TEST*)node)->messages[0], 0, sizeof(const char*) * MAX_MESSAGES);
            return ALGUI_RESULT_OK;

        case ALGUI_MESSAGE_PAINT:
            return test_paint((TEST*)node, (ALGUI_MESSAGE_DATA_PAINT*)data);

        case ALGUI_MESSAGE_DO_LAYOUT:
            return test_layout((TEST*)node);

        case ALGUI_MESSAGE_MOUSE_ENTER:
            ((TEST*)node)->messages[0] = "MouseEnter";
            ((TEST*)node)->colors[0] = random_color();
            return algui_node_proc(node, id, data);

        case ALGUI_MESSAGE_MOUSE_MOVE:
            ((TEST*)node)->messages[1] = "MouseMove";
            ((TEST*)node)->colors[1] = random_color();
            return algui_node_proc(node, id, data);

        case ALGUI_MESSAGE_MOUSE_LEAVE:
            ((TEST*)node)->messages[2] = "MouseLeave";
            ((TEST*)node)->colors[2] = random_color();
            return algui_node_proc(node, id, data);

        case ALGUI_MESSAGE_MOUSE_BUTTON_DOWN:
            ((TEST*)node)->messages[3] = "ButtonDown";
            ((TEST*)node)->colors[3] = random_color();
            algui_set_focus_to_node(node);
            return algui_node_proc(node, id, data);

        case ALGUI_MESSAGE_MOUSE_BUTTON_UP:
            ((TEST*)node)->messages[4] = "ButtonUp";
            ((TEST*)node)->colors[4] = random_color();
            return algui_node_proc(node, id, data);

        case ALGUI_MESSAGE_CLICK:
            ((TEST*)node)->messages[5] = "Click";
            ((TEST*)node)->colors[5] = random_color();
            return algui_node_proc(node, id, data);

        case ALGUI_MESSAGE_FOCUS:
            ((TEST*)node)->messages[6] = "Focused";
            ((TEST*)node)->colors[6] = random_color();
            return algui_node_proc(node, id, data);

        case ALGUI_MESSAGE_DEFOCUS:
            ((TEST*)node)->messages[7] = "Defocused";
            ((TEST*)node)->colors[7] = random_color();
            return algui_node_proc(node, id, data);

        case ALGUI_MESSAGE_KEY_CHAR:
            ((TEST*)node)->messages[8] = "Keychar";
            ((TEST*)node)->colors[8] = random_color();
            return algui_node_proc(node, id, data);

        case ALGUI_MESSAGE_UNUSED_KEY_CHAR:
            ((TEST*)node)->messages[9] = "Unused Keychar";
            ((TEST*)node)->colors[9] = random_color();
            return algui_node_proc(node, id, data);
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
    algui_insert_child_node(&root.node, &form1.node, -1);

    TEST form2;
    algui_init_node(&form2.node, test_proc, false, NULL);
    form2.node.position.left = 200;
    form2.node.position.top = 150;
    form2.node.position.right = form2.node.position.left + 200;
    form2.node.position.bottom = form2.node.position.top + 150;
    //form2.layout = LAYOUT_VER;
    algui_insert_child_node(&root.node, &form2.node, -1);

    TEST form3;
    algui_init_node(&form3.node, test_proc, false, NULL);
    form3.node.position.left = 300;
    form3.node.position.top = 250;
    form3.node.position.right = form3.node.position.left + 200;
    form3.node.position.bottom = form3.node.position.top + 150;
    algui_insert_child_node(&root.node, &form3.node, -1);

    TEST button1;
    algui_init_node(&button1.node, test_proc, false, NULL);
    button1.node.position.left = 50;
    button1.node.position.top = 40;
    button1.node.position.right = button1.node.position.left + 50;
    button1.node.position.bottom = button1.node.position.top + 40;
    algui_insert_child_node(&form2.node, &button1.node, -1);

    TEST button2;
    algui_init_node(&button2.node, test_proc, false, NULL);
    button2.node.position.left = 70;
    button2.node.position.top = 60;
    button2.node.position.right = button2.node.position.left + 50;
    button2.node.position.bottom = button2.node.position.top + 40;
    algui_insert_child_node(&form2.node, &button2.node, -1);

    TEST button3;
    algui_init_node(&button3.node, test_proc, false, NULL);
    button3.node.position.left = 90;
    button3.node.position.top = 80;
    button3.node.position.right = button3.node.position.left + 50;
    button3.node.position.bottom = button3.node.position.top + 40;
    algui_insert_child_node(&form2.node, &button3.node, -1);

    //form2.node.active = 1;

    algui_do_node_layout(&root.node);

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

        algui_dispatch_event(&root.node, &event);

        if (redraw) {
            redraw = false;
            //al_clear_to_color(al_map_rgb_f(0, 0, 0));
            algui_paint_node(&root.node);
            al_flip_display();
        }
    }

    return 0;
}
