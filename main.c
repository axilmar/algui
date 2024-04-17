#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include <allegro5/allegro_ttf.h>


extern void algui_run_tests();


static ALLEGRO_FONT* test_font;


unsigned char random255() {
    return (unsigned char)(rand() * 255);
}


ALLEGRO_COLOR random_color() {
    return al_map_rgb(random255(), random255(), random255());
}


int main(int argc, const char* argv[])
{
    algui_run_tests();
    system("pause");
    return 0;

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
            al_flip_display();
        }
    }

    return 0;
}
