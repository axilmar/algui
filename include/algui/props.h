#ifndef ALG_PROPS_H
#define ALG_PROPS_H


/**
 * Properties enumeration.
 */
enum ALG_PROP {
    ///null property used for terminating the property list.
    ALG_PROP_NULL,

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

    ///first available prop id for apps.
    ALG_PROP_USER = 10000
};


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
 * Adds a property.
 * @param id property id; should not be 0.
 * @param name property name; should not be null.
 * @return non-zero on success, zero on reallocation error or if id is 0 or if name is null.
 */
int alg_add_property_enum(int id, const char* name);


#endif //ALG_PROPS_H
