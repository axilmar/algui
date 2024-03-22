#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "algui/props.h"


typedef struct PROPERTY {
    int id;
    const char* name;
} PROPERTY;


static PROPERTY property[] = {
    { ALG_PROP_PROC       , "proc"        },
    { ALG_PROP_X          , "x"           },
    { ALG_PROP_Y          , "y"           },
    { ALG_PROP_WIDTH      , "width"       },
    { ALG_PROP_HEIGHT     , "height"      },
    { ALG_PROP_DATA       , "data"        },
    { ALG_PROP_ID         , "id"          },
    { ALG_PROP_VISIBLE    , "visible"     },
    { ALG_PROP_ENABLED    , "enabled"     },
    { ALG_PROP_HIGHLIGHTED, "highlighted" },
    { ALG_PROP_PRESSED    , "pressed"     },
    { ALG_PROP_SELECTED   , "selected"    },
    { ALG_PROP_ACTIVE     , "active"      },
    { ALG_PROP_ERROR      , "error"       },
    { ALG_PROP_FOCUSED    , "focused"     },
    { ALG_PROP_NULL       , NULL          }
};


//Returns the name of a property.
const char* alg_get_prop_name(int prop_id) {
    for (int i = 0; property[i].id; ++i) {
        if (prop_id == property[i].id) {
            return property[i].name;
        }
    }
    return NULL;
}


//get prop id
int alg_get_prop_id(const char* name) {
    assert(name);
    for (int i = 0; property[i].id; ++i) {
        if (strcmp(property[i].name, name) == 0) {
            return property[i].id;
        }
    }
    return 0;
}