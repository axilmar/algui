#ifndef ALGUI_ARRAY_H
#define ALGUI_ARRAY_H


#include "bool.h"
#include "index.h"
#include "comparator.h"


/**
 * An array.
 */
typedef struct ALGUI_ARRAY {
    char* data;
    size_t element_size;
    size_t size;
} ALGUI_ARRAY;


/**
 * Initializes an array.
 * @param array array to initialize; if null, it returns false and sets errno to EINVAL.
 * @param element_size element size, in bytes. If 0, it returns false and sets errno to EINVAL.
 * @param size initial array size, in elements. If allocation fails, it returns false and sets errno to ENOMEM.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_init_array(ALGUI_ARRAY* array, size_t element_size, size_t size);


/**
 * Cleans up an array.
 * Frees the memory the array occupies.
 * @param array array to cleanup; if null, it returns false and sets errno to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_cleanup_array(ALGUI_ARRAY* array);


/**
 * Returns the element size of an array.
 * @param array array to get the element size of; if null, it returns false and sets errno to EINVAL.
 * @return element size on success, -1 on error.
 */
size_t algui_get_array_element_size(const ALGUI_ARRAY* array);


/**
 * Returns the size of an array.
 * @param array array to get the size of; if null, it returns false and sets errno to EINVAL.
 * @return size on success, -1 on error.
 */
size_t algui_get_array_size(const ALGUI_ARRAY* array);


/**
 * Sets the size of an array.
 * The array is resized via realloc.
 * If new size is zero, then the memory occupied by the array is freed.
 * @param array array to set the size of; if null, it returns false and sets errno to EINVAL.
 * @param size new array size. On memory reallocation failure, it return false and sets errno to ENOMEM.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_set_array_size(ALGUI_ARRAY* array, size_t size);


/**
 * Returns an array element from index.
 * @param array array to get the element of; if null, it returns NULL and sets errno to EINVAL.
 * @param index index of array element; if out of bounds, it returns NULL and sets errno to EINVAL.
 * @return pointer to element at the specific index, NULL on error.
 */
void* algui_get_array_element(const ALGUI_ARRAY* array, size_t index);


/**
 * Copies an element into the array.
 * @param array array to set the element of; if null, it returns false and sets errno to EINVAL.
 * @param index index of array element; if out of bounds, it returns false and sets errno to EINVAL.
 * @param element pointer to element to copy into the array; if null, it returns false and sets errno to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_set_array_element(ALGUI_ARRAY* array, size_t index, const void* element);


/**
 * Returns the index of an element into an array.
 * @param array array that contains the element; if null, it returns ALGUI_INVALID_INDEX and sets errno to EINVAL.
 * @param element pointer to element to get the index of; if null, or outside of the array, it returns ALGUI_INVALID_INDEX and sets errno to EINVAL.
 * @return index of array element or ALGUI_INVALID_INDEX on error.
 */
size_t algui_get_array_element_index(const ALGUI_ARRAY* array, const void* element);


/**
 * Searches the array sequentially in order to find an element index from start to end.
 * @param array array to search; if null, it returns ALGUI_INVALID_INDEX and sets errno to EINVAL.
 * @param start_index index to start from; if out of bounds, it returns ALGUI_INVALID_INDEX and sets errno to EINVAL.
 * @param element pointer to element to search for; if null, it returns ALGUI_INVALID_INDEX and sets errno to EINVAL.
 * @param compare the compare function; if null, then it uses memcmp to compare array elements with the given element.
 * @return index of element found or ALGUI_INVALID_INDEX if not found or on error.
 */
size_t algui_find_array_element_index(const ALGUI_ARRAY* array, size_t start_index, const void* element, ALGUI_COMPARATOR compare);


/**
 * Searches the array sequentially in order to find an element index from last position to start position.
 * @param array array to search; if null, it returns ALGUI_INVALID_INDEX and sets errno to EINVAL.
 * @param start_index index to start from; if -1, the search starts from last element; if out of bounds, it returns ALGUI_INVALID_INDEX and sets errno to EINVAL.
 * @param element pointer to element to search for; if null, it returns ALGUI_INVALID_INDEX and sets errno to EINVAL.
 * @param compare the compare function; if null, then it uses memcmp to compare array elements with the given element.
 * @return index of element found or ALGUI_INVALID_INDEX if not found or on error.
 */
size_t algui_find_array_element_index_reverse(const ALGUI_ARRAY* array, size_t start_index, const void* element, ALGUI_COMPARATOR compare);


