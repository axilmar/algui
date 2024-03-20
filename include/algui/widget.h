#ifndef ALGUI_WIDGET_H
#define ALGUI_WIDGET_H


#include <stdint.h>
#include <stdarg.h>
#include "tree.h"


/**
 * ALGUI messages for widgets.
 */
enum ALG_MSG {
    ///allocate widget structure.
    ALG_MSG_MALLOC,

    ///free widget structure.
    ALG_MSG_FREE,

    ///initialize widget structure.
    ALG_MSG_INIT,

    ///cleanup widget structure.
    ALG_MSG_CLEANUP,

    ///get prop.
    ALG_MSG_GET_PROP,

    ///set prop.
    ALG_MSG_SET_PROP,

    ///paint widget.
    ALG_MSG_PAINT,

    ///if widget wants the focus.
    ALG_MSG_WANT_FOCUS,

    ///widget got the focus.
    ALG_MSG_GOT_FOCUS,

    ///descentant got the focus.
    ALG_MSG_DESC_GOT_FOCUS,

    ///lost the focus.
    ALG_MSG_LOST_FOCUS,

    ///descentant lost the focus.
    ALG_MSG_DESC_LOST_FOCUS,

    ///first available message for apps.
    ALG_MSG_USER = 10000
};


/**
 * Properties enumeration.
 */
enum ALG_PROP {
    ///null property used for terminating the property list.
    ALG_PROP_NULL,

    ///x property.
    ALG_PROP_X,

    ///y property.
    ALG_PROP_Y,

    ///width of widget.
    ALG_PROP_WIDTH,

    ///height of widget.
    ALG_PROP_HEIGHT,

    ///user data of widget.
    ALG_PROP_DATA,

    ///visible state of widget.
    ALG_PROP_VISIBLE,

    ///enabled state of widget.
    ALG_PROP_ENABLED,

    ///highlighted state of widget.
    ALG_PROP_HIGHLIGHTED,

    ///pressed state of widget.
    ALG_PROP_PRESSED,

    ///selected state of widget.
    ALG_PROP_SELECTED,

    ///active state of widget.
    ALG_PROP_ACTIVE,

    ///error state of widget.
    ALG_PROP_ERROR,

    ///focused state of widget.
    ALG_PROP_FOCUSED,

    ///first available prop id for apps.
    ALG_PROP_USER = 10000
};


/**
 * Widget procedure type.
 * @param wgt target widget.
 * @param id message id.
 * @param data message data.
 * @return depends on message.
 */
typedef uintptr_t (*ALG_WIDGET_PROC)(struct ALG_WIDGET* wgt, int id, void* data);


/**
 * Widget structure.
 */
typedef struct ALG_WIDGET {
    ///tree.
    ALG_TREE tree;

    ///procedure.
    ALG_WIDGET_PROC proc;

    ///left coordinate relative to parent.
    int x;

    ///top coordinate relative to parent.
    int y;
    
    ///width.
    int width;

    ///height.
    int height;

    ///user data.
    void* data;

    ///if widget is visible.
    int visible : 1;

    ///if widget is enabled.
    int enabled : 1;

    ///if widget is highlighted.
    int highlighted : 1;

    ///if widget is pressed.
    int pressed : 1;

    ///if widget is selected.
    int selected : 1;

    ///if widget contains the focus.
    int active : 1;

    ///if widget has an error.
    int error : 1;

    ///if widget has the focus.
    int focused : 1;
} ALG_WIDGET;


/**
 * Data for the init message.
 */
typedef struct ALG_DATA_INIT {
    ///widget proc.
    ALG_WIDGET_PROC proc;
} ALG_DATA_INIT;


/**
 * Data for the property messages.
 */
typedef struct ALG_DATA_PROP {
    ///id of the prop.
    int id;

    ///pointer to the value of the prop.
    void* value;
} ALG_DATA_PROP;


/**
 * Data for the paint message.
 */
typedef struct ALG_DATA_PAINT {
    ///left coordinate of the widget, relative to the target bitmap.
    int x1;

    ///top coordinate of the widget, relative to the target bitmap.
    int y1;

    ///right coordinate of the widget, relative to the target bitmap.
    int x2;

    ///bottom coordinate of the widget, relative to the target bitmap.
    int y2;

    ///if the widget belongs to an enabled widget tree.
    int enabled : 1;

    ///if the widget belongs to a highlighted widget tree.
    int highlighted : 1;

    ///if the widget belongs to a pressed widget tree.
    int pressed : 1;

    ///if the widget belongs to a selected widget tree.
    int selected : 1;

    ///if the widget belongs to an active widget tree.
    int active : 1;

    ///if the widget belongs to an error widget tree.
    int error : 1;

    ///if the widget belongs to a focused widget tree.
    int focused : 1;
} ALG_DATA_PAINT;


