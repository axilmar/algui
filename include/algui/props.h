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
 * Returns the name of a property.
 * @param prop_id id of property.
 * @return name of property.
 */
const char* alg_get_prop_name(int prop_id);


/**
 * Returns the id of a property.
 * @param name property name.
 * @return the id of a property or 0 if not found.
 */
int alg_get_prop_id(const char* name);


#endif //ALG_PROPS_H
