#include <cassert>
#include "algui/widget.h"


//currently focused widget
static ALGUI_WIDGET* focused_widget = NULL;


//updates a child's rect
static void update_child_rect(ALGUI_WIDGET* wgt) {
    wgt->x1 = wgt->x + wgt->parent->x1;
    wgt->y1 = wgt->y + wgt->parent->y1;
    wgt->x2 = wgt->x1 + wgt->width;
    wgt->y2 = wgt->y1 + wgt->height;
}


//updates the child's state
static void update_child(ALGUI_WIDGET* wgt) {
    update_child_rect(wgt);
    wgt->tree_visible = wgt->visible && wgt->parent->tree_visible;
    wgt->tree_enabled = wgt->enabled && wgt->parent->tree_enabled;
    wgt->tree_has_mouse = wgt->has_mouse || wgt->parent->tree_has_mouse;
    wgt->tree_has_focus = wgt->has_focus || wgt->parent->tree_has_focus;
    wgt->tree_pressed = wgt->pressed || wgt->parent->tree_pressed;
    wgt->tree_selected = wgt->selected || wgt->parent->tree_selected;
    wgt->tree_active = wgt->active || wgt->parent->tree_active;
}


//updates the root's global rect
static void update_root_rect(ALGUI_WIDGET* wgt) {
    wgt->x1 = wgt->x;
    wgt->y1 = wgt->y;
    wgt->x2 = wgt->x1 + wgt->width;
    wgt->y2 = wgt->y1 + wgt->height;
}


//update the root's state
static void update_root(ALGUI_WIDGET* wgt) {
    update_root_rect(wgt);
    wgt->tree_visible = wgt->visible;
    wgt->tree_enabled = wgt->enabled;
    wgt->tree_has_mouse = wgt->has_mouse;
    wgt->tree_has_focus = wgt->has_focus;
    wgt->tree_pressed = wgt->pressed;
    wgt->tree_selected = wgt->selected;
    wgt->tree_active = wgt->active;
}


//updates a child's tree rect
static void update_child_tree_rect(ALGUI_WIDGET* wgt) {
    //if locked, don't update
    if (wgt->lock_count > 0) {
        return;
    }

    //update child rect
    update_child_rect(wgt);

    //update children
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_rect(child);
    }
}


//update root's tree rect
static void update_root_tree_rect(ALGUI_WIDGET* wgt) {
    //if locked, don't update
    if (wgt->lock_count > 0) {
        return;
    }

    //update rect
    update_root_rect(wgt);

    //update children
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_rect(child);
    }
}


//if the widget is not locked for updating, then it updates the global position of the widget
static void update_tree_rect(ALGUI_WIDGET* wgt) {
    if (wgt->parent) {
        update_child_tree_rect(wgt);
    }
    else {
        update_root_tree_rect(wgt);
    }
}


//update child tree visible
static void update_child_tree_visible(ALGUI_WIDGET* wgt) {
    if (wgt->lock_count > 0) {
        return;
    }
    wgt->tree_visible = wgt->visible && wgt->parent->tree_visible;
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_visible(child);
    }
}


//update root tree visible
static void update_root_tree_visible(ALGUI_WIDGET* wgt) {
    if (wgt->lock_count > 0) {
        return;
    }
    wgt->tree_visible = wgt->visible;
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_visible(child);
    }
}


//update tree visible
static void update_tree_visible(ALGUI_WIDGET* wgt) {
    if (wgt->parent) {
        update_child_tree_visible(wgt);
    }
    else {
        update_root_tree_visible(wgt);
    }
}


//update child tree enabled
static void update_child_tree_enabled(ALGUI_WIDGET* wgt) {
    if (wgt->lock_count > 0) {
        return;
    }
    wgt->tree_enabled = wgt->enabled && wgt->parent->tree_enabled;
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_enabled(child);
    }
}


