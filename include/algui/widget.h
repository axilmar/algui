#ifndef ALG_WIDGET_H
#define ALG_WIDGET_H


#include "props.h"
#include "msgs.h"
#include "tree.h"


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

    ///id.
    uintptr_t id;

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
void alg_get_widget_properties(ALG_WIDGET* wgt, ...);


/**
 * Sets the properties of a widget.
 * The widget receives a set prop message for each property.
 * @param wgt widget to aet the properties.
 * @param null-terminated list of property ids and value pairs.
 */
void alg_set_widget_properties(ALG_WIDGET* wgt, ...);


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
 * Returns the current proc of the widget.
 * @param wgt widget to thet the proc of.
 * @return the widget's proc.
 */
ALG_WIDGET_PROC alg_get_widget_proc(ALG_WIDGET* wgt);


/**
 * Sets the proc of a widget.
 * @param wgt widget to set the proc of.
 * @param proc the widget proc.
 */
void alg_set_widget_proc(ALG_WIDGET* wgt, ALG_WIDGET_PROC proc);


/**
 * Returns the x coordinate of a widget.
 * @param wgt widget to get the x coordinate of.
 * @return the x coordinate of the widget.
 */
int alg_get_widget_x(ALG_WIDGET* wgt);


/**
 * Sets the x coordinate of a widget.
 * @param wgt widget to set the x coordinate of.
 * @param x the x coordinate of a widget.
 */
void alg_set_widget_x(ALG_WIDGET* wgt, int x);


/**
 * Returns the y coordinate of a widget.
 * @param wgt widget to get the y coordinate of.
 * @return the y coordinate of the widget.
 */
int alg_get_widget_y(ALG_WIDGET* wgt);


/**
 * Sets the y coordinate of a widget.
 * @param wgt widget to set the y coordinate of.
 * @param y the y coordinate of a widget.
 */
void alg_set_widget_y(ALG_WIDGET* wgt, int y);


/**
 * Returns the width of a widget.
 * @param wgt widget to get the width of.
 * @return the width of the widget.
 */
int alg_get_widget_width(ALG_WIDGET* wgt);


/**
 * Sets the width of a widget.
 * @param wgt widget to set the width of.
 * @param width the width of a widget.
 */
void alg_set_widget_width(ALG_WIDGET* wgt, int width);


/**
 * Returns the height of a widget.
 * @param wgt widget to get the height of.
 * @return the height of the widget.
 */
int alg_get_widget_height(ALG_WIDGET* wgt);


/**
 * Sets the height of a widget.
 * @param wgt widget to set the height of.
 * @param height the height of a widget.
 */
void alg_set_widget_height(ALG_WIDGET* wgt, int height);


/**
 * Retrieves the x and y coordinates of a widget.
 * @param wgt widget to get the coordinates of.
 * @param x variable to store the x coordinate.
 * @param y variable to store the y coordinate.
 */
void alg_get_widget_position(ALG_WIDGET* wgt, int* x, int* y);


/**
 * Sets the x and y coordinates of a widget.
 * @param wgt widget to get the coordinates of.
 * @param x the x coordinate.
 * @param y the y coordinate.
 */
void alg_set_widget_position(ALG_WIDGET* wgt, int x, int y);


/**
 * Retrieves the size of a widget.
 * @param wgt widget to get the coordinates of.
 * @param width variable to store the width.
 * @param height variable to store the height.
 */
void alg_get_widget_size(ALG_WIDGET* wgt, int* width, int* height);


/**
 * Sets the size of a widget.
 * @param wgt widget to get the size of.
 * @param width the width.
 * @param height the height.
 */
void alg_set_widget_size(ALG_WIDGET* wgt, int width, int height);


/**
 * Retrieves the position and size of a widget.
 * @param wgt widget to get the position and size of.
 * @param x variable to store the x coordinate.
 * @param y variable to store the y coordinate.
 * @param width variable to store the width.
 * @param height variable to store the height.
 */
void alg_get_widget_position_and_size(ALG_WIDGET* wgt, int* x, int* y, int* width, int* height);


