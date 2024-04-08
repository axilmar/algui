#ifndef ALGUI_OBJECT_H
#define ALGUI_OBJECT_H


#include "properties.h"
#include "messages.h"
#include "map.h"
#include "buffer.h"


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
 * @param obj target object; if null, then false is returned and errno is set to EINVAL.
 * @param id id of message handler.
 * @param data message data; optional.
 * @param access_token; optional; if the handler is protected, then it must be the right token in order to access the handler; otherwise, the result is false and errno is set to EINVAL.
 * @return what the message handler returns or 0 if there was an error.
 */
ALGUI_RESULT algui_do_object_message(ALGUI_OBJECT* obj, int id, void* data, const ALGUI_BUFFER* access_token);


#endif //ALGUI_OBJECT_H
