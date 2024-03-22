#include <stdlib.h>
#include "algui/props.h"
#include "algui/enum.h"
#include "algui/static_init.h"


//property enumeration
static ALG_ENUM prop_enum;


//cleanup the enumeration at exit
static void cleanup(void) {
    alg_cleanup_enum(&prop_enum);
}


//initialize the property enumeration
static void init() {
    ALG_STATIC_INIT({
        alg_init_enum(&prop_enum);
        atexit(cleanup);
        alg_add_enum_value(&prop_enum, ALG_PROP_PROC       , "ALG_PROP_PROC"       );
        alg_add_enum_value(&prop_enum, ALG_PROP_X          , "ALG_PROP_X"          );
        alg_add_enum_value(&prop_enum, ALG_PROP_Y          , "ALG_PROP_Y"          );
        alg_add_enum_value(&prop_enum, ALG_PROP_WIDTH      , "ALG_PROP_WIDTH"      );
        alg_add_enum_value(&prop_enum, ALG_PROP_HEIGHT     , "ALG_PROP_HEIGHT"     );
        alg_add_enum_value(&prop_enum, ALG_PROP_DATA       , "ALG_PROP_DATA"       );
        alg_add_enum_value(&prop_enum, ALG_PROP_ID         , "ALG_PROP_ID"         );
        alg_add_enum_value(&prop_enum, ALG_PROP_VISIBLE    , "ALG_PROP_VISIBLE"    );
        alg_add_enum_value(&prop_enum, ALG_PROP_ENABLED    , "ALG_PROP_ENABLED"    );
        alg_add_enum_value(&prop_enum, ALG_PROP_HIGHLIGHTED, "ALG_PROP_HIGHLIGHTED");
        alg_add_enum_value(&prop_enum, ALG_PROP_PRESSED    , "ALG_PROP_PRESSED"    );
        alg_add_enum_value(&prop_enum, ALG_PROP_SELECTED   , "ALG_PROP_SELECTED"   );
        alg_add_enum_value(&prop_enum, ALG_PROP_ACTIVE     , "ALG_PROP_ACTIVE"     );
        alg_add_enum_value(&prop_enum, ALG_PROP_ERROR      , "ALG_PROP_ERROR"      );
        alg_add_enum_value(&prop_enum, ALG_PROP_FOCUSED    , "ALG_PROP_FOCUSED"    );
    });
}


//Returns a property id from property name.
int alg_get_property_id(const char* name) {
    init();
    return alg_get_enum_id(&prop_enum, name);
}


//Returns a property name from a property id.
const char* alg_get_property_name(int id) {
    init();
    return alg_get_enum_name(&prop_enum, id);
}


//Adds a property.
int alg_add_property_enum(int id, const char* name) {
    init();
    return alg_add_enum_value(&prop_enum, id, name);
}
