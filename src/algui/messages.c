#include <stdlib.h>
#include <errno.h>
#include "algui/messages.h"
#include "algui/enum.h"
#include "algui/static_init.h"


static ALGUI_ENUM messages_enum;


//cleanup at exit
static void cleanup(void) {
    algui_cleanup_enum(&messages_enum);
}


//init function
static void init_messages_func(void) {
    algui_init_enum(&messages_enum);
    atexit(cleanup);
    algui_add_enum_value(&messages_enum, ALGUI_MSG_UNKNOWN, "ALGUI_MSG_UNKNOWN", NULL);
    algui_add_enum_value(&messages_enum, ALGUI_MSG_CLEANUP, "ALGUI_MSG_CLEANUP", NULL);
}


//init messages
static void init_messages() {
    ALGUI_STATIC_INIT(init_messages_func);
}


//add a message
ALGUI_BOOL algui_register_message(int id, const char* name) {
    init_messages();

    if (id == ALGUI_MSG_NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    if (name == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    return algui_add_enum_value(&messages_enum, id, name, NULL);
}


//get message id
int algui_get_message_id(const char* name) {
    init_messages();

    if (name == NULL) {
        errno = EINVAL;
        return ALGUI_MSG_NULL;
    }

    return algui_get_enum_value_id(&messages_enum, name);
}


//get message name
const char* algui_get_message_name(int id) {
    init_messages();

    if (id == ALGUI_MSG_NULL) {
        errno = EINVAL;
        return NULL;
    }

    return algui_get_enum_value_name(&messages_enum, id);
}
