#ifndef ALGUI_NODE_H
#define ALGUI_NODE_H


#include <allegro5/allegro.h>


/**
 * Default messages supported by the library.
 */
enum ALGUI_MESSAGE_ID {
    /**
     * The cleanup message.
     */
    ALGUI_MESSAGE_CLEANUP,

    /**
     * The update message.
     */
    ALGUI_MESSAGE_UPDATE,

    /**
     * The insert node message.
     */
    ALGUI_MESSAGE_INSERT_NODE,

    /**
     * The change node z-order message.
     */
     ALGUI_MESSAGE_SET_NODE_Z_ORDER,

    /**
     * The remove node message.
     */
    ALGUI_MESSAGE_REMOVE_NODE,

    /**
     * The draw message.
     */
    ALGUI_MESSAGE_DRAW,

    /**
     * The paint message.
     */
    ALGUI_MESSAGE_PAINT,

    /**
     * First available message for external libraries.
     */
    ALGUI_MESSAGE_USER = 10000,
};


/**
 * Type definition for node.
 */
typedef struct ALGUI_NODE ALGUI_NODE;


/**
 * Node procedure.
 * @param node target node.
 * @param msg_id id of message.
 * @param msg_data data of message.
 * @return depends on message.
 */
typedef int (*ALGUI_NODE_PROC)(ALGUI_NODE* node, int msg_id, void *msg_data);


/**
 * Node structure.
 */
struct ALGUI_NODE {
    ///procedure of node.
    ALGUI_NODE_PROC proc;

    ///parent node.
    ALGUI_NODE* parent;

    ///Previous sibling node.
    ALGUI_NODE* prev_sibling;

    ///next sibling node.
    ALGUI_NODE* next_sibling;

    ///first child node.
    ALGUI_NODE* first_child;

    ///last child node.
    ALGUI_NODE* last_child;

    ///left coordinate relative to parent.
    float x;

    ///top coordinate relative to parent.
    float y;

    ///width of node.
    float width;

    ///height of node.
    float height;

    ///left coordinate relative to the screen.
    float x1;

    ///top coordinate relative to the screen.
    float y1;

    ///right coordinate relative to the screen.
    float x2;

    ///bottom coordinate relative to the screen.
    float y2;

    ///user data.
    void* data;

    ///horizontal scaling of coordinates relative to parent.
    float h_scaling;

    ///vertical scaling of coordinates relative to parent.
    float v_scaling;

    ///absolute horizontal scaling of coordinates as defined by position in tree.
    float tree_h_scaling;

    ///absolute vertical scaling of coordinates as defined by position in tree.
    float tree_v_scaling;

    ///visible state.
    int visible : 1;

    ///enabled state.
    int enabled : 1;

    ///highlighted state.
    int highlighted : 1;

    ///pressed state.
    int pressed : 1;

    ///selected state.
    int selected : 1;

    ///active state (a node is active when a descentant has the input focus).
    int active : 1;

    ///error state.
    int error : 1;

    ///focused state.
    int focused : 1;

    ///if node and all of its ancestors are visible.
    int tree_visible : 1;

    ///if node and all of its ancestors are enabled.
    int tree_enabled : 1;

    ///if node or any of its ancestors are highlighted.
    int tree_highlighted : 1;

    ///if node or any of its ancestors is pressed.
    int tree_pressed : 1;

    ///if node or any of its ancestors is selected.
    int tree_selected : 1;

    ///if node or any of its ancestors is active.
    int tree_active : 1;

    ///if node or any of its ancestors indicates an error.
    int tree_error : 1;

    ///if node or any of its ancestors is focused.
    int tree_focused : 1;

    ///if true, the node is freed upon cleanup using free().
    int heap_allocated : 1;

    /** 
     * if true, the paint area for the node is restricted to the node's screen rectangle;
     * If false (the default), the node can paint outside of its area, restricted only by the clipping of its ancestors.
     */
    int clipped : 1;
};


/**
 * Message data for insert node, remove node and change node z-order messages.
 */
typedef struct ALGUI_NODE_TREE_OPERATION {
    ///child to be inserted/removed.
    ALGUI_NODE* child;

    ///z-order.
    int z_order;
} ALGUI_NODE_TREE_OPERATION;


/**
 * Message data for drawing a node tree.
 */
typedef struct ALGUI_NODE_DRAW_OPERATION {
    ///left clip coordinate.
    int clip_x1;

    ///top clip coordinate.
    int clip_y1;

    ///right clip coordinate.
    int clip_x2;

    ///bottom clip coordinate.
    int clip_y2;
} ALGUI_NODE_DRAW_OPERATION;


