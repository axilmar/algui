#ifndef ALGUI_NODE_H
#define ALGUI_NODE_H


#include "allegro5/allegro.h"
#include "ALGUI_RECT.h"


/**
 * Message ids.
 */
typedef enum ALGUI_MESSAGE {
    /** 
     * Initialize a node.
     * The default implementation resets all fields of node to their default values.
     */
    ALGUI_MESSAGE_INIT,

    /**
     * Clean up a node.
     * The default implementation does the following:
     *  - removes the node from its parent, if the node is a child node.
     *  - removes and cleans up the children.
     *  - frees the node, if it is heap-allocated.
     */
    ALGUI_MESSAGE_CLEANUP,

    /**
     * Insert a child node.
     * The default implementation inserts the child into a node.
     */
    ALGUI_MESSAGE_INSERT_CHILD,

    /**
     * Remove a child node.
     * The default implementation removes the child.
     */
     ALGUI_MESSAGE_REMOVE_CHILD,

    /**
     * Paints the node.
     * The default implementation paints the children of a node.
     */
    ALGUI_MESSAGE_PAINT,

    /**
     * Set up the geometry of the node and its subnodes.
     * The default implementation passes the message to its children.
     */
    ALGUI_MESSAGE_DO_LAYOUT,

    /**
     * Dispatches an Allegro event.
     * The default implementation uses the Allegro event to dispath it 
     * to the appropriate child, based on event type.
     */
    ALGUI_MESSAGE_DISPATCH_EVENT,

    /**
     * A mouse enter event.
     * The default implementation sends the event to the appropriate child under the mouse coordinates.
     */
    ALGUI_MESSAGE_MOUSE_ENTER,

    /**
     * A mouse move event.
     * The default implementation sends the event to the appropriate child under the mouse coordinates.
     * If the mouse cursor crosses children, then the child node that lost the mouse receives the mouse leave event,
     * while the child node that got the mouse receives a mouse enter event.
     */
    ALGUI_MESSAGE_MOUSE_MOVE,

    /**
     * A mouse leave event.
     * The default implementation sends the event to the child that had the mouse.
     */
     ALGUI_MESSAGE_MOUSE_LEAVE,

    /**
     * Mouse button down event.
     * The default implementation sends the event to the child that has the mouse.
     */
    ALGUI_MESSAGE_MOUSE_BUTTON_DOWN,

    /**
     * Mouse button up event.
     * The default implementation sends the event to the child that has the mouse.
     */
    ALGUI_MESSAGE_MOUSE_BUTTON_UP,

    /**
     * Click event.
     * It is fired when the user presses and releases the mouse over a node.
     * The default implementation sends the event to the child that has the mouse.
     * If the mouse moves outside of a node, then the event is not sent.
     */
    ALGUI_MESSAGE_CLICK,

    /**
     * Sent to a node that it should receive the focus.
     * The default implementation gives the input focus to the node.
     * If another node has the focus, then it must lose the focus first
     * in order for the focus to change.
     */
    ALGUI_MESSAGE_FOCUS,

    /**
     * Sent to a node to remove focus from it.
     * The default implementation removes the focus from it,
     * if it has the focus.
     */
    ALGUI_MESSAGE_DEFOCUS,

    /**
     * Sent to a node when a descentant node receives the input focus.
     * The default implementation sends the message to the parent.
     */
    ALGUI_MESSAGE_ACTIVATE,

    /**
     * Sent to a node when a descentant node loses the input focus.
     * The default implementation sends the message to the parent.
     */
    ALGUI_MESSAGE_DEACTIVATE,

    /**
     * Sent to the node with the input focus.
     * The default implementation sends the event to the parent.
     */
    ALGUI_MESSAGE_KEY_DOWN,

    /**
     * Sent to the node with the input focus.
     * The default implementation sends the event to the parent.
     */
    ALGUI_MESSAGE_KEY_UP,

    /**
     * Sent to the node with the input focus.
     * The default implementation sends the event to the parent.
     */
    ALGUI_MESSAGE_KEY_CHAR,

    /**
     * Sent to a node from root if there is no focused node or the focused node does not process the event.
     * The default implementation sends the event to the parent.
     */
    ALGUI_MESSAGE_UNUSED_KEY_DOWN,

    /**
     * Sent to a node from root if there is no focused node or the focused node does not process the event.
     * The default implementation sends the event to the parent.
     */
    ALGUI_MESSAGE_UNUSED_KEY_UP,

    /**
     * Sent to a node from root if there is no focused node or the focused node does not process the event.
     * The default implementation sends the event to the parent.
     */
    ALGUI_MESSAGE_UNUSED_KEY_CHAR,

} ALGUI_MESSAGE;


/**
 * Operation/message result.
 */
