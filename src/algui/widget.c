#include <cassert>
#include "algui/widget.h"


/**********************************************************************************************************************
    PRIVATE
 **********************************************************************************************************************/


//find max
#define __MAX__(A, B) ((A) > (B) ? (A) : (B)) 


#define FOR_EACH_CHILD(WGT, CHILD)\
    for(ALGUI_WIDGET* CHILD = WGT->first; CHILD != NULL; CHILD = CHILD->next) 


static ALGUI_WIDGET* focused_widget = NULL;


static int doing_layout = 0;


static int do_widget_message(ALGUI_WIDGET * wgt, int id, void* data) {
    return wgt->proc(wgt, id, data);
}


static void do_ancestor_message(ALGUI_WIDGET * wgt, int id, void* data) {
    for(wgt = wgt->parent; wgt != NULL; wgt = wgt->parent) {
        do_widget_message(wgt, id, data);
    }
}


static int widget_tree_contains(ALGUI_WIDGET * root, ALGUI_WIDGET * wgt) {
    for(; wgt != NULL; wgt = wgt->parent) {
        if (wgt == root) {
            return 1;
        }
    }
    return 0;
}


static int is_widget_focused(ALGUI_WIDGET* wgt) {
    return wgt == focused_widget;
}


static int widget_contains_focus(ALGUI_WIDGET* wgt) {
    return focused_widget ? widget_tree_contains(wgt, focused_widget) : 0;
}


static void compute_child_tree_rect_x1(ALGUI_WIDGET* wgt) {
    wgt->x1 = wgt->x + wgt->parent->x1;
    wgt->x2 = wgt->x1 + wgt->width;
}


static void compute_child_tree_rect_y1(ALGUI_WIDGET* wgt) {
    wgt->y1 = wgt->y + wgt->parent->y1;
    wgt->y2 = wgt->y1 + wgt->height;
}


static void compute_child_tree_rect(ALGUI_WIDGET* wgt) {
    compute_child_tree_rect_x1(wgt);
    compute_child_tree_rect_y1(wgt);
}


static void compute_child_tree_visible(ALGUI_WIDGET* wgt) {
    wgt->tree_visible = wgt->visible && wgt->parent->tree_visible;
}


static void compute_child_tree_enabled(ALGUI_WIDGET* wgt) {
    wgt->tree_enabled = wgt->enabled && wgt->parent->tree_enabled;
}


static void compute_child_tree_highlighted(ALGUI_WIDGET* wgt) {
    wgt->tree_highlighted = wgt->highlighted || wgt->parent->tree_highlighted;
}


static void compute_child_tree_pressed(ALGUI_WIDGET* wgt) {
    wgt->tree_pressed = wgt->pressed || wgt->parent->tree_pressed;
}


static void compute_child_tree_selected(ALGUI_WIDGET* wgt) {
    wgt->tree_selected = wgt->selected || wgt->parent->tree_selected;
}


static void compute_child_tree_active(ALGUI_WIDGET* wgt) {
    wgt->tree_active = wgt->active || wgt->parent->tree_active;
}


static void compute_child_tree_error(ALGUI_WIDGET* wgt) {
    wgt->tree_error = wgt->error || wgt->parent->tree_error;
}


static void compute_child_tree_focused(ALGUI_WIDGET* wgt) {
    wgt->tree_focused = is_widget_focused(wgt) || wgt->parent->tree_focused;
}


static void update_child_tree_rect_x1(ALGUI_WIDGET* wgt) {
    compute_child_tree_rect_x1(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_rect_x1(child);
    }
}


static void update_child_tree_rect_y1(ALGUI_WIDGET* wgt) {
    compute_child_tree_rect_y1(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_rect_y1(child);
    }
}


static void update_child_tree_rect(ALGUI_WIDGET* wgt) {
    compute_child_tree_rect(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_rect(child);
    }
}