/**
 * The default node procedure.
 * 
 * All other node implementations should invoke this function
 * in order to reuse the default message implementation.
 * 
 * By default, it handles the following messages:
 * 
 *  1) ALGUI_MESSAGE_CLEANUP:
 *      a) sends the message to children, from last to first order.
 *      b) frees the node, if it is heap allocated.
 * 
 *  2) ALGUI_MESSAGE_UPDATE:
 *      a) it computes the tree scaling of the node (members tree_h_scaling, tree_v_scaling).
 *      b) it computes the screen rectangle of the node (members x1, y1, x2, y2), scaled by tree scaling.
 *      c) it computes the tree visual state of the node (members tree_visible, tree_enabled, tree_highlighted, tree_pressed, tree_selected, tree_active, tree_error, tree_focused).
 *      d) sends the message to children.
 * 
 *  3) ALGUI_MESSAGE_INSERT_NODE:
 *      It inserts a child node at a specific z-order. Message data should be of type ALGUI_NODE_TREE_OPERATION.
 * 
 *  4) ALGUI_MESSAGE_SET_NODE_Z_ORDER:
 *      It modifies a child node's z-order. Message data should be of type ALGUI_NODE_TREE_OPERATION.
 *
 *  5) ALGUI_MESSAGE_REMOVE_NODE:
 *      It removes a child node. Message data should be of type ALGUI_NODE_TREE_OPERATION.
 * 
 *  6) ALGUI_MESSAGE_DRAW:
 *      a) sets the clipping rectangle, if needed.
 *      b) sends the paint message to the node.
 *      c) draws the children.
 *      d) restores the clipping rectangle, if it was previously set.
 * 
 * @param node target node.
 * @param msg_id id of message.
 * @param msg_data message data.
 * @return depends on message.
 */
int algui_default_node_proc(ALGUI_NODE* node, int msg_id, void* msg_data);


/**
 * Invokes the node's procedure for the given message.
 * @param node target node.
 * @param msg_id id of message.
 * @param msg_data message data.
 * @return depends on message.
 */
int algui_do_node_message(ALGUI_NODE* node, int msg_id, void* msg_data);


/**
 * Initializes a node.
 * @param node node to initialize.
 * @param heap_allocated if true, the node is freed when cleaned up.
 */
void algui_init_node(ALGUI_NODE* node, int heap_allocated);


/**
 * Sends the cleanup message to the node.
 * @param node node to cleanup.
 */
void algui_cleanup_node(ALGUI_NODE* node);


/**
 * Sends the update message to the node.
 * 
 * It should be called after:
 * 
 *  1) updating a node's geometry (members x, y, width, height).
 *  2) updating a node's visual state (members visible, enabled, highlighted, pressed, selected, active, error).
 *  3) inserting or removing a node from another node.
 *  4) any other modification of a node-derived structure.
 * 
 * @param node target node.
 */
void algui_update_node(ALGUI_NODE* node);


/**
 * Sends the insert node message to parent.
 * @param parent parent node.
 * @param node node to insert.
 * @param z_order the node's z-order; 
    if zero or positive, then it counts from first child to last child;
    if negative, then it counds from last child to first child.
 */
void algui_insert_node(ALGUI_NODE* parent, ALGUI_NODE* node, int z_order);


/**
 * Shortcut for inserting a node as the top child of another node.
 * Equal to algui_insert_node(parent, node, -1).
 * @param parent parent node.
 * @param node node to insert.
 */
void algui_add_node(ALGUI_NODE* parent, ALGUI_NODE* node);


/**
 * Changes the z-order of a node within its parent, if it has a parent.
 * The parent receives a change z-order message.
 * @param node target node.
 * @param z_order the new z-order of the node.
 */
void algui_set_node_z_order(ALGUI_NODE* node, int z_order);


/**
 * Removes a node from its parent, if it has a parent.
 * The parent receives a remove node message.
 * @param node node to remove.
 */
void algui_remove_node(ALGUI_NODE* node);


/**
 * Draws a node and its children clipped within the given clipping coordinates by sending a draw message to the node.
 * The clipping coordinates are relative to the screen.
 * The node receives a paint message after the screen is clipped as needed.
 * On return, the screen clipping is restored.
 * @param node target node.
 * @param clip_x left coordinate of clipping area.
 * @param clip_y top coordinate of clipping area.
 * @param clip_w width of clipping area.
 * @param clip_h height of clipping area.
 */
void algui_draw_node_clipped(ALGUI_NODE* node, int clip_x, int clip_y, int clip_w, int clip_h);


/**
 * Draws the given node, using as clipping the current clipping rectangle.
 * @param node target node.
 */
void algui_draw_node(ALGUI_NODE* node);


#endif //ALGUI_NODE_H