//update root tree enabled
static void update_root_tree_enabled(ALGUI_WIDGET* wgt) {
    if (wgt->lock_count > 0) {
        return;
    }
    wgt->tree_enabled = wgt->enabled;
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_enabled(child);
    }
}


//update tree enabled
static void update_tree_enabled(ALGUI_WIDGET* wgt) {
    if (wgt->parent) {
        update_child_tree_enabled(wgt);
    }
    else {
        update_root_tree_enabled(wgt);
    }
}


//update child tree has_mouse
static void update_child_tree_has_mouse(ALGUI_WIDGET* wgt) {
    if (wgt->lock_count > 0) {
        return;
    }
    wgt->tree_has_mouse = wgt->has_mouse || wgt->parent->tree_has_mouse;
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_has_mouse(child);
    }
}


//update root tree has_mouse
static void update_root_tree_has_mouse(ALGUI_WIDGET* wgt) {
    if (wgt->lock_count > 0) {
        return;
    }
    wgt->tree_has_mouse = wgt->has_mouse;
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_has_mouse(child);
    }
}


//update tree has_mouse
static void update_tree_has_mouse(ALGUI_WIDGET* wgt) {
    if (wgt->parent) {
        update_child_tree_has_mouse(wgt);
    }
    else {
        update_root_tree_has_mouse(wgt);
    }
}


//update child tree has_focus
static void update_child_tree_has_focus(ALGUI_WIDGET* wgt) {
    if (wgt->lock_count > 0) {
        return;
    }
    wgt->tree_has_focus = wgt->has_focus || wgt->parent->tree_has_focus;
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_has_focus(child);
    }
}


//update root tree has_focus
static void update_root_tree_has_focus(ALGUI_WIDGET* wgt) {
    if (wgt->lock_count > 0) {
        return;
    }
    wgt->tree_has_focus = wgt->has_focus;
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_has_focus(child);
    }
}


//update tree has_focus
static void update_tree_has_focus(ALGUI_WIDGET* wgt) {
    if (wgt->parent) {
        update_child_tree_has_focus(wgt);
    }
    else {
        update_root_tree_has_focus(wgt);
    }
}


//update child tree pressed
static void update_child_tree_pressed(ALGUI_WIDGET* wgt) {
    if (wgt->lock_count > 0) {
        return;
    }
    wgt->tree_pressed = wgt->pressed || wgt->parent->tree_pressed;
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_pressed(child);
    }
}


//update root tree pressed
static void update_root_tree_pressed(ALGUI_WIDGET* wgt) {
    if (wgt->lock_count > 0) {
        return;
    }
    wgt->tree_pressed = wgt->pressed;
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_pressed(child);
    }
}


//update tree pressed
static void update_tree_pressed(ALGUI_WIDGET* wgt) {
    if (wgt->parent) {
        update_child_tree_pressed(wgt);
    }
    else {
        update_root_tree_pressed(wgt);
    }
}


//update child tree selected
static void update_child_tree_selected(ALGUI_WIDGET* wgt) {
    if (wgt->lock_count > 0) {
        return;
    }
    wgt->tree_selected = wgt->selected || wgt->parent->tree_selected;
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_selected(child);
    }
}


//update root tree selected
static void update_root_tree_selected(ALGUI_WIDGET* wgt) {
    if (wgt->lock_count > 0) {
        return;
    }
    wgt->tree_selected = wgt->selected;
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_selected(child);
    }
}


//update tree selected
static void update_tree_selected(ALGUI_WIDGET* wgt) {
    if (wgt->parent) {
        update_child_tree_selected(wgt);
    }
    else {
        update_root_tree_selected(wgt);
    }
}


//update child tree active
static void update_child_tree_active(ALGUI_WIDGET* wgt) {
    if (wgt->lock_count > 0) {
        return;
    }
    wgt->tree_active = wgt->active || wgt->parent->tree_active;
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_active(child);
    }
}


//update root tree active
static void update_root_tree_active(ALGUI_WIDGET* wgt) {
    if (wgt->lock_count > 0) {
        return;
    }
    wgt->tree_active = wgt->active;
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_active(child);
    }
}


