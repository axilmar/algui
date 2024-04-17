#ifndef ALGUI_STRING_H
#define ALGUI_STRING_H


#include "element_vtable.h"


/**
 * The vtable for a string element.
 * The element pointer shall point to char*.
 * The vtable initializes, copies, and frees strings as required.
 */
extern const ALGUI_ELEMENT_VTABLE algui_string_element_vtable;


#endif //ALGUI_STRING_H
