#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "algui/node.h"


//min/max
#define _MIN(A, B) ((A) < (B) ? (A) : (B))
#define _MAX(A, B) ((A) > (B) ? (A) : (B))


//check if given node is in the given tree
static int node_in_tree(ALGUI_NODE* tree, ALGUI_NODE* node) {
    for (; node; node = node->parent) {
        if (node == tree) {
            return 1;
        }
    }
    return 0;
}


//insert node
static void insert_node(ALGUI_NODE* parent, ALGUI_NODE* child, int z_order) {
    ALGUI_NODE* prev = NULL;
    ALGUI_NODE* next = NULL;

    //find prev-next based on z-order
    if (z_order >= 0) {
        next = parent->first_child;
        for (; next && z_order > 0; --z_order, next = next->next_sibling);
        prev = next ? next->prev_sibling : NULL;
    }
    else {
        prev = parent->last_child;
        for (; prev && z_order < -1; ++z_order, prev = prev->prev_sibling);
        next = prev ? prev->next_sibling : NULL;
    }

    //connect the child
    child->parent = parent;
    child->prev_sibling = prev;
    child->next_sibling = next;
    if (prev) {
        prev->next_sibling = child;
    }
    else {
        parent->first_child = child;
    }
    if (next) {
        next->prev_sibling = child;
    }
    else {
        parent->last_child = child;
    }
}


//remove node
static void remove_node(ALGUI_NODE* parent, ALGUI_NODE* child) {
    if (child->prev_sibling) {
        child->prev_sibling->next_sibling = child->next_sibling;
    }
    else {
        parent->first_child = child->next_sibling;
    }
    if (child->next_sibling) {
        child->next_sibling->prev_sibling = child->prev_sibling;
    }
    else {
        parent->last_child = child->prev_sibling;
    }
    child->parent = child->prev_sibling = child->next_sibling = NULL;
}


//do the draw message
static void do_draw_message(ALGUI_NODE* node, int clip_x1, int clip_y1, int clip_x2, int clip_y2) {
    ALGUI_NODE_DRAW_OPERATION data;
    data.clip_x1 = clip_x1;
    data.clip_y1 = clip_y1;
    data.clip_x2 = clip_x2;
    data.clip_y2 = clip_y2;
    algui_do_node_message(node, ALGUI_MESSAGE_DRAW, &data);
}


//draws a node
static void draw_node(ALGUI_NODE* node, int clip_x1, int clip_y1, int clip_x2, int clip_y2) {
    //the node must be visible
    if (!node->tree_visible) {
        return;
    }

    //in order to compute the actual clipping, convert the float coordinates of the node to integers
    int x1 = roundf(node->x1);
    int y1 = roundf(node->y1);
    int x2 = roundf(node->x2);
    int y2 = roundf(node->y2);

    //compute the actual clipping, which is the intersection of the clipping coordinates
    //by the coordinates of the node
    clip_x1 = _MAX(clip_x1, x1);
    clip_y1 = _MAX(clip_y1, y1);
    clip_x2 = _MIN(clip_x2, x2);
    clip_y2 = _MIN(clip_y2, y2);

    //check if clipping is negative; if so, then it means the node did not intersect with the clipping,
    //so don't draw anything
    if (clip_x2 < clip_x1 || clip_y2 < clip_y1) {
        return;
    }
    
    //draw with clipping
    if (node->clipped) {
        //get current clipping
        int prev_clip_x, prev_clip_y, prev_clip_w, prev_clip_h;
        al_get_clipping_rectangle(&prev_clip_x, &prev_clip_y, &prev_clip_w, &prev_clip_h);

        //set new clipping
        al_set_clipping_rectangle(clip_x1, clip_y1, clip_x2 - clip_x1 + 1, clip_y2 - clip_y1 + 1);

        //paint the node
        algui_do_node_message(node, ALGUI_MESSAGE_PAINT, NULL);

        //draw the children
        for (ALGUI_NODE* child = node->first_child; child; child = child->next_sibling) {
            do_draw_message(child, clip_x1, clip_y1, clip_x2, clip_y2);
        }

        //restore the clipping
        al_set_clipping_rectangle(prev_clip_x, prev_clip_y, prev_clip_w, prev_clip_h);
    }

    //else draw without clipping
    else {
        //paint the node
        algui_do_node_message(node, ALGUI_MESSAGE_PAINT, NULL);

        //draw the children
        for (ALGUI_NODE* child = node->first_child; child; child = child->next_sibling) {
            do_draw_message(child, clip_x1, clip_y1, clip_x2, clip_y2);
        }
    }
}


