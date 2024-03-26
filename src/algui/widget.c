#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
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
    wgt->clicked_button = 0;
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
    data.x2 = data.x1 + wgt->width + 0.5f;
    data.y2 = data.y1 + wgt->height + 0.5f;
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


//reset the clicked button member helper
static void reset_clicked_helper(ALG_WIDGET* wgt) {
    wgt->clicked_button = 0;
    wgt->pressed = 0;
    for (ALG_WIDGET* child = alg_get_last_child_widget(wgt); child; child = alg_get_prev_sibling_widget(child)) {
        if (child->clicked_button) {
            reset_clicked_helper(child);
            break;
        }
    }
}


//reset the clicked button member
static void reset_clicked(ALG_WIDGET* wgt) {
    if (wgt->clicked_button) {
        reset_clicked_helper(wgt);
    }
}


//find highlighted child
static ALG_WIDGET* get_highlighted_child(ALG_WIDGET* wgt) {
    for (ALG_WIDGET* child = alg_get_last_child_widget(wgt); child; child = alg_get_prev_sibling_widget(child)) {
        if (child->highlighted) {
            return child;
        }
    }
    return NULL;
}


//reset highlighted status helper
static void reset_highlighted_helper(ALG_WIDGET* wgt) {
    wgt->highlighted = 0;
    ALG_WIDGET* child = get_highlighted_child(wgt);
    if (child) {
        reset_highlighted_helper(child);
    }
}


//reset highlighted status
static void reset_highlighted(ALG_WIDGET* wgt) {
    if (wgt->highlighted) {
        reset_highlighted_helper(wgt);
    }
}


//sends message size from content, to children first
static void size_from_content(ALG_WIDGET* wgt) {
    for (ALG_WIDGET* child = alg_get_first_child_widget(wgt); child; child = alg_get_next_sibling_widget(child)) {
        size_from_content(child);
    }
    alg_send_message(wgt, ALG_MSG_SIZE_FROM_CONTENT, NULL);
}