//update tree active
static void update_tree_active(ALGUI_WIDGET* wgt) {
    if (wgt->parent) {
        update_child_tree_active(wgt);
    }
    else {
        update_root_tree_active(wgt);
    }
}


//adjusts the lock count of every widget in the tree, by the given amount
static void adjust_tree_lock_count(ALGUI_WIDGET* wgt, int lock_count_adjustment) {
    wgt->lock_count += lock_count_adjustment;
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        adjust_tree_lock_count(child, lock_count_adjustment);
    }
}


//updates a child's tree rect and also adjust lock count
static void update_child_tree_rect_adjust_lock_count(ALGUI_WIDGET* wgt, int lock_count_adjustment) {
    wgt->lock_count += lock_count_adjustment;

    //if locked, don't update
    if (wgt->lock_count > 0) {
        for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
            adjust_tree_lock_count(child, lock_count_adjustment);
        }
        return;
    }

    //update child rect
    update_child_rect(wgt);

    //update children
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_rect_adjust_lock_count(child, lock_count_adjustment);
    }
}


//update root's tree rect and also adjust lock count
static void update_root_tree_rect_adjust_lock_count(ALGUI_WIDGET* wgt, int lock_count_adjustment) {
    wgt->lock_count += lock_count_adjustment;

    //if locked, don't update
    if (wgt->lock_count > 0) {
        for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
            adjust_tree_lock_count(child, lock_count_adjustment);
        }
        return;
    }

    //update rect
    update_root_rect(wgt);

    //update children
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_rect_adjust_lock_count(child, lock_count_adjustment);
    }
}


//if the widget is not locked for updating, then it updates the global position of the widget; also adjusts lock count
static void update_tree_rect_adjust_lock_count(ALGUI_WIDGET* wgt, int lock_count_adjustment) {
    if (wgt->parent) {
        update_child_tree_rect_adjust_lock_count(wgt, lock_count_adjustment);
    }
    else {
        update_root_tree_rect_adjust_lock_count(wgt, lock_count_adjustment);
    }
}


//if the widget is not locked for updating, then it updates the global position and state of the widget
static void update_child_tree_adjust_lock_count(ALGUI_WIDGET* wgt, int lock_count_adjustment) {
    //adjust the lock count
    wgt->lock_count += lock_count_adjustment;

    //if locked, don't update
    if (wgt->lock_count > 0) {
        for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
            adjust_tree_lock_count(child, lock_count_adjustment);
        }
        return;
    }

    //update child position/state
    update_child(wgt);

    //update children
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_adjust_lock_count(child, lock_count_adjustment);
    }
}


//if the widget is not locked for updating, then it updates the global position and state of the widget
static void update_root_tree_adjust_lock_count(ALGUI_WIDGET* wgt, int lock_count_adjustment) {
    //adjust the lock count
    wgt->lock_count += lock_count_adjustment;

    //if locked, don't update
    if (wgt->lock_count > 0) {
        for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
            adjust_tree_lock_count(child, lock_count_adjustment);
        }
        return;
    }

    //update root position/state
    update_root(wgt);

    //update children
    for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; child = child->next_sibling) {
        update_child_tree_adjust_lock_count(child, lock_count_adjustment);
    }
}


//update tree
static void update_tree_adjust_lock_count(ALGUI_WIDGET* wgt, int lock_count_adjustment) {
    if (wgt->parent) {
        update_child_tree_adjust_lock_count(wgt, lock_count_adjustment);
    }
    else {
        update_root_tree_adjust_lock_count(wgt, lock_count_adjustment);
    }
}


ALGUI_WIDGET_PROC algui_get_widget_proc(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->proc;
}


ALGUI_WIDGET* algui_get_parent_widget(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->parent;
}


ALGUI_WIDGET* algui_get_prev_sibling_widget(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->prev_sibling;
}


ALGUI_WIDGET* algui_get_next_sibling_widget(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->next_sibling;
}


