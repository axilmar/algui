#ifndef ALGUI_TESTS_UTIL_H
#define ALGUI_TESTS_UTIL_H


#include <stdlib.h>


//int comparator used for tests
static int int_comparator(const void* a, const void* b) {
    return *(const int*)a - *(const int*)b;
}


//reverse int comparator used for tests
static int int_comparator_reverse(const void* a, const void* b) {
    return *(const int*)b - *(const int*)a;
}


//returns random double between 0 and 1
static double rnd() {
    return (double)rand() / RAND_MAX;
}


//random integer within the given range
static int random_int(int min, int max) {
    int len = max - min;
    return min + (int)(rnd() * len);
}


//sum int array
static int sum_int_array(const int* data, size_t size) {
    int result = 0;
    for (size_t i = 0; i < size; ++i) {
        result += data[i];
    }
    return result;
}


#endif //ALGUI_TESTS_UTIL_H
