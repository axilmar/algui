#include "algui/test.h"

extern void test_array(ALGUI_TEST_STATISTICS* stats);
extern void test_map(ALGUI_TEST_STATISTICS* stats);
extern void test_buffer(ALGUI_TEST_STATISTICS* stats);
extern void test_object(ALGUI_TEST_STATISTICS* stats);

void algui_run_tests() {
    ALGUI_TEST_STATISTICS stats;
    algui_init_test(&stats);
    //test_array(&stats);
    test_map(&stats);
    //test_buffer(&stats);
    test_object(&stats);
    algui_print_test_statistics(&stats);
}
