#ifndef ALGUI_STDLIB_H
#define ALGUI_STDLIB_H


#include <stdlib.h>


#ifdef ALGUI_CONFIG_TEST


#include <stdint.h>
#include "test.h"


#undef malloc
#undef realloc
#undef free


#define realloc (*algui_realloc)
#define malloc  (*algui_malloc)
#define free    (*algui_free)


#endif


#endif //ALGUI_STDLIB_H
