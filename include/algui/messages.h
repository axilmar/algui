#ifndef ALGUI_MESSAGE_H
#define ALGUI_MESSAGES_H


#include <stdint.h>
#include "bool.h"


/**
 * Max message id.
 */
#define ALGUI_MAX_MESSAGE_ID 65535


 /**
  * Message id type.
  */
typedef uint16_t ALGUI_MESSAGE_ID;


/**
 * Message ids.
 */
enum ALGUI_MESSAGE_ID {
    ///null message.
    ALGUI_MSG_NULL,

    ///cleanup object.
    ALGUI_MSG_CLEANUP,

    ///last message id.
    ALGUI_MSG_LAST = 10000
};


/**
 * Registers a message.
 * @param id message id to register. If null id, then false is returned and errno is set to EINVAL.
 * @param name name of the message; if null, then false is returned and errno is set to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_register_message(ALGUI_MESSAGE_ID id, const char* name);


/**
 * Returns a message id from message name.
 * @param name name of message; if null, then null id is returned and errno is set to EINVAL.
 * @return on success, the message id; on error, the null id enum value.
 */
ALGUI_MESSAGE_ID algui_get_message_id(const char* name);


/**
 * Returns a message name from message id.
 * @param id id of message; if null id, then null is returned and errno is set to EINVAL.
 * @return on success, the message name; on error, NULL.
 */
const char* algui_get_message_name(ALGUI_MESSAGE_ID id);


#endif //ALGUI_MESSAGE_H
