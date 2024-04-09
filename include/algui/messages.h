#ifndef ALGUI_MESSAGE_H
#define ALGUI_MESSAGES_H


#include <stdint.h>
#include "bool.h"


/**
 * Message ids.
 */
enum ALGUI_MSG_ID_TYPE {
    ///null message.
    ALGUI_MSG_NULL,

    ///unknown message.
    ALGUI_MSG_UNKNOWN,

    ///cleanup object.
    ALGUI_MSG_CLEANUP,

    ///last message id.
    ALGUI_MSG_LAST = 10000
};


/**
 * Message structure.
 */
typedef struct ALGUI_MESSAGE {
    ///id of message.
    int id;

    ///message data.
    void* data;

    ///access token.
    const struct ALGUI_BUFFER* access_token;
} ALGUI_MESSAGE;


/**
 * Registers a message.
 * @param id message id to register. If null id, then false is returned and errno is set to EINVAL.
 * @param name name of the message; if null, then false is returned and errno is set to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_register_message(int id, const char* name);


/**
 * Returns a message id from message name.
 * @param name name of message; if null, then null id is returned and errno is set to EINVAL.
 * @return on success, the message id; on error, the null id enum value.
 */
int algui_get_message_id(const char* name);


/**
 * Returns a message name from message id.
 * @param id id of message; if null id, then null is returned and errno is set to EINVAL.
 * @return on success, the message name; on error, NULL.
 */
const char* algui_get_message_name(int id);


#endif //ALGUI_MESSAGE_H
