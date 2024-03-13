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
     * It is different from the paint message: the draw message does checks and preparations for the paint message.
     * The paint message is used to simply paint the screen using Allegro.
     */
    ALGUI_MESSAGE_DRAW,

    /**
     * The paint message.
     * It is different from the draw message: the draw message does checks and preparations for the paint message.
     * The paint message is used to simply to paint the screen using Allegro.
     */
    ALGUI_MESSAGE_PAINT,

    /**
     * Sets the geometry of a node.
     */
    ALGUI_MESSAGE_SET_GEOMETRY,

    /**
     * Initialize a node's size.
     * Sent to a node to compute its size based on its content.
     */
    ALGUI_MESSAGE_INIT_SIZE,

    /**
     * Position children according to preferred layout.
     * Send to a node to allow it to position its children according to some layout algorithm.
     * The message data point to an ALGUI_LAYOUT_OPERATION structure,
     * which contains the interior size of a node, as scaled by its horizontal and vertical scaling parameters.
     */
    ALGUI_MESSAGE_DO_LAYOUT,

    /**
     * Shows or hides a node.
     */
    ALGUI_MESSAGE_SET_VISIBLE,

    /**
     * Enable or disable a node.
     */
    ALGUI_MESSAGE_SET_ENABLED,

    /**
     * Set a node as highlighted or not highlighted.
     */
    ALGUI_MESSAGE_SET_HIGHLIGHTED,

    /**
     * Set a node as pressed or not pressed.
     */
    ALGUI_MESSAGE_SET_PRESSED,

    /**
     * Set a node as selected or not selected.
     */
    ALGUI_MESSAGE_SET_SELECTED,

    /**
     * Set a node as active or not active.
     */
    ALGUI_MESSAGE_SET_ACTIVE,

    /**
     * Set a node to be in error status or not in error status.
     */
    ALGUI_MESSAGE_SET_ERROR,

    /**
     * Sets the input focus to a node.
     */
    ALGUI_MESSAGE_GET_FOCUS,

    /**
     * Removes the input focus from a node.
     */
    ALGUI_MESSAGE_LOSE_FOCUS,

    /**
     * The node received the input focus.
     */
    ALGUI_MESSAGE_GOT_FOCUS,

    /**
     * The node lost the input focus.
     * Sent to a node when it loses the focus if it becomes disabled,
     * or when blurred, 
     * or when destroyed and has the input focus.
     */
    ALGUI_MESSAGE_LOST_FOCUS,

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
     * If false (the default), the node can paint outside of its area, restricted only by the clipping of its ancestors
     * and the requested clip area.
     */
    int clipped : 1;

    /**
     * Flag to indicate that the node needs updating.
     */
    int dirty : 1;

    /**
     * Flag to indicate that a descentant node needs updating.
     */
    int tree_dirty : 1;
};


/**
 * Message data for insert node, remove node and change node z-order messages.
 */
typedef struct ALGUI_TREE_OPERATION {
    ///child to be inserted/removed.
    ALGUI_NODE* child;

    ///z-order.
    int z_order;
} ALGUI_TREE_OPERATION;


/**
 * Message data for drawing a node tree.
 */
typedef struct ALGUI_DRAW_OPERATION {
    ///left clip coordinate.
    int clip_x1;

    ///top clip coordinate.
    int clip_y1;

    ///right clip coordinate.
    int clip_x2;

    ///bottom clip coordinate.
    int clip_y2;
} ALGUI_DRAW_OPERATION;


/**
 * Data sent for ALGUI_MESSAGE_SET_GEOMETRY message.
 */
typedef struct ALGUI_GEOMETRY_OPERATION {
    ///left side coordinate relative to parent.
    float x;

    ///top side coordinate relative to parent.
    float y;

    ///width.
    float width;

    ///height.
    float height;
} ALGUI_GEOMETRY_OPERATION;


/**
 * Data sent for ALGUI_MESSAGE_INIT_SIZE and ALGUI_MESSAGE_DO_LAYOUT.
 */
typedef struct ALGUI_LAYOUT_OPERATION {
    ///interior width of a node, i.e. the node's width multiplied by its horizontal scaling.
    float width;

    ///interior height of a node, i.e. the node's width multiplied by its vertical scaling.
    float height;
} ALGUI_LAYOUT_OPERATION;


/**
 * Data sent for setting enabled, highlighted, pressed, selected, active and error states.
 */