ALGUI_WIDGET* algui_get_first_child_widget(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->first_child;
}


ALGUI_WIDGET* algui_get_last_child_widget(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->last_child;
}


void algui_get_widget_geometry(ALGUI_WIDGET* wgt, float* x, float* y, float* width, float* height) {
    assert(wgt != NULL);
    assert(x != NULL);
    assert(y != NULL);
    assert(width != NULL);
    assert(height != NULL);
    *x = wgt->x;
    *y = wgt->y;
    *width = wgt->width;
    *height = wgt->height;
}


void algui_get_widget_position(ALGUI_WIDGET* wgt, float* x, float* y) {
    assert(wgt != NULL);
    assert(x != NULL);
    assert(y != NULL);
    *x = wgt->x;
    *y = wgt->y;
}


void algui_get_widget_size(ALGUI_WIDGET* wgt, float* width, float* height) {
    assert(wgt != NULL);
    assert(width != NULL);
    assert(height != NULL);
    *width = wgt->width;
    *height = wgt->height;
}


float algui_get_widget_x(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->x;
}


float algui_get_widget_y(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->y;
}


float algui_get_widget_width(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->width;
}


float algui_get_widget_height(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->height;
}


void algui_get_widget_rect(ALGUI_WIDGET* wgt, float* x1, float* y1, float* x2, float* y2) {
    assert(wgt != NULL);
    assert(x1 != NULL);
    assert(y1 != NULL);
    assert(x2 != NULL);
    assert(y2 != NULL);
    *x1 = wgt->x1;
    *y1 = wgt->y1;
    *x2 = wgt->x2;
    *y2 = wgt->y2;
}


float algui_get_widget_x1(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->x1;
}


float algui_get_widget_y1(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->y1;
}


float algui_get_widget_x2(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->x2;
}


float algui_get_widget_y2(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->y2;
}


void* algui_get_widget_user_data(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->user_data;
}


int algui_get_widget_lock_count(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->lock_count;
}


int algui_is_widget_heap_allocated(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->heap_allocated;
}


int algui_widget_is_visible(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->visible;
}


int algui_widget_is_enabled(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->enabled;
}


int algui_widget_has_mouse(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->has_mouse;
}


int algui_widget_has_focus(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->has_focus;
}


int algui_widget_is_pressed(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->pressed;
}


int algui_widget_is_selected(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->selected;
}


int algui_widget_is_active(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->active;
}


int algui_widget_tree_is_visible(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->tree_visible;
}


int algui_widget_tree_is_enabled(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->tree_enabled;
}


int algui_widget_tree_has_mouse(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->tree_has_mouse;
}


int algui_widget_tree_has_focus(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->tree_has_focus;
}


int algui_widget_tree_is_pressed(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->tree_pressed;
}


int algui_widget_tree_is_selected(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->tree_selected;
}


int algui_widget_tree_is_active(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    return wgt->tree_active;
}


int algui_is_widget_in_tree(ALGUI_WIDGET* root, ALGUI_WIDGET* wgt) {
    assert(root != NULL);
    assert(wgt != NULL);

    //found the widget
    if (wgt == root) {
        return 1;
    }

    //search children
    for (ALGUI_WIDGET* child = root->first_child; child != NULL; child = child->next_sibling) {
        if (algui_is_widget_in_tree(child, wgt)) {
            return 1;
        }
    }

    //not found
    return 0;
}


ALGUI_WIDGET* algui_get_focused_widget() {
    return focused_widget;
}


ALGUI_WIDGET* algui_get_child_widget_at_z_order(ALGUI_WIDGET* parent, int z_order) {
    assert(parent != NULL);
    ALGUI_WIDGET* child;
    if (z_order >= 0) {
        for (child = parent->first_child; child != NULL && z_order > 0; child = child->next_sibling, --z_order) {
        }
    }
    else {
        for (child = parent->last_child; child != NULL && z_order < -1; child = child->prev_sibling, ++z_order) {
        }
    }
    return child;
}


