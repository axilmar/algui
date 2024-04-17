#ifndef ALGUI_ELEMENT_VTABLE_H
#define ALGUI_ELEMENT_VTABLE_H


#include <stdint.h>


/**
 * A structure that contains pointers to functions a container needs in order to manage elements.
 */
typedef struct {
    /**
     * The default constructor.
     * Elements should be initialized to their default state.
     * @param context pointer to context data; usually, the container that contains the elements.
     * @param elem pointer to elements to construct.
     * @param elem_size element size, in bytes.
     * @param array_size number of elements in the array.
     */
    void (*const default_constructor)(void* context, void* elem, size_t elem_size, size_t array_size);

    /**
     * The copy constructor.
     * Elements should be initialized from copies of source elements.
     * @param context pointer to context data; usually, the container that contains the elements.
     * @param elem pointer to elements to copy-construct.
     * @param src pointer to elements to copy.
     * @param elem_size element size, in bytes.
     * @param array_size number of elements in the arrays.
     */
    void (*const copy_constructor)(void* context, void* elem, const void* src, size_t elem_size, size_t array_size);

    /**
     * The copy assignment operator.
     * Elements should be destroyed, then the source elements must be copied. 
     * @param context pointer to context data; usually, the container that contains the elements.
     * @param elem pointer to elements to copy-assign.
     * @param src pointer to elements to copy.
     * @param elem_size element size, in bytes.
     * @param array_size number of elements in the arrays.
     */
    void (*const copy_assignment)(void* context, void* elem, const void* src, size_t elem_size, size_t array_size);

    /**
     * The destructor.
     * Elements should be destroyed.
     * @param context pointer to context data; usually, the container that contains the elements.
     * @param elem pointer to elements to destroy.
     * @param elem_size element size, in bytes.
     * @param array_size number of elements in the array.
     */
    void (*const destructor)(void* context, void* elem, size_t elem_size, size_t array_size);
} ALGUI_ELEMENT_VTABLE;


/**
 * Element vtable for null. All functions are empty.
 */
extern const ALGUI_ELEMENT_VTABLE algui_null_element_vtable;


#endif //ALGUI_ELEMENT_VTABLE_H

