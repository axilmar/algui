#ifndef ALGUI_OBJECT_H
#define ALGUI_OBJECT_H


#include <errno.h>
#include "properties.h"
#include "messages.h"
#include "map.h"
#include "buffer.h"


/**
 * Macro that allows the definition of a property for a specific type.
 * Before defining the property, it sets errno to 0 to allow for error checking after the macro.
 * @param TYPE type of property.
 * @param RESULT result of call to algui_define_object_property(); boolean.
 * @param OBJECT pointer to object.
 * @param ID property id.
 * @param PROPERTY_DEFINITION instance of ALGUI_PROPERTY_DEFINITION that defines the property; value.size need not be set, it is set to sizeof(TYPE).
 * @param INITIAL_VALUE initial value of property.
 * @param ACCESS_TOKEN_STRING optional access token string.
 */
#define ALGUI_DEFINE_OBJECT_PROPERTY(TYPE, OBJECT, ID, PROPERTY_DEFINITION, INITIAL_VALUE, ACCESS_TOKEN_STRING)\
{\
    ALGUI_BUFFER initial_value;\
    TYPE initial_value_data = INITIAL_VALUE;\
    algui_init_buffer(&initial_value, &(initial_value_data), sizeof(initial_value_data), ALGUI_FALSE);\
    ALGUI_BUFFER access_token;\
    if (ACCESS_TOKEN_STRING) {\
        algui_init_buffer(&access_token, ACCESS_TOKEN_STRING, strlen(ACCESS_TOKEN_STRING), ALGUI_FALSE);\
    }\
    if ((PROPERTY_DEFINITION).type == ALGUI_PROPERTY_TYPE_VALUE) {\
        (PROPERTY_DEFINITION).value.size = sizeof(TYPE);\
    }\
    errno = 0;\
    algui_define_object_property(OBJECT, ID, &(PROPERTY_DEFINITION), &initial_value, (ACCESS_TOKEN_STRING) != NULL ? &access_token : NULL);\
}


/**
 * Macro that allows the retrieval of a property for a specific type.
 * Before getting the property, it sets errno to 0 to allow for error checking after the macro.
 * @param TYPE type of property.
 * @param RESULT variable to store the property value.
 * @param OBJECT pointer to object.
 * @param ID property id.
 * @param DEFAULT_VALUE default value of property.
 * @param ACCESS_TOKEN_STRING optional access token string.
 */
#define ALGUI_GET_OBJECT_PROPERTY(TYPE, RESULT, OBJECT, ID, DEFAULT_VALUE, ACCESS_TOKEN_STRING)\
{\
    ALGUI_BUFFER result_value;\
    algui_init_buffer(&result_value, &RESULT, sizeof(TYPE), ALGUI_FALSE);\
    ALGUI_BUFFER default_value;\
    TYPE default_value_data = DEFAULT_VALUE;\
    algui_init_buffer(&default_value, &default_value_data, sizeof(default_value_data), ALGUI_FALSE);\
    ALGUI_BUFFER access_token;\
    if (ACCESS_TOKEN_STRING) {\
        algui_init_buffer(&access_token, ACCESS_TOKEN_STRING, strlen(ACCESS_TOKEN_STRING), ALGUI_FALSE);\
    }\
    errno = 0;\
    algui_get_object_property(OBJECT, ID, &result_value, &default_value, (ACCESS_TOKEN_STRING) != NULL ? &access_token : NULL);\
}


/**
 * Macro that allows to set a property from a specific type.
 * Before setting the property, it sets errno to 0 to allow for error checking after the macro.
 * @param TYPE type of property.
 * @param OBJECT pointer to object.
 * @param ID property id.
 * @param DEFAULT_VALUE default value of property.
 * @param ACCESS_TOKEN_STRING optional access token string.
 */
#define ALGUI_SET_OBJECT_PROPERTY(TYPE, OBJECT, ID, VALUE, ACCESS_TOKEN_STRING)\
{\
    ALGUI_BUFFER value;\
    TYPE value_data = VALUE;\
    algui_init_buffer(&value, &value_data, sizeof(value_data), ALGUI_FALSE);\
    ALGUI_BUFFER access_token;\
    if (ACCESS_TOKEN_STRING) {\
        algui_init_buffer(&access_token, ACCESS_TOKEN_STRING, strlen(ACCESS_TOKEN_STRING), ALGUI_FALSE);\
    }\
    errno = 0;\
    algui_set_object_property(OBJECT, ID, &value, (ACCESS_TOKEN_STRING) != NULL ? &access_token : NULL);\
}


