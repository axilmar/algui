#ifndef ALGUI_OBJECT_H
#define ALGUI_OBJECT_H


#include <stdbool.h>


/**
 * Object message proc.
 * @param obj target object.
 * @param id message id.
 * @param data message data.
 * @return true if the message was processed, false otherwise.
 */
typedef bool (*ALGUI_OBJECT_PROC)(struct ALGUI_OBJECT* obj, int id, void* data);


/**
 * Object.
 */
typedef struct ALGUI_OBJECT {
    ALGUI_OBJECT_PROC proc;
} ALGUI_OBJECT;


/**
 * The default object message procedure.
 * It handles the following messages:
 *  -ALGUI_MSG_FREE: uses the function free() to deallocate the object.
 * @param obj target object; if null, errno is set to EINVAL and false is returned.
 * @param id message id.
 * @param data message data.
 * @return true if the message was processed successfully, false otherwise.
 */
bool algui_object_proc(ALGUI_OBJECT* obj, int id, void* data);


/**
 * Asks an object to process a message.
 * @param obj target object; if null, errno is set to EINVAL and false is returned.
 * @param id message id.
 * @param data message data.
 * @return true if the message was processed successfully, false otherwise.
 */
bool algui_do_message(ALGUI_OBJECT* obj, int id, void* data);


/**
 * Initializes an object.
 * The object gets an ALGUI_MSG_INIT message.
 * @param obj object to initialize; if null, errno is set to EINVAL and false is returned.
 * @param proc object message procedure; if null, errno is set to EINVAL and false is returned.
 * @return true on success, false on error.
 */
bool algui_init_object(ALGUI_OBJECT* obj, ALGUI_OBJECT_PROC proc);


/**
 * Cleans up an object.
 * The object gets an ALGUI_MSG_CLEANUP message.
 * @param obj object to cleanup; if null, errno is set to EINVAL and false is returned.
 * @return true on success, false on error.
 */
bool algui_cleanup_object(ALGUI_OBJECT* obj);


/**
 * Allocates an object on the heap, then initializes it.
 * The proc gets an ALGUI_MSG_MALLOC message, and a pointer to an object pointer as data.
 * The proc must allocate enough space for an object.
 * If the object memory is allocated successfully, then the object gets an ALGUI_MSG_INIT message.
 * @param proc object message procedure; if null, errno is set to EINVAL and false is returned.
 * @return pointer to created object or NULL if no object is created.
 */
ALGUI_OBJECT* algui_create_object(ALGUI_OBJECT_PROC proc);


/**
 * Cleans up, then destroys an object.
 * The object gets an ALGUI_MSG_CLEANUP message first, then an ALGUI_MSG_FREE message in order to
 * free the memory occupied by the object.
 * @param obj object to cleanup and destroy; if null, errno is set to EINVAL and false is returned.
 * @return true on success, false on error.
 */
bool algui_destroy_object(ALGUI_OBJECT* obj);


#endif //ALGUI_OBJECT_H
