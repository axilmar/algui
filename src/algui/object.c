#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include "algui/object.h"
#include "algui/bool.h"


typedef long long long_long;
typedef unsigned char unsigned_char;
typedef unsigned short unsigned_short;
typedef unsigned int unsigned_int;
typedef unsigned long unsigned_long;
typedef unsigned long long unsigned_long_long;
typedef long double long_double;
typedef ALGUI_BOOL bool;


//define property for specific type
#define DEFINE_PROPERTY_FUNCTION(TYPE)\
ALGUI_BOOL algui_define_##TYPE##_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, TYPE initial_value, const char* access_token) {\
    ALGUI_BUFFER initial_value_buffer;\
    algui_init_buffer(&initial_value_buffer, &initial_value, sizeof(initial_value), ALGUI_FALSE);\
    ALGUI_BUFFER access_token_buffer;\
    if (access_token != NULL) {\
        algui_init_buffer(&access_token_buffer, (void*)access_token, strlen(access_token), ALGUI_FALSE);\
    }\
    if (prop->type == ALGUI_PROPERTY_TYPE_VALUE) {\
        prop->value.size = sizeof(TYPE);\
    }\
    return algui_define_object_property(obj, id, prop, &initial_value_buffer, access_token ? &access_token_buffer : NULL);\
}


//get property of specific type
#define GET_PROPERTY_FUNCTION(TYPE)\
TYPE algui_get_##TYPE##_object_property(ALGUI_OBJECT* obj, int id, TYPE default_value, const char* access_token) {\
    ALGUI_BUFFER result_buffer;\
    TYPE result;\
    algui_init_buffer(&result_buffer, &result, sizeof(result), ALGUI_FALSE);\
    ALGUI_BUFFER default_value_buffer;\
    algui_init_buffer(&default_value_buffer, &default_value, sizeof(default_value), ALGUI_FALSE);\
    ALGUI_BUFFER access_token_buffer;\
    if (access_token != NULL) {\
        algui_init_buffer(&access_token_buffer, (void*)access_token, strlen(access_token), ALGUI_FALSE);\
    }\
    if (!algui_get_object_property(obj, id, &result_buffer, &default_value_buffer, access_token ? &access_token_buffer : NULL)) {\
        return ALGUI_FALSE;\
    }\
    return result;\
}


//set property of specific type
#define SET_PROPERTY_FUNCTION(TYPE)\
ALGUI_BOOL algui_set_##TYPE##_object_property(ALGUI_OBJECT* obj, int id, TYPE value, const char* access_token) {\
    ALGUI_BUFFER value_buffer;\
    algui_init_buffer(&value_buffer, &value, sizeof(value), ALGUI_FALSE);\
    ALGUI_BUFFER access_token_buffer;\
    if (access_token != NULL) {\
        algui_init_buffer(&access_token_buffer, (void*)access_token, strlen(access_token), ALGUI_FALSE);\
    }\
    return algui_set_object_property(obj, id, &value_buffer, access_token ? &access_token_buffer : NULL);\
}


//declare property functions for given type
#define PROPERTY_FUNCTIONS(TYPE)\
    DEFINE_PROPERTY_FUNCTION(TYPE)\
    GET_PROPERTY_FUNCTION(TYPE)\
    SET_PROPERTY_FUNCTION(TYPE)


//entry type
enum ENTRY_TYPE {
    ENTRY_TYPE_EMBEDDED_VALUE,
    ENTRY_TYPE_STATIC_HEAP_VALUE,
    ENTRY_TYPE_DYNAMIC_HEAP_VALUE,
    ENTRY_TYPE_ACCESSOR,
    ENTRY_TYPE_MSG_HANDLER
};


//if value size is equal or less than that of a buffer, then the value is embedded into the data
enum { EMBEDDED_VALUE_SIZE = sizeof(ALGUI_BUFFER) };


//actual message ids start from INT_MAX / 2 so as that they do not coincide with property ids
enum { MESSAGE_HANDLER_ID_OFFSET = INT_MAX / 2 };


