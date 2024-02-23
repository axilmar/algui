#include <stdlib.h>
#include "ALGUI_NODE.h"


//init a node
static ALGUI_RESULT node_init(ALGUI_NODE* node, ALGUI_MESSAGE_DATA_INIT* data) {
    node->proc = data->proc;
    node->parent = NULL;
    node->prev = NULL;
    node->next = NULL;
    node->first = NULL;
    node->last = NULL;
    node->position.left = 0;
    node->position.right = 0;
    node->position.top = 0;
    node->position.bottom = 0;
    node->data = data->data;
    node->heap_allocated = data->heap_allocated;
    node->visible = 1;
    node->enabled = 1;
    node->highlighted = 0;
    node->focused = 0;
    node->pressed = 0;
    node->selected = 0;
    node->active = 0;
    return ALGUI_RESULT_OK;
}


//node cleanup
static ALGUI_RESULT node_cleanup(ALGUI_NODE* node) {
    //if the node is still a child, remove it from its parent
    if (node->parent) {
        algui_remove_child(node->parent, node);
    }

    //remove and cleanup the children
    for (ALGUI_NODE* child = node->first; child; ) {
        ALGUI_NODE* next = child->next;
        algui_cleanup_node(child);
        child = next;
    }

    //free node if heap allocated
    if (node->heap_allocated) {
        free(node);
    }

    return ALGUI_RESULT_OK;
}


//insert child
static ALGUI_RESULT node_insert_child(ALGUI_NODE* node, ALGUI_MESSAGE_DATA_INSERT_CHILD* data) {
    //check the data
    if (!data) {
        return ALGUI_RESULT_ERROR_NULL_DATA;
    }

    //check the child
    ALGUI_NODE* child = data->child;
    if (!child) {
        return ALGUI_RESULT_ERROR_NULL_CHILD;
    }

    //check the child parent
    if (child->parent) {
        return ALGUI_RESULT_ERROR_CHILD_INVALID_PARENT;
    }

    //find next sibling from z-order
    ALGUI_NODE* next = algui_find_child_at_z_order(node, data->z_order);

    //connect the child to the ui tree
    child->parent = node;
    if (next) {
        child->prev = next->prev;
        child->next = next;
        if (next->prev) {
            next->prev->next = child;
        }
        else {
            node->first = child;
        }
        next->prev = child;
    }
    else {
        child->prev = node->last;
        if (node->last) {
            node->last->next = child;
        }
        else {
            node->first = child;
        }
        node->last = child;
    }

    return ALGUI_RESULT_OK;
}


//remove child
static ALGUI_RESULT node_remove_child(ALGUI_NODE* node, ALGUI_MESSAGE_DATA_REMOVE_CHILD* data) {
    //check the child
    ALGUI_NODE* child = data->child;
    if (!child) {
        return ALGUI_RESULT_ERROR_NULL_CHILD;
    }

    //check the child parent
    if (child->parent != node) {
        return ALGUI_RESULT_ERROR_CHILD_INVALID_PARENT;
    }

    //disconnect the child from the ui tree
    child->parent = NULL;
    if (child->prev) {
        child->prev->next = child->next;
        child->prev = NULL;
    }
    else {
        node->first = child->next;
    }
    if (child->next) {
        child->next->prev = child->prev;
        child->next = NULL;
    }
    else {
        node->last = child->prev;
    }

    return ALGUI_RESULT_OK;
}


