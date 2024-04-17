#include "algui/element_vtable.h"


static void null_constructor(void* context, void* elems, size_t elem_size, size_t array_size) {
}


static void null_copy_constructor(void* context, void* elems, const void* src, size_t elem_size, size_t array_size) {
}


static void null_copy_assignment(void* context, void* elems, const void* src, size_t elem_size, size_t array_size) {
}


static void null_destructor(void* context, void* elems, size_t elem_size, size_t array_size) {
}


const ALGUI_ELEMENT_VTABLE algui_null_element_vtable = {
    null_constructor,
    null_copy_constructor,
    null_copy_assignment,
    null_destructor
};
