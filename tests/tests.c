#include "algui/test.h"
#include "test_array.h"


void algui_run_tests() {
    ALGUI_TEST_STATISTICS stats;
    algui_init_test(&stats);

    run_tests_array(&stats);

    algui_print_test_statistics(&stats);
}