//sends message content from size, to parent first
static void content_from_size(ALG_WIDGET* wgt) {
    alg_send_message(wgt, ALG_MSG_CONTENT_FROM_SIZE, NULL);
    for (ALG_WIDGET* child = alg_get_first_child_widget(wgt); child; child = alg_get_next_sibling_widget(child)) {
        content_from_size(child);
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

        case ALG_MSG_PROPS_CHANGED: {
            ALG_DATA_PROPS_CHANGED* props_changed = (ALG_DATA_PROPS_CHANGED*)data;
            if (alg_test_bitvector_bits(&props_changed->props_changed_bits, ALG_PROP_WIDTH, ALG_PROP_HEIGHT, 0)) {
                return alg_send_message(wgt, ALG_MSG_RESIZED, NULL);
            }
            break;
        }

        case ALG_MSG_RESIZED: {
            ALG_WIDGET* parent = alg_get_parent_widget(wgt);
            if (parent) {
                return alg_send_message(parent, ALG_MSG_CHILD_RESIZED, wgt);
            }
            else {
                content_from_size(wgt);
            }
            break;
        }

        case ALG_MSG_CHILD_RESIZED: 
        case ALG_MSG_CHILD_INSERTED:
        case ALG_MSG_CHILD_REMOVED:
        case ALG_MSG_CHILD_Z_ORDER_CHANGED:
        {
            int w = wgt->width;
            int h = wgt->height;
            int r = alg_send_message(wgt, ALG_MSG_SIZE_FROM_CONTENT, data);
            if (wgt->width == w && wgt->height == h) {
                content_from_size(wgt);
            }
            return r;
        }

        case ALG_MSG_WANT_FOCUS:
            return 1;

        case ALG_MSG_DESC_GOT_FOCUS:
            wgt->active = 1;
            return 1;

        case ALG_MSG_DESC_LOST_FOCUS:
            wgt->active = 0;
            return 1;

        case ALG_MSG_HIT_TEST: {
            const ALG_DATA_HIT_TEST* hit_test = (ALG_DATA_HIT_TEST*)data;
            return wgt->visible && hit_test->x >= 0 && hit_test->y >= 0 && hit_test->x < wgt->width && hit_test->y < wgt->height;
        }

        case ALG_MSG_MOUSE_DOWN: 
        {
            ALG_DATA_MOUSE* mouse = (ALG_DATA_MOUSE*)data;
            wgt->pressed = 1;
            ALG_WIDGET* child = alg_get_child_widget_from_point(wgt, mouse->x, mouse->y);
            if (child && child->enabled) {
                mouse->x -= child->x;
                mouse->y -= child->y;
                if (!child->clicked_button) {
                    child->clicked_button = mouse->button;
                }
                return alg_send_message(child, id, mouse);
            }
            break;
        }

        case ALG_MSG_MOUSE_UP:
        {
            ALG_DATA_MOUSE* mouse = (ALG_DATA_MOUSE*)data;
            ALG_WIDGET* child = alg_get_child_widget_from_point(wgt, mouse->x, mouse->y);
            if (child && child->enabled) {
                mouse->x -= child->x;
                mouse->y -= child->y;
                return alg_send_message(child, id, mouse);
            }
            break;
        }

        case ALG_MOUSE_MOUSE_CLICK:
        {
            ALG_DATA_MOUSE* mouse = (ALG_DATA_MOUSE*)data;
            wgt->pressed = 0;
            ALG_WIDGET* child = alg_get_child_widget_from_point(wgt, mouse->x, mouse->y);
            if (child && child->enabled && child->clicked_button == mouse->button) {
                mouse->x -= child->x;
                mouse->y -= child->y;
                return alg_send_message(child, id, mouse);
            }
            else {
                reset_clicked(wgt);
            }
            break;
        }

        case ALG_MSG_MOUSE_ENTER: {
            ALG_DATA_MOUSE* mouse = (ALG_DATA_MOUSE*)data;
            wgt->highlighted = 1;
            ALG_WIDGET* child = alg_get_child_widget_from_point(wgt, mouse->x, mouse->y);
            if (child && child->enabled) {
                mouse->x -= child->x;
                mouse->y -= child->y;
                return alg_send_message(child, id, mouse);
            }
            break;
        }

        case ALG_MSG_MOUSE_MOVE: {
            ALG_DATA_MOUSE* mouse = (ALG_DATA_MOUSE*)data;
            int org_mouse_x = mouse->x;
            int org_mouse_y = mouse->y;
            ALG_WIDGET* new_child = alg_get_child_widget_from_point(wgt, mouse->x, mouse->y);
            ALG_WIDGET* old_child = get_highlighted_child(wgt);

            //mouse moved over child that already had the mouse
            if (old_child == new_child) {
                if (old_child) {
                    if (old_child->enabled) {
                        mouse->x = org_mouse_x - old_child->x;
                        mouse->y = org_mouse_y - old_child->y;
                        return alg_send_message(old_child, id, mouse);
                    }
                    else {
                        reset_highlighted(old_child);
                    }
                }
                break;
            }

            int r = 0;

            //mouse left a child
            if (old_child) {
                if (old_child->enabled) {
                    mouse->x = org_mouse_x - old_child->x;
                    mouse->y = org_mouse_y - old_child->y;
                    r = alg_send_message(old_child, ALG_MSG_MOUSE_LEAVE, mouse) || r;
                }
                else {
                    reset_highlighted(old_child);
                }
            }

            //mouse moved over a new child
            if (new_child && new_child->enabled) {
                mouse->x = org_mouse_x - new_child->x;
                mouse->y = org_mouse_y - new_child->y;
                r = alg_send_message(new_child, ALG_MSG_MOUSE_ENTER, mouse) || r;
            }

            return r;
        }

        case ALG_MSG_MOUSE_LEAVE: {
            ALG_DATA_MOUSE* mouse = (ALG_DATA_MOUSE*)data;
            wgt->highlighted = 0;
            ALG_WIDGET* child = get_highlighted_child(wgt);
            if (child) {
                if (child->enabled) {
                    mouse->x -= child->x;
                    mouse->y -= child->y;
                    return alg_send_message(child, id, mouse);
                }
                else {
                    reset_highlighted(child);
                }
            }
            break;
        }
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
void alg_get_widget_properties(ALG_WIDGET* wgt, ...) {
    va_list props;
    va_start(props, wgt);
    get_props(wgt, props);
    va_end(props);
}


//set props
void alg_set_widget_properties(ALG_WIDGET* wgt, ...) {
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
    alg_send_message(parent, ALG_MSG_CHILD_INSERTED, child);
}


//insert child as top
void alg_add_widget(ALG_WIDGET* parent, ALG_WIDGET* child) {
    alg_insert_widget(parent, child, -1);
}


//Returns the z-order of a widget.
int alg_get_widget_z_order(ALG_WIDGET* wgt) {
    assert(wgt);
    return alg_get_tree_position(&wgt->tree);
}


//Sets the z-order of a widget, if it is a child widget.
void alg_set_widget_z_order(ALG_WIDGET* wgt, int z_order) {
    assert(wgt);
    ALG_WIDGET* parent = alg_get_parent_widget(wgt);
    if (parent) {
        ALG_WIDGET* prev = alg_get_prev_sibling_widget(wgt);
        ALG_WIDGET* next = alg_get_next_sibling_widget(wgt);
        alg_remove_tree(&wgt->tree);
        alg_insert_tree(&parent->tree, &wgt->tree, z_order);
        if (alg_get_prev_sibling_widget(wgt) != prev || alg_get_next_sibling_widget(wgt) != next) {
            alg_send_message(parent, ALG_MSG_CHILD_Z_ORDER_CHANGED, wgt);
        }
    }
}


//remove child widget
void alg_remove_widget(ALG_WIDGET* wgt) {
    assert(wgt);
    ALG_WIDGET* parent = alg_get_parent_widget(wgt);
    if (parent) {
        alg_remove_tree(&wgt->tree);
        alg_send_message(parent, ALG_MSG_CHILD_REMOVED, wgt);
    }
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
    alg_get_widget_properties(wgt, ALG_PROP_PROC, &proc, ALG_PROP_NULL);
    return proc;
}


//Sets the proc of a widget.
void alg_set_widget_proc(ALG_WIDGET* wgt, ALG_WIDGET_PROC proc) {
    alg_set_widget_properties(wgt, ALG_PROP_PROC, proc, ALG_PROP_NULL);
}


//Returns the x coordinate of a widget.
int alg_get_widget_x(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_properties(wgt, ALG_PROP_X, &value, ALG_PROP_NULL);
    return value;
}


//Sets the x coordinate of a widget.
void alg_set_widget_x(ALG_WIDGET* wgt, int x) {
    alg_set_widget_properties(wgt, ALG_PROP_X, x, ALG_PROP_NULL);
}


//Returns the y coordinate of a widget.
int alg_get_widget_y(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_properties(wgt, ALG_PROP_Y, &value, ALG_PROP_NULL);
    return value;
}


//Sets the y coordinate of a widget.
void alg_set_widget_y(ALG_WIDGET* wgt, int y) {
    alg_set_widget_properties(wgt, ALG_PROP_Y, y, ALG_PROP_NULL);
}


//Returns the width of a widget.
int alg_get_widget_width(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_properties(wgt, ALG_PROP_WIDTH, &value, ALG_PROP_NULL);
    return value;
}


//Sets the width of a widget.
void alg_set_widget_width(ALG_WIDGET* wgt, int width) {
    alg_set_widget_properties(wgt, ALG_PROP_WIDTH, width, ALG_PROP_NULL);
}


//Returns the height of a widget.
int alg_get_widget_height(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_properties(wgt, ALG_PROP_HEIGHT, &value, ALG_PROP_NULL);
    return value;
}


//Sets the height of a widget.
void alg_set_widget_height(ALG_WIDGET* wgt, int height) {
    alg_set_widget_properties(wgt, ALG_PROP_HEIGHT, height, ALG_PROP_NULL);
}


//Retrieves the x and y coordinates of a widget.
void alg_get_widget_position(ALG_WIDGET* wgt, int* x, int* y) {
    alg_get_widget_properties(wgt,
        ALG_PROP_X, x,
        ALG_PROP_Y, y,
        ALG_PROP_NULL);
}


//Sets the x and y coordinates of a widget.
void alg_set_widget_position(ALG_WIDGET* wgt, int x, int y) {
    alg_set_widget_properties(wgt,
        ALG_PROP_X, x,
        ALG_PROP_Y, y,
        ALG_PROP_NULL);
}


//Retrieves the size of a widget.
void alg_get_widget_size(ALG_WIDGET* wgt, int* width, int* height) {
    alg_get_widget_properties(wgt,
        ALG_PROP_WIDTH, width,
        ALG_PROP_HEIGHT, height,
        ALG_PROP_NULL);
}


//Sets the size of a widget.
void alg_set_widget_size(ALG_WIDGET* wgt, int width, int height) {
    alg_set_widget_properties(wgt,
        ALG_PROP_WIDTH, width,
        ALG_PROP_HEIGHT, height,
        ALG_PROP_NULL);
}


//Retrieves the position and size of a widget.
void alg_get_widget_position_and_size(ALG_WIDGET* wgt, int* x, int* y, int* width, int* height) {
    alg_get_widget_properties(wgt,
        ALG_PROP_X, x,
        ALG_PROP_Y, y,
        ALG_PROP_WIDTH, width,
        ALG_PROP_HEIGHT, height,
        ALG_PROP_NULL);
}


//Sets the position and size of a widget.
void alg_set_widget_position_and_size(ALG_WIDGET* wgt, int x, int y, int width, int height) {
    alg_set_widget_properties(wgt, 
        ALG_PROP_X, x,
        ALG_PROP_Y, y,
        ALG_PROP_WIDTH, width,
        ALG_PROP_HEIGHT, height,
        ALG_PROP_NULL);
}


//Retrieves a widget's data.
void* alg_get_widget_data(ALG_WIDGET* wgt) {
    void* value;
    alg_get_widget_properties(wgt, ALG_PROP_DATA, &value, ALG_PROP_NULL);
    return value;
}


//Sets a widget's data.
void alg_set_widget_data(ALG_WIDGET* wgt, void* data) {
    alg_set_widget_properties(wgt, ALG_PROP_DATA, data, ALG_PROP_NULL);
}


//Retrieves a widget's id.
uintptr_t alg_get_widget_id(ALG_WIDGET* wgt) {
    uintptr_t value;
    alg_get_widget_properties(wgt, ALG_PROP_ID, &value, ALG_PROP_NULL);
    return value;
}


//Sets a widget's id.
void alg_set_widget_id(ALG_WIDGET* wgt, uintptr_t id) {
    alg_set_widget_properties(wgt, ALG_PROP_ID, id, ALG_PROP_NULL);
}


//Returns the visible property of a widget.
int alg_is_widget_visible(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_properties(wgt, ALG_PROP_VISIBLE, &value, ALG_PROP_NULL);
    return value;
}


//Sets the visible property of a widget.
void alg_set_widget_visible(ALG_WIDGET* wgt, int value) {
    alg_set_widget_properties(wgt, ALG_PROP_VISIBLE, value, ALG_PROP_NULL);
}


//Returns the enabled property of a widget.
int alg_is_widget_enabled(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_properties(wgt, ALG_PROP_ENABLED, &value, ALG_PROP_NULL);
    return value;
}


//Sets the enabled property of a widget.
void alg_set_widget_enabled(ALG_WIDGET* wgt, int value) {
    alg_set_widget_properties(wgt, ALG_PROP_ENABLED, value, ALG_PROP_NULL);
}


//Returns the highlighted property of a widget.
int alg_is_widget_highlighted(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_properties(wgt, ALG_PROP_HIGHLIGHTED, &value, ALG_PROP_NULL);
    return value;
}


//Sets the highlighted property of a widget.
void alg_set_widget_highlighted(ALG_WIDGET* wgt, int value) {
    alg_set_widget_properties(wgt, ALG_PROP_HIGHLIGHTED, value, ALG_PROP_NULL);
}


//Returns the pressed property of a widget.
int alg_is_widget_pressed(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_properties(wgt, ALG_PROP_PRESSED, &value, ALG_PROP_NULL);
    return value;
}


//Sets the pressed property of a widget.
void alg_set_widget_pressed(ALG_WIDGET* wgt, int value) {
    alg_set_widget_properties(wgt, ALG_PROP_PRESSED, value, ALG_PROP_NULL);
}


//Returns the active property of a widget.
int alg_is_widget_active(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_properties(wgt, ALG_PROP_ACTIVE, &value, ALG_PROP_NULL);
    return value;
}


//Sets the active property of a widget.
void alg_set_widget_active(ALG_WIDGET* wgt, int value) {
    alg_set_widget_properties(wgt, ALG_PROP_ACTIVE, value, ALG_PROP_NULL);
}


//Returns the error property of a widget.
int alg_is_widget_error(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_properties(wgt, ALG_PROP_ERROR, &value, ALG_PROP_NULL);
    return value;
}


//Sets the error property of a widget.
void alg_set_widget_error(ALG_WIDGET* wgt, int value) {
    alg_set_widget_properties(wgt, ALG_PROP_ERROR, value, ALG_PROP_NULL);
}


//Returns the focused widget.
ALG_WIDGET* alg_get_focused_widget() {
    return focused_widget;
}


//Returns the focused property of a widget.
int alg_is_widget_focused(ALG_WIDGET* wgt) {
    int value;
    alg_get_widget_properties(wgt, ALG_PROP_FOCUSED, &value, ALG_PROP_NULL);
    return value;
}


//set the focus property of the widget
int alg_set_widget_focused(ALG_WIDGET* wgt, int focused) {
    assert(wgt);
    alg_set_widget_properties(wgt, ALG_PROP_FOCUSED, focused, ALG_PROP_NULL);
    return wgt->focused == focused;
}


//manage layout
void alg_manage_layout(ALG_WIDGET* wgt) {
    assert(wgt);
    size_from_content(wgt);
    content_from_size(wgt);
}


//get child from point
ALG_WIDGET* alg_get_child_widget_from_point(ALG_WIDGET* parent, int x, int y) {
    assert(parent);
    for (ALG_WIDGET* child = alg_get_last_child_widget(parent); child; child = alg_get_prev_sibling_widget(child)) {
        ALG_DATA_HIT_TEST hit_test;
        hit_test.x = x - child->x;
        hit_test.y = y - child->y;
        if (alg_send_message(child, ALG_MSG_HIT_TEST, &hit_test)) {
            return child;
        }
    }
    return NULL;
}


//translate point
void alg_translate_point(ALG_WIDGET* src, int src_x, int src_y, ALG_WIDGET* dst, int* dst_x, int* dst_y) {
    for (; src; src = alg_get_parent_widget(src)) {
        src_x += src->x;
        src_y += src->y;
    }
    for (; dst; dst = alg_get_parent_widget(dst)) {
        src_x -= dst->x;
        src_y -= dst->y;
    }
    *dst_x = src_x;
    *dst_y = src_y;
}


//test properties of tree.
int alg_test_widget_tree_properties(ALG_WIDGET* wgt, int prop1, ...) {
    int v;

    assert(wgt);

    for (; wgt; wgt = alg_get_parent_widget(wgt)) {

        //test first prop
        v = 0;
        alg_get_widget_properties(wgt, prop1, &v, 0);
        if (!v) {
            return 0;
        }

        //test rest of props
        va_list props;
        va_start(props, prop1);
        for (;;) {
            const int prop = va_arg(props, int);
            if (!prop) {
                break;
            }
            v = 0;
            alg_get_widget_properties(wgt, prop, &v, 0);
            if (!v) {
                return 0;
            }
        }
        va_end(props);
    }

    //success
    return 1;
}


//dispatch event
int alg_dispatch_event(ALG_WIDGET* wgt, ALLEGRO_EVENT* ev) {
    assert(wgt);
    assert(ev);

    switch (ev->type) {
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            if (alg_test_widget_tree_properties(wgt, ALG_PROP_ENABLED, ALG_PROP_VISIBLE, 0)) {
                ALG_DATA_HIT_TEST hit_test;
                alg_translate_point(NULL, ev->mouse.x, ev->mouse.y, wgt, &hit_test.x, &hit_test.y);
                const is_hit = alg_send_message(wgt, ALG_MSG_HIT_TEST, &hit_test);
                if (is_hit) {
                    ALG_DATA_MOUSE data;
                    data.event = ev;
                    data.x = hit_test.x;
                    data.y = hit_test.y;
                    data.z = ev->mouse.z;
                    data.w = ev->mouse.w;
                    data.button = ev->mouse.button;
                    if (!wgt->clicked_button) {
                        wgt->clicked_button = ev->mouse.button;
                    }
                    return alg_send_message(wgt, ALG_MSG_MOUSE_DOWN, &data);
                }
            }
            break;

        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            if (alg_test_widget_tree_properties(wgt, ALG_PROP_ENABLED, ALG_PROP_VISIBLE, 0)) {
                ALG_DATA_HIT_TEST hit_test;
                alg_translate_point(NULL, ev->mouse.x, ev->mouse.y, wgt, &hit_test.x, &hit_test.y);
                const is_hit = alg_send_message(wgt, ALG_MSG_HIT_TEST, &hit_test);
                if (is_hit) {
                    ALG_DATA_MOUSE data;
                    int r = 0;

                    //send mouse up
                    {
                        data.event = ev;
                        data.x = hit_test.x;
                        data.y = hit_test.y;
                        data.z = ev->mouse.z;
                        data.w = ev->mouse.w;
                        data.button = ev->mouse.button;
                        r = alg_send_message(wgt, ALG_MSG_MOUSE_UP, &data) || r;
                    }

                    //send click
                    if (wgt->clicked_button == ev->mouse.button) {
                        data.event = ev;
                        data.x = hit_test.x;
                        data.y = hit_test.y;
                        data.z = ev->mouse.z;
                        data.w = ev->mouse.w;
                        data.button = ev->mouse.button;
                        r = alg_send_message(wgt, ALG_MOUSE_MOUSE_CLICK, &data) || r;
                    }
                    else {
                        reset_clicked(wgt);
                    }

                    return r;
                }
                else {
                    reset_clicked(wgt);
                }
            }
            else {
                reset_clicked(wgt);
            }
            break;

        case ALLEGRO_EVENT_MOUSE_AXES:
        case ALLEGRO_EVENT_MOUSE_WARPED:
            if (alg_test_widget_tree_properties(wgt, ALG_PROP_ENABLED, ALG_PROP_VISIBLE, 0)) {
                ALG_DATA_HIT_TEST hit_test;
                alg_translate_point(NULL, ev->mouse.x, ev->mouse.y, wgt, &hit_test.x, &hit_test.y);
                const is_hit = alg_send_message(wgt, ALG_MSG_HIT_TEST, &hit_test);

                ALG_DATA_MOUSE data;
                data.event = ev;
                data.x = hit_test.x;
                data.y = hit_test.y;
                data.z = ev->mouse.z;
                data.w = ev->mouse.w;
                data.button = ev->mouse.button;

                //mouse move
                if (wgt->highlighted && is_hit) {
                    return alg_send_message(wgt, ALG_MSG_MOUSE_MOVE, &data);
                }

                //mouse enter
                if (is_hit) {
                    return alg_send_message(wgt, ALG_MSG_MOUSE_ENTER, &data);
                }

                //mouse leave
                if (wgt->highlighted) {
                    return alg_send_message(wgt, ALG_MSG_MOUSE_LEAVE, &data);
                }
            }
            else {
                reset_highlighted(wgt);
            }
            break;

        case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
            if (!wgt->highlighted) {
                if (alg_test_widget_tree_properties(wgt, ALG_PROP_ENABLED, ALG_PROP_VISIBLE, 0)) {
                    ALG_DATA_HIT_TEST hit_test;
                    alg_translate_point(NULL, ev->mouse.x, ev->mouse.y, wgt, &hit_test.x, &hit_test.y);
                    const is_hit = alg_send_message(wgt, ALG_MSG_HIT_TEST, &hit_test);
                    if (is_hit) {
                        ALG_DATA_MOUSE data;
                        data.event = ev;
                        data.x = hit_test.x;
                        data.y = hit_test.y;
                        data.z = ev->mouse.z;
                        data.w = ev->mouse.w;
                        data.button = ev->mouse.button;
                        return alg_send_message(wgt, ALG_MSG_MOUSE_ENTER, &data);
                    }
                }
            }
            break;

        case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
            if (wgt->highlighted) {
                if (alg_test_widget_tree_properties(wgt, ALG_PROP_ENABLED, ALG_PROP_VISIBLE, 0)) {
                    ALG_DATA_MOUSE data;
                    alg_translate_point(NULL, ev->mouse.x, ev->mouse.y, wgt, &data.x, &data.y);
                    data.event = ev;
                    data.z = ev->mouse.z;
                    data.w = ev->mouse.w;
                    data.button = ev->mouse.button;
                    return alg_send_message(wgt, ALG_MSG_MOUSE_LEAVE, &data);
                }
                else {
                    reset_highlighted(wgt);
                }
            }
            break;
    }

    return 0;
}
