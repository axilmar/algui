#ifndef ALGUI_PROPERTIES_H
#define ALGUI_PROPERTIES_H


#include <stdint.h>
#include "buffer.h"


/**
 * Property ids.
 */
enum ALGUI_PROP_ID {
    ///null property id.
    ALGUI_PROP_NULL,

    ///id property.
    ALGUI_PROP_ID,

    ///name property.
    ALGUI_PROP_NAME,

    ///last property.
    ALGUI_PROP_LAST = 10000
};


/**
 * Registers a property.
 * @param id property id; if null id, then false is returned and errno is set to EINVAL.
 * @param name property name; if null, then false is returned and errno is set to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_register_property(int id, const char* name);


/**
 * Returns a property id from property name.
 * @param name name of property; if null, then null id is returned and errno is set to EINVAL.
 * @return on success, the property id; on error, the null id enum value.
 */
int algui_get_property_id(const char* name);


/**
 * Returns a property name from property id.
 * @param id id of property; if null id, then null is returned and errno is set to EINVAL.
 * @return on success, the property name; on error, NULL.
 */
const char* algui_get_property_name(int id);


#endif // ALGUI_PROPERTIES_H
