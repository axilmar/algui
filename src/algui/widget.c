#include <assert.h>
#include <stdlib.h>
#include "algui/widget.h"


//initial props count is 64
#define INITIAL_PROPS_CHANGED_PROP_COUNT 64


//number of words initially allocated for the props changed vector.
#define INITIAL_PROPS_CHANGED_WORD_COUNT (INITIAL_PROPS_CHANGED_PROP_COUNT / (sizeof(uintptr_t) * 8))


//the currently focused widget
static ALG_WIDGET* focused_widget = NULL;


//init widget
static void init(ALG_WIDGET* wgt, ALG_DATA_INIT* data) {
    alg_init_tree(&wgt->tree);
    wgt->proc = data->proc;
    wgt->x = 0;
    wgt->y = 0;
    wgt->width = 0;
    wgt->height = 0;
    wgt->data = NULL;
    wgt->id = 0;
    wgt->visible = 1;
    wgt->enabled = 1;
    wgt->highlighted = 0;
    wgt->pressed = 0;
    wgt->selected = 0;
    wgt->active = 0;
    wgt->error = 0;
    wgt->focused = 0;
}


//cleanup widget
static void cleanup(ALG_WIDGET* wgt) {
    //remove widget from parent
    alg_remove_widget(wgt);

    //reset focused widget
    if (wgt == focused_widget) {
        focused_widget = NULL;
    }

    //destroy children
    for (ALG_WIDGET* child = alg_get_first_child_widget(wgt); child; ) {
        ALG_WIDGET* next = alg_get_next_sibling_widget(child);
        alg_destroy_widget(child);
        child = next;
    }
}


//set the given prop as changed
static void set_prop_changed(ALG_DATA_PROPS_CHANGED* data, int prop_id) {
    alg_set_bitvector_bit(&data->props_changed_bits, prop_id, 1);
    ++data->props_changed_count;
}


//set/reset focus
static int set_focused(ALG_WIDGET* wgt, int focused, ALG_DATA_PROPS_CHANGED* props_changed) {
    //check for change
    if (wgt->focused == focused) {
        return 1;
    }

    //focus
    if (focused) {
        //ask wigdet if it wants the focus
        if (!alg_send_message(wgt, ALG_MSG_WANT_FOCUS, NULL)) {
            return 0;
        }

        //unfocus focused widget
        if (focused_widget) {
            alg_set_widget_focused(focused_widget, 0);
        }

        //got focus
        wgt->focused = 1;
        focused_widget = wgt;
        set_prop_changed(props_changed, ALG_PROP_FOCUSED);
        alg_send_message(wgt, ALG_MSG_GOT_FOCUS, NULL);

        //inform ancestors
        for (ALG_WIDGET* anc = alg_get_parent_widget(wgt); anc; anc = alg_get_parent_widget(anc)) {
            alg_send_message(wgt, ALG_MSG_DESC_GOT_FOCUS, NULL);
        }
    }

    //else unfocus
    else {
        //lost focus
        wgt->focused = 0;
        focused_widget = NULL;
        set_prop_changed(props_changed, ALG_PROP_FOCUSED);
        alg_send_message(wgt, ALG_MSG_LOST_FOCUS, NULL);

        //inform ancestors
        for (ALG_WIDGET* anc = alg_get_parent_widget(wgt); anc; anc = alg_get_parent_widget(anc)) {
            alg_send_message(wgt, ALG_MSG_DESC_LOST_FOCUS, NULL);
        }
    }

    //success
    return 1;
}


