#ifndef ALG_MSGS_H
#define ALG_MSGS_H


#include <stdint.h>
#include <allegro5/allegro.h>
#include "bitvector.h"


/**
 * ALGUI messages for widgets.
 */
enum ALG_MSG {
    ///no message.
    ALG_MSG_NULL,

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

    ///props changed message.
    ALG_MSG_PROPS_CHANGED,

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

    ///hit test 
    ALG_MSG_HIT_TEST,

    ///mouse button down event.
    ALG_MSG_MOUSE_DOWN,

    ///mouse button up event.
    ALG_MSG_MOUSE_UP,

    ///click (differs from button up because click happens only in the widget that the button was pressed).
    ALG_MSG_CLICK,

    ///mouse enter.
    ALG_MSG_MOUSE_ENTER,

    ///mouse move.
    ALG_MSG_MOUSE_MOVE,

    ///mouse leave.
    ALG_MSG_MOUSE_LEAVE,

    ///first available message for apps.
    ALG_MSG_USER = 10000
};


/**
 * Widget procedure type.
 * @param wgt target widget.
 * @param id message id.
 * @param data message data.
 * @return depends on message.
 */
typedef uintptr_t(*ALG_WIDGET_PROC)(struct ALG_WIDGET* wgt, int id, void* data);


/**
 * Data for the init message.
 */
typedef struct ALG_DATA_INIT {
    ///widget proc.
    ALG_WIDGET_PROC proc;
} ALG_DATA_INIT;


/**
 * Data for the props changed message.
 */
typedef struct ALG_DATA_PROPS_CHANGED {
    ///bit vector; one bit set for each message that is changed.
    ALG_BITVECTOR props_changed_bits;

    ///number of changed props.
    int props_changed_count;
} ALG_DATA_PROPS_CHANGED;


/**
 * Data for the get/set message messages.
 */
typedef struct ALG_DATA_PROP {
    ///id of the prop.
    int id;

    ///pointer to the value of the prop.
    void* value;

    ///valid only for set prop message.
    ALG_DATA_PROPS_CHANGED* props_changed;
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
 * Data for the hit test message.
 */
typedef struct ALG_DATA_HIT_TEST {
    ///horizontal coordinate relative to widget.
    int x;

    ///vertical coordinate relative to widget.
    int y;
} ALG_DATA_HIT_TEST;


/**
 * Data for mouse events.
 */
typedef struct ALG_DATA_MOUSE {
    ///related allegro event.
    ALLEGRO_EVENT* event;

    ///the horizontal mouse coordinate.
    int x;

    ///the vertical mouse coordinate.
    int y;

    ///the z coordinate.
    int z;

    ///the w coordinate.
    int w;

    ///the mouse button, starting from 1.
    unsigned button;
} ALG_DATA_MOUSE;


/**
 * Returns a message id from message name.
 * @param name message name.
 * @return id of message or 0 if not found.
 */
int alg_get_message_id(const char* name);


/**
 * Returns a message name from a message id.
 * @param id message id.
 * @return message name or null if not found.
 */
const char* alg_get_message_name(int id);


/**
 * Adds a message.
 * @param id message id; should not be 0.
 * @param name message name; should not be null.
 * @return non-zero on success, zero on reallocation error or if id is 0 or if name is null.
 */
int alg_add_message_enum(int id, const char* name);


#endif //ALG_MSGS_H
