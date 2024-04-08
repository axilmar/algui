#include <errno.h>
#include "algui/enum.h"


//entry
typedef struct ENUM_ENTRY {
    const char* name;
    const void* metadata;
} ENUM_ENTRY;


//Initializes an enumeration.
ALGUI_BOOL algui_init_enum(ALGUI_ENUM* en) {
    //check the enumeration
    if (en == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //init the id to name map
    if (algui_init_map(&en->id_to_entry, sizeof(int), sizeof(ENUM_ENTRY), algui_int_comparator, NULL, NULL) == ALGUI_FALSE) {
        return ALGUI_FALSE;
    }

    //init the name to id map
    if (algui_init_map(&en->name_to_id, sizeof(const char*), sizeof(int), algui_string_comparator, NULL, NULL) == ALGUI_FALSE) {
        return ALGUI_FALSE;
    }

    //success
    return ALGUI_TRUE;
}


//Cleans up an enumeration.
ALGUI_BOOL algui_cleanup_enum(ALGUI_ENUM* en) {
    //check the enumeration
    if (en == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //cleanup the maps
    ALGUI_BOOL ok = algui_cleanup_map(&en->id_to_entry);
    ok = algui_cleanup_map(&en->name_to_id) && ok;

    //return map operations success
    return ok;
}


//Adds an enumerated value to an enumeration.
ALGUI_BOOL algui_add_enum_value(ALGUI_ENUM* en, int id, const char* name, const void* metadata) {
    //check the enumeration
    if (en == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the name
    if (name == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //add the values to maps
    const ENUM_ENTRY entry = { name, metadata };
    ALGUI_BOOL ok = algui_set_map_element(&en->id_to_entry, &id, &entry) != NULL;
    ok = algui_set_map_element(&en->name_to_id, &name, &id) != NULL && ok;

    //return map operations success
    return ok;
}


//returns the name of an enumerated value.
const char* algui_get_enum_value_name(ALGUI_ENUM* en, int id) {
    //check the enumeration
    if (en == NULL) {
        errno = EINVAL;
        return NULL;
    }

    //get the value
    const ENUM_ENTRY* entry = (ENUM_ENTRY*)algui_get_map_element(&en->id_to_entry, &id);

    //return the result
    return entry ? entry->name : NULL;
}


//returns the meta data of an enum value.
const void* algui_get_enum_value_metadata(ALGUI_ENUM* en, int id) {
    //check the enumeration
    if (en == NULL) {
        errno = EINVAL;
        return NULL;
    }

    //get the value
    const ENUM_ENTRY* entry = (ENUM_ENTRY*)algui_get_map_element(&en->id_to_entry, &id);

    //return the result
    return entry ? entry->metadata : NULL;
}


//Returns the id of an enumerated value.
int algui_get_enum_value_id(ALGUI_ENUM* en, const char* name) {
    //check the enumeration
    if (en == NULL) {
        errno = EINVAL;
        return -1;
    }

    //check the name
    if (name == NULL) {
        return -1;
    }

    //get the value
    const void* valueptr = algui_get_map_element(&en->name_to_id, &name);

    //return the result
    return valueptr != NULL ? *(int*)valueptr : -1;
}
