#ifndef ALGUI_TRIVIAL_VALUE_H
#define ALGUI_TRIVIAL_VALUE_H


#include "element_vtable.h"


/**
 * The vtable for a trivial value, i.e. an element that does not require a destructor.
 * The default constructor and the destructor are empty functions;
 * data are copied using memcpy.
 */
extern const ALGUI_ELEMENT_VTABLE algui_trivial_value_element_vtable;


#endif //ALGUI_TRIVIAL_VALUE_H