//get property
static int get_prop(ALG_WIDGET* wgt, ALG_DATA_PROP* prop) {
    assert(prop->value);

    switch (prop->id) {
        case ALG_PROP_PROC:
            *(ALG_WIDGET_PROC*)prop->value = wgt->proc;
            return 1;

        case ALG_PROP_X:
            *(int*)prop->value = wgt->x;
            return 1;

        case ALG_PROP_Y:
            *(int*)prop->value = wgt->y;
            return 1;

        case ALG_PROP_WIDTH:
            *(int*)prop->value = wgt->width;
            return 1;

        case ALG_PROP_HEIGHT:
            *(int*)prop->value = wgt->height;
            return 1;

        case ALG_PROP_DATA:
            *(void**)prop->value = wgt->data;
            return 1;

        case ALG_PROP_VISIBLE:
            *(int*)prop->value = wgt->visible;
            return 1;

        case ALG_PROP_ENABLED:
            *(int*)prop->value = wgt->enabled;
            return 1;

        case ALG_PROP_HIGHLIGHTED:
            *(int*)prop->value = wgt->highlighted;
            return 1;

        case ALG_PROP_PRESSED:
            *(int*)prop->value = wgt->pressed;
            return 1;

        case ALG_PROP_SELECTED:
            *(int*)prop->value = wgt->selected;
            return 1;

        case ALG_PROP_ACTIVE:
            *(int*)prop->value = wgt->active;
            return 1;

        case ALG_PROP_ERROR:
            *(int*)prop->value = wgt->error;
            return 1;

        case ALG_PROP_FOCUSED:
            *(int*)prop->value = wgt->focused;
            return 1;
    }
    return 0;
}


//set property
static int set_prop(ALG_WIDGET* wgt, ALG_DATA_PROP* prop) {
    assert(prop->value);
    assert(prop->props_changed);

    switch (prop->id) {
        case ALG_PROP_PROC:
            if (wgt->proc != *(ALG_WIDGET_PROC*)prop->value) {
                wgt->proc = *(ALG_WIDGET_PROC*)prop->value;
                set_prop_changed(prop->props_changed, prop->id);
            }
            return 1;

        case ALG_PROP_X:
            if (wgt->x != *(int*)prop->value) {
                wgt->x = *(int*)prop->value;
                set_prop_changed(prop->props_changed, prop->id);
            }
            return 1;

        case ALG_PROP_Y:
            if (wgt->y != *(int*)prop->value) {
                wgt->y = *(int*)prop->value;
                set_prop_changed(prop->props_changed, prop->id);
            }
            return 1;

        case ALG_PROP_WIDTH:
            if (wgt->width != *(int*)prop->value) {
                wgt->width = *(int*)prop->value;
                set_prop_changed(prop->props_changed, prop->id);
            }
            return 1;

        case ALG_PROP_HEIGHT:
            if (wgt->height != *(int*)prop->value) {
                wgt->height = *(int*)prop->value;
                set_prop_changed(prop->props_changed, prop->id);
            }
            return 1;

        case ALG_PROP_DATA:
            if (wgt->data != *(void**)prop->value) {
                wgt->data = *(void**)prop->value;
                set_prop_changed(prop->props_changed, prop->id);
            }
            return 1;

        case ALG_PROP_ID:
            if (wgt->id != *(uintptr_t*)prop->value) {
                wgt->id = *(uintptr_t*)prop->value;
                set_prop_changed(prop->props_changed, prop->id);
            }
            return 1;

        case ALG_PROP_VISIBLE:
            if (wgt->visible != *(int*)prop->value) {
                wgt->visible = *(int*)prop->value;
                set_prop_changed(prop->props_changed, prop->id);
            }
            return 1;

        case ALG_PROP_ENABLED:
            if (wgt->enabled != *(int*)prop->value) {
                wgt->enabled = *(int*)prop->value;
                set_prop_changed(prop->props_changed, prop->id);
            }
            return 1;

        case ALG_PROP_HIGHLIGHTED:
            if (wgt->highlighted != *(int*)prop->value) {
                wgt->highlighted = *(int*)prop->value;
                set_prop_changed(prop->props_changed, prop->id);
            }
            return 1;

        case ALG_PROP_PRESSED:
            if (wgt->pressed != *(int*)prop->value) {
                wgt->pressed = *(int*)prop->value;
                set_prop_changed(prop->props_changed, prop->id);
            }
            return 1;

        case ALG_PROP_SELECTED:
            if (wgt->selected != *(int*)prop->value) {
                wgt->selected = *(int*)prop->value;
                set_prop_changed(prop->props_changed, prop->id);
            }
            return 1;

        case ALG_PROP_ACTIVE:
            if (wgt->active != *(int*)prop->value) {
                wgt->active = *(int*)prop->value;
                set_prop_changed(prop->props_changed, prop->id);
            }
            return 1;

        case ALG_PROP_ERROR:
            if (wgt->error != *(int*)prop->value) {
                wgt->error = *(int*)prop->value;
                set_prop_changed(prop->props_changed, prop->id);
            }
            return 1;

        case ALG_PROP_FOCUSED:            
            return set_focused(wgt, *(int*)prop->value, prop->props_changed);
    }
    return 0;
}


