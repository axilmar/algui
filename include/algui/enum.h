#ifndef ALG_ENUM_H
#define ALG_ENUM_H


#include "vector.h"


/**
 * An enumeration.
 * It provides functions for setting and retrieving ids and names of ids at runtime.
 */
typedef struct ALG_ENUM {
    ALG_VECTOR vec;
} ALG_ENUM;


/**
 * Initializes an enumeration.
 * @param en enumeration to initialize.
 */
void alg_init_enum(ALG_ENUM* en);


/**
 * Cleans up an enum.
 * @param en enumeration to cleanup.
 */
void alg_cleanup_enum(ALG_ENUM* en);


/**
 * Returns the name of an enum, from its id.
 * @param en enum to get the name of.
 * @param id the id of the enum value.
 * @return name of enum or null if not found.
 */
const char* alg_get_enum_name(ALG_ENUM* en, int id);


/**
 * Returns the id of an enum, from its name.
 * @param en enum to get the id of.
 * @param name the name of the enum value.
 * @return the id of the enum value or 0 if not found.
 */
int alg_get_enum_id(ALG_ENUM* en, const char* name);


/**
 * Adds a value to an enum.
 * @param en enum to add the value to.
 * @param id id of the enum value.
 * @param name name of the enum value.
 * @return non-zero on success, zero on reallocation error or if id is 0 or name is null.
 */
int alg_add_enum_value(ALG_ENUM* en, int id, const char* name);


#endif //ALG_ENUM_H
