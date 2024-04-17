#include "algui/test.h"


extern void byte_array_test(ALGUI_TEST*);


void algui_run_tests() {
    algui_test_enable_test_memory_allocation_functions();
    ALGUI_TEST test;
    algui_test_init(&test);
    byte_array_test(&test);
    algui_test_print(&test);
    algui_test_disable_test_memory_allocation_functions();
}
