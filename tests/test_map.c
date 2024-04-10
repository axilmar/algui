#include <memory.h>
#include "algui/test.h"
#include "algui/map.h"
#include "algui/array_util.h"
#include "util.h"


static size_t key_test_counter = 0;
static size_t value_test_counter = 0;


static void key_dtor(void* value) {
    ++key_test_counter;
}


static void value_dtor(void* value) {
    ++value_test_counter;
}


static ALGUI_BOOL test_init_map(void* external_data) {
    //test null map
    {
        ALGUI_ENSURE_ERROR(algui_init_map(NULL, sizeof(int), sizeof(int), int_comparator, NULL, NULL) == ALGUI_FALSE, EINVAL);
    }

    //test zero key size
    {
        ALGUI_MAP map;
        ALGUI_ENSURE_ERROR(algui_init_map(&map, 0, sizeof(int), int_comparator, NULL, NULL) == ALGUI_FALSE, EINVAL);
    }

    //test big key size
    {
        ALGUI_MAP map;
        ALGUI_ENSURE_ERROR(algui_init_map(&map, ALGUI_MAX_ELEMENT_SIZE + 1, sizeof(int), int_comparator, NULL, NULL) == ALGUI_FALSE, EINVAL);
    }

    //test big value size
    {
        ALGUI_MAP map;
        ALGUI_ENSURE_ERROR(algui_init_map(&map, sizeof(int), ALGUI_MAX_ELEMENT_SIZE + 1, int_comparator, NULL, NULL) == ALGUI_FALSE, EINVAL);
    }

    //test null comparator
    {
        ALGUI_MAP map;
        ALGUI_ENSURE_ERROR(algui_init_map(&map, sizeof(int), sizeof(int), NULL, NULL, NULL) == ALGUI_FALSE, EINVAL);
    }

    //test normal
    {
        ALGUI_MAP map;
        ALGUI_ENSURE_ERROR(algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL) == ALGUI_TRUE, 0);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_cleanup_map(void* external_data) {
    //test null map
    {
        ALGUI_ENSURE_ERROR(algui_cleanup_map(NULL) == ALGUI_FALSE, EINVAL);
    }

    //test normal
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);
        ALGUI_ENSURE_ERROR(algui_cleanup_map(&map) == ALGUI_TRUE, 0);
    }

    //cleanup with dtor
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, key_dtor, value_dtor);

        int v = 1;
        algui_set_map_element(&map, &v, &v);
        v = 2;
        algui_set_map_element(&map, &v, &v);
        v = 3;
        algui_set_map_element(&map, &v, &v);

        key_test_counter = 0;
        value_test_counter = 0;

        algui_cleanup_map(&map);

        ALGUI_ENSURE(key_test_counter == 3);
        ALGUI_ENSURE(value_test_counter == 3);

    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_is_empty_map(void* external_data) {
    //test null map
    {
        ALGUI_ENSURE_ERROR(algui_is_empty_map(NULL) == ALGUI_FALSE, EINVAL);
    }

    //test empty map
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);
        ALGUI_ENSURE_ERROR(algui_is_empty_map(&map) == ALGUI_TRUE, 0);
    }

    //test non-empty map
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);
        int v = 0;
        algui_set_map_element(&map, &v, &v);
        ALGUI_ENSURE_ERROR(algui_is_empty_map(&map) == ALGUI_FALSE, 0);
        algui_cleanup_map(&map);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_get_set_remove_map_element(void* external_data) {
    int k = 0;
    int v = 0;

    //test null map
    {
        ALGUI_ENSURE_ERROR(algui_get_map_element(NULL, &k) == NULL, EINVAL);
        ALGUI_ENSURE_ERROR(algui_set_map_element(NULL, &k, &v) == NULL, EINVAL);
    }

    //test null key
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);
        ALGUI_ENSURE_ERROR(algui_get_map_element(&map, NULL) == ALGUI_FALSE, EINVAL);
        ALGUI_ENSURE_ERROR(algui_set_map_element(&map, NULL, &v) == NULL, EINVAL);
    }

    //test null value
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);
        ALGUI_ENSURE_ERROR(algui_set_map_element(&map, &k, NULL) == NULL, EINVAL);
    }

    //test empty map, add 1, remove 1
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);
        
        //insert element
        k = 5;
        v = 5;
        ALGUI_ENSURE_ERROR(algui_set_map_element(&map, &k, &v) != NULL, 0);

        //get non existing element
        k = 55;
        ALGUI_ENSURE_ERROR(algui_get_map_element(&map, &k) == NULL, 0);

        //get existing element
        k = 5;
        ALGUI_ENSURE_ERROR(*(int*)algui_get_map_element(&map, &k) == v, 0);

        //remove non existing element
        k = 55;
        ALGUI_ENSURE_ERROR(algui_remove_map_element(&map, &k) == ALGUI_FALSE, 0);

        //get existing element
        k = 5;
        ALGUI_ENSURE_ERROR(*(int*)algui_get_map_element(&map, &k) == v, 0);

        //remove existing element
        k = 5;
        ALGUI_ENSURE_ERROR(algui_remove_map_element(&map, &k) == ALGUI_TRUE, 0);

        //get non existing element that previously existed
        k = 5;
        ALGUI_ENSURE_ERROR(algui_get_map_element(&map, &k) == NULL, 0);

        algui_cleanup_map(&map);
    }

    //test empty map, add multiple, remove 1
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);

        //insert elements
        for (int i = 5; i < 20; ++i) {
            ALGUI_ENSURE_ERROR(algui_set_map_element(&map, &i, &i) != NULL, 0);
        }

        //get non existing element
        k = 55;
        ALGUI_ENSURE_ERROR(algui_get_map_element(&map, &k) == NULL, 0);

        //get existing element
        k = 5;
        ALGUI_ENSURE_ERROR(*(int*)algui_get_map_element(&map, &k) == v, 0);

        //remove non existing element
        k = 55;
        ALGUI_ENSURE_ERROR(algui_remove_map_element(&map, &k) == ALGUI_FALSE, 0);

        //get existing element
        k = 5;
        ALGUI_ENSURE_ERROR(*(int*)algui_get_map_element(&map, &k) == v, 0);

        //remove existing element
        k = 5;
        ALGUI_ENSURE_ERROR(algui_remove_map_element(&map, &k) == ALGUI_TRUE, 0);

        //get non existing element that previously existed
        k = 5;
        ALGUI_ENSURE_ERROR(algui_get_map_element(&map, &k) == NULL, 0);

        algui_cleanup_map(&map);
    }

    //test empty map, add multiple, remove multiple
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);

        //insert elements
        for (int i = 5; i < 20; ++i) {
            ALGUI_ENSURE_ERROR(algui_set_map_element(&map, &i, &i) != NULL, 0);
        }

        //get non existing element
        k = 55;
        ALGUI_ENSURE_ERROR(algui_get_map_element(&map, &k) == NULL, 0);

        //get existing elements
        for (int i = 5; i < 20; ++i) {
            ALGUI_ENSURE_ERROR(*(int*)algui_get_map_element(&map, &i) == i, 0);
        }

        //remove non existing element
        k = 55;
        ALGUI_ENSURE_ERROR(algui_remove_map_element(&map, &k) == ALGUI_FALSE, 0);

        //get existing elements
        for (int i = 5; i < 20; ++i) {
            ALGUI_ENSURE_ERROR(*(int*)algui_get_map_element(&map, &i) == i, 0);
        }

        //remove existing elements
        for (int i = 5; i < 20; ++i) {
            ALGUI_ENSURE_ERROR(algui_remove_map_element(&map, &i) == ALGUI_TRUE, 0);
        }

        //get non existing element that previously existed
        for (int i = 5; i < 20; ++i) {
            ALGUI_ENSURE_ERROR(algui_get_map_element(&map, &i) == NULL, 0);
        }

        algui_cleanup_map(&map);
    }

    //remove elements with dtor
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, key_dtor, value_dtor);

        int v = 1;
        algui_set_map_element(&map, &v, &v);
        v = 2;
        algui_set_map_element(&map, &v, &v);
        v = 3;
        algui_set_map_element(&map, &v, &v);

        key_test_counter = 0;
        value_test_counter = 0;

        v = 1;
        algui_remove_map_element(&map, &v);
        v = 2;
        algui_remove_map_element(&map, &v);
        v = 3;
        algui_remove_map_element(&map, &v);

        ALGUI_ENSURE(key_test_counter == 3);
        ALGUI_ENSURE(value_test_counter == 3);

        algui_cleanup_map(&map);

        ALGUI_ENSURE(key_test_counter == 3);
        ALGUI_ENSURE(value_test_counter == 3);
    }

    return ALGUI_TRUE;
}


