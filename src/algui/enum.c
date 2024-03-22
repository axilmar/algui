#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "algui/enum.h"


//enum entry
typedef struct ENUM_ENTRY {
    uintptr_t id;
    const char* name;
} ENUM_ENTRY;


//Initializes an enumeration.
void alg_init_enum(ALG_ENUM* en) {
    assert(en);
    alg_init_vector(&en->vec, sizeof(ENUM_ENTRY), 0, NULL, 0);
}


//Cleans up an enum.
void alg_cleanup_enum(ALG_ENUM* en) {
    assert(en);
    alg_cleanup_vector(&en->vec);
}


//Returns the name of an enum, from its id.
const char* alg_get_enum_name(ALG_ENUM* en, int id) {
    assert(en);
    for (int i = 0; i < alg_get_vector_size(&en->vec); ++i) {
        ENUM_ENTRY* ee = (ENUM_ENTRY*)alg_get_vector_element(&en->vec, i);
        if (ee->id == id) {
            return ee->name;
        }
    }
    return NULL;
}


//Returns the id of an enum, from its name.
int alg_get_enum_id(ALG_ENUM* en, const char* name) {
    assert(en);
    assert(name);
    for (int i = 0; i < alg_get_vector_size(&en->vec); ++i) {
        ENUM_ENTRY* ee = (ENUM_ENTRY*)alg_get_vector_element(&en->vec, i);
        if (strcmp(ee->name, name)) {
            return ee->id;
        }
    }
    return 0;

}


//Adds a value to an enum.
int alg_add_enum_value(ALG_ENUM* en, int id, const char* name) {
    assert(en);

    //check params
    if (id == 0 || name == NULL) {
        return 0;
    }

    //add one entry
    int index = alg_get_vector_size(&en->vec);
    if (!alg_set_vector_size(&en->vec, index + 1)) {
        return 0;
    }

    //set element
    ENUM_ENTRY ee = { id, name };
    alg_set_vector_element(&en->vec, index, &ee);

    //success
    return 1;
}