/**
 * The default widget proc.
 * It implements the following messages:
 *  -init: initializes a widget structure.
 *  -cleanup: removes the widget from its parent and destroys its children.
 *  -free: frees the widget structure using the free() function.
 *  -get prop: returns one of the widget properties.
 *  -set prop: sets one of the widget properties.
 *  - want focus: returns 1 (true).
 *  - descentant got/lost focus: sets/resets the active flag for the widget.
 * @param wgt target widget.
 * @param id message id.
 * @param data message data.
 * @return depends on message.
 */
uintptr_t alg_widget_proc(ALG_WIDGET* wgt, int id, void* data);


/**
 * Sends message to widget.
 * @param wgt target widget.
 * @param id message id.
 * @param data message data.
 * @return depends on message.
 */
uintptr_t alg_send_message(ALG_WIDGET* wgt, int id, void* data);


/**
 * Creates a widget.
 * The widget proc receives a malloc message, and then an init message, then a series of set prop messages, one for each property.
 * @param proc widget proc.
 * @param null-terminated list of property ids and value pairs.
 * @return pointer to the widget.
 */
ALG_WIDGET* alg_create_widget(ALG_WIDGET_PROC proc, ...);


/**
 * Creates a widget, then inserts it into the given parent.
 * @param parent parent widget.
 * @param proc widget proc.
 * @param null-terminated list of property ids and value pairs.
 * @return pointer to the widget.
 */
ALG_WIDGET* alg_create_child_widget(ALG_WIDGET* parent, ALG_WIDGET_PROC proc, ...);


/**
 * Removes the widget from its parent, then destroys it.
 * The widget receives a cleanup, then a free message.
 * @param wgt target widget.
 */
void alg_destroy_widget(ALG_WIDGET* wgt);


/**
 * Retrieves the properties of a widget.
 * The widget receives a get prop message for each property.
 * @param wgt widget to get the properties.
 * @param null-terminated list of property ids and value pointer pairs.
 */
void alg_get_widget_props(ALG_WIDGET* wgt, ...);


/**
 * Sets the properties of a widget.
 * The widget receives a set prop message for each property.
 * @param wgt widget to aet the properties.
 * @param null-terminated list of property ids and value pairs.
 */
void alg_set_widget_props(ALG_WIDGET* wgt, ...);


/**
 * Returns the root widget of the widget tree that the given widget is member of.
 * @param wgt widget to get the root of.
 * @return the root of the given widget tree.
 */
ALG_WIDGET* alg_get_root_widget(ALG_WIDGET* wgt);


/**
 * Returns the parent widget.
 * @param wgt widget to get the relative of.
 * @return the specific relative to the given widget.
 */
ALG_WIDGET* alg_get_parent_widget(ALG_WIDGET* wgt);


/**
 * Returns the prev_sibling widget.
 * @param wgt widget to get the relative of.
 * @return the specific relative to the given widget.
 */
ALG_WIDGET* alg_get_prev_sibling_widget(ALG_WIDGET* wgt);


/**
 * Returns the next sibling widget.
 * @param wgt widget to get the relative of.
 * @return the specific relative to the given widget.
 */
ALG_WIDGET* alg_get_next_sibling_widget(ALG_WIDGET* wgt);


/**
 * Returns the first child widget.
 * @param wgt widget to get the relative of.
 * @return the specific relative to the given widget.
 */
ALG_WIDGET* alg_get_first_child_widget(ALG_WIDGET* wgt);


/**
 * Returns the last child widget.
 * @param wgt widget to get the relative of.
 * @return the specific relative to the given widget.
 */
ALG_WIDGET* alg_get_last_child_widget(ALG_WIDGET* wgt);


/**
 * Inserts a widget into another widget.
 * @param parent parent widget.
 * @paraqm child child widget.
 * @param z_order z-order of the widget; same as tree position.
 */
void alg_insert_widget(ALG_WIDGET* parent, ALG_WIDGET* child, int z_order);


/**
 * Removes a widget from its parent, if there is one.
 * @param wgt widget to remove from its parent.
 */
void alg_remove_widget(ALG_WIDGET* wgt);


/**
 * Paints a widget.
 * The widget receives a paint message before painting the children.
 * The data point to a paint data structure, which contains the painting state of the widget.
 * @param wgt wgt to paint.
 */
void alg_paint_widget(ALG_WIDGET* wgt);


/**
 * Returns the focused widget.
 * @return the focused widget.
 */
ALG_WIDGET* alg_get_focused_widget();


/**
 * Set the focus to/remove the focus from the given widget.
 * Sets the focused property of the widget.
 * @param wgt widget.
 * @param focused the focused flag.
 * @return non-zero if the widget lost/got the focus, zero otherwise.
 */
int alg_focus_widget(ALG_WIDGET* wgt, int focused);


#endif //ALGUI_WIDGET_H
