#include "algui/test.h"


extern void run_tests_array(ALGUI_TEST_STATISTICS* stats);
extern void run_tests_map(ALGUI_TEST_STATISTICS* stats);

void algui_run_tests() {
    ALGUI_TEST_STATISTICS stats;
    algui_init_test(&stats);
    run_tests_array(&stats);
    run_tests_map(&stats);
    algui_print_test_statistics(&stats);
}
