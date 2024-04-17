#ifndef ALGUI_STDLIB_H
#define ALGUI_STDLIB_H


#include <stdlib.h>


#ifdef ALGUI_CONFIG_TEST


#include <stdint.h>
#include "test.h"


#undef malloc
#undef realloc
#undef free


#define realloc algui_test_realloc
#define malloc  algui_test_malloc
#define free    algui_test_free


#endif


#endif //ALGUI_STDLIB_H
