#ifdef _WIN32


#include <windows.h>
#include "algui/once_flag.h"


void algui_call_once(ALGUI_ONCE_FLAG* flag, void (*func)(void)) {
    InitOnceExecuteOnce((PINIT_ONCE)flag, (PINIT_ONCE_FN)func, NULL, NULL);
}


#endif //_WIN32
