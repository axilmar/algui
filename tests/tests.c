#include "algui/test.h"


extern void byte_array_test(ALGUI_TEST*);


void algui_run_tests() {
    ALGUI_TEST test;
    algui_test_init(&test);
    byte_array_test(&test);
    algui_test_print(&test);
}
