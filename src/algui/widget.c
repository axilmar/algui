#include <assert.h>
#include <stdlib.h>
#include "algui/widget.h"


//init widget
static void init(ALG_WIDGET* wgt, ALG_DATA_INIT* data) {
    alg_init_tree(&wgt->tree);
    wgt->proc = data->proc;
    wgt->x = 0;
    wgt->y = 0;
    wgt->width = 0;
    wgt->height = 0;
    wgt->data = NULL;
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

    //destroy children
    for (ALG_WIDGET* child = alg_get_first_child_widget(wgt); child; ) {
        ALG_WIDGET* next = alg_get_next_sibling_widget(child);
        alg_destroy_widget(child);
        child = next;
    }
}


//get property
static int get_prop(ALG_WIDGET* wgt, ALG_DATA_PROP* prop) {
    switch (prop->id) {
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
    switch (prop->id) {
        case ALG_PROP_X:
            wgt->x = *(int*)prop->value;
            return 1;

        case ALG_PROP_Y:
            wgt->y = *(int*)prop->value;
            return 1;

        case ALG_PROP_WIDTH:
            wgt->width = *(int*)prop->value;
            return 1;

        case ALG_PROP_HEIGHT:
            wgt->height = *(int*)prop->value;
            return 1;

        case ALG_PROP_DATA:
            wgt->data = *(void**)prop->value;
            return 1;

        case ALG_PROP_VISIBLE:
            wgt->visible = *(int*)prop->value;
            return 1;

        case ALG_PROP_ENABLED:
            wgt->enabled = *(int*)prop->value;
            return 1;

        case ALG_PROP_HIGHLIGHTED:
            wgt->highlighted = *(int*)prop->value;
            return 1;

        case ALG_PROP_PRESSED:
            wgt->pressed = *(int*)prop->value;
            return 1;

        case ALG_PROP_SELECTED:
            wgt->selected = *(int*)prop->value;
            return 1;

        case ALG_PROP_ACTIVE:
            wgt->active = *(int*)prop->value;
            return 1;

        case ALG_PROP_ERROR:
            wgt->error = *(int*)prop->value;
            return 1;

        case ALG_PROP_FOCUSED:
            //TODO
            return 1;
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
            case ALG_PROP_DATA:
                data.value = &(va_arg(props, void*));
                break;

            //property is sizeof(int)
            default:
                data.value = &(va_arg(props, int));
        }

        //set prop
        alg_send_message(wgt, ALG_MSG_SET_PROP, &data);
    }
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