//get props
static void get_props(ALG_WIDGET* wgt, va_list props) {
    ALG_DATA_PROP data;

    for (;;) {
        //get prop id
        data.id = va_arg(props, int);

        //handle prop id
        switch (data.id) {

            //end of list
            case ALG_PROP_NULL:
                return;

            //property is sizeof(void*)
            default:
                data.value = va_arg(props, void*);
        }

        //get prop
        alg_send_message(wgt, ALG_MSG_GET_PROP, &data);
    }
}


//set props
static void set_props(ALG_WIDGET* wgt, va_list props) {
    uintptr_t initial_props_changed_storage[INITIAL_PROPS_CHANGED_WORD_COUNT] = { 0 };

    //the props changed message
    ALG_DATA_PROPS_CHANGED props_changed;
    alg_init_bitvector(&props_changed.props_changed_bits, initial_props_changed_storage, INITIAL_PROPS_CHANGED_WORD_COUNT, 0);
    props_changed.props_changed_count = 0;

    //the data message
    ALG_DATA_PROP data;
    data.props_changed = &props_changed;

    for (;;) {
        //get prop id
        data.id = va_arg(props, int);

        //handle prop id
        switch (data.id) {

            //end of list
            case ALG_PROP_NULL:
                goto END;

            //property is sizeof(void*)
            case ALG_PROP_PROC:
            case ALG_PROP_DATA:
            case ALG_PROP_ID:
                data.value = &(va_arg(props, void*));
                break;

            //property is sizeof(int)
            default:
                data.value = &(va_arg(props, int));
        }

        //set prop
        alg_send_message(wgt, ALG_MSG_SET_PROP, &data);
    }

    END:

    //if there are properties that are changed, notify the widget
    if (props_changed.props_changed_count > 0) {
        alg_send_message(wgt, ALG_MSG_PROPS_CHANGED, &props_changed);
    }

    //cleanup
    alg_cleanup_bitvector(&props_changed.props_changed_bits);
}


//create widget
static ALG_WIDGET* create_widget(ALG_WIDGET_PROC proc, va_list props) {
    //malloc widget
    ALG_WIDGET* wgt = (ALG_WIDGET*)proc(NULL, ALG_MSG_MALLOC, NULL);
    if (!wgt) {
        return NULL;
    }

    //init the widget
    ALG_DATA_INIT data;
    data.proc = proc;
    proc(wgt, ALG_MSG_INIT, &data);

    //set the properties of the widget
    set_props(wgt, props);

    //success
    return wgt;
}


//paint widget
static void paint_widget(ALG_WIDGET* wgt, const ALG_DATA_PAINT* parent_data) {
    //compute the global state of the widget
    ALG_DATA_PAINT data;
    data.x1 = parent_data->x1 + wgt->x;
    data.y1 = parent_data->y1 + wgt->y;
    data.x2 = data.x1 + wgt->width;
    data.y2 = data.y1 + wgt->height;
    data.enabled = wgt->enabled & parent_data->enabled;
    data.highlighted = wgt->highlighted | parent_data->highlighted;
    data.pressed = wgt->pressed | parent_data->pressed;
    data.selected = wgt->selected | parent_data->selected;
    data.active = wgt->active | parent_data->active;
    data.error = wgt->error | parent_data->error;
    data.focused = wgt->focused | parent_data->focused;

    //send the paint message to the widget
    alg_send_message(wgt, ALG_MSG_PAINT, &data);

    //paint the children
    for (ALG_WIDGET* child = alg_get_first_child_widget(wgt); child; child = alg_get_next_sibling_widget(child)) {
        if (child->visible) {
            paint_widget(child, &data);
        }
    }
}


//the default widget proc
uintptr_t alg_widget_proc(ALG_WIDGET* wgt, int id, void* data) {
    assert(wgt);

    switch (id) {
        case ALG_MSG_INIT:
            init(wgt, (ALG_DATA_INIT*)data);
            return 1;

        case ALG_MSG_CLEANUP:
            cleanup(wgt);
            return 1;

        case ALG_MSG_FREE:
            free(wgt);
            return 1;

        case ALG_MSG_GET_PROP:
            return get_prop(wgt, (ALG_DATA_PROP*)data);

        case ALG_MSG_SET_PROP:
            return set_prop(wgt, (ALG_DATA_PROP*)data);

        case ALG_MSG_WANT_FOCUS:
            return 1;

        case ALG_MSG_DESC_GOT_FOCUS:
            wgt->active = 1;
            return 1;

        case ALG_MSG_DESC_LOST_FOCUS:
            wgt->active = 0;
            return 1;
    }

    return 0;
}


