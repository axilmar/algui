#ifndef ALGUI_TESTS_UTIL_H
#define ALGUI_TESTS_UTIL_H


//int comparator used for tests
static int int_comparator(const void* a, const void* b) {
    return *(const int*)a - *(const int*)b;
}


#endif //ALGUI_TESTS_UTIL_H