typedef enum ALGUI_RESULT {
    ///message not processed.
    ALGUI_RESULT_UNKNOWN = -1,

    ///false.
    ALGUI_RESULT_FALSE = 0,

    ///true.
    ALGUI_RESULT_TRUE = 1,

    ///no error occured.
    ALGUI_RESULT_OK = 1,

    ///node was null.
    ALGUI_RESULT_ERROR_NULL_NODE = -2,

    ///node proc was null.
    ALGUI_RESULT_ERROR_NULL_NODE_PROC = -3,

    ///the data argument is null.
    ALGUI_RESULT_ERROR_NULL_DATA = -4,

    ///the child is null.
    ALGUI_RESULT_ERROR_NULL_CHILD = -5,

    ///operation could not be completed because because the child 's parent is invalid (not null on insert or different than parent on remove).
    ALGUI_RESULT_ERROR_CHILD_INVALID_PARENT = -6,

    ///node is disabled and cannot handle events.
    ALGUI_RESULT_ERROR_DISABLED_NODE = -7,

    ///node is invisible
    ALGUI_RESULT_ERROR_INVISIBLE_NODE = -8,

    ///root node was null.
    ALGUI_RESULT_ERROR_NULL_ROOT = -9,

    ///focused node refused to lose the focus
    ALGUI_RESULT_ERROR_CANNOT_DEFOCUS_FOCUSED_NODE = -10,

} ALGUI_RESULT;


/**
 * Message procedure type for UI nodes.
 * @param node target node.
 * @param id message id.
 * @param data message data.
 * @return depends on message.
 */
typedef ALGUI_RESULT(*ALGUI_NODE_MESSAGE_PROC)(struct ALGUI_NODE* node, int id, void* data);


/**
 * Message data for the init message.
 */
typedef struct ALGUI_MESSAGE_DATA_INIT {
    ///message procedure.
    ALGUI_NODE_MESSAGE_PROC proc;

    ///if true, then the node is allocated on the heap and freed automatically with free.
    unsigned heap_allocated;

    ///user data attached to the node.
    void* data;
} ALGUI_MESSAGE_DATA_INIT;


/**
 * Message data for the insert child message.
 */
typedef struct ALGUI_MESSAGE_DATA_INSERT_CHILD {
    ///child node to insert.
    struct ALGUI_NODE* child;

    ///z-order of child.
    unsigned z_order;
} ALGUI_MESSAGE_DATA_INSERT_CHILD;


/**
 * Message data for the remove child message.
 */
typedef struct ALGUI_MESSAGE_DATA_REMOVE_CHILD {
    ///child node to remove.
    struct ALGUI_NODE* child;
} ALGUI_MESSAGE_DATA_REMOVE_CHILD;


/**
 * Message data for the paint message.
 */
typedef struct ALGUI_MESSAGE_DATA_PAINT {
    ///screen rectangle of the node
    ALGUI_RECT position;

    ///enabled flag; may be set to false if ancestor is disabled.
    unsigned enabled : 1;

    ///highlighted flag; set if the mouse is within the node; may be set to true if ancestor is highlighted.
    unsigned highlighted : 1;

    ///focused flag; set if the node has the input focus; may be set to true if ancestor is focused.
    unsigned focused : 1;

    ///pressed flag; set if a mouse button is held down over the node; may be set to true if ancestor is pressed.
    unsigned pressed : 1;

    ///selected flag; if the node is selected; may be set to true if ancestor is selected.
    unsigned selected : 1;

    ///active flag; if the node is active; may be set to true if ancestor is active.
    unsigned active : 1;
} ALGUI_MESSAGE_DATA_PAINT;


/**
 * Event data.
 */
typedef struct ALGUI_MESSAGE_DATA_EVENT {
    //the associated allegro event.
    ALLEGRO_EVENT* event;

    ///screen rectangle of the node; valid only with mouse-related messages or other messages that node screen coordinates are needed.
    ALGUI_RECT position;
} ALGUI_MESSAGE_DATA_EVENT;


/**
 * Base struct for UI nodes.
 */
typedef struct ALGUI_NODE {
    ///message handling procedure.
    ALGUI_NODE_MESSAGE_PROC proc;

    ///parent node.
    struct ALGUI_NODE* parent;

    ///previous sibling node.
    struct ALGUI_NODE* prev;

    ///next sibling node.
    struct ALGUI_NODE* next;

    ///first child node.
    struct ALGUI_NODE* first;

    ///last child node.
    struct ALGUI_NODE* last;

    ///position.
    ALGUI_RECT position;

    ///user data.
    void* data;

    ///if it is allocated on the heap.
    unsigned heap_allocated : 1;

    ///if it has a click.
    unsigned has_click : 1;

    ///if it has the mouse.
    unsigned has_mouse : 1;

    ///visible flag
    unsigned visible : 1;

    ///enabled flag.
    unsigned enabled : 1;

    ///highlighted flag; set if the mouse is within the node.
    unsigned highlighted : 1;

    ///focused flag; set if the node has the input focus.
    unsigned focused : 1;

    ///pressed flag; set if a mouse button is held down over the node.
    unsigned pressed : 1;

    ///selected flag; if the node is selected.
    unsigned selected : 1;

    ///active flag; if the node contains the focus (but it is not focused itself).
    unsigned active : 1;
} ALGUI_NODE;


