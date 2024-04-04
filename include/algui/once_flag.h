#ifndef ALGUI_ONCE_FLAG_H
#define ALGUI_ONCE_FLAG_H


#ifdef _WIN32


#include <stdint.h>


/**
 * Once flag init value.
 */
#define ALGUI_ONCE_FLAG_INIT 0


 /**
  * Once flag type.
  */
typedef uintptr_t ALGUI_ONCE_FLAG;


/**
 * Call given function once.
 * @param flag flag to call once. No checks are performed on this pointer.
 * @param func func to invoke once. No checks are performed on this pointer.
 */
void algui_call_once(ALGUI_ONCE_FLAG* flag, void (*func)(void));


#else
#include <threads.h>


/**
 * Once flag init value.
 */
#define ALGUI_ONCE_FLAG_INIT ONCE_FLAG_INIT


 /**
  * Once flag type.
  */
typedef once_flag ALGUI_ONCE_FLAG;


/**
 * Call given function once.
 * @param flag flag to call once. No checks are performed on this pointer.
 * @param func func to invoke once. No checks are performed on this pointer.
 */
#define algui_call_once call_once


#endif


#endif //ALGUI_ONCE_FLAG_H