/**
 * Searches the array sequentially in order to find an element from start to end.
 * @param array array to search; if null, it returns NULL and sets errno to EINVAL.
 * @param start_index index to start from; if out of bounds, it returns NULL and sets errno to EINVAL.
 * @param element pointer to element to search for; if null, it returns NULL and sets errno to EINVAL.
 * @param compare the compare function; if null, then it uses memcmp to compare array elements with the given element.
 * @return index of element found or NULL if not found or on error.
 */
void* algui_find_array_element(const ALGUI_ARRAY* array, size_t start_index, const void* element, ALGUI_COMPARATOR compare);


/**
 * Searches the array sequentially in order to find an element from last position to start position.
 * @param array array to search; if null, it returns NULL and sets errno to EINVAL.
 * @param start_index index to start from; if -1, the search starts from last element; if out of bounds, it returns NULL and sets errno to EINVAL.
 * @param element pointer to element to search for; if null, it returns NULL and sets errno to EINVAL.
 * @param compare the compare function; if null, then it uses memcmp to compare array elements with the given element.
 * @return pointer of element found or NULL if not found or on error.
 */
void* algui_find_array_element_reverse(const ALGUI_ARRAY* array, size_t start_index, const void* element, ALGUI_COMPARATOR compare);


/**
 * Inserts elements into the array.
 * @param array array to insert elements into; if null, it returns false and sets errno to EINVAL.
 * @param index insertion index; if greater than array size, it returns false and sets errno to EINVAL.
 * @param element pointer to elements to copy; if null, it returns NULL and sets errno to EINVAL.
 * @param count number of elements to insert; if 0, no insertion happens.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_insert_array_elements(ALGUI_ARRAY* array, size_t index, const void* element, size_t count);


/**
 * Inserts elements into the beginning of the array.
 * @param array array to insert elements into; if null, it returns false and sets errno to EINVAL.
 * @param element pointer to elements to copy; if null, it returns NULL and sets errno to EINVAL.
 * @param count number of elements to insert; if 0, no insertion happens.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_prepend_array_elements(ALGUI_ARRAY* array, const void* element, size_t count);


/**
 * Inserts elements at the end of the array.
 * @param array array to insert elements into; if null, it returns false and sets errno to EINVAL.
 * @param element pointer to elements to copy; if null, it returns NULL and sets errno to EINVAL.
 * @param count number of elements to insert; if 0, no insertion happens.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_append_array_elements(ALGUI_ARRAY* array, const void* element, size_t count);


/**
 * Inserts one element into the array.
 * @param array array to insert an element into; if null, it returns false and sets errno to EINVAL.
 * @param index insertion index; if greater than array size, it returns false and sets errno to EINVAL.
 * @param element pointer to element to copy; if null, it returns NULL and sets errno to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_insert_array_element(ALGUI_ARRAY* array, size_t index, const void* element);


/**
 * Inserts one element into the beginning of the array.
 * @param array array to insert an element into; if null, it returns false and sets errno to EINVAL.
 * @param index insertion index; if greater than array size, it returns false and sets errno to EINVAL.
 * @param element pointer to element to copy; if null, it returns NULL and sets errno to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_prepend_array_element(ALGUI_ARRAY* array, const void* element);


/**
 * Inserts one element at the end of the array.
 * @param array array to insert an element into; if null, it returns false and sets errno to EINVAL.
 * @param index insertion index; if greater than array size, it returns false and sets errno to EINVAL.
 * @param element pointer to element to copy; if null, it returns NULL and sets errno to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_append_array_element(ALGUI_ARRAY* array, const void* element);


/**
 * Removes one or more elements from an array.
 * @param array array to remove elements from; if null, it returns false and sets errno to EINVAL.
 * @param index index of first element to remove; if out of bounds, it returns false and sets errno to EINVAL.
 * @param count number of elements to remove; if 0, no removal happens; if index + count is greater than array size, it returns false and sets errno to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_remove_array_elements(ALGUI_ARRAY* array, size_t index, size_t count);


/**
 * Removes one element from an array.
 * @param array array to remove an element from; if null, it returns false and sets errno to EINVAL.
 * @param index index of first element to remove; if out of bounds, it returns false and sets errno to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_remove_array_element(ALGUI_ARRAY* array, size_t index);


/**
 * Sorts an array using the function qsort.
 * @param array array to sort; if null, it returns false and sets errno to EINVAL.
 * @param compare the compare function to use for sorting; if null, it returns false and sets errno to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_qsort_array(ALGUI_ARRAY* array, ALGUI_COMPARATOR compare);


#endif //ALGUI_ARRAY_H
