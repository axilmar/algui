#ifndef ALGUI_ALGORITHM_H
#define ALGUI_ALGORITHM_H


#include <stdint.h>
#include "bool.h"
#include "element_vtable.h"


/**
 * Searches an array of elements for a specific value, testing each element of the block in sequence.
 * @param elem_size element size; if 0, errno is set to EINVAL and NULL is returned.
 * @param data pointer to data; if NULL, and size > 0, errno is set to EINVAL and NULL is returned.
 * @param size number of elements in the data; if 0 and data != NULL, errno is set to EINVAL and NULL is returned.
 * @param index index to start the search from; if out of bounds, errno is set to EINVAL and NULL is returned.
 * @param elem pointer to element to find; if null, errno is set to EINVAL and NULL is returned.
 * @param compare the compare function; it shall return a negative number if a < b, 0 if a = b and a positive number if a > b; if null, errno is set to EINVAL and NULL is returned.
 * @param context pointer to context data to pass to the compare function.
 * @return if an element is found, then a pointer to it is returned; otherwise, or if there is an error, NULL is returned.
 */
void* algui_find_element_sequential_search(size_t elem_size, void* data, size_t size, size_t index, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context);


/**
 * Searches an array of elements for a specific value, testing each element of the block in sequence, in reverse order.
 * @param elem_size element size; if 0, errno is set to EINVAL and NULL is returned.
 * @param data pointer to data; if NULL, and size > 0, errno is set to EINVAL and NULL is returned.
 * @param index index to start the search from; if -1, then search starts from last element; if out of bounds, errno is set to EINVAL and NULL is returned.
 * @param size number of elements in the data; if 0 and data != NULL, errno is set to EINVAL and NULL is returned.
 * @param elem pointer to element to find; if null, errno is set to EINVAL and NULL is returned.
 * @param compare the compare function; it shall return a negative number if a < b, 0 if a = b and a positive number if a > b; if null, errno is set to EINVAL and NULL is returned.
 * @param context pointer to context data to pass to the compare function.
 * @return if an element is found, then a pointer to it is returned; otherwise, if the size is 0, or if there is an error, NULL is returned.
 */
void* algui_find_element_sequential_search_reverse(size_t elem_size, void* data, size_t size, size_t index, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context);


/**
 * Searches a sorted array of elements for a specific value, using binary search.
 * The function finds either the element that is equal to the searched element, 
 * or the element that comes before the searched element,
 * or the element that comes after the searched element.
 * @param elem_size element size; if 0, errno is set to EINVAL and NULL is returned.
 * @param data pointer to data; if NULL, and size > 0, errno is set to EINVAL and NULL is returned.
 * @param size number of elements in the data; if 0 and data != NULL, errno is set to EINVAL and NULL is returned.
 * @param elem pointer to element to find; if null, errno is set to EINVAL and NULL is returned.
 * @param compare the compare function; it shall return a negative number if a < b, 0 if a = b and a positive number if a > b; if null, errno is set to EINVAL and NULL is returned.
 * @param context pointer to context data to pass to the compare function.
 * @param comp_result final comparison result; if null, then errno is set to EINVAL and NULL is returned.
 *  if -1, then the result element is the element that is less than the searched element;
 *  if 1, then the result element is the element that is greater than the searched element;
 *  if 0, then the result element is the element that is equal to the searched element.
 * @return pointer to an element that is equal, less or greater than the searched element; if there is an error, NULL is returned.
 */
void* algui_find_element_binary_search(size_t elem_size, void* data, size_t size, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context, int* comp_result);


/**
 * Computes the index of an element within an array.
 * @param data pointer to data; if NULL, errno is set to EINVAL and -1 is returned.
 * @param size number of elements in the data.
 * @param elem pointer to element; it can point to the middle of an element; if null, errno is set to EINVAL and -1 is returned.
 * #return index of element or -1 if the element is not in the array.
 */
size_t algui_get_element_index(size_t elem_size, const void* data, size_t size, const void* elem);


