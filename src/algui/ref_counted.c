#include <stdlib.h>
#include <errno.h>
#include <memory.h>
#include "algui/ref_counted.h"


//ref counted ptr default constructor; pointers are initialized to null.
static void ref_counted_ptr_default_constructor(void* context, void* elem, size_t elem_size, size_t array_size) {
    ALGUI_REF_COUNTED* rcp = (ALGUI_REF_COUNTED*)elem;
    for (size_t i = 0; i < array_size; ++i) {
        rcp[i] = NULL;
    }
}


//ref counted ptr copy constructor; elements are acquired.
static void ref_counted_ptr_copy_constructor(void* context, void* elem, const void* src, size_t elem_size, size_t array_size) {
    ALGUI_REF_COUNTED* rcp = (ALGUI_REF_COUNTED*)elem;
    const ALGUI_REF_COUNTED* src_rcp = (const ALGUI_REF_COUNTED*)src;
    for (size_t i = 0; i < array_size; ++i) {
        rcp[i] = src_rcp[i];
        algui_ref_counted_acquire(rcp[i]);
    }
}


//ref counted ptr copy assignment; the new elements are acquired, the old ones are released.
static void ref_counted_ptr_copy_assignment(void* context, void* elem, const void* src, size_t elem_size, size_t array_size) {
    ALGUI_REF_COUNTED* rcp = (ALGUI_REF_COUNTED*)elem;
    ALGUI_REF_COUNTED* src_rcp = (ALGUI_REF_COUNTED*)src;
    for (size_t i = 0; i < array_size; ++i) {
        ALGUI_REF_COUNTED old = rcp[i];
        rcp[i] = src_rcp[i];
        algui_ref_counted_acquire(rcp[i]);
        algui_ref_counted_release(old);
    }
}


//ref counted ptr destructor; elements are released.
static void ref_counted_ptr_destructor(void* context, void* elem, size_t elem_size, size_t array_size) {
    ALGUI_REF_COUNTED* rcp = (ALGUI_REF_COUNTED*)elem;
    for (size_t i = array_size; i > 0; --i) {
        algui_ref_counted_release(rcp[i - 1]);
    }
}


/**************************************************************************************************/


//creates a ref counted object.
ALGUI_REF_COUNTED algui_ref_counted_create(void* obj, void (*destroy)(void* elem)) {
    //check the obj
    if (!obj) {
        errno = EINVAL;
        return NULL;
    }

    //allocate a ref counted structure
    ALGUI_REF_COUNTED rc = (ALGUI_REF_COUNTED)malloc(sizeof(*(ALGUI_REF_COUNTED)0));

    //check for allocation failure
    if (!rc) {
        errno = ENOMEM;
        return NULL;
    }

    //setup the ref counted structure
    rc->ref_count = 1;
    rc->object = obj;
    rc->destroy = destroy;

    //success
    return rc;
}


//increments the reference count of the given object.
void* algui_ref_counted_acquire(ALGUI_REF_COUNTED rc) {
    //check the rc
    if (!rc) {
        return NULL;
    }

    //increment the ref count
    ++rc->ref_count;

    //return the object
    return rc->object;
}


//decrements the reference count of the given object.
void algui_ref_counted_release(ALGUI_REF_COUNTED rc) {
    //check the rc
    if (!rc) {
        return;
    }

    //decrement the ref count; if it reaches 0, destroy the object
    if ((--rc->ref_count) == 0) {
        if (rc->destroy) {
            rc->destroy(rc->object);
        }
        free(rc);
    }
}


//Element vtable for a ref counted object ptr.
extern const ALGUI_ELEMENT_VTABLE algui_ref_counted_element_vtable = {
    ref_counted_ptr_default_constructor,
    ref_counted_ptr_copy_constructor,
    ref_counted_ptr_copy_assignment,
    ref_counted_ptr_destructor
};