int algui_widget_proc(ALGUI_WIDGET* wgt, int msgId, void* msgData) {
    switch (msgId) {
        //cleanup
        case ALGUI_MESSAGE_CLEANUP:
            //remove widget from its parent
            algui_detach_widget(wgt);

            //if the widget is the focus widget, then reset the focus widget
            if (wgt == focused_widget) {
                focused_widget = NULL;
                algui_do_widget_message(wgt, ALGUI_MESSAGE_LOST_FOCUS, NULL);
            }

            //cleanup children
            for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; ) {
                ALGUI_WIDGET* next_child = child->next_sibling;
                algui_cleanup_widget(child);
                child = next_child;
            }

            //free memory if needed
            if (wgt->heap_allocated) {
                free(wgt);
            }
            return 1;

        //layout; pass message to children
        case ALGUI_MESSAGE_LAYOUT:
            for (ALGUI_WIDGET* child = wgt->first_child; child != NULL; ) {
                ALGUI_WIDGET* next_child = child->next_sibling;
                algui_do_widget_layout(child);
                child = next_child;
            }
            return 1;

        //descentant got/lost focus; allow the message to bubble up
        case ALGUI_MESSAGE_DESCENTANT_GOT_FOCUS:
        case ALGUI_MESSAGE_DESCENTANT_LOST_FOCUS:
            return 1;
    }
    return 0;
}


int algui_do_widget_message(ALGUI_WIDGET* wgt, int msgId, void* msgData) {
    assert(wgt != NULL);
    assert(wgt->proc != NULL);
    return wgt->proc(wgt, msgId, msgData);
}


void algui_init_widget(ALGUI_WIDGET* wgt, ALGUI_WIDGET_PROC proc, void* user_data, int heap_allocated) {
    assert(wgt != NULL);
    assert(proc != NULL);
    wgt->proc = proc;
    wgt->parent = NULL;
    wgt->prev_sibling = NULL;
    wgt->next_sibling = NULL;
    wgt->first_child = NULL;
    wgt->last_child = NULL;
    wgt->x = 0;
    wgt->y = 0;
    wgt->width = 0;
    wgt->height = 0;
    wgt->user_data = user_data;
    wgt->lock_count = 0;
    wgt->heap_allocated = heap_allocated;
    wgt->visible = 1;
    wgt->enabled = 1;
    wgt->has_mouse = 0;
    wgt->has_focus = 0;
    wgt->pressed = 0;
    wgt->selected = 0;
    wgt->active = 0;
    wgt->tree_visible = 1;
    wgt->tree_enabled = 1;
    wgt->tree_has_mouse = 0;
    wgt->tree_has_focus = 0;
    wgt->tree_pressed = 0;
    wgt->tree_selected = 0;
    wgt->tree_active = 0;
}


void algui_set_widget_proc(ALGUI_WIDGET* wgt, ALGUI_WIDGET_PROC proc) {
    assert(wgt != NULL);
    assert(proc != NULL);
    wgt->proc = proc;
}

void algui_cleanup_widget(ALGUI_WIDGET* wgt) {
    algui_do_widget_message(wgt, ALGUI_MESSAGE_CLEANUP, NULL);
}


void algui_insert_widget(ALGUI_WIDGET* parent, ALGUI_WIDGET* wgt, int z_order) {
    assert(parent != NULL);
    assert(wgt != NULL);
    assert(wgt->parent == NULL);
    assert(!algui_is_widget_in_tree(parent, wgt));

    //find previous/next sibling
    ALGUI_WIDGET* prev_sibling = NULL;
    ALGUI_WIDGET* next_sibling = NULL;
    if (z_order >= 0) {
        for (next_sibling = parent->first_child; next_sibling != NULL && z_order > 0; next_sibling = next_sibling->next_sibling, --z_order) {
        }
    }
    else {
        for (prev_sibling = parent->last_child; prev_sibling != NULL && z_order < -1; prev_sibling = prev_sibling->prev_sibling, ++z_order) {
        }
    }

    //connect the widget to the tree
    wgt->parent = parent;
    wgt->prev_sibling = prev_sibling;
    wgt->next_sibling = next_sibling;
    if (prev_sibling) {
        prev_sibling->next_sibling = wgt;
    }
    else {
        parent->first_child = wgt;
    }
    if (next_sibling) {
        next_sibling->prev_sibling = wgt;
    }
    else {
        parent->last_child = wgt;
    }

    //update the newly inserted widget tree
    update_child_tree_adjust_lock_count(wgt, parent->lock_count);
}