//embedded value property
typedef struct EMBEDDED_VALUE {
    size_t size;
    ALGUI_BOOL(*init)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* prop);
    ALGUI_BOOL(*copy)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* dst, const ALGUI_BUFFER* src);
    ALGUI_BOOL(*cleanup)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* prop);
    int (*compare)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* a, const ALGUI_BUFFER* b);
    void (*changed)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* prop);
    char value[EMBEDDED_VALUE_SIZE];
} EMBEDDED_VALUE;


//static heap value property
typedef struct STATIC_HEAP_VALUE {
    ALGUI_BOOL(*init)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* prop);
    ALGUI_BOOL(*copy)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* dst, const ALGUI_BUFFER* src);
    ALGUI_BOOL(*cleanup)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* prop);
    int (*compare)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* a, const ALGUI_BUFFER* b);
    void (*changed)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* prop);
    ALGUI_BUFFER value;
    size_t size;
} STATIC_HEAP_VALUE;


//dynamic heap value property
typedef struct DYNAMIC_HEAP_VALUE {
    ALGUI_BOOL(*init)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* prop);
    ALGUI_BOOL(*copy)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* dst, const ALGUI_BUFFER* src);
    ALGUI_BOOL(*cleanup)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* prop);
    int (*compare)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* a, const ALGUI_BUFFER* b);
    void (*changed)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* prop);
    ALGUI_BUFFER value;
} DYNAMIC_HEAP_VALUE;


//accessor property
typedef struct ACCESSOR {
    ALGUI_BOOL(*get)(ALGUI_OBJECT* obj, ALGUI_BUFFER* dst);
    ALGUI_BOOL(*set)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* src);
} ACCESSOR;


//object data entry
typedef struct ENTRY {
    ALGUI_OBJECT* object;
    int type;
    ALGUI_BUFFER access_token;
    union {
        EMBEDDED_VALUE embedded_value;
        STATIC_HEAP_VALUE static_heap_value;
        DYNAMIC_HEAP_VALUE dynamic_heap_value;
        ACCESSOR accessor;
        ALGUI_OBJECT_MESSAGE_HANDLER msg_handler;
    };
} ENTRY;


//sets up the access token
static void setup_access(ENTRY* entry, const ALGUI_BUFFER* access_token) {
    algui_init_buffer(&entry->access_token, NULL, 0, ALGUI_TRUE);
    if (access_token) {
        algui_copy_buffer(&entry->access_token, access_token);
    }
}


//check access to entry
static ALGUI_BOOL check_access(const ENTRY* entry, const ALGUI_BUFFER* access_token) {
    //if the entry has no access token, access is allowed
    if (algui_is_empty_buffer(&entry->access_token)) {
        return ALGUI_TRUE;
    }

    //if the access token is null/empty and the entry access token is not empty, access is not allowed
    if (access_token == NULL || algui_is_empty_buffer(access_token)) {
        return ALGUI_FALSE;
    }

    //compare the two tokens; they must be equal for access to be allowed
    return algui_memcmp_buffers(&entry->access_token, access_token) == 0;
}


//set accessor value
static ALGUI_BOOL set_accessor_value(ALGUI_OBJECT* obj, ACCESSOR* prop, const ALGUI_BUFFER* value) {
    //check the value
    if (value == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check if the prop has a setter
    if (prop->set == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //invoke the setter
    return prop->set(obj, value);
}


//compare values
static int compare_values(ALGUI_OBJECT* obj, int (*compare)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* a, const ALGUI_BUFFER* b), const ALGUI_BUFFER* a, const ALGUI_BUFFER* b) {
    return compare ? compare(obj, a, b) : algui_memcmp_buffers(a, b);
}


//initializes a value
static ALGUI_BOOL init_value(ALGUI_OBJECT* obj, ALGUI_BOOL(*init)(ALGUI_OBJECT*, const ALGUI_BUFFER*), ALGUI_BUFFER* value) {
    //if there is an init function, call it
    if (init) {
        return init(obj, value);
    }

    //init value to zeros
    memset(value->data, 0, value->size);

    //success
    return ALGUI_TRUE;
}


//copy value
static ALGUI_BOOL copy_value(ALGUI_OBJECT* obj, ALGUI_BOOL (*copy)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* dst, const ALGUI_BUFFER* src), const ALGUI_BUFFER* dst, const ALGUI_BUFFER* src) {
    //if there is a copy function, call it
    if (copy) {
        return copy(obj, dst, src);
    }

    //copy the data
    memmove(dst->data, src->data, src->size);

    //success
    return ALGUI_TRUE;
}