/**
 * An object structure.
 */
typedef struct ALGUI_OBJECT {
    ///internal map of property/message handler ids to property/message handler data.
    ALGUI_MAP data;
} ALGUI_OBJECT;


/**
 * Property type.
 */
typedef enum ALGUI_PROPERTY_TYPE {
    /**
     * Accessor type property: property managed via getter/setter.
     */
    ALGUI_PROPERTY_TYPE_ACCESSOR,

    /**
     * Value type property: property is stored in the object.
     */
    ALGUI_PROPERTY_TYPE_VALUE
} ALGUI_PROPERTY_TYPE;


/**
 * Property definition.
 */
typedef struct ALGUI_PROPERTY_DEFINITION {
    ///property type.
    ALGUI_PROPERTY_TYPE type;
    union {

        ///value property definition.
        struct {
            /**
             * Size of property value, in bytes.
             * If 0, then the size of the value is dynamic.
             * Otherwise, if it is <= sizeof(ALGUI_BUFFER), it is stored embedded into the object data,
             * else it is allocated on the heap.
             */
            size_t size;

            /**
             * Optional value initialization handler.
             * If not defined, property value bytes are initialized to 0.
             * @param obj target object.
             * @param prop buffer buffer with property data to initialize.
             * @return true on success, false on error.
             */
            ALGUI_BOOL(*init)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* prop);

            /**
             * Optional value copy handler.
             * If not defined, property values are copied via memcpy.
             * @param obj target object.
             * @param dst destination buffer.
             * @param src src buffer.
             * @return true on success, false on error.
             */
            ALGUI_BOOL(*copy)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* dst, const ALGUI_BUFFER* src);

            /**
             * Optional value cleanup handler.
             * @param obj target object.
             * @param prop buffer buffer with property data to cleanup.
             * @return true on success, false on error.
             */
            ALGUI_BOOL(*cleanup)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* prop);

            /**
             * Optional value comparison handler.
             * If not defined, property values are compared via memcmp.
             * Used to compare existing property values with new ones, when a property value changes.
             * If the two values compare equal, then no property value update is done.
             * @param obj target object.
             * @param a first buffer.
             * @param b second buffer.
             * @return negative if a < b, 0 if a == b, positive if a > b.
             */
            int (*compare)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* a, const ALGUI_BUFFER* b);
            
            /**
             * Optional handler invoked when a value property changes.
             * @param obj target object.
             * @param prop buffer buffer with property data.
             */
            void (*changed)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* prop);
        } value;

        ///accessor property definition.
        struct {
            /**
             * Getter function.
             * If not given, then the property is write-only.
             * @param obj target object.
             * @param dst buffer to put the property value into.
             * @return true on success, false on error.
             */
            ALGUI_BOOL(*get)(ALGUI_OBJECT* obj, ALGUI_BUFFER* dst);

            /**
             * Setter function.
             * If not given, then the property is read-only.
             * @param obj target object.
             * @param src buffer with value.
             * @return true on success, false on error.
             */
            ALGUI_BOOL(*set)(ALGUI_OBJECT* obj, const ALGUI_BUFFER* src);
        } accessor;
    };
} ALGUI_PROPERTY_DEFINITION;


/**
 * Result of a message handler invocation.
 */
typedef uintptr_t ALGUI_RESULT;


/**
 * Message handler type.
 * @param obj target object.
 * @param msg_data message data.
 * @return depends on message.
 */
typedef ALGUI_RESULT (*ALGUI_OBJECT_MESSAGE_HANDLER)(ALGUI_OBJECT* obj, void* msg_data);