//sends message to widget
uintptr_t alg_send_message(ALG_WIDGET* wgt, int id, void* data) {
    assert(wgt);
    return wgt->proc(wgt, id, data);
}


//create widget
ALG_WIDGET* alg_create_widget(ALG_WIDGET_PROC proc, ...) {
    va_list props;
    va_start(props, proc);
    ALG_WIDGET* wgt = create_widget(proc, props);
    va_end(props);
    return wgt;
}


//create child widget
ALG_WIDGET* alg_create_child_widget(ALG_WIDGET* parent, ALG_WIDGET_PROC proc, ...) {
    //create the widget
    va_list props;
    va_start(props, proc);
    ALG_WIDGET* wgt = create_widget(proc, props);
    va_end(props);

    //insert widget to parent
    if (parent) {
        alg_insert_widget(parent, wgt, -1);
    }
    return wgt;
}


//destroys the widget
void alg_destroy_widget(ALG_WIDGET* wgt) {
    assert(wgt);
    alg_send_message(wgt, ALG_MSG_CLEANUP, NULL);
    alg_send_message(wgt, ALG_MSG_FREE, NULL);
}


//get props
void alg_get_widget_props(ALG_WIDGET* wgt, ...) {
    va_list props;
    va_start(props, wgt);
    get_props(wgt, props);
    va_end(props);
}


//set props
void alg_set_widget_props(ALG_WIDGET* wgt, ...) {
    va_list props;
    va_start(props, wgt);
    set_props(wgt, props);
    va_end(props);
}


//get root
ALG_WIDGET* alg_get_root_widget(ALG_WIDGET* wgt) {
    assert(wgt);
    return (ALG_WIDGET*)alg_get_root_tree(&wgt->tree);
}


//get parent
ALG_WIDGET* alg_get_parent_widget(ALG_WIDGET* wgt) {
    assert(wgt);
    return (ALG_WIDGET*)wgt->tree.parent;
}


//Returns the prev_sibling widget.
ALG_WIDGET* alg_get_prev_sibling_widget(ALG_WIDGET* wgt) {
    assert(wgt);
    return (ALG_WIDGET*)wgt->tree.prev;
}


//Returns the next sibling widget.
ALG_WIDGET* alg_get_next_sibling_widget(ALG_WIDGET* wgt) {
    assert(wgt);
    return (ALG_WIDGET*)wgt->tree.next;
}


//Returns the first child widget.
ALG_WIDGET* alg_get_first_child_widget(ALG_WIDGET* wgt) {
    assert(wgt);
    return (ALG_WIDGET*)wgt->tree.first;
}


//Returns the last child widget.
ALG_WIDGET* alg_get_last_child_widget(ALG_WIDGET* wgt) {
    assert(wgt);
    return (ALG_WIDGET*)wgt->tree.last;
}


//Inserts a widget into another widget.
void alg_insert_widget(ALG_WIDGET* parent, ALG_WIDGET* child, int z_order) {
    assert(parent);
    assert(child);
    alg_insert_tree(&parent->tree, &child->tree, z_order);
}


//remove child widget
void alg_remove_widget(ALG_WIDGET* wgt) {
    assert(wgt);
    alg_remove_tree(&wgt->tree);
}


//paint widget
void alg_paint_widget(ALG_WIDGET* wgt) {
    assert(wgt);

    //widget is not visible
    if (!wgt->visible) {
        return;
    }

    //compute the state of the tree above the widget;
    //if ancestor is not visible, do not paint the widget
    ALG_DATA_PAINT data;
    data.x1 = 0;
    data.y1 = 0;
    data.enabled = 1;
    data.highlighted = 0;
    data.pressed = 0;
    data.selected = 0;
    data.active = 0;
    data.error = 0;
    data.focused = 0;
    for (ALG_WIDGET* anc = alg_get_parent_widget(wgt); anc; anc = alg_get_parent_widget(anc)) {
        if (!anc->visible) {
            return;
        }
        data.x1 += anc->x;
        data.y1 += anc->y;
        data.enabled &= anc->enabled;
        data.highlighted |= anc->highlighted;
        data.pressed |= anc->pressed;
        data.selected |= anc->selected;
        data.active |= anc->active;
        data.error |= anc->error;
        data.focused |= anc->focused;
    }

    //paint the widget
    paint_widget(wgt, &data);
}