//initializes an embedded value
static ALGUI_BOOL init_embedded_value(ALGUI_OBJECT* obj, EMBEDDED_VALUE* prop) {
    ALGUI_BUFFER buffer;
    algui_init_buffer(&buffer, prop->value, EMBEDDED_VALUE_SIZE, ALGUI_FALSE);
    return init_value(obj, prop->init, &buffer);
}


//sets an embedded value
static ALGUI_BOOL set_embedded_value(ALGUI_OBJECT* obj, EMBEDDED_VALUE* prop, const ALGUI_BUFFER* value) {
    //check the value; if null, don't set the value
    if (value == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //if the value size is different than the prop value size, return error
    if (value->size != prop->size) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    ALGUI_BUFFER buffer;
    algui_init_buffer(&buffer, prop->value, EMBEDDED_VALUE_SIZE, ALGUI_FALSE);

    //compare values; if there is no change, do nothing else
    if (compare_values(obj, prop->compare, &buffer, value) == 0) {
        return ALGUI_TRUE;
    }

    //destroy the existing value
    if (prop->cleanup && !prop->cleanup(obj, &buffer)) {
        return ALGUI_FALSE;
    }

    //init the new value
    if (!init_value(obj, prop->init, &buffer)) {
        return ALGUI_FALSE;
    }

    //copy the value
    if (!copy_value(obj, prop->copy, &buffer, value)) {
        return ALGUI_FALSE;
    }

    //inform the object about the change
    if (prop->changed != NULL) {
        prop->changed(obj, &buffer);
    }

    //success
    return ALGUI_TRUE;
}


//initializes a static heap value
static ALGUI_BOOL init_static_heap_value(ALGUI_OBJECT* obj, STATIC_HEAP_VALUE* prop) {
    //init the buffer to the specified size
    if (!algui_init_buffer(&prop->value, NULL, prop->size, ALGUI_TRUE)) {
        return ALGUI_FALSE;
    }

    //init the value
    if (!init_value(obj,prop->init, &prop->value)) {
        return ALGUI_FALSE;
    }

    //success
    return ALGUI_TRUE;
}


//sets a static heap value
static ALGUI_BOOL set_static_heap_value(ALGUI_OBJECT* obj, STATIC_HEAP_VALUE* prop, const ALGUI_BUFFER* value) {
    //check the value; if null, don't set the value
    if (value == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //if the value size is different than the prop value size, return error
    if (value->size != prop->size) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //compare values; if there is no change, do nothing else
    if (compare_values(obj, prop->compare, &prop->value, value) == 0) {
        return ALGUI_TRUE;
    }

    //destroy the existing value
    if (prop->cleanup && !prop->cleanup(obj, &prop->value)) {
        return ALGUI_FALSE;
    }

    //init the new value
    if (!init_value(obj, prop->init, &prop->value)) {
        return ALGUI_FALSE;
    }

    //copy the value
    if (!copy_value(obj, prop->copy, &prop->value, value)) {
        return ALGUI_FALSE;
    }

    //inform the object about the change
    if (prop->changed != NULL) {
        prop->changed(obj, &prop->value);
    }

    //success
    return ALGUI_TRUE;
}


//initializes a dynamic heap value
static ALGUI_BOOL init_dynamic_heap_value(ALGUI_OBJECT* obj, DYNAMIC_HEAP_VALUE* prop) {
    //init the buffer to empty
    algui_init_buffer(&prop->value, NULL, 0, ALGUI_TRUE);

    //init the value
    return prop->init != NULL ? prop->init(obj, &prop->value) : ALGUI_TRUE;
}


//sets a dynamic heap value
static ALGUI_BOOL set_dynamic_heap_value(ALGUI_OBJECT* obj, DYNAMIC_HEAP_VALUE* prop, const ALGUI_BUFFER* value) {
    //check the value; if null, don't set the value
    if (value == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //compare values; if there is no change, do nothing else
    if (compare_values(obj, prop->compare, &prop->value, value) == 0) {
        return ALGUI_TRUE;
    }

    //destroy the existing value
    if (prop->cleanup != NULL && !prop->cleanup(obj, &prop->value)) {
        return ALGUI_FALSE;
    }

    //make room for the value
    algui_set_buffer_size(&prop->value, value->size);

    //init the new value
    if (prop->init != NULL && !prop->init(obj, &prop->value)) {
        algui_cleanup_buffer(&prop->value);
        return ALGUI_FALSE;
    }

    //copy the value
    if (!copy_value(obj, prop->copy, &prop->value, value)) {
        algui_cleanup_buffer(&prop->value);
        return ALGUI_FALSE;
    }

    //inform the object about the change
    if (prop->changed != NULL) {
        prop->changed(obj, &prop->value);
    }

    //success
    return ALGUI_TRUE;
}


//entry destructor
static void destroy_entry(void* p) {
    ENTRY* entry = (ENTRY*)p;

    algui_cleanup_buffer(&entry->access_token);

    switch (entry->type) {
        case ENTRY_TYPE_EMBEDDED_VALUE:
            if (entry->embedded_value.cleanup) {
                ALGUI_BUFFER buffer;
                algui_init_buffer(&buffer, entry->embedded_value.value, EMBEDDED_VALUE_SIZE, ALGUI_FALSE);
                entry->embedded_value.cleanup(entry->object, &buffer);
            }
            break;

        case ENTRY_TYPE_STATIC_HEAP_VALUE:
            if (entry->static_heap_value.cleanup) {
                entry->static_heap_value.cleanup(entry->object, &entry->static_heap_value.value);
            }
            algui_cleanup_buffer(&entry->static_heap_value.value);
            break;

        case ENTRY_TYPE_DYNAMIC_HEAP_VALUE:
            if (entry->dynamic_heap_value.cleanup) {
                entry->dynamic_heap_value.cleanup(entry->object, &entry->dynamic_heap_value.value);
            }
            algui_cleanup_buffer(&entry->dynamic_heap_value.value);
            break;
    }
}


//do object message implementation
static uintptr_t do_object_message(ALGUI_OBJECT* obj, int id, void* data, const ALGUI_BUFFER* access_token) {
    //actual message ids start from an offset so as that they are not mixed up with property ids
    id += MESSAGE_HANDLER_ID_OFFSET;

    //get the entry
    ENTRY* entry = (ENTRY*)algui_get_map_element(&obj->data, &id);

    //if the entry is not found, return
    if (entry == NULL) {
        errno = EINVAL;
        return 0;
    }

    //if the entry is not a message, return
    if (entry->type != ENTRY_TYPE_MSG_HANDLER) {
        errno = EINVAL;
        return 0;
    }

    //check access
    if (!check_access(entry, access_token)) {
        errno = EINVAL;
        return 0;
    }

    //invoke the message handler
    return entry->msg_handler(obj, data);
}


/*************************************************************************************************/


//init object
ALGUI_BOOL algui_init_object(ALGUI_OBJECT* obj) {
    //check the obj
    if (obj == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //init the data
    if (!algui_init_map(&obj->data, sizeof(int), sizeof(ENTRY), algui_int_comparator, NULL, destroy_entry)) {
        return ALGUI_FALSE;
    }

    //success
    return ALGUI_TRUE;
}


//clean up the object
ALGUI_BOOL algui_cleanup_object(ALGUI_OBJECT* obj) {
    //check the obj
    if (obj == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //send the cleanup message to the object
    do_object_message(obj, ALGUI_MSG_CLEANUP, NULL, NULL);

    //cleanup the data
    return algui_cleanup_map(&obj->data);
}


//create object
ALGUI_OBJECT* algui_create_object(size_t size) {
    //check the size
    if (size == 0) {
        size = sizeof(ALGUI_OBJECT);
    }
    else if (size < sizeof(ALGUI_OBJECT)) {
        errno = EINVAL;
        return NULL;
    }

    //allocate memory
    ALGUI_OBJECT* obj = (ALGUI_OBJECT*)malloc(size);

    //check allocation failure
    if (obj == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    //initialize the object
    if (!algui_init_object(obj)) {
        free(obj);
        return NULL;
    }

    //success; return the object
    return obj;
}


//destroy object
ALGUI_BOOL algui_destroy_object(ALGUI_OBJECT* obj) {
    //check the object
    if (obj == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //cleanup the object
    algui_cleanup_object(obj);

    //free the memory it occupies
    free(obj);

    //success
    return ALGUI_TRUE;
}


//define property
ALGUI_BOOL algui_define_object_property(ALGUI_OBJECT* obj, int id, const ALGUI_PROPERTY_DEFINITION* prop, const ALGUI_BUFFER* initial_value, const ALGUI_BUFFER* access_token) {
    //check the obj
    if (obj == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the prop
    if (prop == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the id
    if (id >= MESSAGE_HANDLER_ID_OFFSET) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the property type
    if (prop->type != ALGUI_PROPERTY_TYPE_ACCESSOR && prop->type != ALGUI_PROPERTY_TYPE_VALUE) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check if the entry exists; if so, return an error
    if (algui_get_map_element(&obj->data, &id) != NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }


    //prepare an entry
    ENTRY entry;

    //accessor property
    if (prop->type == ALGUI_PROPERTY_TYPE_ACCESSOR) {
        //if both set and get are NULL, then the property definition is invalid
        if (prop->accessor.get == NULL && prop->accessor.set == NULL) {
            errno = EINVAL;
            return ALGUI_FALSE;
        }

        //set the entry
        entry.type = ENTRY_TYPE_ACCESSOR;
        entry.accessor.get = prop->accessor.get;
        entry.accessor.set = prop->accessor.set;
        
        //set the value
        if (initial_value != NULL) {
            if (prop->accessor.set == NULL) {
                errno = EINVAL;
                return ALGUI_FALSE;
            }
            set_accessor_value(obj, &entry.accessor, initial_value);
        }
    }

    //else dynamic value property
    else if (prop->value.size == 0) {
        //set the entry
        entry.type = ENTRY_TYPE_DYNAMIC_HEAP_VALUE;
        entry.dynamic_heap_value.changed = prop->value.changed;
        entry.dynamic_heap_value.cleanup = prop->value.cleanup;
        entry.dynamic_heap_value.compare = prop->value.compare;
        entry.dynamic_heap_value.copy = prop->value.copy;
        entry.dynamic_heap_value.init = prop->value.init;

        //init the property value
        if (!init_dynamic_heap_value(obj, &entry.dynamic_heap_value)) {
            return ALGUI_FALSE;
        }

        //set the heap value
        if (initial_value != NULL) {
            set_dynamic_heap_value(obj, &entry.dynamic_heap_value, initial_value);
        }
    }

    //else embedded value property
    else if (prop->value.size <= EMBEDDED_VALUE_SIZE) {
        //set the entry
        entry.type = ENTRY_TYPE_EMBEDDED_VALUE;
        entry.embedded_value.changed = prop->value.changed;
        entry.embedded_value.cleanup = prop->value.cleanup;
        entry.embedded_value.compare = prop->value.compare;
        entry.embedded_value.copy = prop->value.copy;
        entry.embedded_value.init = prop->value.init;
        entry.embedded_value.size = prop->value.size;

        //init the property value
        if (!init_embedded_value(obj, &entry.embedded_value)) {
            return ALGUI_FALSE;
        }

        //set the property value
        if (initial_value != NULL) {
            set_embedded_value(obj, &entry.embedded_value, initial_value);
        }
    }

    //else static heap value property
    else {
        //set the entry
        entry.type = ENTRY_TYPE_STATIC_HEAP_VALUE;
        entry.static_heap_value.changed = prop->value.changed;
        entry.static_heap_value.cleanup = prop->value.cleanup;
        entry.static_heap_value.compare = prop->value.compare;
        entry.static_heap_value.copy = prop->value.copy;
        entry.static_heap_value.init = prop->value.init;
        entry.static_heap_value.size = prop->value.size;

        //init the property value
        if (!init_static_heap_value(obj, &entry.static_heap_value)) {
            return ALGUI_FALSE;
        }

        //set the heap value
        if (initial_value != NULL) {
            set_static_heap_value(obj, &entry.static_heap_value, initial_value);
        }
    }

    //setup the access token
    setup_access(&entry, access_token);

    //add the entry
    if (!algui_set_map_element(&obj->data, &id, &entry)) {
        destroy_entry(&entry);
        return ALGUI_FALSE;
    }

    //success
    return ALGUI_TRUE;
}


//delete object property
ALGUI_BOOL algui_delete_object_property(ALGUI_OBJECT* obj, int id, const ALGUI_BUFFER* access_token) {
    //check the obj
    if (obj == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //find the entry
    ENTRY* entry = (ENTRY*)algui_get_map_element(&obj->data, &id);

    //if the entry is not found, return false
    if (entry == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //if the entry is not a property, then return false
    if (entry->type == ENTRY_TYPE_MSG_HANDLER) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check access
    if (!check_access(entry, access_token)) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //remove the entry
    return algui_remove_map_element(&obj->data, &id);
}



//get property
ALGUI_BOOL algui_get_object_property(ALGUI_OBJECT* obj, int id, ALGUI_BUFFER* prop, const ALGUI_BUFFER* default_value, const ALGUI_BUFFER* access_token) {
    //check the obj
    if (obj == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the prop
    if (prop == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //find the entry
    ENTRY* entry = (ENTRY*)algui_get_map_element(&obj->data, &id);

    //if the entry is not found, either return the default value, if not given, or return null
    if (entry == NULL) {
        errno = EINVAL;
        goto DEFAULT_VALUE;
    }

    //access is not allowed, either return the default value, if not given, or return null
    if (!check_access(entry, access_token)) {
        errno = EINVAL;
        goto DEFAULT_VALUE;
    }

    //get the value
    switch (entry->type) {
        case ENTRY_TYPE_ACCESSOR:
            if (entry->accessor.get) {
                return entry->accessor.get(obj, prop);
            }
            errno = EINVAL;
            break;

        case ENTRY_TYPE_DYNAMIC_HEAP_VALUE:
            if (entry->dynamic_heap_value.copy) {
                algui_set_buffer_size(prop, entry->dynamic_heap_value.value.size);
                return entry->dynamic_heap_value.copy(obj, prop, &entry->dynamic_heap_value.value);
            }
            algui_copy_buffer(prop, &entry->dynamic_heap_value.value);
            return ALGUI_TRUE;

        case ENTRY_TYPE_EMBEDDED_VALUE:
            ALGUI_BUFFER buffer;
            algui_init_buffer(&buffer, entry->embedded_value.value, entry->embedded_value.size, ALGUI_FALSE);
            if (entry->embedded_value.copy) {
                return entry->embedded_value.copy(obj, prop, &buffer);
            }
            algui_copy_buffer(prop, &buffer);
            return ALGUI_TRUE;

        case ENTRY_TYPE_STATIC_HEAP_VALUE:
            if (entry->static_heap_value.copy) {
                return entry->static_heap_value.copy(obj, prop, &entry->static_heap_value.value);
            }
            algui_copy_buffer(prop, &entry->static_heap_value.value);
            return ALGUI_TRUE;
    }

    DEFAULT_VALUE:

    //return the default value, if it exists
    if (default_value) {
        algui_copy_buffer(prop, default_value);
        return ALGUI_TRUE;
    }

    //wrong property id
    errno = EINVAL;
    return ALGUI_FALSE;
}


//set property
ALGUI_BOOL algui_set_object_property(ALGUI_OBJECT* obj, int id, const ALGUI_BUFFER* prop, const ALGUI_BUFFER* access_token) {
    //check the obj
    if (obj == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the prop
    if (prop == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //find the entry
    ENTRY* entry = (ENTRY*)algui_get_map_element(&obj->data, &id);

    //if the entry is not found
    if (entry == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check access
    if (!check_access(entry, access_token)) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //set the value
    switch (entry->type) {
        case ENTRY_TYPE_ACCESSOR:
            return set_accessor_value(obj, &entry->accessor, prop);

        case ENTRY_TYPE_DYNAMIC_HEAP_VALUE:
            return set_dynamic_heap_value(obj, &entry->dynamic_heap_value, prop);

        case ENTRY_TYPE_EMBEDDED_VALUE:
            return set_embedded_value(obj, &entry->embedded_value, prop);

        case ENTRY_TYPE_STATIC_HEAP_VALUE:
            return set_static_heap_value(obj, &entry->static_heap_value, prop);
    }

    //wrong property id
    errno = EINVAL;
    return ALGUI_FALSE;
}


//retrieve an object's message handler
ALGUI_OBJECT_MESSAGE_HANDLER algui_get_object_message_handler(ALGUI_OBJECT* obj, int id, const ALGUI_BUFFER* access_token) {
    //check the obj
    if (obj == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //actual message ids start from an offset so as that they are not mixed up with property ids
    id += MESSAGE_HANDLER_ID_OFFSET;

    //locate the entry
    ENTRY* entry = (ENTRY*)algui_get_map_element(&obj->data, &id);

    //if not found
    if (entry == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the entry type
    if (entry->type != ENTRY_TYPE_MSG_HANDLER) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the access token
    if (!check_access(entry, access_token)) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //success
    return entry->msg_handler;
}


//Sets an object's message handler.
ALGUI_BOOL algui_set_object_message_handler(ALGUI_OBJECT* obj, int id, ALGUI_OBJECT_MESSAGE_HANDLER handler, const ALGUI_BUFFER* access_token, ALGUI_OBJECT_MESSAGE_HANDLER* prev_handler) {
    //check the obj
    if (obj == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the handler
    if (handler == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //actual message ids start from an offset so as that they are not mixed up with property ids
    id += MESSAGE_HANDLER_ID_OFFSET;

    //locate the previous entry
    ENTRY* entry = (ENTRY*)algui_get_map_element(&obj->data, &id);

    //if found
    if (entry) {
        //check the entry type
        if (entry->type != ENTRY_TYPE_MSG_HANDLER) {
            errno = EINVAL;
            return ALGUI_FALSE;
        }

        //check the access token
        if (!check_access(entry, access_token)) {
            errno = EINVAL;
            return ALGUI_FALSE;
        }

        //set the previous handler
        if (prev_handler) {
            *prev_handler = entry->msg_handler;
        }

        //set the entry
        entry->msg_handler = handler;
    }

    //else create new entry
    else {
        //set the previous handler
        if (prev_handler) {
            *prev_handler = NULL;
        }

        //prepare a new entry
        ENTRY e;
        e.type = ENTRY_TYPE_MSG_HANDLER;
        setup_access(&e, access_token);
        e.msg_handler = handler;

        //add the new entry
        if (algui_set_map_element(&obj->data, &id, &e) == NULL) {
            destroy_entry(&e);
            return ALGUI_FALSE;
        }
    }

    //success
    return ALGUI_TRUE;
}


//do object message
ALGUI_RESULT  algui_do_object_message(ALGUI_OBJECT* obj, int id, void* data, const ALGUI_BUFFER* access_token) {
    int original_errno = errno;

    //check the obj
    if (obj == NULL) {
        errno = EINVAL;
        return 0;
    }

    //do the message
    errno = 0;
    ALGUI_RESULT result = do_object_message(obj, id, data, access_token);

    //if there was no error, it means the message was executed, therefore return the result
    if (errno == 0) {
        return result;
    }

    //reset the error so that messaging is tried again
    errno = original_errno;

    //set up an unknown message
    ALGUI_MESSAGE unknown_message = { id, data, access_token };

    //do the unknown message
    return do_object_message(obj, ALGUI_MSG_UNKNOWN, &unknown_message, NULL);
}


//declare property functions for common types, except string which requires special handling
PROPERTY_FUNCTIONS(char);
PROPERTY_FUNCTIONS(short);
PROPERTY_FUNCTIONS(int);
PROPERTY_FUNCTIONS(long);
PROPERTY_FUNCTIONS(long_long);
PROPERTY_FUNCTIONS(unsigned_char);
PROPERTY_FUNCTIONS(unsigned_short);
PROPERTY_FUNCTIONS(unsigned_int);
PROPERTY_FUNCTIONS(unsigned_long);
PROPERTY_FUNCTIONS(unsigned_long_long);
PROPERTY_FUNCTIONS(float);
PROPERTY_FUNCTIONS(double);
PROPERTY_FUNCTIONS(long_double);
PROPERTY_FUNCTIONS(bool);
PROPERTY_FUNCTIONS(int8_t);
PROPERTY_FUNCTIONS(int16_t);
PROPERTY_FUNCTIONS(int32_t);
PROPERTY_FUNCTIONS(int64_t);
PROPERTY_FUNCTIONS(uint8_t);
PROPERTY_FUNCTIONS(uint16_t);
PROPERTY_FUNCTIONS(uint32_t);
PROPERTY_FUNCTIONS(uint64_t);
PROPERTY_FUNCTIONS(intptr_t);
PROPERTY_FUNCTIONS(uintptr_t);


//define string property
ALGUI_BOOL algui_define_string_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, const char* initial_value, const char* access_token) {
    //initial value buffer
    ALGUI_BUFFER initial_value_buffer;
    algui_init_buffer(&initial_value_buffer, (void*)initial_value, initial_value != NULL ? strlen(initial_value) : 0, ALGUI_FALSE);

    //access token buffer
    ALGUI_BUFFER access_token_buffer;
    if (access_token != NULL) {
        algui_init_buffer(&access_token_buffer, (void*)access_token, strlen(access_token), ALGUI_FALSE);
    }

    //since strings are dynamic, set the value size to ensure proper operation
    if (prop->type == ALGUI_PROPERTY_TYPE_VALUE) {
        prop->value.size = 0;
    }

    //define the property
    return algui_define_object_property(obj, id, prop, &initial_value_buffer, access_token ? &access_token_buffer : NULL);
}


//retrieve string property.
char* algui_get_string_object_property(ALGUI_OBJECT* obj, int id, const char* default_value, const char* access_token) {
    ALGUI_BUFFER result_buffer = ALGUI_BUFFER_INIT;

    //default value buffer
    ALGUI_BUFFER default_value_buffer;
    algui_init_buffer(&default_value_buffer, (void*)default_value, default_value != NULL ? strlen(default_value) : 0, ALGUI_FALSE);

    //access token buffer
    ALGUI_BUFFER access_token_buffer;
    if (access_token != NULL) {
        algui_init_buffer(&access_token_buffer, (void*)access_token, strlen(access_token), ALGUI_FALSE);
    }

    //get property
    if (!algui_get_object_property(obj, id, &result_buffer, &default_value_buffer, access_token ? &access_token_buffer : NULL)) {
        return ALGUI_FALSE;
    }

    //empty buffer
    if (result_buffer.size == 0) {
        return NULL;
    }

    //if the result buffer ends in '\0', return the buffer result
    if (result_buffer.data[result_buffer.size - 1] == '\0') {
        return result_buffer.data;
    }

    //make room for '\0' in the end
    char* null_terminated_str = realloc(result_buffer.data, result_buffer.size + 1);

    //if reallocation failed
    if (null_terminated_str == NULL) {
        errno = ENOMEM;
        algui_cleanup_buffer(&result_buffer);
        return NULL;
    }

    //set the null-terminated character
    null_terminated_str[result_buffer.size] = '\0';

    //success; buffer is not freed, it should be freed by the caller
    return null_terminated_str;
}


//set string property.
ALGUI_BOOL algui_set_string_object_property(ALGUI_OBJECT* obj, int id, const char* value, const char* access_token) {
    //value buffer
    ALGUI_BUFFER value_buffer;
    algui_init_buffer(&value_buffer, (void*)value, value != NULL ? strlen(value) : 0, ALGUI_FALSE);

    //access token buffer
    ALGUI_BUFFER access_token_buffer;
    if (access_token != NULL) {
        algui_init_buffer(&access_token_buffer, (void*)access_token, strlen(access_token), ALGUI_FALSE);
    }

    //set property
    return algui_set_object_property(obj, id, &value_buffer, access_token ? &access_token_buffer : NULL);
}