static void update_child_tree_visible(ALGUI_WIDGET* wgt) {
    compute_child_tree_visible(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_visible(child);
    }
}


static void update_child_tree_enabled(ALGUI_WIDGET* wgt) {
    compute_child_tree_enabled(wgt);
    compute_child_tree_highlighted(wgt);
    compute_child_tree_pressed(wgt);
    compute_child_tree_active(wgt);
    compute_child_tree_error(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_enabled(child);
    }
}


static void update_child_tree_highlighted(ALGUI_WIDGET* wgt) {
    compute_child_tree_highlighted(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_highlighted(child);
    }
}


static void update_child_tree_pressed(ALGUI_WIDGET* wgt) {
    compute_child_tree_pressed(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_pressed(child);
    }
}


static void update_child_tree_selected(ALGUI_WIDGET* wgt) {
    compute_child_tree_selected(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_selected(child);
    }
}


static void update_child_tree_active(ALGUI_WIDGET* wgt) {
    compute_child_tree_active(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_active(child);
    }
}


static void update_child_tree_error(ALGUI_WIDGET* wgt) {
    compute_child_tree_error(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_error(child);
    }
}


static void update_child_tree_focused(ALGUI_WIDGET* wgt) {
    compute_child_tree_focused(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_focused(child);
    }
}


static void update_child_tree(ALGUI_WIDGET* wgt) {
    compute_child_tree_visible(wgt);
    compute_child_tree_enabled(wgt);
    compute_child_tree_highlighted(wgt);
    compute_child_tree_pressed(wgt);
    compute_child_tree_selected(wgt);
    compute_child_tree_active(wgt);
    compute_child_tree_error(wgt);
    compute_child_tree_focused(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree(child);
    }
}


static void compute_root_tree_rect_x1(ALGUI_WIDGET* wgt) {
    wgt->x1 = wgt->x;
    wgt->x2 = wgt->x1 + wgt->width;
}


static void compute_root_tree_rect_y1(ALGUI_WIDGET* wgt) {
    wgt->y1 = wgt->y;
    wgt->y2 = wgt->y1 + wgt->height;
}


static void compute_root_tree_rect(ALGUI_WIDGET* wgt) {
    compute_root_tree_rect_x1(wgt);
    compute_root_tree_rect_y1(wgt);
}


static void compute_root_tree_visible(ALGUI_WIDGET* wgt) {
    wgt->tree_visible = wgt->visible;
}


static void compute_root_tree_enabled(ALGUI_WIDGET* wgt) {
    wgt->tree_enabled = wgt->enabled;
}


static void compute_root_tree_highlighted(ALGUI_WIDGET* wgt) {
    wgt->tree_highlighted = wgt->highlighted;
}


static void compute_root_tree_pressed(ALGUI_WIDGET* wgt) {
    wgt->tree_pressed = wgt->pressed;
}


static void compute_root_tree_selected(ALGUI_WIDGET* wgt) {
    wgt->tree_selected = wgt->selected;
}


static void compute_root_tree_active(ALGUI_WIDGET* wgt) {
    wgt->tree_active = wgt->active;
}


static void compute_root_tree_error(ALGUI_WIDGET* wgt) {
    wgt->tree_error = wgt->error;
}


static void compute_root_tree_focused(ALGUI_WIDGET* wgt) {
    wgt->tree_focused = is_widget_focused(wgt);
}


static void update_root_tree_rect_x1(ALGUI_WIDGET* wgt) {
    compute_root_tree_rect_x1(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_rect_x1(child);
    }
}


static void update_root_tree_rect_y1(ALGUI_WIDGET* wgt) {
    compute_root_tree_rect_y1(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_rect_y1(child);
    }
}


static void update_root_tree_rect(ALGUI_WIDGET* wgt) {
    compute_root_tree_rect(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_rect(child);
    }
}


static void update_root_tree_visible(ALGUI_WIDGET* wgt) {
    compute_root_tree_visible(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_visible(child);
    }
}