//get the widget proc.
ALG_WIDGET_PROC alg_get_widget_proc(ALG_WIDGET* wgt) {
    ALG_WIDGET_PROC proc;
    alg_get_widget_props(wgt, ALG_PROP_PROC, &proc, ALG_PROP_NULL);
    return proc;
}


//Sets the proc of a widget.
void alg_set_widget_proc(ALG_WIDGET* wgt, ALG_WIDGET_PROC proc) {
    alg_set_widget_props(wgt, ALG_PROP_PROC, proc, ALG_PROP_NULL);
}


//Returns the x coordinate of a widget.
int alg_get_widget_x(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_props(wgt, ALG_PROP_X, &value, ALG_PROP_NULL);
    return value;
}


//Sets the x coordinate of a widget.
void alg_set_widget_x(ALG_WIDGET* wgt, int x) {
    alg_set_widget_props(wgt, ALG_PROP_X, x, ALG_PROP_NULL);
}


//Returns the y coordinate of a widget.
int alg_get_widget_y(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_props(wgt, ALG_PROP_Y, &value, ALG_PROP_NULL);
    return value;
}


//Sets the y coordinate of a widget.
void alg_set_widget_y(ALG_WIDGET* wgt, int y) {
    alg_set_widget_props(wgt, ALG_PROP_Y, y, ALG_PROP_NULL);
}


//Returns the width of a widget.
int alg_get_widget_width(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_props(wgt, ALG_PROP_WIDTH, &value, ALG_PROP_NULL);
    return value;
}


//Sets the width of a widget.
void alg_set_widget_width(ALG_WIDGET* wgt, int width) {
    alg_set_widget_props(wgt, ALG_PROP_WIDTH, width, ALG_PROP_NULL);
}


//Returns the height of a widget.
int alg_get_widget_height(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_props(wgt, ALG_PROP_HEIGHT, &value, ALG_PROP_NULL);
    return value;
}


//Sets the height of a widget.
void alg_set_widget_height(ALG_WIDGET* wgt, int height) {
    alg_set_widget_props(wgt, ALG_PROP_HEIGHT, height, ALG_PROP_NULL);
}


//Retrieves the x and y coordinates of a widget.
void alg_get_widget_position(ALG_WIDGET* wgt, int* x, int* y) {
    alg_get_widget_props(wgt,
        ALG_PROP_X, x,
        ALG_PROP_Y, y,
        ALG_PROP_NULL);
}


//Sets the x and y coordinates of a widget.
void alg_set_widget_position(ALG_WIDGET* wgt, int x, int y) {
    alg_set_widget_props(wgt,
        ALG_PROP_X, x,
        ALG_PROP_Y, y,
        ALG_PROP_NULL);
}


//Retrieves the size of a widget.
void alg_get_widget_size(ALG_WIDGET* wgt, int* width, int* height) {
    alg_get_widget_props(wgt,
        ALG_PROP_WIDTH, width,
        ALG_PROP_HEIGHT, height,
        ALG_PROP_NULL);
}


//Sets the size of a widget.
void alg_set_widget_size(ALG_WIDGET* wgt, int width, int height) {
    alg_set_widget_props(wgt,
        ALG_PROP_WIDTH, width,
        ALG_PROP_HEIGHT, height,
        ALG_PROP_NULL);
}


//Retrieves the position and size of a widget.
void alg_get_widget_position_and_size(ALG_WIDGET* wgt, int* x, int* y, int* width, int* height) {
    alg_get_widget_props(wgt,
        ALG_PROP_X, x,
        ALG_PROP_Y, y,
        ALG_PROP_WIDTH, width,
        ALG_PROP_HEIGHT, height,
        ALG_PROP_NULL);
}


