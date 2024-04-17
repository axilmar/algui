#include <memory.h>
#include "algui/trivial_value.h"


//constructor
static void trivial_value_constructor(void* context, void* elem, size_t elem_size, size_t array_size) {
}


//copy constructor
static void trivial_value_copy_constructor(void* context, void* elem, const void* src, size_t elem_size, size_t array_size) {
    memcpy(elem, src, elem_size * array_size);
}


//copy assignment
static void trivial_value_copy_assignment(void* context, void* elem, const void* src, size_t elem_size, size_t array_size) {
    memcpy(elem, src, elem_size * array_size);
}


//destructor
static void trivial_value_destructor(void* context, void* elem, size_t elem_size, size_t array_size) {
}


//value vtable
const ALGUI_ELEMENT_VTABLE algui_trivial_value_element_vtable = {
    trivial_value_constructor,
    trivial_value_copy_constructor,
    trivial_value_copy_assignment,
    trivial_value_destructor
};