static void update_root_tree_enabled(ALGUI_WIDGET* wgt) {
    compute_root_tree_enabled(wgt);
    compute_root_tree_highlighted(wgt);
    compute_root_tree_pressed(wgt);
    compute_root_tree_active(wgt);
    compute_root_tree_error(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_enabled(child);
    }
}


static void update_root_tree_highlighted(ALGUI_WIDGET* wgt) {
    compute_root_tree_highlighted(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_highlighted(child);
    }
}


static void update_root_tree_pressed(ALGUI_WIDGET* wgt) {
    compute_root_tree_pressed(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_pressed(child);
    }
}


static void update_root_tree_selected(ALGUI_WIDGET* wgt) {
    compute_root_tree_selected(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_selected(child);
    }
}


static void update_root_tree_active(ALGUI_WIDGET* wgt) {
    compute_root_tree_active(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_active(child);
    }
}


static void update_root_tree_error(ALGUI_WIDGET* wgt) {
    compute_root_tree_error(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_error(child);
    }
}


static void update_root_tree_focused(ALGUI_WIDGET* wgt) {
    compute_root_tree_focused(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree_focused(child);
    }
}


static void update_root_tree(ALGUI_WIDGET* wgt) {
    compute_root_tree_visible(wgt);
    compute_root_tree_enabled(wgt);
    compute_root_tree_highlighted(wgt);
    compute_root_tree_pressed(wgt);
    compute_root_tree_selected(wgt);
    compute_root_tree_active(wgt);
    compute_root_tree_error(wgt);
    compute_root_tree_focused(wgt);
    FOR_EACH_CHILD(wgt, child) {
        update_child_tree(child);
    }
}


static void update_tree_rect_x1(ALGUI_WIDGET* wgt) {
    wgt->parent ? update_child_tree_rect_x1(wgt) : update_root_tree_rect_x1(wgt);
}


static void update_tree_rect_y1(ALGUI_WIDGET* wgt) {
    wgt->parent ? update_child_tree_rect_y1(wgt) : update_root_tree_rect_y1(wgt);
}


static void update_tree_rect(ALGUI_WIDGET* wgt) {
    wgt->parent ? update_child_tree_rect(wgt) : update_root_tree_rect(wgt);
}


static void update_tree_visible(ALGUI_WIDGET* wgt) {
    wgt->parent ? update_child_tree_visible(wgt) : update_root_tree_visible(wgt);
}


static void update_tree_enabled(ALGUI_WIDGET* wgt) {
    wgt->parent ? update_child_tree_enabled(wgt) : update_root_tree_enabled(wgt);
}


static void update_tree_highlighted(ALGUI_WIDGET* wgt) {
    wgt->parent ? update_child_tree_highlighted(wgt) : update_root_tree_highlighted(wgt);
}


static void update_tree_pressed(ALGUI_WIDGET* wgt) {
    wgt->parent ? update_child_tree_pressed(wgt) : update_root_tree_pressed(wgt);
}


static void update_tree_selected(ALGUI_WIDGET* wgt) {
    wgt->parent ? update_child_tree_selected(wgt) : update_root_tree_selected(wgt);
}


static void update_tree_active(ALGUI_WIDGET* wgt) {
    wgt->parent ? update_child_tree_active(wgt) : update_root_tree_active(wgt);
}


static void update_tree_error(ALGUI_WIDGET* wgt) {
    wgt->parent ? update_child_tree_error(wgt) : update_root_tree_error(wgt);
}


static void update_tree_focused(ALGUI_WIDGET* wgt) {
    wgt->parent ? update_child_tree_focused(wgt) : update_root_tree_focused(wgt);
}


static void update_tree(ALGUI_WIDGET* wgt) {
    wgt->parent ? update_child_tree(wgt) : update_root_tree(wgt);
}


