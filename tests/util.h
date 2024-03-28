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


//random integer within the given range
static int random_int(int min, int max) {
    int len = max - min;
    return min + (int)(rand() * (double)len);
}


#endif //ALGUI_TESTS_UTIL_H
