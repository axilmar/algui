#include "algui/test.h"


extern void test_object(ALGUI_TEST* test);


void algui_run_tests() {
    ALGUI_TEST test;
    algui_init_test(&test);
    test_object(&test);
    algui_print_test(&test);
}
