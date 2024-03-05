#ifndef ALGUI_WIDGET_H
#define ALGUI_WIDGET_H


#include <allegro5/allegro.h>


enum ALGUI_MESSAGE_ID {
    ALGUI_MESSAGE_CLEANUP,
    ALGUI_MESSAGE_PAINT,
    ALGUI_MESSAGE_LAYOUT,
    ALGUI_MESSAGE_GET_FOCUS,
    ALGUI_MESSAGE_GOT_FOCUS,
    ALGUI_MESSAGE_LOSE_FOCUS,
    ALGUI_MESSAGE_LOST_FOCUS,
    ALGUI_MESSAGE_DESCENTANT_GOT_FOCUS,
    ALGUI_MESSAGE_DESCENTANT_LOST_FOCUS,
    ALGUI_MESSAGE_MOUSE_ENTER,
    ALGUI_MESSAGE_MOUSE_MOVE,
    ALGUI_MESSAGE_MOUSE_LEAVE,
    ALGUI_MESSAGE_MOUSE_WHEEL,
    ALGUI_MESSAGE_KEY_DOWN,
    ALGUI_MESSAGE_KEY_UP,
    ALGUI_MESSAGE_KEY_CHAR,
    ALGUI_MESSAGE_USER = 10000
};


typedef int (*ALGUI_WIDGET_PROC)(struct ALGUI_WIDGET*, int msgId, void* msgData);


typedef struct ALGUI_WIDGET {
    ALGUI_WIDGET_PROC proc;
    struct ALGUI_WIDGET* parent;
    struct ALGUI_WIDGET* prev_sibling;
    struct ALGUI_WIDGET* next_sibling;
    struct ALGUI_WIDGET* first_child;
    struct ALGUI_WIDGET* last_child;
    float x;
    float y;
    float width;
    float height;
    float x1;
    float y1;
    float x2;
    float y2;
    void* user_data;
    int lock_count : 16;
    int heap_allocated : 1;
    int visible : 1;
    int enabled : 1;
    int has_mouse : 1;
    int has_focus : 1;
    int pressed : 1;
    int selected : 1;
    int active : 1;
    int tree_visible : 1;
    int tree_enabled : 1;
    int tree_has_mouse : 1;
    int tree_has_focus : 1;
    int tree_pressed : 1;
    int tree_selected : 1;
    int tree_active : 1;
} ALGUI_WIDGET;


ALGUI_WIDGET_PROC algui_get_widget_proc(ALGUI_WIDGET* wgt);


ALGUI_WIDGET* algui_get_parent_widget(ALGUI_WIDGET* wgt);


ALGUI_WIDGET* algui_get_prev_sibling_widget(ALGUI_WIDGET* wgt);


ALGUI_WIDGET* algui_get_next_sibling_widget(ALGUI_WIDGET* wgt);


ALGUI_WIDGET* algui_get_first_child_widget(ALGUI_WIDGET* wgt);


ALGUI_WIDGET* algui_get_last_child_widget(ALGUI_WIDGET* wgt);


void algui_get_widget_geometry(ALGUI_WIDGET* wgt, float* x, float* y, float* width, float* height);


void algui_get_widget_position(ALGUI_WIDGET* wgt, float* x, float* y);


void algui_get_widget_size(ALGUI_WIDGET* wgt, float* width, float* height);


float algui_get_widget_x(ALGUI_WIDGET* wgt);


float algui_get_widget_y(ALGUI_WIDGET* wgt);


float algui_get_widget_width(ALGUI_WIDGET* wgt);


float algui_get_widget_height(ALGUI_WIDGET* wgt);


void algui_get_widget_rect(ALGUI_WIDGET* wgt, float* x1, float* y1, float* x2, float* y2);


float algui_get_widget_x1(ALGUI_WIDGET* wgt);


float algui_get_widget_y1(ALGUI_WIDGET* wgt);


float algui_get_widget_x2(ALGUI_WIDGET* wgt);


float algui_get_widget_y2(ALGUI_WIDGET* wgt);


void* algui_get_widget_user_data(ALGUI_WIDGET* wgt);