typedef struct ALGUI_STATE_OPERATION {
    ///the current state value.
    int state;
} ALGUI_STATE_OPERATION;


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
 *      It inserts a child node at a specific z-order. The message data should be of type ALGUI_TREE_OPERATION.
 * 
 *  4) ALGUI_MESSAGE_SET_NODE_Z_ORDER:
 *      It modifies a child node's z-order. The message data should be of type ALGUI_TREE_OPERATION.
 *
 *  5) ALGUI_MESSAGE_REMOVE_NODE:
 *      It removes a child node. The message data should be of type ALGUI_TREE_OPERATION.
 * 
 *  6) ALGUI_MESSAGE_DRAW:
 *      Handles drawing of a node. The message data should be of type ALGUI_DRAW_OPERATION.
 *      a) sets the clipping rectangle, if needed.
 *      b) sends the paint message to the node.
 *      c) draws the children.
 *      d) restores the clipping rectangle, if it was previously set.
 * 
 *  7) ALGUI_MESSAGE_SET_GEOMETRY:
 *      Handles updating the geometry of a node. The message data should be of type ALGUI_GEOMETRY_OPERATION.
 *      a) copies the position and size to the node structure.
 *      b) sets the node as dirty so as that it can be updated from the main loop.
 * 
 *  8) ALGUI_MESSAGE_SET_VISIBLE:
 *      Shows or hides the node. The message data should of be of type ALGUI_STATE_OPERATION.
 *
 *  9) ALGUI_MESSAGE_SET_ENABLED:
 *      Enables or disables the node. The message data should of be of type ALGUI_STATE_OPERATION.
 * 
 *  10) ALGUI_MESSAGE_SET_HIGHLIGHTED:
 *      Sets the node as highlighted or not highlighted. The message data should of be of type ALGUI_STATE_OPERATION.
 *
 *  11) ALGUI_MESSAGE_SET_PRESSED:
 *      Sets the node as pressed or not pressed. The message data should of be of type ALGUI_STATE_OPERATION.
 *
 *  11) ALGUI_MESSAGE_SET_SELECTED:
 *      Sets the node as selected or not selected. The message data should of be of type ALGUI_STATE_OPERATION.
 *
 *  12) ALGUI_MESSAGE_SET_ACTIVE:
 *      a) Sets the node as active or not active. The message data should of be of type ALGUI_STATE_OPERATION.
 *      b) Sets the ancestor as active.
 *
 *  13) ALGUI_MESSAGE_SET_ERROR:
 *      Sets the node's error status. The message data should of be of type ALGUI_STATE_OPERATION.
 * 
 *  14) ALGUI_MESSAGE_GET_FOCUS:
 *      Sets the focus to the node, if it is not diabled.
 *      a) if there is a currently focused node, then it asks the currently focused node to blur;
 *         if the currently focused node denies the blur, then the operation is interrupted and 0 is returned.
 *      b) Sets the input focus to the node.
 * 
 *  15) ALGUI_MESSAGE_LOSE_FOCUS:
 *      Removes the focus from the node, if the node had the focus.
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
 * The child node is set to dirty, so as that it can be updated from the main loop.
 * @param parent parent node.
 * @param node node to insert.
 * @param z_order the node's z-order; 
    if zero or positive, then it counts from first child to last child;
    if negative, then it counds from last child to first child.
 */
void algui_insert_node(ALGUI_NODE* parent, ALGUI_NODE* node, int z_order);


/**
 * Shortcut for inserting a node as the top child of another node.
 * Same as algui_insert_node(parent, node, -1).
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


/**
 * Sets the node as dirty, so as that it can be updated from the main loop if dirty.
 * @param node target node.
 */
void algui_set_node_dirty(ALGUI_NODE* node);


/**
 * Updates the nodes that were marked as dirty.
 * @param root root of the node tree that contains dirty nodes.
 */
void algui_update_dirty_nodes(ALGUI_NODE* root);


/**
 * Sets the position and size of a node.
 * The node receives a ALGUI_MESSAGE_SET_GEOMETRY message, and the ALGUI_GEOMETRY_OPERATION struct.
 * The node is set to dirty, so as that it can be updated from the main loop.
 * @param node target node.
 * @param x left coordinate relative to parent.
 * @param y top coordinate relative to parent.
 * @param width width.
 * @param height height.
 */
void algui_set_node_geometry(ALGUI_NODE* node, float x, float y, float width, float height);


/**
 * Sets the position of a node.
 * Same as algui_set_node_geometry(node, x, y, node->width, node->height).
 * @param node target node.
 * @param x left coordinate relative to parent.
 * @param y top coordinate relative to parent.
 */
void algui_set_node_position(ALGUI_NODE* node, float x, float y);


/**
 * Sets the size of a node.
 * Same as algui_set_node_geometry(node, node->x, node->y, width, height).
 * @param node target node.
 * @param width width.
 * @param height height.
 */
