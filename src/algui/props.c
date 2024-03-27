#include <stdlib.h>
#include <assert.h>
#include "algui/props.h"
#include "algui/enum.h"
#include "algui/static_init.h"
#include "algui/vector.h"


//property enumeration
static ALG_ENUM prop_enums;


//table of property read procs
static ALG_VECTOR read_prop_procs;


//cleanup the enumeration at exit
static void cleanup(void) {
    alg_cleanup_enum(&prop_enums);
    alg_cleanup_vector(&read_prop_procs);
}


//adds an enum value and a size
static int add_property(int id, const char* name, ALG_READ_PROPERTY_PROC proc) {
    return alg_add_enum_value(&prop_enums, id, name) && alg_resize_and_set_vector_element(&read_prop_procs, id, &proc);
}


//read int prop
static void* read_int_proc(va_list* list) {
    return &va_arg(*list, int);
}


//read void* prop
static void* read_pvoid_proc(va_list* list) {
    return &va_arg(*list, void*);
}


//initialize the property enumeration
static void init() {
    ALG_STATIC_INIT({
        alg_init_enum(&prop_enums);
        alg_init_vector(&read_prop_procs, sizeof(ALG_READ_PROPERTY_PROC), 0, NULL, 0);
        atexit(cleanup);
        add_property(ALG_PROP_PROC          , "ALG_PROP_PROC"          , read_pvoid_proc);
        add_property(ALG_PROP_X             , "ALG_PROP_X"             , read_int_proc  );
        add_property(ALG_PROP_Y             , "ALG_PROP_Y"             , read_int_proc  );
        add_property(ALG_PROP_WIDTH         , "ALG_PROP_WIDTH"         , read_int_proc  );
        add_property(ALG_PROP_HEIGHT        , "ALG_PROP_HEIGHT"        , read_int_proc  );
        add_property(ALG_PROP_DATA          , "ALG_PROP_DATA"          , read_int_proc  );
        add_property(ALG_PROP_ID            , "ALG_PROP_ID"            , read_pvoid_proc);
        add_property(ALG_PROP_VISIBLE       , "ALG_PROP_VISIBLE"       , read_int_proc  );
        add_property(ALG_PROP_ENABLED       , "ALG_PROP_ENABLED"       , read_int_proc  );
        add_property(ALG_PROP_HIGHLIGHTED   , "ALG_PROP_HIGHLIGHTED"   , read_int_proc  );
        add_property(ALG_PROP_PRESSED       , "ALG_PROP_PRESSED"       , read_int_proc  );
        add_property(ALG_PROP_SELECTED      , "ALG_PROP_SELECTED"      , read_int_proc  );
        add_property(ALG_PROP_ACTIVE        , "ALG_PROP_ACTIVE"        , read_int_proc  );
        add_property(ALG_PROP_ERROR         , "ALG_PROP_ERROR"         , read_int_proc  );
        add_property(ALG_PROP_FOCUSED       , "ALG_PROP_FOCUSED"       , read_int_proc  );
        add_property(ALG_PROP_MANAGED_LAYOUT, "ALG_PROP_MANAGED_LAYOUT", read_int_proc  );
    });
}


//Returns a property id from property name.
int alg_get_property_id(const char* name) {
    init();
    return alg_get_enum_id(&prop_enums, name);
}


//Returns a property name from a property id.
const char* alg_get_property_name(int id) {
    init();
    return alg_get_enum_name(&prop_enums, id);
}


//read property
void* alg_read_property(int id, va_list* list) {
    init();
    if (id >= 0 && id < alg_get_vector_size(&read_prop_procs)) {
        ALG_READ_PROPERTY_PROC* proc = (ALG_READ_PROPERTY_PROC*)alg_get_vector_element(&read_prop_procs, id);
        return (*proc)(list);
    }
    return NULL;
}


//Adds a property.
int alg_add_property_enum(int id, const char* name, ALG_READ_PROPERTY_PROC proc) {
    init();
    assert(id > 0);
    assert(name);
    assert(proc);
    return add_property(id, name, proc);
}