//computes the visual state of an object from root
static void compute_visual_state_from_root(ALGUI_NODE* node, ALGUI_MESSAGE_DATA_PAINT* state) {
    //init the state from node
    state->position.left = node->position.left;
    state->position.top = node->position.top;
    state->enabled = node->enabled;
    state->focused = node->focused;
    state->highlighted = node->highlighted;
    state->pressed = node->pressed;
    state->selected = node->selected;
    state->active = node->active;

    //compute the state based on ancestors
    for (ALGUI_NODE* ancestor = node->parent; ancestor; ancestor = ancestor->parent) {
        state->position.left += ancestor->position.left;
        state->position.top += ancestor->position.top;
        state->enabled = state->enabled && ancestor->enabled;
        state->focused = state->focused || ancestor->focused;
        state->highlighted = state->highlighted || ancestor->highlighted;
        state->pressed = state->pressed || ancestor->pressed;
        state->selected = state->selected || ancestor->selected;
        state->active = state->active || ancestor->active;
    }

    //finalize the size
    const float w = node->position.right - node->position.left;
    const float h = node->position.bottom - node->position.top;
    state->position.right = state->position.left + w;
    state->position.bottom = state->position.top + h;
}


//computes the visual state from data
static void compute_visual_state_from_data(ALGUI_NODE* node, const ALGUI_MESSAGE_DATA_PAINT* data, ALGUI_MESSAGE_DATA_PAINT* state) {
    //compute the state
    state->position.left = data->position.left + node->position.left;
    state->position.top = data->position.top + node->position.top;
    state->enabled = node->enabled && data->enabled;
    state->focused = node->focused || data->focused;
    state->highlighted = node->highlighted || data->highlighted;
    state->pressed = node->pressed || data->pressed;
    state->selected = node->selected || data->selected;
    state->active = node->active || data->active;

    //finalize the size
    const float w = node->position.right - node->position.left;
    const float h = node->position.bottom - node->position.top;
    state->position.right = state->position.left + w;
    state->position.bottom = state->position.top + h;
}


//paint node
static ALGUI_RESULT node_paint(ALGUI_NODE* node, ALGUI_MESSAGE_DATA_PAINT* data) {
    //if node is not visible, do nothing
    if (!node->visible) {
        return ALGUI_RESULT_OK;
    }

    //compute the visual state
    ALGUI_MESSAGE_DATA_PAINT visual_state;
    if (data) {
        compute_visual_state_from_data(node, data, &visual_state);
    }
    else {
        compute_visual_state_from_root(node, &visual_state);
    }

    //paint the background
    node->proc(node, ALGUI_MESSAGE_PAINT_BACKGROUND, &visual_state);

    //paint the children
    if (node->first) {
        for (ALGUI_NODE* child = node->first; child; ) {
            ALGUI_NODE* next = child->next;
            algui_send_message(child, ALGUI_MESSAGE_PAINT, &visual_state);
            child = next;
        }
    }

    //paint the foreground
    node->proc(node, ALGUI_MESSAGE_PAINT_FOREGROUND, &visual_state);

    return ALGUI_RESULT_OK;
}


static ALGUI_RESULT node_do_layout(ALGUI_NODE* node) {
    for (ALGUI_NODE* child = node->first; child; ) {
        ALGUI_NODE* next = child->next;
        algui_send_message(child, ALGUI_MESSAGE_DO_LAYOUT, NULL);
        child = next;
    }
    return ALGUI_RESULT_OK;
}


//default proc
ALGUI_RESULT algui_node_proc(ALGUI_NODE* node, int id, void* data) {
    switch (id) {
        case ALGUI_MESSAGE_INIT:
            return node_init(node, (ALGUI_MESSAGE_DATA_INIT*)data);

        case ALGUI_MESSAGE_CLEANUP:
            return node_cleanup(node);

        case ALGUI_MESSAGE_INSERT_CHILD:
            return node_insert_child(node, (ALGUI_MESSAGE_DATA_INSERT_CHILD*)data);

        case ALGUI_MESSAGE_REMOVE_CHILD:
            return node_remove_child(node, (ALGUI_MESSAGE_DATA_REMOVE_CHILD*)data);

        case ALGUI_MESSAGE_PAINT:
            return node_paint(node, (ALGUI_MESSAGE_DATA_PAINT*)data);

        case ALGUI_MESSAGE_DO_LAYOUT:
            return node_do_layout(node);
    }

    return ALGUI_RESULT_UNKNOWN;
}