int algui_get_widget_lock_count(ALGUI_WIDGET* wgt);


int algui_is_widget_heap_allocated(ALGUI_WIDGET* wgt);


int algui_widget_is_visible(ALGUI_WIDGET* wgt);


int algui_widget_is_enabled(ALGUI_WIDGET* wgt);


int algui_widget_has_mouse(ALGUI_WIDGET* wgt);


int algui_widget_has_focus(ALGUI_WIDGET* wgt);


int algui_widget_is_pressed(ALGUI_WIDGET* wgt);


int algui_widget_is_selected(ALGUI_WIDGET* wgt);


int algui_widget_is_active(ALGUI_WIDGET* wgt);


int algui_widget_tree_is_visible(ALGUI_WIDGET* wgt);


int algui_widget_tree_is_enabled(ALGUI_WIDGET* wgt);


int algui_widget_tree_has_mouse(ALGUI_WIDGET* wgt);


int algui_widget_tree_has_focus(ALGUI_WIDGET* wgt);


int algui_widget_tree_is_pressed(ALGUI_WIDGET* wgt);


int algui_widget_tree_is_selected(ALGUI_WIDGET* wgt);


int algui_widget_tree_is_active(ALGUI_WIDGET* wgt);


int algui_is_widget_in_tree(ALGUI_WIDGET* root, ALGUI_WIDGET* wgt);


ALGUI_WIDGET* algui_get_focused_widget();


ALGUI_WIDGET* algui_get_child_widget_at_z_order(ALGUI_WIDGET* parent, int z_order);


int algui_widget_proc(ALGUI_WIDGET* wgt, int msgId, void* msgData);


int algui_do_widget_message(ALGUI_WIDGET* wgt, int msgId, void* msgData);


void algui_init_widget(ALGUI_WIDGET* wgt, ALGUI_WIDGET_PROC proc, void* user_data, int heap_allocated);


void algui_set_widget_proc(ALGUI_WIDGET* wgt, ALGUI_WIDGET_PROC proc);


void algui_cleanup_widget(ALGUI_WIDGET* wgt);


void algui_insert_widget(ALGUI_WIDGET* parent, ALGUI_WIDGET* wgt, int z_order);


void algui_add_widget(ALGUI_WIDGET* parent, ALGUI_WIDGET* wgt);


void algui_remove_widget(ALGUI_WIDGET* parent, ALGUI_WIDGET* wgt);


void algui_detach_widget(ALGUI_WIDGET* wgt);


void algui_set_widget_geometry(ALGUI_WIDGET* wgt, float x, float y, float width, float height);


void algui_set_widget_position(ALGUI_WIDGET* wgt, float x, float y);


void algui_set_widget_size(ALGUI_WIDGET* wgt, float width, float height);


void algui_set_widget_x(ALGUI_WIDGET* wgt, float x);


void algui_set_widget_y(ALGUI_WIDGET* wgt, float y);


void algui_set_widget_width(ALGUI_WIDGET* wgt, float width);


void algui_set_widget_height(ALGUI_WIDGET* wgt, float height);


void algui_do_widget_layout(ALGUI_WIDGET* wgt);


void algui_set_widget_user_data(ALGUI_WIDGET* wgt, void* user_data);


void algui_lock_widget(ALGUI_WIDGET* wgt);


void algui_unlock_widget(ALGUI_WIDGET* wgt);


void algui_set_widget_visible(ALGUI_WIDGET* wgt, int visible);


void algui_set_widget_enabled(ALGUI_WIDGET* wgt, int enabled);


int algui_set_widget_focused(ALGUI_WIDGET* wgt, int focused);


void algui_focus_widget(ALGUI_WIDGET* wgt);


void algui_blur_widget(ALGUI_WIDGET* wgt);


void algui_set_widget_pressed(ALGUI_WIDGET* wgt, int pressed);


void algui_set_widget_selected(ALGUI_WIDGET* wgt, int selected);


void algui_set_widget_active(ALGUI_WIDGET* wgt, int active);


void algui_paint_widget(ALGUI_WIDGET* wgt);


#endif //ALGUI_WIDGET_H
