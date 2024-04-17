#ifdef ALGUI_CONFIG_TEST


#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "algui/test.h"


#ifdef _WIN32
#include <windows.h>
#endif


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


//test realloc.
void* (*algui_realloc)(void* ptr, size_t new_size) = &realloc;


//test malloc.
void* (*algui_malloc)(size_t size) = &malloc;


//test free.
void (*algui_free)(void* ptr) = &free;


///original realloc.
void* (* const algui_original_realloc)(void* ptr, size_t new_size) = &realloc;


///original malloc.
void* (* const algui_original_malloc)(size_t size) = &malloc;


///original free.
void (* const algui_original_free)(void* ptr) = &free;


//allocated bytes count
static size_t allocated_bytes = 0;


//realloc for testing; counts the number of allocated bytes.
void* algui_test_realloc(void* ptr, size_t new_size) {
    size_t old_size;
    if (ptr) {
        ptr = (size_t*)ptr - 1;
        old_size = *(size_t*)ptr;
    }
    else {
        old_size = 0;
    }
    ptr = realloc(ptr, sizeof(size_t) + new_size);
    if (!ptr) {
        return NULL;
    }
    *(size_t*)ptr = new_size;
    allocated_bytes -= old_size;
    allocated_bytes += new_size;
    return (size_t*)ptr + 1;
}


//malloc for testing; counts the number of allocated bytes.
void* algui_test_malloc(size_t size) {
    void* mem = malloc(sizeof(size_t) + size);
    if (!mem) {
        return NULL;
    }
    *(size_t*)mem = size;
    allocated_bytes += size;
    return (size_t*)mem + 1;
}


//free for testing; counts the number of allocated bytes.
void algui_test_free(void* ptr) {
    if (!ptr) {
        return;
    }
    ptr = (size_t*)ptr - 1;
    size_t size = *(size_t*)ptr;
    allocated_bytes -= size;
    free(ptr);
}


//returns number of allocated bytes.
size_t algui_test_get_allocated_bytes() {
    return allocated_bytes;
}


//sets pointers to malloc, realloc and free to point to the test functions.
void algui_test_enable_test_memory_allocation_functions() {
    algui_realloc = &algui_test_realloc;
    algui_malloc = &algui_test_malloc;
    algui_free = &algui_test_free;
}


//sets pointers to malloc, realloc and free to point to the original functions.
void algui_test_disable_test_memory_allocation_functions() {
    algui_realloc = algui_original_realloc;
    algui_malloc = algui_original_malloc;
    algui_free = algui_original_free;
}


#endif //ALGUI_CONFIG_TEST
