#ifndef ALGUI_NODE_H
#define ALGUI_NODE_H


/**
 * Rectangle.
 */
typedef struct ALGUI_RECT {
    ///left coordinate.
    float left;

    ///top coordinate.
    float top;

    ///right coordinate.
    float right;

    ///bottom coordinate.
    float bottom;
} ALGUI_RECT;


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
     * The default implementation computes the visual state of a node,
     * then paints the node's background, then paints the children, then paints the node's foreground.
     */
    ALGUI_MESSAGE_PAINT,

    /**
     * The background of a node is to be painted.
     * The default implementation does nothing.
     * Invoked before the children are painted.
     */
    ALGUI_MESSAGE_PAINT_BACKGROUND,

    /**
     * Paints the children.
     * Each child receives a paint message with the message data containing the appropriate child state.
     */
    ALGUI_MESSAGE_PAINT_CHILDREN,

    /**
     * The foreground of a node is to be painted.
     * The default implementation does nothing.
     * Invoked after the children are painted.
     */
     ALGUI_MESSAGE_PAINT_FOREGROUND,
} ALGUI_MESSAGE;


/**
 * Operation/message result.
 */
typedef enum ALGUI_RESULT {
    ///message not processed.
    ALGUI_RESULT_UNKNOWN,

    ///no error occured.
    ALGUI_RESULT_OK,

    ///node was null.
    ALGUI_RESULT_ERROR_NULL_NODE,

    ///node proc was null.
    ALGUI_RESULT_ERROR_NULL_NODE_PROC,

    ///the data argument is null.
    ALGUI_RESULT_ERROR_NULL_DATA,

    ///the child is null.
    ALGUI_RESULT_ERROR_NULL_CHILD,

    ///operation could not be completed because because the child 's parent is invalid (not null on insert or different than parent on remove).
    ALGUI_RESULT_ERROR_CHILD_INVALID_PARENT,

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

    ///if node is allocated on the heap
    unsigned heap_allocated : 1;

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
ALGUI_RESULT algui_send_message(ALGUI_NODE* node, int id, void* data);


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
ALGUI_RESULT algui_insert_child(ALGUI_NODE* node, ALGUI_NODE* child, unsigned z_order);


/**
 * Sends the remove child message to the node.
 * @param node target node.
 * @param child child node.
 * @return result of operation.
 */
ALGUI_RESULT algui_remove_child(ALGUI_NODE* node, ALGUI_NODE* child);


/**
 * Sends the paint message to the node.
 * @param node target node.
 * @param data paint data; if not given, an internal structure will be used.
 * @return result of operation.
 */
ALGUI_RESULT algui_paint_node(ALGUI_NODE* node, ALGUI_MESSAGE_DATA_PAINT* data);


/**
 * Locates the child at the given z-order.
 * @param node node to get a child of.
 * @param z_order z-order.
 * @return child at the given z-order or null if none exists.
 */
ALGUI_NODE* algui_find_child_at_z_order(ALGUI_NODE* node, unsigned z_order);


#endif //ALGUI_NODE_H