//Sets the position and size of a widget.
void alg_set_widget_position_and_size(ALG_WIDGET* wgt, int x, int y, int width, int height) {
    alg_set_widget_props(wgt, 
        ALG_PROP_X, x,
        ALG_PROP_Y, y,
        ALG_PROP_WIDTH, width,
        ALG_PROP_HEIGHT, height,
        ALG_PROP_NULL);
}


//Retrieves a widget's data.
void* alg_get_widget_data(ALG_WIDGET* wgt) {
    void* value;
    alg_get_widget_props(wgt, ALG_PROP_DATA, &value, ALG_PROP_NULL);
    return value;
}


//Sets a widget's data.
void alg_set_widget_data(ALG_WIDGET* wgt, void* data) {
    alg_set_widget_props(wgt, ALG_PROP_DATA, data, ALG_PROP_NULL);
}


//Retrieves a widget's id.
uintptr_t alg_get_widget_id(ALG_WIDGET* wgt) {
    uintptr_t value;
    alg_get_widget_props(wgt, ALG_PROP_ID, &value, ALG_PROP_NULL);
    return value;
}


//Sets a widget's id.
void alg_set_widget_id(ALG_WIDGET* wgt, uintptr_t id) {
    alg_set_widget_props(wgt, ALG_PROP_ID, id, ALG_PROP_NULL);
}


//Returns the visible property of a widget.
int alg_is_widget_visible(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_props(wgt, ALG_PROP_VISIBLE, &value, ALG_PROP_NULL);
    return value;
}


//Sets the visible property of a widget.
void alg_set_widget_visible(ALG_WIDGET* wgt, int value) {
    alg_set_widget_props(wgt, ALG_PROP_VISIBLE, value, ALG_PROP_NULL);
}


//Returns the enabled property of a widget.
int alg_is_widget_enabled(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_props(wgt, ALG_PROP_ENABLED, &value, ALG_PROP_NULL);
    return value;
}


//Sets the enabled property of a widget.
void alg_set_widget_enabled(ALG_WIDGET* wgt, int value) {
    alg_set_widget_props(wgt, ALG_PROP_ENABLED, value, ALG_PROP_NULL);
}


//Returns the highlighted property of a widget.
int alg_is_widget_highlighted(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_props(wgt, ALG_PROP_HIGHLIGHTED, &value, ALG_PROP_NULL);
    return value;
}


//Sets the highlighted property of a widget.
void alg_set_widget_highlighted(ALG_WIDGET* wgt, int value) {
    alg_set_widget_props(wgt, ALG_PROP_HIGHLIGHTED, value, ALG_PROP_NULL);
}


//Returns the pressed property of a widget.
int alg_is_widget_pressed(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_props(wgt, ALG_PROP_PRESSED, &value, ALG_PROP_NULL);
    return value;
}


//Sets the pressed property of a widget.
void alg_set_widget_pressed(ALG_WIDGET* wgt, int value) {
    alg_set_widget_props(wgt, ALG_PROP_PRESSED, value, ALG_PROP_NULL);
}


//Returns the active property of a widget.
int alg_is_widget_active(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_props(wgt, ALG_PROP_ACTIVE, &value, ALG_PROP_NULL);
    return value;
}


//Sets the active property of a widget.
void alg_set_widget_active(ALG_WIDGET* wgt, int value) {
    alg_set_widget_props(wgt, ALG_PROP_ACTIVE, value, ALG_PROP_NULL);
}


//Returns the error property of a widget.
int alg_is_widget_error(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_props(wgt, ALG_PROP_ERROR, &value, ALG_PROP_NULL);
    return value;
}


//Sets the error property of a widget.
void alg_set_widget_error(ALG_WIDGET* wgt, int value) {
    alg_set_widget_props(wgt, ALG_PROP_ERROR, value, ALG_PROP_NULL);
}


//Returns the focused widget.
ALG_WIDGET* alg_get_focused_widget() {
    return focused_widget;
}


//Returns the focused property of a widget.
int alg_is_widget_focused(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_props(wgt, ALG_PROP_FOCUSED, &value, ALG_PROP_NULL);
    return value;
}


//set the focus property of the widget
int alg_set_widget_focused(ALG_WIDGET* wgt, int focused) {
    assert(wgt);
    alg_set_widget_props(wgt, ALG_PROP_FOCUSED, focused, ALG_PROP_NULL);
    return wgt->focused == focused;
}


