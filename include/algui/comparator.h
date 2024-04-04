#ifndef ALGUI_COMPARATOR_H
#define ALGUI_COMPARATOR_H


/**
 * Comparator type.
 * @param a pointer to first element.
 * @param b pointer to second element.
 * @return a negative number when a < b, a positive number when a > b, and 0 for a == b.
 */
typedef int (*ALGUI_COMPARATOR)(const void* a, const void* b);


/**
 * Comparator for type int.
 * @param a pointer to int a.
 * @param b pointer to int b.
 * @return a - b.
 */
int algui_int_comparator(const void* a, const void* b);


/**
 * Comparator for type uint32_t.
 * @param a pointer to int a.
 * @param b pointer to int b.
 * @return a - b.
 */
int algui_uint32_comparator(const void* a, const void* b);


/**
 * Comparator for type const char*.
 * It uses the strcmp function.
 * @param a pointer to string a.
 * @param b pointer to string b.
 * @return what strcmp returns.
 */
int algui_string_comparator(const void* a, const void* b);


#endif //ALGUI_COMPARATOR_H
