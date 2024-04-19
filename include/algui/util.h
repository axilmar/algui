#ifndef ALGUI_UTIL_H
#define ALGUI_UTIL_H


#include <stdlib.h>
#include <errno.h>


/**
 * Executes a function only if checks pass.
 * @param EXPR test expression.
 * @param ERRNO value to set errno to.
 * @param ERRES result if error.
 * @param CALL call to do if there is no error.
 */
#define ALGUI_CHECKED_CALL(EXPR, ERRNO, ERRES, CALL)\
    if (!(EXPR)) {\
        errno = ERRNO;\
        return ERRES;\
    }\
    return CALL;


/**
 * Allocate memory for object type.
 * @param TYPE object type.
 * @return pointer to TYPE allocated on the heap.
 */
#define ALGUI_NEW(TYPE) ((TYPE*)malloc(sizeof(TYPE)))


 /**
  * Cast pointer to specific type 
  * @param TYPE type to cast the pointer to.
  * @param PTR pointer variable to cast.
  * @return ptr to specific type.
  */
#define ALGUI_PTR(TYPE, PTR) ((TYPE*)(PTR))


  /**
 * Cast pointer to specific type and return reference to value.
 * @param TYPE type to cast the pointer to.
 * @param PTR pointer variable to cast.
 * @return reference to ptr value.
 */
#define ALGUI_PTR_VAL(TYPE, PTR) (*((TYPE*)(PTR)))


#endif //ALGUI_UTIL_H
