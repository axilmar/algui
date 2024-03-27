#ifndef ALG_PROPS_H
#define ALG_PROPS_H


#include <stdarg.h>


/**
 * Properties enumeration.
 */
enum ALG_PROP {
    ///null property used for terminating the property list.
    ALG_PROP_NULL = 0,

    ///widget procedure.
    ALG_PROP_PROC,

    ///x property.
    ALG_PROP_X,

    ///y property.
    ALG_PROP_Y,

    ///width of widget.
    ALG_PROP_WIDTH,

    ///height of widget.
    ALG_PROP_HEIGHT,

    ///user data of widget.
    ALG_PROP_DATA,

    ///id of widget.
    ALG_PROP_ID,

    ///visible state of widget.
    ALG_PROP_VISIBLE,

    ///enabled state of widget.
    ALG_PROP_ENABLED,

    ///highlighted state of widget.
    ALG_PROP_HIGHLIGHTED,

    ///pressed state of widget.
    ALG_PROP_PRESSED,

    ///selected state of widget.
    ALG_PROP_SELECTED,

    ///active state of widget.
    ALG_PROP_ACTIVE,

    ///error state of widget.
    ALG_PROP_ERROR,

    ///focused state of widget.
    ALG_PROP_FOCUSED,

    ///if the widget participates in the layout management system.
    ALG_PROP_MANAGED_LAYOUT,

    ///first available prop id for apps.
    ALG_PROP_LAST
};


/**
 * Function for reading a property from a va list and returning a pointer to its value.
 * @param list variable-argument list to get the property of.
 * @return pointer to the property.
 */
typedef void* (*ALG_READ_PROPERTY_PROC)(va_list* list);


/**
 * Returns a property id from property name.
 * @param name property name.
 * @return id of property or 0 if not found.
 */
int alg_get_property_id(const char* name);


/**
 * Returns a property name from a property id.
 * @param id property id.
 * @return property name or null if not found.
 */
const char* alg_get_property_name(int id);


/**
 * Invokes the internal handler for reading a property off a va list.
 * @param id property id.
 * @param list property list.
 * @return pointer to property value or null if id is invalid.
 */
void* alg_read_property(int id, va_list* list);


/**
 * Reads an integer property from a va list.
 * @param list va list to read the property of.
 * @return pointer to property.
 */
void* alg_read_int_property(va_list* list);


/**
 * Reads an pointer-to-void property from a va list.
 * @param list va list to read the property of.
 * @return pointer to property.
 */
void* alg_read_pvoid_property(va_list* list);


/**
 * Adds a property.
 * @param id property id; should be greater than 0.
 * @param name property name; should not be null.
 * @param proc procedure to call for reading the property off a va list.
 * @return non-zero on success, zero on reallocation error or if id is 0 or if name is null.
 */
int alg_register_property(int id, const char* name, ALG_READ_PROPERTY_PROC proc);


#endif //ALG_PROPS_H