static void do_widget_init_layout(ALGUI_WIDGET* wgt) {
    FOR_EACH_CHILD(wgt, child) {
        do_widget_init_layout(child);
    }
    do_widget_message(wgt, ALGUI_MESSAGE_INIT_LAYOUT, NULL);
}


static void do_widget_commit_layout(ALGUI_WIDGET* wgt) {
    do_widget_message(wgt, ALGUI_MESSAGE_COMMIT_LAYOUT, NULL);
    FOR_EACH_CHILD(wgt, child) {
        do_widget_commit_layout(child);
    }
}


static void do_widget_lost_focus(ALGUI_WIDGET* wgt) {
    focused_widget = NULL;
    update_tree_focused(wgt);
    do_widget_message(wgt, ALGUI_MESSAGE_LOST_FOCUS, NULL);
    do_ancestor_message(wgt, ALGUI_MESSAGE_DESCENTANT_LOST_FOCUS, NULL);
}


/**********************************************************************************************************************
    PUBLIC
 **********************************************************************************************************************/


int algui_widget_proc(ALGUI_WIDGET* wgt, int id, void* data) {
    assert(wgt);
    switch (id) {
    }
    return 0;
}


ALGUI_WIDGET* algui_get_parent_widget(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->parent;
}


ALGUI_WIDGET* algui_get_prev_sibling_widget(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->prev;
}


ALGUI_WIDGET* algui_get_next_sibling_widget(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->next;
}


ALGUI_WIDGET* algui_get_first_child_widget(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->first;
}


ALGUI_WIDGET* algui_get_last_child_widget(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->last;
}


int algui_is_widget_in_tree(ALGUI_WIDGET* root, ALGUI_WIDGET* wgt) {
    assert(root);
    assert(wgt);
    return widget_tree_contains(root, wgt);
}


void algui_insert_widget(ALGUI_WIDGET* parent, ALGUI_WIDGET* wgt, int z_order) {
    assert(parent);
    assert(wgt);
    assert(wgt->parent == NULL);
    assert(algui_is_widget_in_tree(wgt, parent) == 0);

    ALGUI_WIDGET* prev;
    ALGUI_WIDGET* next;

    if (z_order >= 0) {
        for (next = parent->first; next != NULL && z_order > 0; next = next->next, --z_order) {
        }
        prev = next != NULL ? next->prev : NULL;
    }
    else {
        for (prev = parent->last; prev != NULL && z_order < -1; prev = prev->prev, ++z_order) {
        }
        next = prev != NULL ? prev->next : NULL;
    }

    if (prev) {
        prev->next = wgt;
    }
    else {
        parent->first = wgt;
    }
    if (next) {
        next->prev = wgt;
    }
    else {
        parent->last = wgt;
    }

    wgt->parent = parent;
    wgt->prev = prev;
    wgt->next = next;

    update_child_tree(wgt);
}


void algui_add_widget(ALGUI_WIDGET* parent, ALGUI_WIDGET* wgt) {
    algui_insert_widget(parent, wgt, -1);
}


void algui_remove_widget(ALGUI_WIDGET* parent, ALGUI_WIDGET* wgt) {
    assert(parent);
    assert(wgt);
    assert(wgt->parent == parent);

    if (wgt->prev) {
        wgt->prev->next = wgt->next;
    }
    else {
        parent->first = wgt->next;
    }
    if (wgt->next) {
        wgt->next->prev = wgt->prev;
    }
    else {
        parent->last = wgt->prev;
    }

    wgt->parent = NULL;
    wgt->prev = NULL;
    wgt->next = NULL;

    update_root_tree(wgt);
}


void algui_detach_widget(ALGUI_WIDGET* wgt) {
    assert(wgt);
    if (wgt->parent) {
        algui_remove_widget(wgt->parent, wgt);
    }
}


void algui_get_widget_geometry(ALGUI_WIDGET* wgt, float* x, float* y, float* width, float* height) {
    assert(wgt);
    assert(x);
    assert(y);
    assert(width);
    assert(height);
    *x = wgt->x;
    *y = wgt->y;
    *width = wgt->width;
    *height = wgt->height;
}


