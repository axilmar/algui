#ifndef ALGUI_ENUM_H
#define ALGUI_ENUM_H


#include "map.h"


/**
 * Enumeration.
 */
typedef struct ALGUI_ENUM {
    ALGUI_MAP id_to_entry;
    ALGUI_MAP name_to_id;
} ALGUI_ENUM;


/**
 * Initializes an enumeration.
 * @param en enumeration to initialize; if null, then false is returned and errno is set to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_init_enum(ALGUI_ENUM* en);


/**
 * Cleans up an enumeration.
 * @param en enumeration to cleanup; if null, then false is returned and errno is set to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_cleanup_enum(ALGUI_ENUM* en);


/**
 * Adds an enumerated value to an enumeration.
 * @param en enumeration to add the value to; if null, then false is returned and errno is set to EINVAL.
 * @param id id of enumerated value. If it exists, then it replaces an existing value.
 * @param name of enumerated value; if null, then false is returned and errno is set to EINVAL.
 * @param metadata metadata to attach to each entry.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_add_enum_value(ALGUI_ENUM* en, int id, const char* name, const void* metadata);


/**
 * Returns the name of an enumerated value.
 * @param en enumeration that contains the value; if null, then NULL is returned and errno is set to EINVAL.
 * @param id enumeration value id.
 * @return pointer to name of the value on success, NULL on error.
 */
const char* algui_get_enum_value_name(ALGUI_ENUM* en, int id);


/**
 * Returns the metadata of an enumerated value.
 * @param en enumeration that contains the value; if null, then NULL is returned and errno is set to EINVAL.
 * @param id enumeration value id.
 * @return pointer to the meta data of an enum value on success, NULL on error.
 */
const void* algui_get_enum_value_metadata(ALGUI_ENUM* en, int id);


/**
 * Returns the id of an enumerated value.
 * @param en enumeration that contains the value; if null, then -1 is returned and errno is set to EINVAL.
 * @param name the enumerated value name.
 * @return pointer to id of the value on success, -1 on error.
 */
int algui_get_enum_value_id(ALGUI_ENUM* en, const char* name);


#endif //ALGUI_ENUM_H
