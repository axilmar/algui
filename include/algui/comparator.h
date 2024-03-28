#ifndef ALGUI_COMPARATOR_H
#define ALGUI_COMPARATOR_H


/**
 * Comparator type.
 * @param a pointer to first element.
 * @param b pointer to second element.
 * @return a negative number when a < b, a positive number when a > b, and 0 for a == b.
 */
typedef int (*ALGUI_COMPARATOR)(const void* a, const void* b);


#endif //ALGUI_COMPARATOR_H
