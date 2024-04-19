#include "algui/test.h"


//enable colors on win32
#ifdef _WIN32
#include <windows.h>
static void enable_virtual_terminal() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwOriginalOutMode = 0;
    GetConsoleMode(hOut, &dwOriginalOutMode);
    DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
    DWORD dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
    SetConsoleMode(hOut, dwOutMode);
}
#else
#define enable_virtual_terminal()
#endif



//init test
bool algui_init_test(ALGUI_TEST* test) {
    enable_virtual_terminal();

    //check arg
    if (test == NULL) {
        errno = EINVAL;
        return false;
    }

    test->test_count = 0;
    test->successful_test_count = 0;
    test->failed_test_count = 0;

    return true;
}


//run test
bool algui_run_test_case(ALGUI_TEST* test, const char* name, ALGUI_TEST_FUNCTION func, void* context) {
    //check args
    if (test == NULL || name == NULL || func == NULL) {
        errno = EINVAL;
        return false;
    }

    //print the test name
    printf("\033[0;93m%s\033[0m", name);
    const int name_len = strlen(name);
    int dots = 70 - name_len;
    for (; dots > 0; --dots) {
        printf(".");
    }

    //run the test
    const bool success = func(context);

    //count one more test
    ++test->test_count;

    //count success/failure
    if (success) {
        ++test->successful_test_count;
        printf("\033[0;92mSUCCESS\033[0m\n");
    }
    else {
        ++test->failed_test_count;
    }

    return success;
}


//print test results
bool algui_print_test(const ALGUI_TEST* test) {
    if (test == NULL) {
        errno = EINVAL;
        return false;
    }

    printf("\nTotal test count     : %i\n", test->test_count);

    if (test->successful_test_count > 0) {
        printf("\033[0;92mSuccessful test count: %i\033[0m\n", test->successful_test_count);
    }
    
    if (test->failed_test_count > 0) {
        printf("\033[0;91mFailed test count    : %i\033[0m\n", test->failed_test_count);
    }

    printf("\n");

    return true;
}