void algui_get_widget_position(ALGUI_WIDGET* wgt, float* x, float* y) {
    assert(wgt);
    assert(x);
    assert(y);
    *x = wgt->x;
    *y = wgt->y;
}


float algui_get_widget_x(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->x;
}


float algui_get_widget_y(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->y;
}


void algui_get_widget_size(ALGUI_WIDGET* wgt, float* width, float* height) {
    assert(wgt);
    assert(width);
    assert(height);
    *width = wgt->width;
    *height = wgt->height;
}


float algui_get_widget_width(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->width;
}


float algui_get_widget_height(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->height;
}


void algui_set_widget_geometry(ALGUI_WIDGET* wgt, float x, float y, float width, float height) {
    assert(wgt);
    if (doing_layout > 0) {
        wgt->x = x;
        wgt->y = y;
        wgt->width = __MAX__(width, 0);
        wgt->height = __MAX__(height, 0);
    }
    else if (x != wgt->x || y != wgt->y || width != wgt->width || height != wgt->height) {
        wgt->x = x;
        wgt->y = y;
        wgt->width = width;
        wgt->height = height;
        update_tree_rect(wgt);
    }
}


void algui_set_widget_position(ALGUI_WIDGET* wgt, float x, float y) {
    assert(wgt);
    algui_set_widget_geometry(wgt, x, y, wgt->width, wgt->height);
}


void algui_set_widget_x(ALGUI_WIDGET* wgt, float x) {
    assert(wgt);
    algui_set_widget_geometry(wgt, x, wgt->y, wgt->width, wgt->height);
}


void algui_set_widget_y(ALGUI_WIDGET* wgt, float y) {
    assert(wgt);
    algui_set_widget_geometry(wgt, wgt->x, wgt->y, wgt->width, wgt->height);
}


void algui_set_widget_size(ALGUI_WIDGET* wgt, float width, float height) {
    assert(wgt);
    algui_set_widget_geometry(wgt, wgt->x, wgt->y, width, height);
}


void algui_set_widget_width(ALGUI_WIDGET* wgt, float width) {
    assert(wgt);
    algui_set_widget_geometry(wgt, wgt->x, wgt->y, width, wgt->height);
}


void algui_set_widget_height(ALGUI_WIDGET* wgt, float height) {
    assert(wgt);
    algui_set_widget_geometry(wgt, wgt->x, wgt->y, wgt->width, height);
}


void algui_get_widget_rect(ALGUI_WIDGET* wgt, float* x1, float* y1, float* x2, float* y2) {
    assert(wgt);
    assert(x1);
    assert(y1);
    assert(x2);
    assert(y2);
    *x1 = wgt->x1;
    *y1 = wgt->y1;
    *x2 = wgt->x2;
    *y2 = wgt->y2;
}


void algui_do_widget_layout(ALGUI_WIDGET* wgt) {
    assert(wgt);
    ++doing_layout;
    do_widget_init_layout(wgt);
    do_widget_commit_layout(wgt);
    --doing_layout;
    update_tree_rect(wgt);
}


int algui_is_widget_visible(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->visible;
}


int algui_is_widget_in_visible_tree(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->tree_visible;
}


void algui_set_widget_visible(ALGUI_WIDGET* wgt, int visible) {
    assert(wgt);
    if ((visible != 0) != wgt->visible) {
        wgt->visible = visible != 0;
        update_tree_visible(wgt);
    }
}


int algui_is_widget_enabled(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->enabled;
}


int algui_is_widget_in_enabled_tree(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->tree_enabled;
}


