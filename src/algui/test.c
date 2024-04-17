#ifdef ALGUI_CONFIG_TEST


#include <stdlib.h>


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


//init test
ALGUI_BOOL algui_test_init(ALGUI_TEST* test) {
    enable_virtual_terminal();

    //check arg
    if (test == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    test->test_count = 0;
    test->successful_test_count = 0;
    test->failed_test_count = 0;

    return ALGUI_TRUE;
}


//run test
ALGUI_BOOL algui_test_run(ALGUI_TEST* test, const char* name, ALGUI_TEST_FUNCTION func, void* data) {
    //check args
    if (test == NULL || name == NULL || func == NULL) {
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
ALGUI_BOOL algui_test_print(const ALGUI_TEST* test) {
    if (test == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    printf("\nTotal test count     : %i\n", test->test_count);

    if (test->successful_test_count > 0) {
        printf("\033[0;92mSuccessful test count: %i\033[0m\n", test->successful_test_count);
    }
    
    if (test->failed_test_count > 0) {
        printf("\033[0;91mFailed test count    : %i\033[0m\n", test->failed_test_count);
    }

    printf("\n");

    return ALGUI_TRUE;
}


///global variables for test malloc/realloc/free.
void* algui_realloc_ptr = NULL;
size_t algui_realloc_new_size = 0;
size_t algui_malloc_size = 0;
void* algui_free_ptr = NULL;


void* algui_test_realloc(void* ptr, size_t new_size) {
    algui_realloc_ptr = ptr;
    algui_realloc_new_size = new_size;
    return realloc(ptr, new_size);
}


void* algui_test_malloc(size_t size) {
    algui_malloc_size = size;
    return malloc(size);
}


void algui_test_free(void* ptr) {
    algui_free_ptr = ptr;
    free(ptr);
}


#endif //ALGUI_CONFIG_TEST