static uintptr_t sum_map_elements_callback(const void* key, void* value, void* data) {
    *(int*)data += *(int*)value;
    return ALGUI_FALSE;
}


static uintptr_t find_map_value_callback(const void* key, void* value, void* data) {
    return *(int*)data == *(int*)value;
}


static ALGUI_BOOL test_for_each_map_element(void* external_data) {
    const int test_data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int test_data_size = sizeof(test_data) / sizeof(int);
    const int test_sum = sum_int_array(test_data, test_data_size);
    int sum;

    //test null map
    {
        ALGUI_ENSURE_ERROR(algui_for_each_map_element(NULL, sum_map_elements_callback, &sum) == ALGUI_FALSE, EINVAL);
    }

    //test null func
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);
        ALGUI_ENSURE_ERROR(algui_for_each_map_element(&map, NULL, &sum) == ALGUI_FALSE, EINVAL);
        algui_cleanup_map(&map);
    }

    //test for each on empty map
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);
        sum = 0;
        ALGUI_ENSURE_ERROR(algui_for_each_map_element(&map, sum_map_elements_callback, &sum) == ALGUI_FALSE, 0);
        ALGUI_ENSURE(sum == 0);
        algui_cleanup_map(&map);
    }

    //test for each on non-empty map without duplicate keys
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);
        for (int i = 0; i < test_data_size; ++i) {
            algui_set_map_element(&map, &i, &i);
        }
        sum = 0;
        ALGUI_ENSURE_ERROR(algui_for_each_map_element(&map, sum_map_elements_callback, &sum) == ALGUI_FALSE, 0);
        ALGUI_ENSURE(sum == test_sum);
        algui_cleanup_map(&map);
    }

    //test for each on non-empty map with some duplicate keys
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);
        for (int i = 0; i < test_data_size; ++i) {
            algui_set_map_element(&map, &i, &i);
            if (i & 1) {
                algui_set_map_element(&map, &i, &i);
            }
        }
        sum = 0;
        ALGUI_ENSURE_ERROR(algui_for_each_map_element(&map, sum_map_elements_callback, &sum) == ALGUI_FALSE, 0);
        ALGUI_ENSURE(sum == test_sum);
        algui_cleanup_map(&map);
    }

    //test find value
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);
        for (int i = 0; i < test_data_size; ++i) {
            algui_set_map_element(&map, &i, &i);
        }
        int v = 5;
        ALGUI_ENSURE_ERROR(algui_for_each_map_element(&map, find_map_value_callback, &v) == ALGUI_TRUE, 0);
        algui_cleanup_map(&map);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_for_each_map_element_reverse(void* external_data) {
    const int test_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    const int test_data_size = sizeof(test_data) / sizeof(int);
    const int test_sum = sum_int_array(test_data, test_data_size);
    int sum;

    //test null map
    {
        ALGUI_ENSURE_ERROR(algui_for_each_map_element_reverse(NULL, sum_map_elements_callback, &sum) == ALGUI_FALSE, EINVAL);
    }

    //test null func
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);
        ALGUI_ENSURE_ERROR(algui_for_each_map_element_reverse(&map, NULL, &sum) == ALGUI_FALSE, EINVAL);
        algui_cleanup_map(&map);
    }

    //test for each on empty map
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);
        sum = 0;
        ALGUI_ENSURE_ERROR(algui_for_each_map_element_reverse(&map, sum_map_elements_callback, &sum) == ALGUI_FALSE, 0);
        ALGUI_ENSURE(sum == 0);
        algui_cleanup_map(&map);
    }

    //test for each on non-empty map without duplicate keys
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);
        for (int i = 0; i < test_data_size; ++i) {
            algui_set_map_element(&map, &i, &i);
        }
        sum = 0;
        ALGUI_ENSURE_ERROR(algui_for_each_map_element_reverse(&map, sum_map_elements_callback, &sum) == ALGUI_FALSE, 0);
        ALGUI_ENSURE(sum == test_sum);
        algui_cleanup_map(&map);
    }

    //test for each on non-empty map with some duplicate keys
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);
        for (int i = 0; i < test_data_size; ++i) {
            algui_set_map_element(&map, &i, &i);
            if (i & 1) {
                algui_set_map_element(&map, &i, &i);
            }
        }
        sum = 0;
        ALGUI_ENSURE_ERROR(algui_for_each_map_element_reverse(&map, sum_map_elements_callback, &sum) == ALGUI_FALSE, 0);
        ALGUI_ENSURE(sum == test_sum);
        algui_cleanup_map(&map);
    }

    //test find value
    {
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), int_comparator, NULL, NULL);
        for (int i = 0; i < test_data_size; ++i) {
            algui_set_map_element(&map, &i, &i);
        }
        int v = 5;
        ALGUI_ENSURE_ERROR(algui_for_each_map_element_reverse(&map, find_map_value_callback, &v) == ALGUI_TRUE, 0);
        algui_cleanup_map(&map);
    }

    return ALGUI_TRUE;
}