void algui_set_widget_enabled(ALGUI_WIDGET* wgt, int enabled) {
    assert(wgt);

    //enable
    if (enabled) {
        if (!wgt->enabled) {
            wgt->enabled = 1;
            update_tree_enabled(wgt);
        }
    }

    //else disable; if it contains the focus, then lose the focus
    else {
        if (wgt->enabled) {
            if (widget_contains_focus(wgt)) {
                do_widget_lost_focus(focused_widget);
            }
            wgt->enabled = 0;
            update_tree_enabled(wgt);
        }
    }
}


int algui_is_widget_highlighted(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->highlighted;
}


int algui_is_widget_in_highlighted_tree(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->tree_highlighted;
}


void algui_set_widget_highlighted(ALGUI_WIDGET* wgt, int highlighted) {
    assert(wgt);
    if ((highlighted != 0) != wgt->highlighted) {
        wgt->highlighted = highlighted != 0;
        update_tree_highlighted(wgt);
    }
}


int algui_is_widget_pressed(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->pressed;
}


int algui_is_widget_in_pressed_tree(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->tree_pressed;
}


void algui_set_widget_pressed(ALGUI_WIDGET* wgt, int pressed) {
    assert(wgt);
    if ((pressed != 0) != wgt->pressed) {
        wgt->pressed = pressed != 0;
        update_tree_pressed(wgt);
    }
}


int algui_is_widget_selected(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->selected;
}


int algui_is_widget_in_selected_tree(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->tree_selected;
}


void algui_set_widget_selected(ALGUI_WIDGET* wgt, int selected) {
    assert(wgt);
    if ((selected != 0) != wgt->selected) {
        wgt->selected = selected != 0;
        update_tree_selected(wgt);
    }
}


int algui_is_widget_active(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->active;
}


int algui_is_widget_in_active_tree(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->tree_active;
}


void algui_set_widget_active(ALGUI_WIDGET* wgt, int active) {
    assert(wgt);
    if ((active != 0) != wgt->active) {
        wgt->active = active != 0;
        update_tree_active(wgt);
    }
}


int algui_is_widget_error(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->error;
}


int algui_is_widget_in_error_tree(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->tree_error;
}


void algui_set_widget_error(ALGUI_WIDGET* wgt, int error) {
    assert(wgt);
    if ((error != 0) != wgt->error) {
        wgt->error = error != 0;
        update_tree_error(wgt);
    }
}


int algui_is_widget_focused(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return is_widget_focused(wgt);
}


int algui_is_widget_in_focused_tree(ALGUI_WIDGET* wgt) {
    assert(wgt);
    return wgt->tree_focused;
}


int algui_set_widget_focused(ALGUI_WIDGET* wgt, int focused) {
    assert(wgt);
    assert(wgt->proc);

    if (focused) {
        //if the widget is disabled, it cannot get the focus
        if (!wgt->tree_enabled) {
            return 0;
        }

        //if the widget already has the focus, there is no need to get it
        if (is_widget_focused(wgt)) {
            return 1;
        }

        //if the widget does not accept the focus, do nothing else
        if (!do_widget_message(wgt, ALGUI_MESSAGE_GET_FOCUS, NULL)) {
            return 0;
        }

        //if there is already a focused widget and doesn't want to lose the focus, do nothing else
        if (focused_widget && !algui_set_widget_focused(focused_widget, 0)) {
            return 0;
        }

        //widget can be focused
        focused_widget = wgt;
        update_tree_focused(wgt);
        do_widget_message(wgt, ALGUI_MESSAGE_GOT_FOCUS, NULL);
        do_ancestor_message(wgt, ALGUI_MESSAGE_DESCENTANT_GOT_FOCUS, NULL);
    }
    else {
        //if the widget doesn't have the focus, do nothing else
        if (!is_widget_focused(wgt)) {
            return 1;
        }

        //if the widget does not want to lose the focus, do nothing else
        if (!do_widget_message(wgt, ALGUI_MESSAGE_LOSE_FOCUS, NULL)) {
            return 0;
        }

        //widget can lose the focus
        do_widget_lost_focus(wgt);
    }

    return 1;
}


