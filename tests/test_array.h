#include "algui/test.h"
#include "algui/array.h"


static ALGUI_BOOL test_init_array(void* data) {
    //test null array
    {
        ALGUI_ENSURE(algui_init_array(NULL, 4, 4) == ALGUI_FALSE);
    }

    //test 0 element size
    {
        ALGUI_ARRAY array;
        ALGUI_ENSURE(algui_init_array(&array, 0, 4) == ALGUI_FALSE);
    }

    //test init with 0 size
    {
        ALGUI_ARRAY array;
        ALGUI_ENSURE(algui_init_array(&array, 4, 0) == ALGUI_TRUE);
        ALGUI_ENSURE(array.data == NULL);
        ALGUI_ENSURE(array.element_size == 4);
        ALGUI_ENSURE(array.size == 0);
        algui_cleanup_array(&array);
    }

    //test init with size
    {
        ALGUI_ARRAY array;
        ALGUI_ENSURE(algui_init_array(&array, sizeof(int), 8) == ALGUI_TRUE);
        ALGUI_ENSURE(array.data != NULL);
        ALGUI_ENSURE(array.element_size == sizeof(int));
        ALGUI_ENSURE(array.size == 8);
        for (size_t i = 0; i < array.size; ++i) {
            *(int*)algui_get_array_element(&array, i) = (int)i;
            const int v = *(int*)algui_get_array_element(&array, i);
            ALGUI_ENSURE(v == (int)i);
        }
        algui_cleanup_array(&array);
    }

    return ALGUI_TRUE;
}


static void run_tests_array(ALGUI_TEST_STATISTICS* stats) {
    algui_do_test(stats, "algui_init_array", test_init_array, NULL);
}