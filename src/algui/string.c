#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "algui/string.h"


#ifdef _WIN32
#define strdup _strdup
#endif 


//internal string representation
typedef char* STRING;


//default constructor; strings are initialized to null.
static void string_default_constructor(void* context, void* elem, size_t elem_size, size_t array_size) {
    STRING* str = (STRING*)elem;
    for (size_t i = 0; i < array_size; ++i) {
        str[i] = NULL;
    }
}


//copy constructor; strings are duplicated using strdup.
static void string_copy_constructor(void* context, void* elem, const void* src, size_t elem_size, size_t array_size) {
    STRING* str = (STRING*)elem;
    const STRING* src_str = (const STRING*)src;
    for (size_t i = 0; i < array_size; ++i) {
        str[i] = src_str[i] ? strdup(src_str[i]) : NULL;
    }
}


//copy assignment; strings are duplicated using realloc/memcpy or freed if source is null.
static void string_copy_assignment(void* context, void* elem, const void* src, size_t elem_size, size_t array_size) {
    STRING* str = (STRING*)elem;
    const STRING* src_str = (const STRING*)src;
    for (size_t i = 0; i < array_size; ++i) {

        //if source is not null
        if (src_str[i]) {
            //find length of source, including the the '\0' in the end
            size_t src_len = strlen(src_str[i]) + 1;

            //reallocate destination to fit the source; 
            //faster than freeing the destination and allocating a new buffer
            str[i] = realloc(src_str[i], src_len);

            //reallocation success; copy the string data
            if (str[i]) {
                memcpy(str[i], src_str[i], src_len);
            }

            //else reallocation failure
            else {
                errno = EINVAL;
            }
        }

        //else source is null
        else {
            free(str[i]);
            str[i] = NULL;
        }
    }
}


//destructor
static void string_destructor(void* context, void* elem, size_t elem_size, size_t array_size) {
    STRING* str = (STRING*)elem;
    for (size_t i = array_size; i > 0; --i) {
        free(str[i - 1]);
    }
};


//string vtable
const ALGUI_ELEMENT_VTABLE algui_string_element_vtable = {
    string_default_constructor,
    string_copy_constructor,
    string_copy_assignment,
    string_destructor
};