//the following tests run only in debug version
#ifndef NDEBUG


static uintptr_t test_fix_map_callback(const void* k, void* v, void* data) {
    //get the typed pointer values
    int* key = (int*)k;
    int* value = (int*)v;
    int* test_counter = (int*)data;

    ALGUI_ON_TEST_FAILURE(*key == *test_counter, return 1);
    ALGUI_ON_TEST_FAILURE(*value == *test_counter, return 1);

    //next counter
    ++(*test_counter);

    //keep the loop going
    return 0;
}


static ALGUI_BOOL test_optimize_map(void* external_data) {
    //null map
    {
        ALGUI_ENSURE_ERROR(algui_optimize_map(NULL) == ALGUI_FALSE, EINVAL);
    }

    //test without element destructor
    {
        //declare and initialize a map
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), algui_int_comparator, NULL, NULL);

        //insert entries, some of them repeated
        int v;
        v = 0; algui_set_map_element(&map, &v, &v);
        v = 0; algui_set_map_element(&map, &v, &v);
        v = 0; algui_set_map_element(&map, &v, &v);
        v = 1; algui_set_map_element(&map, &v, &v);
        v = 1; algui_set_map_element(&map, &v, &v);
        v = 2; algui_set_map_element(&map, &v, &v);

        //optimize the map
        algui_optimize_map(&map);

        //ensure the number of elements in the map
        ALGUI_ENSURE(map.size == v + 1);

        //ensure the map counter is 1
        ALGUI_ENSURE(map.counter == 1);

        //ensure all elements in the map are the numbers without duplicates.
        int test_counter = 0;
        algui_for_each_map_element(&map, test_fix_map_callback, &test_counter);
        ALGUI_ENSURE(test_counter == v + 1);

        algui_cleanup_map(&map);
    }

    //test with element destructor
    {
        //declare and initialize a map
        ALGUI_MAP map;
        algui_init_map(&map, sizeof(int), sizeof(int), algui_int_comparator, NULL, value_dtor);

        //insert entries, some of them repeated
        int v;
        v = 0; algui_set_map_element(&map, &v, &v);
        v = 0; algui_set_map_element(&map, &v, &v);
        v = 0; algui_set_map_element(&map, &v, &v);
        v = 1; algui_set_map_element(&map, &v, &v);
        v = 1; algui_set_map_element(&map, &v, &v);
        v = 2; algui_set_map_element(&map, &v, &v);

        //optimize the map
        value_test_counter = 0;
        algui_optimize_map(&map);

        //ensure the number of destroyed elements
        ALGUI_ENSURE(value_test_counter == 3);

        //ensure the number of elements in the map
        ALGUI_ENSURE(map.size == v + 1);

        //ensure the map counter is 1
        ALGUI_ENSURE(map.counter == 1);

        //ensure all elements in the map are the numbers without duplicates.
        int test_counter = 0;
        algui_for_each_map_element(&map, test_fix_map_callback, &test_counter);
        ALGUI_ENSURE(test_counter == v + 1);

        algui_cleanup_map(&map);
    }

    return ALGUI_TRUE;
}


#endif


void test_map(ALGUI_TEST_STATISTICS* stats) {
    algui_do_test(stats, "algui_init_map", test_init_map, NULL);
    algui_do_test(stats, "algui_cleanup_map", test_cleanup_map, NULL);
    algui_do_test(stats, "algui_is_empty_map", test_is_empty_map, NULL);
    algui_do_test(stats, "algui_get/set/remove_map_element", test_get_set_remove_map_element, NULL);
    algui_do_test(stats, "algui_for_each_map_element", test_for_each_map_element, NULL);
    algui_do_test(stats, "algui_for_each_map_element_reverse", test_for_each_map_element_reverse, NULL);
    #ifndef NDEBUG
    algui_do_test(stats, "algui_optimize_map", test_optimize_map, NULL);
    #endif
}
