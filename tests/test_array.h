#include "algui/test.h"
#include "algui/array.h"
#include "util.h"


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


static ALGUI_BOOL test_cleanup_array(void* data) {
    //test null array
    {
        ALGUI_ENSURE(algui_cleanup_array(NULL) == ALGUI_FALSE);
    }

    //test empty array
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, 4, 0);
        ALGUI_ENSURE(algui_cleanup_array(&array) == ALGUI_TRUE);
        ALGUI_ENSURE(array.data == NULL);
        ALGUI_ENSURE(array.size == 0);
        ALGUI_ENSURE(algui_cleanup_array(&array) == ALGUI_TRUE);
        ALGUI_ENSURE(array.data == NULL);
        ALGUI_ENSURE(array.size == 0);
    }

    //test non-empty array
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, 4, 8);
        ALGUI_ENSURE(algui_cleanup_array(&array) == ALGUI_TRUE);
        ALGUI_ENSURE(array.data == NULL);
        ALGUI_ENSURE(array.size == 0);
        ALGUI_ENSURE(algui_cleanup_array(&array) == ALGUI_TRUE);
        ALGUI_ENSURE(array.data == NULL);
        ALGUI_ENSURE(array.size == 0);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_get_array_element_size(void* data) {
    //test null array
    {
        ALGUI_ENSURE(algui_get_array_element_size(NULL) == (size_t)-1);
    }

    //test non-zero element size
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, 4, 8);
        ALGUI_ENSURE(algui_get_array_element_size(&array) == 4);
        algui_cleanup_array(&array);
        ALGUI_ENSURE(algui_get_array_element_size(&array) == 4);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_get_array_size(void* data) {
    //test null array
    {
        ALGUI_ENSURE(algui_get_array_size(NULL) == (size_t)-1);
    }

    //test zero size
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, 4, 0);
        ALGUI_ENSURE(algui_get_array_size(&array) == 0);
        algui_cleanup_array(&array);
        ALGUI_ENSURE(algui_get_array_size(&array) == 0);
    }

    //test non-zero size
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, 4, 8);
        ALGUI_ENSURE(algui_get_array_size(&array) == 8);
        algui_cleanup_array(&array);
        ALGUI_ENSURE(algui_get_array_size(&array) == 0);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_set_array_size(void* data) {
    //set size of null array
    {
        ALGUI_ENSURE(algui_set_array_size(NULL, 0) == ALGUI_FALSE);
        ALGUI_ENSURE(algui_set_array_size(NULL, 10) == ALGUI_FALSE);
    }

    //set size of empty array to zero
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 0);
        ALGUI_ENSURE(algui_set_array_size(&array, 0) == ALGUI_TRUE);
        ALGUI_ENSURE(array.data == NULL);
        ALGUI_ENSURE(array.size == 0);
        algui_cleanup_array(&array);
    }

    //set size of empty array to non-zero
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 0);
        ALGUI_ENSURE(algui_set_array_size(&array, 8) == ALGUI_TRUE);
        ALGUI_ENSURE(array.data != NULL);
        ALGUI_ENSURE(array.size == 8);
        algui_cleanup_array(&array);
    }

    //set size of non-empty array to zero
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 8);
        ALGUI_ENSURE(algui_set_array_size(&array, 0) == ALGUI_TRUE);
        ALGUI_ENSURE(array.data == NULL);
        ALGUI_ENSURE(array.size == 0);
        algui_cleanup_array(&array);
    }

    //set size of non-empty array to bigger value
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 8);
        ALGUI_ENSURE(algui_set_array_size(&array, 16) == ALGUI_TRUE);
        ALGUI_ENSURE(array.data != NULL);
        ALGUI_ENSURE(array.size == 16);
        for (size_t i = 0; i < array.size; ++i) {
            *(int*)algui_get_array_element(&array, i) = (int)i;
            const int v = *(int*)algui_get_array_element(&array, i);
            ALGUI_ENSURE(v == (int)i);
        }
        algui_cleanup_array(&array);
    }

    //set size of non-empty array to smaller value
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 8);
        ALGUI_ENSURE(algui_set_array_size(&array, 4) == ALGUI_TRUE);
        ALGUI_ENSURE(array.data != NULL);
        ALGUI_ENSURE(array.size == 4);
        for (size_t i = 0; i < array.size; ++i) {
            *(int*)algui_get_array_element(&array, i) = (int)i;
            const int v = *(int*)algui_get_array_element(&array, i);
            ALGUI_ENSURE(v == (int)i);
        }
        algui_cleanup_array(&array);
    }

    //set size of non-empty array to same value
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 8);
        const void* data = array.data;
        ALGUI_ENSURE(algui_set_array_size(&array, 8) == ALGUI_TRUE);
        ALGUI_ENSURE(array.data == data);
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


