#ifndef ALGUI_WIDGET_H
#define ALGUI_WIDGET_H


#include <allegro5/allegro.h>


enum ALGUI_MESSAGE {
    ALGUI_MESSAGE_INIT_LAYOUT,
    ALGUI_MESSAGE_COMMIT_LAYOUT,
    ALGUI_MESSAGE_GET_FOCUS,
    ALGUI_MESSAGE_GOT_FOCUS,
    ALGUI_MESSAGE_DESCENTANT_GOT_FOCUS,
    ALGUI_MESSAGE_LOSE_FOCUS,
    ALGUI_MESSAGE_LOST_FOCUS,
    ALGUI_MESSAGE_DESCENTANT_LOST_FOCUS
};


typedef struct ALGUI_WIDGET ALGUI_WIDGET;


typedef int(*ALGUI_WIDGET_PROC)(ALGUI_WIDGET* wgt, int id, void* data);


struct ALGUI_WIDGET {
    ALGUI_WIDGET_PROC proc;
    ALGUI_WIDGET* parent;
    ALGUI_WIDGET* first;
    ALGUI_WIDGET* last;
    ALGUI_WIDGET* prev;
    ALGUI_WIDGET* next;
    float x;
    float y;
    float width;
    float height;
    float x1;
    float y1;
    float x2;
    float y2;
    void* data;
    int heap : 1;
    int has_mouse : 1;
    int visible : 1;
    int enabled : 1;
    int highlighted : 1;
    int pressed : 1;
    int selected : 1;
    int active : 1;
    int error : 1;
    int tree_visible : 1;
    int tree_enabled : 1;
    int tree_highlighted : 1;
    int tree_pressed : 1;
    int tree_selected : 1;
    int tree_active : 1;
    int tree_error : 1;
    int tree_focused : 1;
};


int algui_widget_proc(ALGUI_WIDGET* wgt, int id, void* data);


ALGUI_WIDGET* algui_get_parent_widget(ALGUI_WIDGET* wgt);


ALGUI_WIDGET* algui_get_prev_sibling_widget(ALGUI_WIDGET* wgt);


ALGUI_WIDGET* algui_get_next_sibling_widget(ALGUI_WIDGET* wgt);


ALGUI_WIDGET* algui_get_first_child_widget(ALGUI_WIDGET* wgt);


ALGUI_WIDGET* algui_get_last_child_widget(ALGUI_WIDGET* wgt);


int algui_is_widget_in_tree(ALGUI_WIDGET* root, ALGUI_WIDGET* wgt);


void algui_insert_widget(ALGUI_WIDGET* parent, ALGUI_WIDGET* wgt, int z_order);


void algui_add_widget(ALGUI_WIDGET* parent, ALGUI_WIDGET* wgt);


void algui_remove_widget(ALGUI_WIDGET* parent, ALGUI_WIDGET* wgt);


void algui_detach_widget(ALGUI_WIDGET* wgt);


void algui_get_widget_geometry(ALGUI_WIDGET* wgt, float* x, float* y, float* width, float* height);


void algui_get_widget_position(ALGUI_WIDGET* wgt, float* x, float* y);


float algui_get_widget_x(ALGUI_WIDGET* wgt);


float algui_get_widget_y(ALGUI_WIDGET* wgt);


void algui_get_widget_size(ALGUI_WIDGET* wgt, float* width, float* height);


float algui_get_widget_width(ALGUI_WIDGET* wgt);


float algui_get_widget_height(ALGUI_WIDGET* wgt);


void algui_set_widget_geometry(ALGUI_WIDGET* wgt, float x, float y, float width, float height);


void algui_set_widget_position(ALGUI_WIDGET* wgt, float x, float y);


void algui_set_widget_x(ALGUI_WIDGET* wgt, float x);


void algui_set_widget_y(ALGUI_WIDGET* wgt, float y);


void algui_set_widget_size(ALGUI_WIDGET* wgt, float width, float height);


void algui_set_widget_width(ALGUI_WIDGET* wgt, float width);


void algui_set_widget_height(ALGUI_WIDGET* wgt, float height);


void algui_get_widget_rect(ALGUI_WIDGET* wgt, float* x1, float* y1, float* x2, float* y2);


void algui_do_widget_layout(ALGUI_WIDGET* wgt);


int algui_is_widget_visible(ALGUI_WIDGET* wgt);


int algui_is_widget_in_visible_tree(ALGUI_WIDGET* wgt);


void algui_set_widget_visible(ALGUI_WIDGET* wgt, int visible);


int algui_is_widget_enabled(ALGUI_WIDGET* wgt);


int algui_is_widget_in_enabled_tree(ALGUI_WIDGET* wgt);


void algui_set_widget_enabled(ALGUI_WIDGET* wgt, int enabled);


int algui_is_widget_highlighted(ALGUI_WIDGET* wgt);


int algui_is_widget_in_highlighted_tree(ALGUI_WIDGET* wgt);


void algui_set_widget_highlighted(ALGUI_WIDGET* wgt, int highlighted);


int algui_is_widget_pressed(ALGUI_WIDGET* wgt);


int algui_is_widget_in_pressed_tree(ALGUI_WIDGET* wgt);


void algui_set_widget_pressed(ALGUI_WIDGET* wgt, int pressed);


int algui_is_widget_selected(ALGUI_WIDGET* wgt);


int algui_is_widget_in_selected_tree(ALGUI_WIDGET* wgt);


void algui_set_widget_selected(ALGUI_WIDGET* wgt, int selected);


int algui_is_widget_active(ALGUI_WIDGET* wgt);


int algui_is_widget_in_active_tree(ALGUI_WIDGET* wgt);


void algui_set_widget_active(ALGUI_WIDGET* wgt, int active);


int algui_is_widget_error(ALGUI_WIDGET* wgt);


int algui_is_widget_in_error_tree(ALGUI_WIDGET* wgt);


void algui_set_widget_error(ALGUI_WIDGET* wgt, int error);


int algui_is_widget_focused(ALGUI_WIDGET* wgt);


int algui_is_widget_in_focused_tree(ALGUI_WIDGET* wgt);


int algui_set_widget_focused(ALGUI_WIDGET* wgt, int focused);


#endif //ALGUI_WIDGET_H
