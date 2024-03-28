#ifdef _WIN32
#include <windows.h>
#endif
#include <errno.h>
#include <string.h>
#include "algui/test.h"


static void enable_virtual_terminal() {
    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwOriginalOutMode = 0;
    GetConsoleMode(hOut, &dwOriginalOutMode);
    DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
    DWORD dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
    SetConsoleMode(hOut, dwOutMode);
    #endif
}


//init stats
ALGUI_BOOL algui_init_test(ALGUI_TEST_STATISTICS* stats) {
    enable_virtual_terminal();

    //check arg
    if (stats == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    stats->test_count = 0;
    stats->successful_test_count = 0;
    stats->failed_test_count = 0;

    return ALGUI_TRUE;
}


//run test
ALGUI_BOOL algui_do_test(ALGUI_TEST_STATISTICS* stats, const char* name, ALGUI_TEST_FUNCTION func, void* data) {
    //check args
    if (stats == NULL || name == NULL || func == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //print the test name
    printf("\033[0;93m%s\033[0m", name);
    const int name_len = strlen(name);
    int dots = 70 - name_len;
    for (; dots > 0; --dots) {
        printf(".");
    }

    //run the test
    const ALGUI_BOOL success = func(data);

    //count one more test
    ++stats->test_count;

    //count success/failure
    if (success) {
        ++stats->successful_test_count;
        printf("\033[0;92mSUCCESS\033[0m\n");
    }
    else {
        ++stats->failed_test_count;
    }

    return success;
}


//print test statistics
ALGUI_BOOL algui_print_test_statistics(const ALGUI_TEST_STATISTICS* stats) {
    if (stats == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    printf("\nTotal test count     : %i\n", stats->test_count);

    if (stats->successful_test_count > 0) {
        printf("\033[0;92mSuccessful test count: %i\033[0m\n", stats->successful_test_count);
    }
    
    if (stats->failed_test_count > 0) {
        printf("\033[0;91mFailed test count    : %i\033[0m\n", stats->failed_test_count);
    }

    printf("\n");

    return ALGUI_TRUE;
}