/**
 * Initializes an object.
 * @param obj object to initialize; if null, then false is returned and errno is set to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_init_object(ALGUI_OBJECT* obj);


/**
 * Cleans up an object.
 * The object gets a cleanup message.
 * All value-type property values are destroyed.
 * All message handlers are removed.
 * @param obj object to cleanup; if null, then false is returned and errno is set to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_cleanup_object(ALGUI_OBJECT* obj);


/**
 * Defines an object property.
 * If the property is already defined, it returns false and sets errno to EINVAL.
 * @param obj object to define a property of; if null, then false is returned and errno is set to EINVAL.
 * @param id property id.
 * @param prop property definition; if null, then false is returned and errno is set to EINVAL.
 * @param initial_value buffer with initial value; if null, then no initial value is set, however the property value bytes are initialized to 0.
 * @param access_token; if not null, then the property is considered protected and the same token should be provided when getting or setting a property.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_define_object_property(ALGUI_OBJECT* obj, int id, const ALGUI_PROPERTY_DEFINITION* prop, const ALGUI_BUFFER* initial_value, const ALGUI_BUFFER* access_token);


/**
 * Deletes a property from an object.
 * @param obj object to delete a property of; if null, then false is returned and errno is set to EINVAL.
 * @param id id of property to delete; if it does not exist, then false is returned and errno is set to EINVAL.
 * @param access_token optional access token; if the property is protected, then it must match the original access token, otherwise false is returned and errno is set to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_delete_object_property(ALGUI_OBJECT* obj, int id, const ALGUI_BUFFER* access_token);


/**
 * Retrieves an object's property.
 * If the property is not defined, it returns false and sets errno to EINVAL.
 * @param obj object to get a property of; if null, then false is returned and errno is set to EINVAL.
 * @param id property id.
 * @param prop buffer to receive the property; if null, or has invalid size, then false is returned and errno is set to EINVAL.
 * @param default_value optional; returned if the property value does not exist or it is not accessible; then the result is true, but errno remains set to EINVAL.
 * @param access_token; optional; if the property is protected, then it must be the right token in order to access the property; otherwise, the result is false and errno is set to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_get_object_property(ALGUI_OBJECT* obj, int id, ALGUI_BUFFER* prop, const ALGUI_BUFFER* default_value, const ALGUI_BUFFER* access_token);


/**
 * Sets an object's property.
 * If the property is not defined, it returns false and sets errno to EINVAL.
 * @param obj object to set a property of; if null, then false is returned and errno is set to EINVAL.
 * @param id property id.
 * @param prop buffer that contains the property value; if null, or has invalid size, then false is returned and errno is set to EINVAL.
 * @param access_token; optional; if the property is protected, then it must be the right token in order to access the property; otherwise, the result is false and errno is set to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_set_object_property(ALGUI_OBJECT* obj, int id, const ALGUI_BUFFER* prop, const ALGUI_BUFFER* access_token);


/**
 * Returns the message handler for an object message handler id.
 * @param obj object to get a handler of; if null, then false is returned and errno is set to EINVAL.
 * @param id id of message handler.
 * @param access_token; optional; if the handler is protected, then it must be the right token in order to access the handler; otherwise, the result is false and errno is set to EINVAL.
 * @return the message handler if it exists, null otherwise or on error.
 */
ALGUI_OBJECT_MESSAGE_HANDLER algui_get_object_message_handler(ALGUI_OBJECT* obj, int id, const ALGUI_BUFFER* access_token);


/**
 * Sets the message handler of an object.
 * @param obj object to set the message handler of; if null, then false is returned and errno is set to EINVAL.
 * @param id id of message handler.
 * @param handler pointer to handler; if null, then false is returned and errno is set to EINVAL.
 * @param access_token; optional; if an existing handler is protected, then it must be the right token in order to access the handler; otherwise, the result is false and errno is set to EINVAL.
 *  For new handlers, it is the handler's access token.
 * @param prev_handler previous handler; optional.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_set_object_message_handler(ALGUI_OBJECT* obj, int id, ALGUI_OBJECT_MESSAGE_HANDLER handler, const ALGUI_BUFFER* access_token, ALGUI_OBJECT_MESSAGE_HANDLER* prev_handler);


/**
 * Executes a message handler for an object with the specific message handler id.
 * If the object does not have a message handler for it, then the unknown message is sent to the object with data pointing to an ALGUI_MESSAGE structure.
 * @param obj target object; if null, then false is returned and errno is set to EINVAL.
 * @param id id of message handler.
 * @param data message data; optional.
 * @param access_token; optional; if the handler is protected, then it must be the right token in order to access the handler; otherwise, the result is false and errno is set to EINVAL.
 * @return what the message handler returns or 0 if there was an error.
 */
