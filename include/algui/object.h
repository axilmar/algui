#ifndef ALGUI_OBJECT_H
#define ALGUI_OBJECT_H


#include "properties.h"
#include "messages.h"
#include "map.h"
#include "buffer.h"


typedef struct ALGUI_OBJECT {
    ALGUI_MAP data;
} ALGUI_OBJECT;


typedef enum ALGUI_PROPERTY_TYPE {
    ALGUI_PROPERTY_TYPE_VALUE,
    ALGUI_PROPERTY_TYPE_ACCESSOR
} ALGUI_PROPERTY_TYPE;


typedef struct ALGUI_PROPERTY_DEFINITION {
    ALGUI_PROPERTY_TYPE type;
    union {
        struct {
            size_t size;
            ALGUI_BOOL(*init)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* prop);
            ALGUI_BOOL(*copy)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* dst, const ALGUI_BUFFER* src);
            ALGUI_BOOL(*cleanup)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* prop);
            int (*compare)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* a, const ALGUI_BUFFER* b);
            void (*changed)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* prop);
        } value;
        struct {
            ALGUI_BOOL(*get)(ALGUI_OBJECT* obj, ALGUI_BUFFER* dst);
            ALGUI_BOOL(*set)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* src);
        } accessor;
    };
} ALGUI_PROPERTY_DEFINITION;


typedef uintptr_t(*ALGUI_OBJECT_MESSAGE_HANDLER)(ALGUI_OBJECT* obj, void* msg_data);


ALGUI_BOOL algui_init_object(ALGUI_OBJECT* obj, ALGUI_OBJECT_MESSAGE_HANDLER cleanup);


ALGUI_BOOL algui_cleanup_object(ALGUI_OBJECT* obj);


ALGUI_BOOL algui_define_object_property(ALGUI_OBJECT* obj, int id, const ALGUI_PROPERTY_DEFINITION* prop, const ALGUI_BUFFER* initial_value, const ALGUI_BUFFER* access_token);


ALGUI_BOOL algui_get_object_property(ALGUI_OBJECT* obj, int id, ALGUI_BUFFER* prop, const ALGUI_BUFFER* default_value, const ALGUI_BUFFER* access_token);


ALGUI_BOOL algui_set_object_property(ALGUI_OBJECT* obj, int id, const ALGUI_BUFFER* prop, const ALGUI_BUFFER* access_token);


ALGUI_BOOL algui_set_object_message_handler(ALGUI_OBJECT* obj, int id, ALGUI_OBJECT_MESSAGE_HANDLER handler, const ALGUI_BUFFER* access_token, ALGUI_OBJECT_MESSAGE_HANDLER* prev_handler);


uintptr_t algui_do_object_message(ALGUI_OBJECT* obj, int id, void* data, const ALGUI_BUFFER* access_token);


#endif //ALGUI_OBJECT_H