//send message to node
ALGUI_RESULT algui_send_message(ALGUI_NODE* node, int id, void* data) {
    //check the node
    if (!node) {
        return ALGUI_RESULT_ERROR_NULL_NODE;
    }

    //check the node proc
    if (!node->proc) {
        return ALGUI_RESULT_ERROR_NULL_NODE_PROC;
    }

    //invoke the proc
    return node->proc(node, id, data);
}


//send message to children
ALGUI_RESULT algui_send_message_to_children(ALGUI_NODE* node, int id, void* data, ALGUI_RESULT results[]) {
    //check the node
    if (!node) {
        return ALGUI_RESULT_ERROR_NULL_NODE;
    }

    //set results
    if (results) {
        unsigned index = 0;
        for (ALGUI_NODE* child = node->first; child; ) {
            ALGUI_NODE* next = child->next;
            results[index++] = algui_send_message(child, id, data);
            child = next;
        }
    }

    //else ignore results
    else {
        for (ALGUI_NODE* child = node->first; child; ) {
            ALGUI_NODE* next = child->next;
            algui_send_message(child, id, data);
            child = next;
        }
    }

    return ALGUI_RESULT_OK;
}


//initialize a node
ALGUI_RESULT algui_init_node(ALGUI_NODE* node, ALGUI_NODE_MESSAGE_PROC proc, unsigned heap_allocated, void* data) {
    //check the node
    if (!node) {
        return ALGUI_RESULT_ERROR_NULL_NODE;
    }

    //set the default proc if given proc is null
    if (!proc) {
        proc = algui_node_proc;
    }

    //prepare the message
    ALGUI_MESSAGE_DATA_INIT initData;
    initData.proc = proc;
    initData.heap_allocated = heap_allocated;
    initData.data = data;

    //invoke the proc
    return proc(node, ALGUI_MESSAGE_INIT, &initData);
}


//cleanup a node
ALGUI_RESULT algui_cleanup_node(ALGUI_NODE* node) {
    return algui_send_message(node, ALGUI_MESSAGE_CLEANUP, NULL);
}


//Sends the insert child message to the node.
ALGUI_RESULT algui_insert_child(ALGUI_NODE* node, ALGUI_NODE* child, unsigned z_order) {
    ALGUI_MESSAGE_DATA_INSERT_CHILD data;
    data.child = child;
    data.z_order = z_order;
    return algui_send_message(node, ALGUI_MESSAGE_INSERT_CHILD, &data);
}


//Sends the remove child message to the node.
ALGUI_RESULT algui_remove_child(ALGUI_NODE* node, ALGUI_NODE* child) {
    ALGUI_MESSAGE_DATA_REMOVE_CHILD data;
    data.child = child;
    return algui_send_message(node, ALGUI_MESSAGE_REMOVE_CHILD, &data);
}


//paint a node
ALGUI_RESULT algui_paint_node(ALGUI_NODE* node, ALGUI_MESSAGE_DATA_PAINT* data) {
    return algui_send_message(node, ALGUI_MESSAGE_PAINT, data);
}


//sends the layout message
ALGUI_RESULT algui_do_layout(ALGUI_NODE* node) {
    return algui_send_message(node, ALGUI_MESSAGE_DO_LAYOUT, NULL);
}


//find child at z-order
ALGUI_NODE* algui_find_child_at_z_order(ALGUI_NODE* node, unsigned z_order) {
    //check the node
    if (!node) {
        return NULL;
    }

    //if z-order is negative, then return NULL
    if ((int)z_order < 0) {
        return NULL;
    }

    //iterate children in order to reach the appopriate child
    for (ALGUI_NODE* child = node->first; child; child = child->next, --z_order) {
        if (z_order == 0) {
            return child;
        }
    }

    //not found
    return NULL;
}
