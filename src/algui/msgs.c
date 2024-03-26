#include <stdlib.h>
#include "algui/msgs.h"
#include "algui/enum.h"
#include "algui/static_init.h"


//message enumeration
static ALG_ENUM msg_enum;


//cleanup the enumeration at exit
static void cleanup(void) {
    alg_cleanup_enum(&msg_enum);
}


//initialize the message enumeration
static void init() {
    ALG_STATIC_INIT({
        alg_init_enum(&msg_enum);
        atexit(cleanup);
        alg_add_enum_value(&msg_enum, ALG_MSG_MALLOC               , "ALG_MSG_MALLOC"               );
        alg_add_enum_value(&msg_enum, ALG_MSG_FREE                 , "ALG_MSG_FREE"                 );
        alg_add_enum_value(&msg_enum, ALG_MSG_INIT                 , "ALG_MSG_INIT"                 );
        alg_add_enum_value(&msg_enum, ALG_MSG_CLEANUP              , "ALG_MSG_CLEANUP"              );
        alg_add_enum_value(&msg_enum, ALG_MSG_GET_PROP             , "ALG_MSG_GET_PROP"             );
        alg_add_enum_value(&msg_enum, ALG_MSG_SET_PROP             , "ALG_MSG_SET_PROP"             );
        alg_add_enum_value(&msg_enum, ALG_MSG_PROPS_CHANGED        , "ALG_MSG_PROPS_CHANGED"        );
        alg_add_enum_value(&msg_enum, ALG_MSG_RESIZED              , "ALG_MSG_RESIZED"              );
        alg_add_enum_value(&msg_enum, ALG_MSG_CHILD_RESIZED        , "ALG_MSG_CHILD_RESIZED"        );
        alg_add_enum_value(&msg_enum, ALG_MSG_SIZE_FROM_CONTENT    , "ALG_MSG_SIZE_FROM_CONTENT"    );
        alg_add_enum_value(&msg_enum, ALG_MSG_CONTENT_FROM_SIZE    , "ALG_MSG_CONTENT_FROM_SIZE"    );
        alg_add_enum_value(&msg_enum, ALG_MSG_CHILD_INSERTED       , "ALG_MSG_CHILD_INSERTED"       );
        alg_add_enum_value(&msg_enum, ALG_MSG_CHILD_REMOVED        , "ALG_MSG_CHILD_REMOVED"        );
        alg_add_enum_value(&msg_enum, ALG_MSG_CHILD_Z_ORDER_CHANGED, "ALG_MSG_CHILD_Z_ORDER_CHANGED");
        alg_add_enum_value(&msg_enum, ALG_MSG_PAINT                , "ALG_MSG_PAINT"                );
        alg_add_enum_value(&msg_enum, ALG_MSG_WANT_FOCUS           , "ALG_MSG_WANT_FOCUS"           );
        alg_add_enum_value(&msg_enum, ALG_MSG_GOT_FOCUS            , "ALG_MSG_GOT_FOCUS"            );
        alg_add_enum_value(&msg_enum, ALG_MSG_DESC_GOT_FOCUS       , "ALG_MSG_DESC_GOT_FOCUS"       );
        alg_add_enum_value(&msg_enum, ALG_MSG_LOST_FOCUS           , "ALG_MSG_LOST_FOCUS"           );
        alg_add_enum_value(&msg_enum, ALG_MSG_DESC_LOST_FOCUS      , "ALG_MSG_DESC_LOST_FOCUS"      );
        alg_add_enum_value(&msg_enum, ALG_MSG_HIT_TEST             , "ALG_MSG_HIT_TEST"             );
        alg_add_enum_value(&msg_enum, ALG_MSG_MOUSE_DOWN           , "ALG_MSG_MOUSE_DOWN"           );
        alg_add_enum_value(&msg_enum, ALG_MSG_MOUSE_UP             , "ALG_MSG_MOUSE_UP"             );
        alg_add_enum_value(&msg_enum, ALG_MOUSE_MOUSE_CLICK        , "ALG_MOUSE_MOUSE_CLICK"        );
        alg_add_enum_value(&msg_enum, ALG_MSG_MOUSE_ENTER          , "ALG_MSG_MOUSE_ENTER"          );
        alg_add_enum_value(&msg_enum, ALG_MSG_MOUSE_MOVE           , "ALG_MSG_MOUSE_MOVE"           );
        alg_add_enum_value(&msg_enum, ALG_MSG_MOUSE_LEAVE          , "ALG_MSG_MOUSE_LEAVE"          );
    });
}


//Returns a message id from message name.
int alg_get_message_id(const char* name) {
    init();
    return alg_get_enum_id(&msg_enum, name);
}


//Returns a message name from a message id.
const char* alg_get_message_name(int id) {
    init();
    return alg_get_enum_name(&msg_enum, id);
}


//Adds a message.
int alg_add_message_enum(int id, const char* name) {
    init();
    return alg_add_enum_value(&msg_enum, id, name);
}