ALGUI_RESULT algui_do_object_message(ALGUI_OBJECT* obj, int id, void* data, const ALGUI_BUFFER* access_token);


/**
 * Defines a property of type char.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_char_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, char initial_value, const char* access_token);


/**
 * Retrieves a property of type char.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
char algui_get_char_object_property(ALGUI_OBJECT* obj, int id, char default_value, const char* access_token);


/**
 * Sets a property of type char.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_char_object_property(ALGUI_OBJECT* obj, int id, char value, const char* access_token);


/**
 * Defines a property of type short.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_short_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, short initial_value, const char* access_token);


/**
 * Retrieves a property of type short.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
short algui_get_short_object_property(ALGUI_OBJECT* obj, int id, short default_value, const char* access_token);


/**
 * Sets a property of type short.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_short_object_property(ALGUI_OBJECT* obj, int id, short value, const char* access_token);


/**
 * Defines a property of type int.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_int_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, int initial_value, const char* access_token);


/**
 * Retrieves a property of type int.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
int algui_get_int_object_property(ALGUI_OBJECT* obj, int id, int default_value, const char* access_token);


/**
 * Sets a property of type int.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_int_object_property(ALGUI_OBJECT* obj, int id, int value, const char* access_token);


/**
 * Defines a property of type long.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_long_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, long initial_value, const char* access_token);


/**
 * Retrieves a property of type long.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
long algui_get_long_object_property(ALGUI_OBJECT* obj, int id, long default_value, const char* access_token);


/**
 * Sets a property of type long.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_long_object_property(ALGUI_OBJECT* obj, int id, long value, const char* access_token);


/**
 * Defines a property of type long long.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_long_long_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, long long initial_value, const char* access_token);


/**
 * Retrieves a property of type long long.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
long long algui_get_long_long_object_property(ALGUI_OBJECT* obj, int id, long long default_value, const char* access_token);


/**
 * Sets a property of type long long.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_long_long_object_property(ALGUI_OBJECT* obj, int id, long long value, const char* access_token);


/**
 * Defines a property of type unsigned char.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_unsigned_char_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, unsigned char initial_value, const char* access_token);


/**
 * Retrieves a property of type unsigned char.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
unsigned char algui_get_unsigned_char_object_property(ALGUI_OBJECT* obj, int id, unsigned char default_value, const char* access_token);


/**
 * Sets a property of type unsigned char.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_unsigned_char_object_property(ALGUI_OBJECT* obj, int id, unsigned char value, const char* access_token);


/**
 * Defines a property of type unsigned short.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_unsigned_short_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, unsigned short initial_value, const char* access_token);


/**
 * Retrieves a property of type unsigned short.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
unsigned short algui_get_unsigned_short_object_property(ALGUI_OBJECT* obj, int id, unsigned short default_value, const char* access_token);


/**
 * Sets a property of type unsigned short.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_unsigned_short_object_property(ALGUI_OBJECT* obj, int id, unsigned short value, const char* access_token);


/**
 * Defines a property of type unsigned int.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_unsigned_int_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, unsigned int initial_value, const char* access_token);


/**
 * Retrieves a property of type unsigned int.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
unsigned int algui_get_unsigned_int_object_property(ALGUI_OBJECT* obj, int id, unsigned int default_value, const char* access_token);


/**
 * Sets a property of type unsigned int.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_unsigned_int_object_property(ALGUI_OBJECT* obj, int id, unsigned int value, const char* access_token);


/**
 * Defines a property of type unsigned long.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_unsigned_long_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, unsigned long initial_value, const char* access_token);


/**
 * Retrieves a property of type unsigned long.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
unsigned long algui_get_unsigned_long_object_property(ALGUI_OBJECT* obj, int id, unsigned long default_value, const char* access_token);


/**
 * Sets a property of type unsigned long.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_unsigned_long_object_property(ALGUI_OBJECT* obj, int id, unsigned long value, const char* access_token);


/**
 * Defines a property of type unsigned long long.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_unsigned_long_long_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, unsigned long long initial_value, const char* access_token);


/**
 * Retrieves a property of type unsigned long long.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
unsigned long long algui_get_unsigned_long_long_object_property(ALGUI_OBJECT* obj, int id, unsigned long long default_value, const char* access_token);


/**
 * Sets a property of type unsigned long long.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_unsigned_long_long_object_property(ALGUI_OBJECT* obj, int id, unsigned long long value, const char* access_token);


/**
 * Defines a property of type float.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_float_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, float initial_value, const char* access_token);


/**
 * Retrieves a property of type float.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
float algui_get_float_object_property(ALGUI_OBJECT* obj, int id, float default_value, const char* access_token);


/**
 * Sets a property of type float.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_float_object_property(ALGUI_OBJECT* obj, int id, float value, const char* access_token);


/**
 * Defines a property of type double.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_double_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, double initial_value, const char* access_token);


/**
 * Retrieves a property of type double.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
double algui_get_double_object_property(ALGUI_OBJECT* obj, int id, double default_value, const char* access_token);


/**
 * Sets a property of type double.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_double_object_property(ALGUI_OBJECT* obj, int id, double value, const char* access_token);


/**
 * Defines a property of type long double.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_long_double_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, long double initial_value, const char* access_token);


/**
 * Retrieves a property of type long double.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
long double algui_get_long_double_object_property(ALGUI_OBJECT* obj, int id, long double default_value, const char* access_token);


/**
 * Sets a property of type long double.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_long_double_object_property(ALGUI_OBJECT* obj, int id, long double value, const char* access_token);


/**
 * Defines a property of type ALGUI_BOOL.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_bool_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, ALGUI_BOOL initial_value, const char* access_token);


/**
 * Retrieves a property of type ALGUI_BOOL.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
ALGUI_BOOL algui_get_bool_object_property(ALGUI_OBJECT* obj, int id, ALGUI_BOOL default_value, const char* access_token);


/**
 * Sets a property of type ALGUI_BOOL.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_bool_object_property(ALGUI_OBJECT* obj, int id, ALGUI_BOOL value, const char* access_token);


/**
 * Defines a property of type string.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_string_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, const char* initial_value, const char* access_token);


/**
 * Retrieves a property of type string.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or a copy of the default value, if there is an error; in both cases, the result should be freed by the caller; for error handling, see the function algui_get_object_property().
 */