/**
 * Inserts an element sorted into an already sorted array.
 * @param elem_vtable vtable to use for copy-constructing the new element in place; if null, then errno is set to EINVAL and NULL is returned.
 * @param elem_size element size; if 0, errno is set to EINVAL and NULL is returned.
 * @param data pointer to data; it must have sufficient room for the insertion; if NULL, errno is set to EINVAL and NULL is returned.
 * @param size number of elements in the data; 0 is allowed, but the data should have room for at least one element.
 * @param elem pointer to element to insert; if null, errno is set to EINVAL and NULL is returned.
 * @param compare the compare function; it shall return a negative number if a < b, 0 if a = b and a positive number if a > b; if null, errno is set to EINVAL and NULL is returned.
 * @param context pointer to context data to pass to the compare function.
 * @param fail_if_exists if the array already contains an equal item, and this flag is set, then insertion fails, otherwise the element is copy-assigned.
 * @return the position into the array that the element was inserted into, or NULL if there is an error.
 */
void* algui_insert_element_sorted(const ALGUI_ELEMENT_VTABLE* elem_vtable, size_t elem_size, void* data, size_t size, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context, ALGUI_BOOL fail_if_exists);


/**
 * Removes an element sorted from an already sorted array.
 * @param elem_vtable vtable to use for destructing the existing element in place; if null, then errno is set to EINVAL and NULL is returned.
 * @param elem_size element size; if 0, errno is set to EINVAL and NULL is returned.
 * @param data pointer to data; if NULL, errno is set to EINVAL and NULL is returned.
 * @param size number of elements in the data; if 0, then NULL is returned.
 * @param elem pointer to element to remove; if null, errno is set to EINVAL and NULL is returned.
 * @param compare the compare function; it shall return a negative number if a < b, 0 if a = b and a positive number if a > b; if null, errno is set to EINVAL and NULL is returned.
 * @param elem_vtable vtable to use for destructing the existing element in place; if null, then errno is set to EINVAL and NULL is returned.
 * @return the position into the array that the element was removed from, or NULL if there is an error.
 */
void* algui_remove_element_sorted(const ALGUI_ELEMENT_VTABLE* elem_vtable, size_t elem_size, void* data, size_t size, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context);


/**
 * Executes a callback for each element of an array.
 * @param elem_size element size; if 0, errno is set to EINVAL and 0 is returned.
 * @param data pointer to data; if NULL, and size > 0, errno is set to EINVAL and 0 is returned.
 * @param size number of elements in the data; if 0 and data != NULL, errno is set to EINVAL and 0 is returned.
 * @param callback callback to execute for each element; if it does not return 0, then the loop stops and that value is returned;
 *  otherwise, it continues until every element is processed; if null, errno is set to EINVAL and 0 is returned.
 * @param context pointer to context data to pass to the compare function.
 * @return what the function returns or 0 on error.
 */
uintptr_t algui_for_each_element(size_t elem_size, void* data, size_t size, uintptr_t(*callback)(void* elem, size_t index, void* context), void* context);


/**
 * Executes a callback for each element of an array in reverse order.
 * @param elem_size element size; if 0, errno is set to EINVAL and 0 is returned.
 * @param data pointer to data; if NULL, and size > 0, errno is set to EINVAL and 0 is returned.
 * @param size number of elements in the data; if 0 and data != NULL, errno is set to EINVAL and 0 is returned.
 * @param callback callback to execute for each element; if it does not return 0, then the loop stops and that value is returned; 
 *  otherwise, it continues until every element is processed; if null, errno is set to EINVAL and 0 is returned.
 * @param context pointer to context data to pass to the compare function.
 * @return what the function returns or 0 on error.
 */
uintptr_t algui_for_each_element_reverse(size_t elem_size, void* data, size_t size, uintptr_t(*callback)(void* elem, size_t index, void* context), void* context);


/**
 * Quicksort sorting algorithm for data.
 * Wrapper around the platform's qsort_s function.
 * Provided because Microsof'ts version uses reverse order of parameters.
 * @param elem_size element size; if 0, errno is set to EINVAL.
 * @param data pointer to data; if NULL, and size > 0, errno is set to EINVAL.
 * @param size number of elements in the data; if 0 and data != NULL, errno is set to EINVAL.
 * @param compare the compare function; it shall return a negative number if a < b, 0 if a = b and a positive number if a > b; if null, errno is set to EINVAL.
 * @param context context.
 */
void algui_qsort_elements(size_t elem_size, void* data, size_t size, int (*compare)(const void* a, const void* b, void* context), void* context);


#endif //ALGUI_ALGORITHM_H