void algui_set_node_size(ALGUI_NODE* node, float width, float height);


/**
 * Allows a node and its descentants to be placed at specific positions,
 * that depend on specific layout algorithms implemented by nodes.
 * 
 * The procedure has two steps:
 * 
 * 1) all nodes receive an ALGUI_MESSAGE_INIT_SIZE, in order to allow themselves to get their optimal size.
 *    This message is sent to children first, then to parents, to allow parents to size themselves based on their children.
 * 
 * 2) all nodes receive an ALGUI_MESSAGE_DO_LAYOUT, in order to position their children to the appropriate positions
 *    according to the layout algorithms they prefer. This message is sent to parents first, then to children.
 *    The message data are of type ALGUI_LAYOUT_OPERATION.
 *
 * Nodes shall use the function algui_set_node_geometry or any related function to set the geometry of their children or their size.
 * The function algui_set_node_geometry does not recompute the global geometry of a node immediately, but sets the node to be dirty,
 * and then the global geometry of the nodes can be computed from the main loop using the function algui_update_dirty_nodes.
 * 
 * @param node root of tree to manage the layout of.
 */
void algui_manage_node_layout(ALGUI_NODE* node);


/**
 * Sets the horizontal and vertical scaling of the node.
 * The node is set to dirty.
 * @param node target node.
 * @param h_scaling the horizontal scaling factor.
 * @param v_scaling the vertical scaling factor.
 */
void algui_set_node_scaling(ALGUI_NODE* node, float h_scaling, float v_scaling);


/**
 * Sets a node's visible state.
 * Sends the ALGUI_MESSAGE_SET_VISIBLE message to the node.
 * The node is set to dirty.
 * @param node target node.
 * @param visible the new visible state.
 */
void algui_set_node_visible(ALGUI_NODE* node, int visible);


/**
 * Sets a node's enabled state.
 * Sends the ALGUI_MESSAGE_SET_ENABLED message to the node.
 * The node is set to dirty.
 * @param node target node.
 * @param enabled the new enabled state.
 */
void algui_set_node_enabled(ALGUI_NODE* node, int enabled);


/**
 * Sets a node's highlighted state.
 * Sends the ALGUI_MESSAGE_SET_HIGHLIGHTED message to the node.
 * The node is set to dirty.
 * @param node target node.
 * @param highlighted the new highlighted state.
 */
void algui_set_node_highlighted(ALGUI_NODE* node, int highlighted);


/**
 * Sets a node's pressed state.
 * Sends the ALGUI_MESSAGE_SET_PRESSED message to the node.
 * The node is set to dirty.
 * @param node target node.
 * @param pressed the new pressed state.
 */
void algui_set_node_pressed(ALGUI_NODE* node, int pressed);


/**
 * Sets a node's selected state.
 * Sends the ALGUI_MESSAGE_SET_SELECTED message to the node.
 * The node is set to dirty.
 * @param node target node.
 * @param selected the new selected state.
 */
void algui_set_node_selected(ALGUI_NODE* node, int selected);


/**
 * Sets a node's active state.
 * Sends the ALGUI_MESSAGE_SET_ACTIVE message to the node.
 * The node is set to dirty.
 * @param node target node.
 * @param active the new active state.
 */
void algui_set_node_active(ALGUI_NODE* node, int active);


/**
 * Sets a node's error state.
 * Sends the ALGUI_MESSAGE_SET_ERROR message to the node.
 * The node is set to dirty.
 * @param node target node.
 * @param error the new error state.
 */
void algui_set_node_error(ALGUI_NODE* node, int error);


/**
 * Returns the node with the focus.
 * @return the node with the focus.
 */
ALGUI_NODE* algui_get_focused_node();


/**
 * Sets the input focus to the given node.
 * The node receives an ALGUI_MESSAGE_GET_FOCUS message.
 * If another node has the focus, it is first asked to be blurred.
 * Then the focus is set to the given node.
 * The node must be enabled in order to get the focus.
 * If the node already has the focus, then nothing happens and non-zero is returned.
 * @param node node to get the input focus.
 * @return non-zero if the focus was successfully set, zero otherwise.
 */
int algui_focus_node(ALGUI_NODE* node);


/**
 * Removes the input focus from the given node.
 * The node receives an ALGUI_MESSAGE_LOSE_FOCUS message.
 * If the node already does not have the focus, then nothing happens and non-zero is returned.
 * @param node node to get the input focus.
 * @return non-zero if the focus was successfully reset, zero otherwise.
 */
int algui_blur_node(ALGUI_NODE* node);


#endif //ALGUI_NODE_H