void algui_add_widget(ALGUI_WIDGET* parent, ALGUI_WIDGET* wgt) {
    algui_insert_widget(parent, wgt, -1);
}


void algui_remove_widget(ALGUI_WIDGET* parent, ALGUI_WIDGET* wgt) {
    assert(parent != NULL);
    assert(wgt != NULL);
    assert(wgt->parent == parent);

    //remove the child
    if (wgt->prev_sibling != NULL) {
        wgt->prev_sibling->next_sibling = wgt->next_sibling;
    }
    else {
        parent->first_child = wgt->next_sibling;
    }
    if (wgt->next_sibling != NULL) {
        wgt->next_sibling->prev_sibling = wgt->prev_sibling;
    }
    else {
        parent->last_child = wgt->prev_sibling;
    }
    wgt->prev_sibling = wgt->next_sibling = wgt->parent = NULL;

    //update widget state
    update_root_tree_adjust_lock_count(wgt, -parent->lock_count);
}


void algui_detach_widget(ALGUI_WIDGET* wgt) {
    assert(wgt != NULL);
    if (wgt->parent != NULL) {
        algui_remove_widget(wgt->parent, wgt);
    }
}


void algui_set_widget_geometry(ALGUI_WIDGET* wgt, float x, float y, float width, float height) {
    assert(wgt);
    if (x != wgt->x || y != wgt->y || width != wgt->width || height != wgt->height) {
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


void algui_set_widget_size(ALGUI_WIDGET* wgt, float width, float height) {
    assert(wgt);
    algui_set_widget_geometry(wgt, wgt->x, wgt->y, width, height);
}


void algui_set_widget_x(ALGUI_WIDGET* wgt, float x) {
    assert(wgt);
    algui_set_widget_geometry(wgt, x, wgt->y, wgt->width, wgt->height);
}


void algui_set_widget_y(ALGUI_WIDGET* wgt, float y) {
    assert(wgt);
    algui_set_widget_geometry(wgt, wgt->x, y, wgt->width, wgt->height);
}


void algui_set_widget_width(ALGUI_WIDGET* wgt, float width) {
    assert(wgt);
    algui_set_widget_geometry(wgt, wgt->x, wgt->y, width, wgt->height);
}


void algui_set_widget_height(ALGUI_WIDGET* wgt, float height) {
    assert(wgt);
    algui_set_widget_geometry(wgt, wgt->x, wgt->y, wgt->width, height);
}


void algui_do_widget_layout(ALGUI_WIDGET* wgt) {
    algui_lock_widget(wgt);
    algui_do_widget_message(wgt, ALGUI_MESSAGE_LAYOUT, NULL);
    update_tree_rect_adjust_lock_count(wgt, -1);
}


void algui_set_widget_user_data(ALGUI_WIDGET* wgt, void* user_data) {
    assert(wgt);
    wgt->user_data = user_data;
}


void algui_lock_widget(ALGUI_WIDGET* wgt) {
    assert(wgt);
    adjust_tree_lock_count(wgt, 1);
}


void algui_unlock_widget(ALGUI_WIDGET* wgt) {
    assert(wgt);
    assert(wgt->lock_count > 0);
    update_tree_adjust_lock_count(wgt, -1);
}


void algui_set_widget_visible(ALGUI_WIDGET* wgt, int visible) {
    assert(wgt);
    if (wgt->visible != visible) {
        wgt->visible = visible;
        update_tree_visible(wgt);
    }
}


void algui_set_widget_enabled(ALGUI_WIDGET* wgt, int enabled) {
    assert(wgt);
    if (wgt->enabled != enabled ) {
        wgt->enabled = enabled;
        update_tree_enabled(wgt);
    }
}


int algui_set_widget_focused(ALGUI_WIDGET* wgt, int focused) {
    assert(wgt);

    //focus
    if (focused) {
        //if the widget already has the focus, do nothing else
        if (wgt->has_focus) {
            return 1;
        }

        //if the widget doesn't want the focus, do nothing else
        if (!algui_do_widget_message(wgt, ALGUI_MESSAGE_GET_FOCUS, NULL)) {
            return 0;
        }

        //if the previously focused widget does not want to lose the focus, do nothing else
        if (focused_widget && !algui_set_widget_focused(focused_widget, 0)) {
            return 0;
        }

        //the widget got the focus
        focused_widget = wgt;
        wgt->has_focus = 1;
        update_tree_has_focus(wgt);
        algui_do_widget_message(wgt, ALGUI_MESSAGE_GOT_FOCUS, NULL);

        //notify the ancestors that a descentant got the focus
        for (ALGUI_WIDGET* ancestor = wgt->parent; ancestor != NULL; ancestor = ancestor->parent) {
            if (!algui_do_widget_message(ancestor, ALGUI_MESSAGE_DESCENTANT_GOT_FOCUS, NULL)) {
                break;
            }
        }
    }

    //else blur 
    else {
        //if the widget doesn't have the focus, do nothing else
        if (!wgt->has_focus) {
            return 1;
        }

        //if the widget doesn't want to lose the focus, do nothing else
        if (!algui_do_widget_message(wgt, ALGUI_MESSAGE_LOSE_FOCUS, NULL)) {
            return 0;
        }

        //the widget lost the focus
        focused_widget = NULL;
        wgt->has_focus = 0;
        update_tree_has_focus(wgt);
        algui_do_widget_message(wgt, ALGUI_MESSAGE_LOST_FOCUS, NULL);

        //notify the ancestors that a descentant lost the focus
        for (ALGUI_WIDGET* ancestor = wgt->parent; ancestor != NULL; ancestor = ancestor->parent) {
            if (!algui_do_widget_message(ancestor, ALGUI_MESSAGE_DESCENTANT_LOST_FOCUS, NULL)) {
                break;
            }
        }
    }

    //success
    return 1;
}


void algui_focus_widget(ALGUI_WIDGET* wgt) {
    algui_set_widget_focused(wgt, 1);
}


void algui_blur_widget(ALGUI_WIDGET* wgt) {
    algui_set_widget_focused(wgt, 0);
}


void algui_set_widget_pressed(ALGUI_WIDGET* wgt, int pressed) {
    assert(wgt);
    if (wgt->pressed != pressed) {
        wgt->pressed = pressed;
        update_tree_pressed(wgt);
    }
}


void algui_set_widget_selected(ALGUI_WIDGET* wgt, int selected) {
    assert(wgt);
    if (wgt->selected != selected) {
        wgt->selected = selected;
        update_tree_selected(wgt);
    }
}


void algui_set_widget_active(ALGUI_WIDGET* wgt, int active) {
    assert(wgt);
    if (wgt->active != active) {
        wgt->active = active;
        update_tree_active(wgt);
    }
}


void algui_paint_widget(ALGUI_WIDGET* wgt) {
    assert(wgt);
    if (wgt->tree_visible) {
        algui_do_widget_message(wgt, ALGUI_MESSAGE_PAINT, NULL);
        for (ALGUI_WIDGET* child = wgt->first_child; child; ) {
            ALGUI_WIDGET* next_sibling = child->next_sibling;
            algui_paint_widget(child);
            child = next_sibling;
        }
    }
}