/**
 * The default node procedure.
 * For information about the default behavior, see the enum ALGUI_MESSAGE;
 * @param node target node.
 * @param id message id.
 * @param data message data.
 * @return operation result.
 */
ALGUI_RESULT algui_node_proc(ALGUI_NODE* node, int id, void* data);


/**
 * Sends a message to a node.
 * @param node target node.
 * @param id message id.
 * @param data message data.
 * @return depends on message.
 */
ALGUI_RESULT algui_send_message_to_node(ALGUI_NODE* node, int id, void* data);


/**
 * Goes through the node child list and counts the children.
 * @param node node to get the number of children.
 * @return the number of children or -1 if there was a problem (e.g. if node was null).
 */
unsigned algui_count_child_nodes(ALGUI_NODE* node);


/**
 * Sends the given message to all the children of the given node.
 * @param node parent node.
 * @param id message id.
 * @param data message data.
 * @param results optional table with message results. Must have enough room for each child of node.
 * @return result of operation.
 */
ALGUI_RESULT algui_send_message_to_child_nodes(ALGUI_NODE* node, int id, void* data, ALGUI_RESULT results[]);


/**
 * Sends an init message to the node.
 * The node's proc must have been setup.
 * @param node target node.
 * @param proc procedure to use for node; if null, the default one is used.
 * @param heap_allocated true if the node is heap-allocated, false otherwise.
 * @param data user data attached to the node.
 * @return result of operation.
 */
ALGUI_RESULT algui_init_node(ALGUI_NODE* node, ALGUI_NODE_MESSAGE_PROC proc, unsigned heap_allocated, void* data);


/**
 * Sends the cleanup message to the node.
 * @param node target node.
 * @return result of operation.
 */
ALGUI_RESULT algui_cleanup_node(ALGUI_NODE* node);


/**
 * Sends the insert child message to the node.
 * @param node target node.
 * @param child child node.
 * @param z_order z-order; 0 for first child, -1 for last child.
 * @return result of operation.
 */
ALGUI_RESULT algui_insert_child_node(ALGUI_NODE* node, ALGUI_NODE* child, unsigned z_order);


/**
 * Sends the remove child message to the node.
 * @param node target node.
 * @param child child node.
 * @return result of operation.
 */
ALGUI_RESULT algui_remove_child_node(ALGUI_NODE* node, ALGUI_NODE* child);


/**
 * Sends the paint message to the node.
 * @param node target node.
 * @param data paint data; if not given, an internal structure will be used.
 * @return result of operation.
 */
ALGUI_RESULT algui_paint_node(ALGUI_NODE* node);


/**
 * Sends the do-layout message to the node.
 * @param node target node.
 * @return result of operation.
 */
ALGUI_RESULT algui_do_node_layout(ALGUI_NODE* node);


/**
 * Locates the child at the given z-order.
 * @param node node to get a child of.
 * @param z_order z-order.
 * @return child at the given z-order or null if none exists.
 */
ALGUI_NODE* algui_find_child_node_at_z_order(ALGUI_NODE* node, unsigned z_order);


/**
 * Locates a child node at the given local coordinates.
 * @param node node to get the child node of.
 * @param x x coordinate.
 * @param y y coordinate.
 * @return the node under the given point or NULL if none found.
 */
ALGUI_NODE* algui_find_child_node_at_point(ALGUI_NODE* node, float x, float y);


/**
 * Sends the given node the given allegro event in order for the node to dispatch it
 * to itself and its children.
 * @param node target node.
 * @param event allegro event to dispatch.
 * @return result of operation.
 */
ALGUI_RESULT algui_dispatch_event(ALGUI_NODE* node, ALLEGRO_EVENT* event);


/**
 * Returns the current node that has the focus.
 * @return the current node that has the focus, or NULL if no node has the focus.
 */
ALGUI_NODE* algui_get_node_with_focus();


/**
 * Sets the focus to the given node or removes the focus from the current node.
 * @param node node to set the focus to; if null, then the focus is removed from the currently focused node.
 * @return result of operation.
 */
ALGUI_RESULT algui_set_focus_to_node(ALGUI_NODE* node);


/**
 * Checks if a node tree contains a node.
 * @param root root of the node.
 * @param node node to check if it belongs in the tree starting from root.
 * @return true if found, false otherwise.
 */
ALGUI_RESULT algui_node_tree_contains_node(ALGUI_NODE* root, ALGUI_NODE* node);


/**
 * Checks if a node belongs to an enabled node tree.
 * @param node node to check.
 * @return true if the node belongs to an enabled tree, false otherwise.
 */
ALGUI_RESULT algui_is_enabled_node_tree(ALGUI_NODE* node);


#endif //ALGUI_NODE_H
