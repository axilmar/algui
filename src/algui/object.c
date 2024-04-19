#include <stdlib.h>
#include "algui/messages.h"
#include "algui/object.h"
#include "algui/util.h"


/**********************************************************************************************************************
    PRIVATE
 **********************************************************************************************************************/


//object proc
static bool object_proc(ALGUI_OBJECT* obj, int id, void* data) {
    switch (id) {
        //free object
        case ALGUI_MSG_FREE:
            free(obj);
            return true;
    }

    //message not processed
    return false;
}


//do message
static bool do_message(ALGUI_OBJECT* obj, int id, void* data) {
    return obj->proc(obj, id, data);
}


//init object
static bool init_object(ALGUI_OBJECT* obj, ALGUI_OBJECT_PROC proc) {
    //set up the object structure
    obj->proc = proc;

    //send the init message
    proc(obj, ALGUI_MSG_INIT, NULL);

    //success
    return true;
}


//cleanup object
static bool cleanup_object(ALGUI_OBJECT* obj) {
    obj->proc(obj, ALGUI_MSG_CLEANUP, NULL);
    return true;
}


//create object
static ALGUI_OBJECT* create_object(ALGUI_OBJECT_PROC proc) {
    ALGUI_OBJECT* obj = NULL;

    //malloc
    if (!proc(NULL, ALGUI_MSG_MALLOC, &obj)) {
        return NULL;
    }

    //failed to malloc
    if (!obj) {
        errno = ENOMEM;
        return NULL;
    }

    //init the object
    init_object(obj, proc);

    //success
    return obj;
}


//destroy object
static bool destroy_object(ALGUI_OBJECT* obj) {
    //cleanup
    cleanup_object(obj);

    //free
    obj->proc(obj, ALGUI_MSG_FREE, NULL);

    //success
    return true;
}


/**********************************************************************************************************************
    PUBLIC
 **********************************************************************************************************************/


//object proc
bool algui_object_proc(ALGUI_OBJECT* obj, int id, void* data) {
    ALGUI_CHECKED_CALL(obj, EINVAL, false, object_proc(obj, id, data));
}


//do message
bool algui_do_message(ALGUI_OBJECT* obj, int id, void* data) {
    ALGUI_CHECKED_CALL(obj, EINVAL, false, do_message(obj, id, data));
}


//init object
bool algui_init_object(ALGUI_OBJECT* obj, ALGUI_OBJECT_PROC proc) {
    ALGUI_CHECKED_CALL(obj && proc, EINVAL, false, init_object(obj, proc));
}


//cleans up an object
bool algui_cleanup_object(ALGUI_OBJECT* obj) {
    ALGUI_CHECKED_CALL(obj, EINVAL, false, cleanup_object(obj));
}


//create object
ALGUI_OBJECT* algui_create_object(ALGUI_OBJECT_PROC proc) {
    ALGUI_CHECKED_CALL(proc, EINVAL, NULL, create_object(proc));
}


//destroy object
bool algui_destroy_object(ALGUI_OBJECT* obj) {
    ALGUI_CHECKED_CALL(obj, EINVAL, false, destroy_object(obj));
}