//the default node proc
int algui_default_node_proc(ALGUI_NODE* node, int msg_id, void* msg_data) {
    ALGUI_NODE_TREE_OPERATION* op_mod;
    ALGUI_NODE_DRAW_OPERATION* op_draw;

    assert(node);

    switch (msg_id) {
        //cleanup
        case ALGUI_MESSAGE_CLEANUP:
            //remove from parent
            algui_remove_node(node);

            //cleanup children
            for (ALGUI_NODE* child = node->last_child; child; ) {
                ALGUI_NODE* prev = child->prev_sibling;
                algui_cleanup_node(child);
                child = prev;
            }

            //free node
            if (node->heap_allocated) {
                free(node);
            }
            return 1;

        //update
        case ALGUI_MESSAGE_UPDATE:
            //update state for child node
            if (node->parent) {
                node->x1 = node->x * node->parent->tree_h_scaling + node->parent->x1;
                node->y1 = node->y * node->parent->tree_v_scaling + node->parent->y1;
                node->x2 = node->x1 + node->width * node->parent->tree_h_scaling;
                node->y2 = node->y1 + node->height * node->parent->tree_v_scaling;
                node->tree_visible = node->visible && node->parent->tree_visible;
                node->tree_enabled = node->enabled && node->parent->tree_enabled;
                node->tree_highlighted = node->highlighted || node->parent->tree_highlighted;
                node->tree_pressed = node->pressed || node->parent->tree_pressed;
                node->tree_selected = node->selected || node->parent->tree_selected;
                node->tree_active = node->active || node->parent->tree_active;
                node->tree_error = node->error || node->parent->tree_error;
                node->tree_focused = node->focused || node->parent->tree_focused;
                node->tree_h_scaling = node->h_scaling * node->parent->tree_h_scaling;
                node->tree_v_scaling = node->v_scaling * node->parent->tree_v_scaling;
            }

            //else update state for root node
            else {
                node->x1 = node->x;
                node->y1 = node->y;
                node->x2 = node->x1 + node->width;
                node->y2 = node->y1 + node->height;
                node->tree_visible = node->visible;
                node->tree_enabled = node->enabled;
                node->tree_highlighted = node->highlighted;
                node->tree_pressed = node->pressed;
                node->tree_selected = node->selected;
                node->tree_active = node->active;
                node->tree_error = node->error;
                node->tree_focused = node->focused;
                node->tree_h_scaling = node->h_scaling;
                node->tree_v_scaling = node->v_scaling;
            }

            //update children
            for (ALGUI_NODE* child = node->first_child; child; child = child->next_sibling) {
                algui_update_node(child);
            }
            return 1;

        case ALGUI_MESSAGE_INSERT_NODE:
            op_mod = (ALGUI_NODE_TREE_OPERATION*)msg_data;
            assert(op_mod->child);
            assert(!op_mod->child->parent);
            assert(!node_in_tree(op_mod->child, node));
            insert_node(node, op_mod->child, op_mod->z_order);
            return 1;

        case ALGUI_MESSAGE_SET_NODE_Z_ORDER:
            op_mod = (ALGUI_NODE_TREE_OPERATION*)msg_data;
            assert(op_mod->child);
            assert(op_mod->child->parent == node);
            remove_node(node, op_mod->child);
            insert_node(node, op_mod->child, op_mod->z_order);
            return 1;

        case ALGUI_MESSAGE_REMOVE_NODE:
            op_mod = (ALGUI_NODE_TREE_OPERATION*)msg_data;
            assert(op_mod->child);
            assert(op_mod->child->parent == node);
            remove_node(node, op_mod->child);
            return 1;

        case ALGUI_MESSAGE_DRAW:
            op_draw = (ALGUI_NODE_DRAW_OPERATION*)msg_data;
            draw_node(node, op_draw->clip_x1, op_draw->clip_y1, op_draw->clip_x2, op_draw->clip_y2);
            return 1;
    }

    return 0;
}


//send message
int algui_do_node_message(ALGUI_NODE* node, int msg_id, void* msg_data) {
    assert(node);
    assert(node->proc);
    return node->proc(node, msg_id, msg_data);
}


//init node
void algui_init_node(ALGUI_NODE* node, int heap_allocated) {
    memset(node, 0, sizeof(ALGUI_NODE));
    node->visible = 1;
    node->enabled = 1;
    node->tree_visible = 1;
    node->tree_enabled = 1;
    node->clipped = 0;
    node->h_scaling = 1;
    node->v_scaling = 1;
    node->tree_h_scaling = 1;
    node->tree_v_scaling = 1;
    node->heap_allocated = heap_allocated;
}


//cleanup node
void algui_cleanup_node(ALGUI_NODE* node) {
    algui_do_node_message(node, ALGUI_MESSAGE_CLEANUP, NULL);
}


//update node
void algui_update_node(ALGUI_NODE* node) {
    algui_do_node_message(node, ALGUI_MESSAGE_UPDATE, NULL);
}


//insert node
void algui_insert_node(ALGUI_NODE* parent, ALGUI_NODE* node, int z_order) {
    assert(parent);

    //prepare message data
    ALGUI_NODE_TREE_OPERATION data;
    data.child = node;
    data.z_order = z_order;

    //send message to parent
    algui_do_node_message(parent, ALGUI_MESSAGE_INSERT_NODE, &data);
}


//add node
void algui_add_node(ALGUI_NODE* parent, ALGUI_NODE* node) {
    algui_insert_node(parent, node, -1);
}


//set z-order
void algui_set_node_z_order(ALGUI_NODE* node, int z_order) {
    assert(node);

    //if no parent, do nothing
    if (!node->parent) {
        return;
    }

    //prepare message data
    ALGUI_NODE_TREE_OPERATION data;
    data.child = node;
    data.z_order = z_order;

    //send message to parent
    algui_do_node_message(node->parent, ALGUI_MESSAGE_SET_NODE_Z_ORDER, &data);
}


//remove node
void algui_remove_node(ALGUI_NODE* node) {
    assert(node);

    //if no parent, do nothing
    if (!node->parent) {
        return;
    }

    //prepare message data
    ALGUI_NODE_TREE_OPERATION data;
    data.child = node;

    //send message to parent
    algui_do_node_message(node->parent, ALGUI_MESSAGE_REMOVE_NODE, &data);
}


//draws a node clipped
void algui_draw_node_clipped(ALGUI_NODE* node, int clip_x, int clip_y, int clip_w, int clip_h) {
    assert(node);
    do_draw_message(node, clip_x, clip_y, clip_x + clip_w - 1, clip_y + clip_h - 1);
}


//draws the node
void algui_draw_node(ALGUI_NODE* node) {
    int clip_x, clip_y, clip_w, clip_h;
    al_get_clipping_rectangle(&clip_x, &clip_y, &clip_w, &clip_h);
    algui_draw_node_clipped(node, clip_x, clip_y, clip_w, clip_h);
}
