#ifndef ALGUI_REF_COUNTED_H
#define ALGUI_REF_COUNTED_H


#include "element_vtable.h"


/**
 * A ref counted object.
 */
typedef struct {
    size_t ref_count;
    void* object;
    void (*destroy)(void* obj);
} *ALGUI_REF_COUNTED;


/**
 * Creates a ref counted object.
 * @param obj pointer to managed object; if null, errno is set to EINVAL and NULL is returned.
 * @param destroy optional destructor.
 * @return pointer to ref counted object on success, NULL on error.
 */
ALGUI_REF_COUNTED algui_ref_counted_create(void* obj, void (*destroy)(void* elem));


/**
 * Increments the reference count of the given object.
 * @param rc pointer to ref counted object to reference-count; if null, nothing happens and NULL is returned.
 */
void* algui_ref_counted_acquire(ALGUI_REF_COUNTED rc);


/**
 * Decrements the reference count of the given object.
 * If the reference count reaches 0, then the object is destroyed, then the reference counted struct is freed.
 * @param rc pointer to ref counted object to reference-count; if null, nothing happens.
 * @return the referenced object on success, null on error.
 */
void algui_ref_counted_release(ALGUI_REF_COUNTED rc);


/**
 * Element vtable for a ref counted object ptr.
 * It manages the lifetime of an object using reference counting.
 */
extern const ALGUI_ELEMENT_VTABLE algui_ref_counted_element_vtable;


#endif //ALGUI_REF_COUNTED_H
