#ifndef ALGUI_STATIC_INIT_H
#define ALGUI_STATIC_INIT_H


#include "once_flag.h"


/**
 * Invokes the given piece of code once.
 * Thread safe.
 * @param FUNC function to run once.
 */
#define ALGUI_STATIC_INIT(FUNC) {\
    static ALGUI_ONCE_FLAG f = ALGUI_ONCE_FLAG_INIT;\
    algui_call_once(&f, FUNC);\
}


#endif //ALGUI_STATIC_INIT_H
