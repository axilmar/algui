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
    ALGUI_DRAW_OPERATION data;
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


//sends the init size message to children first, then to the node
static void init_size(ALGUI_NODE* node) {
    //notify children
    for (ALGUI_NODE* child = node->first_child; child; child = child->next_sibling) {
        init_size(child);
    }

    //send the message
    algui_do_node_message(node, ALGUI_MESSAGE_INIT_SIZE, NULL);
}


//send the do layout message to parent, then to the children
static void do_layout(ALGUI_NODE* node) {
    //prepare the message data
    ALGUI_LAYOUT_OPERATION data;
    data.width = node->width * node->h_scaling;
    data.height = node->height * node->v_scaling;

    //send the message
    algui_do_node_message(node, ALGUI_MESSAGE_DO_LAYOUT, &data);

    //layout children
    for (ALGUI_NODE* child = node->first_child; child; child = child->next_sibling) {
        do_layout(child);
    }
}


//sends a state message
static void do_state_message(ALGUI_NODE* node, int msg_id, int value) {
    ALGUI_STATE_OPERATION data;
    data.state = value;
    algui_do_node_message(node, msg_id, &data);
}


//the default node proc
int algui_default_node_proc(ALGUI_NODE* node, int msg_id, void* msg_data) {
    ALGUI_TREE_OPERATION* op_mod;
    ALGUI_DRAW_OPERATION* op_draw;
    ALGUI_GEOMETRY_OPERATION* op_geom;
    ALGUI_STATE_OPERATION* op_state;

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
            //clamp size to zero or positive
            node->width = _MAX(node->width, 0);
            node->height = _MAX(node->height, 0);

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

            //node no more dirty
            node->tree_dirty = node->dirty = 0;

            //update children
            for (ALGUI_NODE* child = node->first_child; child; child = child->next_sibling) {
                algui_update_node(child);
            }
            return 1;

        case ALGUI_MESSAGE_INSERT_NODE:
            op_mod = (ALGUI_TREE_OPERATION*)msg_data;
            assert(op_mod->child);
            assert(!op_mod->child->parent);
            assert(!node_in_tree(op_mod->child, node));
            insert_node(node, op_mod->child, op_mod->z_order);
            algui_set_node_dirty(op_mod->child);
            return 1;

        case ALGUI_MESSAGE_SET_NODE_Z_ORDER:
            op_mod = (ALGUI_TREE_OPERATION*)msg_data;
            assert(op_mod->child);
            assert(op_mod->child->parent == node);
            remove_node(node, op_mod->child);
            insert_node(node, op_mod->child, op_mod->z_order);
            return 1;

        case ALGUI_MESSAGE_REMOVE_NODE:
            op_mod = (ALGUI_TREE_OPERATION*)msg_data;
            assert(op_mod->child);
            assert(op_mod->child->parent == node);
            remove_node(node, op_mod->child);
            return 1;

        case ALGUI_MESSAGE_DRAW:
            op_draw = (ALGUI_DRAW_OPERATION*)msg_data;
            draw_node(node, op_draw->clip_x1, op_draw->clip_y1, op_draw->clip_x2, op_draw->clip_y2);
            return 1;

        case ALGUI_MESSAGE_SET_GEOMETRY:
            op_geom = (ALGUI_GEOMETRY_OPERATION*)msg_data;
            if (op_geom->x != node->x || op_geom->y != node->y || op_geom->width != node->width || op_geom->height != node->height) {
                node->x = op_geom->x;
                node->y = op_geom->y;
                node->width = op_geom->width;
                node->height = op_geom->height;
                algui_set_node_dirty(node);
            }
            return 1;

        case ALGUI_MESSAGE_SET_VISIBLE:
            op_state = (ALGUI_STATE_OPERATION*)msg_data;
            if (node->visible != op_state->state) {
                node->visible = op_state->state;
                algui_set_node_dirty(node);
            }
            return 1;

        case ALGUI_MESSAGE_SET_ENABLED:
            op_state = (ALGUI_STATE_OPERATION*)msg_data;
            if (node->enabled != op_state->state) {
                node->enabled = op_state->state;
                algui_set_node_dirty(node);
            }
            return 1;

        case ALGUI_MESSAGE_SET_HIGHLIGHTED:
            op_state = (ALGUI_STATE_OPERATION*)msg_data;
            if (node->highlighted != op_state->state) {
                node->highlighted = op_state->state;
                algui_set_node_dirty(node);
            }
            return 1;

        case ALGUI_MESSAGE_SET_PRESSED:
            op_state = (ALGUI_STATE_OPERATION*)msg_data;
            if (node->pressed != op_state->state) {
                node->pressed = op_state->state;
                algui_set_node_dirty(node);
            }
            return 1;

        case ALGUI_MESSAGE_SET_SELECTED:
            op_state = (ALGUI_STATE_OPERATION*)msg_data;
            if (node->selected != op_state->state) {
                node->selected = op_state->state;
                algui_set_node_dirty(node);
            }
            return 1;

        case ALGUI_MESSAGE_SET_ACTIVE:
            op_state = (ALGUI_STATE_OPERATION*)msg_data;
            if (node->active != op_state->state) {
                node->active = op_state->state;
                algui_set_node_dirty(node);
                if (node->parent) {
                    algui_set_node_active(node->parent, op_state->state);
                }
            }
            return 1;

        case ALGUI_MESSAGE_SET_ERROR:
            op_state = (ALGUI_STATE_OPERATION*)msg_data;
            if (node->error != op_state->state) {
                node->error = op_state->state;
                algui_set_node_dirty(node);
            }
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
    node->dirty = 1;
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
    ALGUI_TREE_OPERATION data;
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
    ALGUI_TREE_OPERATION data;
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
    ALGUI_TREE_OPERATION data;
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


//set node dirty
void algui_set_node_dirty(ALGUI_NODE* node) {
    assert(node);

    //do nothing is node is already dirty
    if (node->dirty) {
        return;
    }

    //set node as dirty
    node->dirty = 1;

    //set ancestors to have a dirty descentant
    for (ALGUI_NODE* ancestor = node->parent; ancestor; ancestor = ancestor->parent) {
        if (ancestor->tree_dirty) {
            break;
        }
        ancestor->tree_dirty = 1;
    }
}


//Updates the nodes that were marked as dirty.
void algui_update_dirty_nodes(ALGUI_NODE* node) {
    assert(node);

    //if node is dirty, update it and stop
    if (node->dirty) {
        algui_update_node(node);
        return;
    }

    //if node does not have dirty descentants, do nothing else
    if (!node->tree_dirty) {
        return;
    }

    //set tree dirty as false since this is node is examined
    node->tree_dirty = 0;

    //update children
    for (ALGUI_NODE* child = node->first_child; child; child = child->next_sibling) {
        algui_update_dirty_nodes(child);
    }
}


//sets the geometry of a node.
void algui_set_node_geometry(ALGUI_NODE* node, float x, float y, float width, float height) {
    ALGUI_GEOMETRY_OPERATION data;
    data.x = x;
    data.y = y;
    data.width = width;
    data.height = height;
    algui_do_node_message(node, ALGUI_MESSAGE_SET_GEOMETRY, &data);
}


//set node position
void algui_set_node_position(ALGUI_NODE* node, float x, float y) {
    assert(node);
    algui_set_node_geometry(node, x, y, node->width, node->height);
}


//set node size
void algui_set_node_size(ALGUI_NODE* node, float width, float height) {
    assert(node);
    algui_set_node_geometry(node, node->x, node->y, width, height);
}


//manage node layout
void algui_manage_node_layout(ALGUI_NODE* node) {
    assert(node);
    init_size(node);
    do_layout(node);
}


//set scaling
void algui_set_node_scaling(ALGUI_NODE* node, float h_scaling, float v_scaling) {
    assert(node);
    if (h_scaling != node->h_scaling || v_scaling != node->v_scaling) {
        node->h_scaling = h_scaling;
        node->v_scaling = v_scaling;
        algui_set_node_dirty(node);
    }
}


//set node visible
void algui_set_node_visible(ALGUI_NODE* node, int value) {
    do_state_message(node, ALGUI_MESSAGE_SET_VISIBLE, value);
}


//set node enabled
void algui_set_node_enabled(ALGUI_NODE* node, int value) {
    do_state_message(node, ALGUI_MESSAGE_SET_ENABLED, value);
}


//set node highlighted
void algui_set_node_highlighted(ALGUI_NODE* node, int value) {
    do_state_message(node, ALGUI_MESSAGE_SET_HIGHLIGHTED, value);
}


//set node pressed
void algui_set_node_pressed(ALGUI_NODE* node, int value) {
    do_state_message(node, ALGUI_MESSAGE_SET_PRESSED, value);
}


//set node selected
void algui_set_node_selected(ALGUI_NODE* node, int value) {
    do_state_message(node, ALGUI_MESSAGE_SET_SELECTED, value);
}


//set node active
void algui_set_node_active(ALGUI_NODE* node, int value) {
    do_state_message(node, ALGUI_MESSAGE_SET_ACTIVE, value);
}


//set node error
void algui_set_node_error(ALGUI_NODE* node, int value) {
    do_state_message(node, ALGUI_MESSAGE_SET_ERROR, value);
}