/**
 * Sets the position and size of a widget.
 * @param wgt widget to get the position and size of.
 * @param x the x coordinate.
 * @param y the y coordinate.
 * @param width the width.
 * @param height the height.
 */
void alg_set_widget_position_and_size(ALG_WIDGET* wgt, int x, int y, int width, int height);


/**
 * Retrieves a widget's data.
 * @param wgt widget to get the data of.
 * @return the widget's data.
 */
void* alg_get_widget_data(ALG_WIDGET* wgt);


/**
 * Sets a widget's data.
 * @param wgt widget to set the data of.
 * @param data the widget's data.
 */
void alg_set_widget_data(ALG_WIDGET* wgt, void* data);


/**
 * Retrieves a widget's id.
 * @param wgt widget to get the id of.
 * @return the widget's id.
 */
uintptr_t alg_get_widget_id(ALG_WIDGET* wgt);


/**
 * Sets a widget's id.
 * @param wgt widget to set the id of.
 * @param id the widget's id.
 */
void alg_set_widget_id(ALG_WIDGET* wgt, uintptr_t id);


/**
 * Returns the visible property of a widget.
 * @param wgt widget to get the property of.
 * @return the property value.
 */
int alg_is_widget_visible(ALG_WIDGET* wgt);


/**
 * Sets the visible property of a widget.
 * @param wgt widget to set the property of.
 * @param value the property value.
 */
void alg_set_widget_visible(ALG_WIDGET* wgt, int value);


/**
 * Returns the enabled property of a widget.
 * @param wgt widget to get the property of.
 * @return the property value.
 */
int alg_is_widget_enabled(ALG_WIDGET* wgt);


/**
 * Sets the enabled property of a widget.
 * @param wgt widget to set the property of.
 * @param value the property value.
 */
void alg_set_widget_enabled(ALG_WIDGET* wgt, int value);


/**
 * Returns the highlighted property of a widget.
 * @param wgt widget to get the property of.
 * @return the property value.
 */
int alg_is_widget_highlighted(ALG_WIDGET* wgt);


/**
 * Sets the highlighted property of a widget.
 * @param wgt widget to set the property of.
 * @param value the property value.
 */
void alg_set_widget_highlighted(ALG_WIDGET* wgt, int value);


/**
 * Returns the pressed property of a widget.
 * @param wgt widget to get the property of.
 * @return the property value.
 */
int alg_is_widget_pressed(ALG_WIDGET* wgt);


/**
 * Sets the pressed property of a widget.
 * @param wgt widget to set the property of.
 * @param value the property value.
 */
void alg_set_widget_pressed(ALG_WIDGET* wgt, int value);


/**
 * Returns the active property of a widget.
 * @param wgt widget to get the property of.
 * @return the property value.
 */
int alg_is_widget_active(ALG_WIDGET* wgt);


/**
 * Sets the active property of a widget.
 * @param wgt widget to set the property of.
 * @param value the property value.
 */
void alg_set_widget_active(ALG_WIDGET* wgt, int value);


/**
 * Returns the error property of a widget.
 * @param wgt widget to get the property of.
 * @return the property value.
 */
int alg_is_widget_error(ALG_WIDGET* wgt);


/**
 * Sets the error property of a widget.
 * @param wgt widget to set the property of.
 * @param value the property value.
 */
void alg_set_widget_error(ALG_WIDGET* wgt, int value);


/**
 * Returns the focused widget.
 * @return the focused widget.
 */
ALG_WIDGET* alg_get_focused_widget();


/**
 * Returns the focused property of a widget.
 * @param wgt widget to get the property of.
 * @return the property value.
 */
int alg_is_widget_focused(ALG_WIDGET* wgt);


/**
 * Set the focus to/remove the focus from the given widget.
 * Sets the focused property of the widget.
 * @param wgt widget.
 * @param focused the focused flag.
 * @return non-zero if the widget lost/got the focus, zero otherwise.
 */
int alg_set_widget_focused(ALG_WIDGET* wgt, int focused);


#endif //ALG_WIDGET_H