char* algui_get_string_object_property(ALGUI_OBJECT* obj, int id, const char* default_value, const char* access_token);


/**
 * Sets a property of type string.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_string_object_property(ALGUI_OBJECT* obj, int id, const char* value, const char* access_token);


/**
 * Defines a property of type pointer.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_ptr_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, const void* initial_value, const char* access_token);


/**
 * Retrieves a property of type pointer.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or a copy of the default value, if there is an error; in both cases, the result should be freed by the caller; for error handling, see the function algui_get_object_property().
 */
void* algui_get_ptr_object_property(ALGUI_OBJECT* obj, int id, void* default_value, const char* access_token);


/**
 * Sets a property of type pointer.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_ptr_object_property(ALGUI_OBJECT* obj, int id, void* value, const char* access_token);


/**
 * Defines a property of type int8_t.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_int8_t_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, int8_t initial_value, const char* access_token);


/**
 * Retrieves a property of type int8_t.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
int8_t algui_get_int8_t_object_property(ALGUI_OBJECT* obj, int id, int8_t default_value, const char* access_token);


/**
 * Sets a property of type int8_t.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_int8_t_object_property(ALGUI_OBJECT* obj, int id, int8_t value, const char* access_token);


/**
 * Defines a property of type int16_t.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_int16_t_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, int16_t initial_value, const char* access_token);


/**
 * Retrieves a property of type int16_t.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
int16_t algui_get_int16_t_object_property(ALGUI_OBJECT* obj, int id, int16_t default_value, const char* access_token);


/**
 * Sets a property of type int16_t.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_int16_t_object_property(ALGUI_OBJECT* obj, int id, int16_t value, const char* access_token);


/**
 * Defines a property of type int32_t.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_int32_t_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, int32_t initial_value, const char* access_token);


/**
 * Retrieves a property of type int32_t.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
int32_t algui_get_int32_t_object_property(ALGUI_OBJECT* obj, int id, int32_t default_value, const char* access_token);


/**
 * Sets a property of type int32_t.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_int32_t_object_property(ALGUI_OBJECT* obj, int id, int32_t value, const char* access_token);


/**
 * Defines a property of type int64_t.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_int64_t_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, int64_t initial_value, const char* access_token);


/**
 * Retrieves a property of type int64_t.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
int64_t algui_get_int64_t_object_property(ALGUI_OBJECT* obj, int id, int64_t default_value, const char* access_token);


/**
 * Sets a property of type int64_t.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_int64_t_object_property(ALGUI_OBJECT* obj, int id, int64_t value, const char* access_token);


/**
 * Defines a property of type uint8_t.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_uint8_t_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, uint8_t initial_value, const char* access_token);


/**
 * Retrieves a property of type uint8_t.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
uint8_t algui_get_uint8_t_object_property(ALGUI_OBJECT* obj, int id, uint8_t default_value, const char* access_token);


/**
 * Sets a property of type uint8_t.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_uint8_t_object_property(ALGUI_OBJECT* obj, int id, uint8_t value, const char* access_token);


/**
 * Defines a property of type uint16_t.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_uint16_t_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, uint16_t initial_value, const char* access_token);


/**
 * Retrieves a property of type uint16_t.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
uint16_t algui_get_uint16_t_object_property(ALGUI_OBJECT* obj, int id, uint16_t default_value, const char* access_token);


/**
 * Sets a property of type uint16_t.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_uint16_t_object_property(ALGUI_OBJECT* obj, int id, uint16_t value, const char* access_token);


/**
 * Defines a property of type uint32_t.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_uint32_t_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, uint32_t initial_value, const char* access_token);


/**
 * Retrieves a property of type uint32_t.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
uint32_t algui_get_uint32_t_object_property(ALGUI_OBJECT* obj, int id, uint32_t default_value, const char* access_token);


/**
 * Sets a property of type uint32_t.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_uint32_t_object_property(ALGUI_OBJECT* obj, int id, uint32_t value, const char* access_token);


/**
 * Defines a property of type uint64_t.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_uint64_t_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, uint64_t initial_value, const char* access_token);


/**
 * Retrieves a property of type uint64_t.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
uint64_t algui_get_uint64_t_object_property(ALGUI_OBJECT* obj, int id, uint64_t default_value, const char* access_token);


/**
 * Sets a property of type uint64_t.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_uint64_t_object_property(ALGUI_OBJECT* obj, int id, uint64_t value, const char* access_token);


/**
 * Defines a property of type intptr_t.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_intptr_t_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, intptr_t initial_value, const char* access_token);


/**
 * Retrieves a property of type intptr_t.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
intptr_t algui_get_intptr_t_object_property(ALGUI_OBJECT* obj, int id, intptr_t default_value, const char* access_token);


/**
 * Sets a property of type intptr_t.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_intptr_t_object_property(ALGUI_OBJECT* obj, int id, intptr_t value, const char* access_token);


/**
 * Defines a property of type uintptr_t.
 * Shortcut for algui_define_object_property().
 * @param obj target object.
 * @param id property id.
 * @param prop property definition.
 * @param initial_value initial value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_define_object_property().
 */
ALGUI_BOOL algui_define_uintptr_t_object_property(ALGUI_OBJECT* obj, int id, ALGUI_PROPERTY_DEFINITION* prop, uintptr_t initial_value, const char* access_token);


/**
 * Retrieves a property of type uintptr_t.
 * Shortcut for algui_get_object_property().
 * @param obj target object.
 * @param id property id.
 * @param default_value value to return in case of error.
 * @param access_token optional access token.
 * @return The property value or the default value, if there is an error; for error handling, see the function algui_get_object_property().
 */
uintptr_t algui_get_uintptr_t_object_property(ALGUI_OBJECT* obj, int id, uintptr_t default_value, const char* access_token);


/**
 * Sets a property of type uintptr_t.
 * Shortcut for algui_set_object_property().
 * @param obj target object.
 * @param id property id.
 * @param value property value.
 * @param access_token optional access token.
 * @return true on success, false on error; for error handling, see the function algui_set_object_property().
 */
ALGUI_BOOL algui_set_uintptr_t_object_property(ALGUI_OBJECT* obj, int id, uintptr_t value, const char* access_token);


#endif //ALGUI_OBJECT_H
