#ifndef ALGUI_ARRAY_UTIL_H
#define ALGUI_ARRAY_UTIL_H


#include <stdint.h>
#include "bool.h"
#include "comparator.h"


/**
 * Macro that computes the element address from data, element size, and index.
 * No checks are performed.
 * @param TYPE type to cast the address to.
 * @param DATA array base.
 * @param ELEMENT_SIZE element size.
 * @param INDEX index.
 * @return element at the given index.
 */
#define ALGUI_GET_ARRAY_ELEMENT_UTIL(TYPE, DATA, ELEMENT_SIZE, INDEX)\
    ((TYPE*)(((char *)(DATA)) + ((ELEMENT_SIZE) * (INDEX))))


/**
 * Performs a binary search into the given array, which must be sorted prior to this call; returns index of element found.
 * If duplicates exist in the array, then it returns the index to the rightmost element.
 * @param data pointer to array data to search; if null, it returns ALGUI_INVALID_INDEX and sets errno to EINVAL.
 * @param size array size.
 * @param element_size array element size; if 0, it returns ALGUI_INVALID_INDEX and sets errno to EINVAL.
 * @param element pointer to element to search for; if null, it returns ALGUI_INVALID_INDEX and sets errno to EINVAL.
 * @param compare the compare function to use for locating an element; if null, it returns ALGUI_INVALID_INDEX and sets errno to EINVAL.
 * @return index of element or ALGUI_INVALID_INDEX if not found or on error.
 */
size_t algui_find_array_element_index_binary_search_util(const char* data, size_t size, size_t element_size, const void* element, ALGUI_COMPARATOR compare);


#endif //ALGUI_ARRAY_UTIL_H