static ALGUI_BOOL test_get_array_element(void* data) {
    //get element of null array
    {
        ALGUI_ENSURE(algui_get_array_element(NULL, 0) == ALGUI_FALSE);
    }

    //get element out of bounds for empty array
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 0);
        ALGUI_ENSURE(algui_get_array_element(&array, 0) == NULL);
        ALGUI_ENSURE(algui_get_array_element(&array, array.size) == NULL);
        ALGUI_ENSURE(algui_get_array_element(&array, array.size + 1) == NULL);
    }

    //get element out of bounds for non-empty array
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 8);
        ALGUI_ENSURE(algui_get_array_element(&array, array.size) == NULL);
        ALGUI_ENSURE(algui_get_array_element(&array, array.size + 1) == NULL);
        algui_cleanup_array(&array);
    }

    //get element in bounds for non-empty array
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 8);
        ALGUI_ENSURE(algui_get_array_element(&array, 0) == array.data);
        ALGUI_ENSURE(algui_get_array_element(&array, array.size / 2) == array.data + array.element_size * array.size / 2);
        ALGUI_ENSURE(algui_get_array_element(&array, array.size - 1) == array.data + array.element_size * (array.size - 1));
        algui_cleanup_array(&array);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_set_array_element(void* data) {
    //set array element for null array
    {
        int v = 0;
        ALGUI_ENSURE(algui_set_array_element(NULL, 0, &v) == ALGUI_FALSE);
        ALGUI_ENSURE(algui_set_array_element(NULL, 0, NULL) == ALGUI_FALSE);
    }

    //set null array element for non-empty array
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 8);
        ALGUI_ENSURE(algui_set_array_element(&array, 4, NULL) == ALGUI_FALSE);
        algui_cleanup_array(&array);
    }

    //set array element out of bounds for empty array
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 0);
        int v = 0;
        ALGUI_ENSURE(algui_set_array_element(&array, array.size, &v) == ALGUI_FALSE);
        ALGUI_ENSURE(algui_set_array_element(&array, array.size + 1, &v) == ALGUI_FALSE);
    }

    //set array element out of bounds for non-empty array
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 8);
        int v = 0;
        ALGUI_ENSURE(algui_set_array_element(&array, array.size, &v) == ALGUI_FALSE);
        ALGUI_ENSURE(algui_set_array_element(&array, array.size + 1, &v) == ALGUI_FALSE);
        algui_cleanup_array(&array);
    }

    //set array element in bounds for non-empty array
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 8);
        for (size_t i = 0; i < array.size; ++i) {
            int v = 10 * i;
            ALGUI_ENSURE(algui_set_array_element(&array, i, &v) == ALGUI_TRUE);
            ALGUI_ENSURE(*(int*)algui_get_array_element(&array, i) == v);
        }
        algui_cleanup_array(&array);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_get_array_element_index(void* data) {
    //get element index for null array
    {
        int v = 0;
        ALGUI_ENSURE(algui_get_array_element_index(NULL, &v) == ALGUI_INVALID_INDEX);
        ALGUI_ENSURE(algui_get_array_element_index(NULL, NULL) == ALGUI_INVALID_INDEX);
    }

    //get element index for empty array
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 0);
        int v = 0;
        ALGUI_ENSURE(algui_get_array_element_index(&array, &v) == ALGUI_INVALID_INDEX);
        ALGUI_ENSURE(algui_get_array_element_index(&array, NULL) == ALGUI_INVALID_INDEX);
    }

    //get non-element index for non-empty array
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 8);
        int v = 0;
        ALGUI_ENSURE(algui_get_array_element_index(&array, &v) == ALGUI_INVALID_INDEX);
        ALGUI_ENSURE(algui_get_array_element_index(&array, NULL) == ALGUI_INVALID_INDEX);
        algui_cleanup_array(&array);
    }

    //get element index for non-empty array
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 8);
        for (size_t i = 0; i < array.size; ++i) {
            ALGUI_ENSURE(algui_get_array_element_index(&array, array.data + array.element_size * i) == i);
        }
        algui_cleanup_array(&array);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_find_array_element_index(void* data) {
    //find null array element index 
    {
        int v = 0;
        ALGUI_ENSURE(algui_find_array_element_index(NULL, 0, &v, int_comparator) == ALGUI_INVALID_INDEX);
        ALGUI_ENSURE(algui_find_array_element_index(NULL, 0, &v, NULL) == ALGUI_INVALID_INDEX);
    }

    //find empty array element index 
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 0);
        ALGUI_ENSURE(algui_find_array_element_index(&array, 0, NULL, int_comparator) == ALGUI_INVALID_INDEX);
        ALGUI_ENSURE(algui_find_array_element_index(&array, 0, NULL, NULL) == ALGUI_INVALID_INDEX);
        int v = 0;
        ALGUI_ENSURE(algui_find_array_element_index(&array, 0, &v, int_comparator) == ALGUI_INVALID_INDEX);
        ALGUI_ENSURE(algui_find_array_element_index(&array, 0, &v, NULL) == ALGUI_INVALID_INDEX);
    }

    //find non-empty array element index from start
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 8);
        for (size_t i = 0; i < array.size; ++i) {
            const int v = (int)i;
            algui_set_array_element(&array, i, &v);
        }
        for (size_t i = 0; i < array.size; ++i) {
            const int v = (int)i;
            ALGUI_ENSURE(algui_find_array_element_index(&array, 0, &v, int_comparator) == i);
            ALGUI_ENSURE(algui_find_array_element_index(&array, 0, &v, NULL) == i);
        }
    }

    //find non-empty array element index from non-start
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 8);
        for (size_t i = 0; i < array.size; ++i) {
            const int v = (int)i;
            algui_set_array_element(&array, i, &v);
        }
        for (size_t i = 5; i < array.size; ++i) {
            const int v1 = (int)i - 5;
            ALGUI_ENSURE(algui_find_array_element_index(&array, 5, &v1, int_comparator) == ALGUI_INVALID_INDEX);
            ALGUI_ENSURE(algui_find_array_element_index(&array, 5, &v1, NULL) == ALGUI_INVALID_INDEX);
            const int v2 = (int)i;
            ALGUI_ENSURE(algui_find_array_element_index(&array, 5, &v2, int_comparator) == i);
            ALGUI_ENSURE(algui_find_array_element_index(&array, 5, &v2, NULL) == i);
        }
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_find_array_element_index_reverse(void* data) {
    //find null array element index 
    {
        int v = 0;
        ALGUI_ENSURE(algui_find_array_element_index_reverse(NULL, 0, &v, int_comparator) == ALGUI_INVALID_INDEX);
        ALGUI_ENSURE(algui_find_array_element_index_reverse(NULL, 0, &v, NULL) == ALGUI_INVALID_INDEX);
        ALGUI_ENSURE(algui_find_array_element_index_reverse(NULL, -1, &v, int_comparator) == ALGUI_INVALID_INDEX);
        ALGUI_ENSURE(algui_find_array_element_index_reverse(NULL, -1, &v, NULL) == ALGUI_INVALID_INDEX);
    }

    //find empty array element index 
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 0);
        ALGUI_ENSURE(algui_find_array_element_index_reverse(&array, 0, NULL, int_comparator) == ALGUI_INVALID_INDEX);
        ALGUI_ENSURE(algui_find_array_element_index_reverse(&array, 0, NULL, NULL) == ALGUI_INVALID_INDEX);
        ALGUI_ENSURE(algui_find_array_element_index_reverse(&array, -1, NULL, int_comparator) == ALGUI_INVALID_INDEX);
        ALGUI_ENSURE(algui_find_array_element_index_reverse(&array, -1, NULL, NULL) == ALGUI_INVALID_INDEX);
        int v = 0;
        ALGUI_ENSURE(algui_find_array_element_index_reverse(&array, 0, &v, int_comparator) == ALGUI_INVALID_INDEX);
        ALGUI_ENSURE(algui_find_array_element_index_reverse(&array, 0, &v, NULL) == ALGUI_INVALID_INDEX);
        ALGUI_ENSURE(algui_find_array_element_index_reverse(&array, -1, &v, int_comparator) == ALGUI_INVALID_INDEX);
        ALGUI_ENSURE(algui_find_array_element_index_reverse(&array, -1, &v, NULL) == ALGUI_INVALID_INDEX);
    }

    //find non-empty array element index from start
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 8);
        for (size_t i = 0; i < array.size; ++i) {
            const int v = (int)i;
            algui_set_array_element(&array, i, &v);
        }
        for (size_t i = 0; i < array.size; ++i) {
            const int v = (int)i;
            ALGUI_ENSURE(algui_find_array_element_index_reverse(&array, -1, &v, int_comparator) == i);
            ALGUI_ENSURE(algui_find_array_element_index_reverse(&array, -1, &v, NULL) == i);
        }
    }

    //find non-empty array element index from non-start
    {
        ALGUI_ARRAY array;
        algui_init_array(&array, sizeof(int), 8);
        for (size_t i = 0; i < array.size; ++i) {
            const int v = (int)i;
            algui_set_array_element(&array, i, &v);
        }
        for (size_t i = 0; i < 5; ++i) {
            const int v1 = (int)i;
            ALGUI_ENSURE(algui_find_array_element_index_reverse(&array, 4, &v1, int_comparator) == i);
            ALGUI_ENSURE(algui_find_array_element_index_reverse(&array, 4, &v1, NULL) == i);
            const int v2 = (int)i + 5;
            ALGUI_ENSURE(algui_find_array_element_index_reverse(&array, 4, &v2, int_comparator) == ALGUI_INVALID_INDEX);
            ALGUI_ENSURE(algui_find_array_element_index_reverse(&array, 4, &v2, NULL) == ALGUI_INVALID_INDEX);
        }
    }

    return ALGUI_TRUE;
}


static void run_tests_array(ALGUI_TEST_STATISTICS* stats) {
    algui_do_test(stats, "algui_init_array", test_init_array, NULL);
    algui_do_test(stats, "algui_cleanup_array", test_cleanup_array, NULL);
    algui_do_test(stats, "algui_get_array_element_size", test_get_array_element_size, NULL);
    algui_do_test(stats, "algui_get_array_size", test_get_array_size, NULL);
    algui_do_test(stats, "algui_set_array_size", test_set_array_size, NULL);
    algui_do_test(stats, "algui_get_array_element", test_get_array_element, NULL);
    algui_do_test(stats, "algui_set_array_element", test_set_array_element, NULL);
    algui_do_test(stats, "algui_get_array_element_index", test_get_array_element_index, NULL);
    algui_do_test(stats, "algui_find_array_element_index", test_find_array_element_index, NULL);
    algui_do_test(stats, "algui_find_array_element_index_reverse", test_find_array_element_index_reverse, NULL);
}