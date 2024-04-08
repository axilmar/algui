#include <stdlib.h>
#include <errno.h>
#include "algui/properties.h"
#include "algui/enum.h"
#include "algui/static_init.h"


static ALGUI_ENUM properties_enum;


//cleanup at exit
static void cleanup(void) {
    algui_cleanup_enum(&properties_enum);
}


//init function
static void init_properties_func(void) {
    algui_init_enum(&properties_enum);
    atexit(cleanup);
    algui_add_enum_value(&properties_enum, ALGUI_PROP_ID, "ALGUI_PROP_ID", NULL);
    algui_add_enum_value(&properties_enum, ALGUI_PROP_NAME, "ALGUI_PROP_NAME", NULL);
}


//init properties
static void init_properties() {
    ALGUI_STATIC_INIT(init_properties_func);
}


//add a property
ALGUI_BOOL algui_register_property(int id, const char* name) {
    init_properties();

    if (id == ALGUI_PROP_NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    if (name == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    return algui_add_enum_value(&properties_enum, id, name, NULL);
}


//get property id
int algui_get_property_id(const char* name) {
    init_properties();

    if (name == NULL) {
        errno = EINVAL;
        return ALGUI_PROP_NULL;
    }

    return algui_get_enum_value_id(&properties_enum, name);
}


//get property name
const char* algui_get_property_name(int id) {
    init_properties();

    if (id == ALGUI_PROP_NULL) {
        errno = EINVAL;
        return NULL;
    }

    return algui_get_enum_value_name(&properties_enum, id);
}
