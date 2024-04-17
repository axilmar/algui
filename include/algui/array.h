#ifndef ALGUI_ARRAY_H
#define ALGUI_ARRAY_H


#include "byte_array.h"
#include "element_vtable.h"


/**
 * An array or elements.
 * 
 * It is automatically resized when elements are inserted or removed from it.
 * The resizing is done via realloc, and therefore elements should not contain interior pointers;
 * if interior pointers are needed, they should be replaced with array indices.
 * 
 * The array is not resized every time an element is added or removed; instead, a capacity-based
 * approach is used. For each requested size, the appropriate capacity is computed, and then 
 * the array is reallocated to that capacity, if needed.
 */
typedef struct {
    ALGUI_BYTE_ARRAY data;
    size_t size;
    size_t capacity;
    size_t elem_size;
    const ALGUI_ELEMENT_VTABLE* elem_vtable;
} ALGUI_ARRAY;


/**
 * Constructs an array.
 * @param arr array to construct.
 * @param elem_size element size; if 0, then errno is set to EINVAL and the array is constructed with null element vtable and element size = 1, effectively turning the array into a byte array.
 * @param elem_vtable vtable with element operations; if null, then errno is set to EINVAL and the array is constructed with null element vtable and element size = 1, effectively turning the array into a byte array.
 */
void algui_array_construct(ALGUI_ARRAY* arr, size_t elem_size, const ALGUI_ELEMENT_VTABLE* elem_vtable);


/**
 * Constructs an array from external data.
 * If allocation fails, errno is set to ENOMEM.
 * Data are copy-constructed.
 * @param arr array to construct.
 * @param elem_size element size; if 0, then errno is set to EINVAL and the array is constructed with null element vtable and element size = 1, effectively turning the array into a byte array.
 * @param elem_vtable vtable with element operations; if null, then errno is set to EINVAL and the array is constructed with null element vtable and element size = 1, effectively turning the array into a byte array.
 * @param data pointer to external data; they are copied into the array; if NULL, and size > 0, then errno is set to EINVAL and the array is initialized to empty.
 * @param size number of elements in data; if 0, and data is not null, then errno is set to EINVAL and the array is initialized to empty.
 */
void algui_array_construct_from_data(ALGUI_ARRAY* arr, size_t elem_size, const ALGUI_ELEMENT_VTABLE* elem_vtable, void* data, size_t size);


/**
 * Constructs an array from size.
 * If allocation fails, errno is set to ENOMEM.
 * Data are default-constructed.
 * @param arr array to construct.
 * @param elem_size element size; if 0, then errno is set to EINVAL and the array is constructed with null element vtable and element size = 1, effectively turning the array into a byte array.
 * @param elem_vtable vtable with element operations; if null, then errno is set to EINVAL and the array is constructed with null element vtable and element size = 1, effectively turning the array into a byte array.
 * @param size number of elements in data; if 0, and data is not null, then errno is set to EINVAL and the array is initialized to empty.
 */
void algui_array_construct_from_size(ALGUI_ARRAY* arr, size_t elem_size, const ALGUI_ELEMENT_VTABLE* elem_vtable, size_t size);


/**
 * Constructs an array from another array.
 * If allocation fails, errno is set to ENOMEM.
 * Data are copy-constructed.
 * @param arr array to construct.
 * @param src source array; if NULL, then errno is set to EINVAL and the array is initialized to empty.
 */
void algui_array_construct_copy(ALGUI_ARRAY* arr, const ALGUI_ARRAY* src);


/**
 * Copies an array into another array.
 * If allocation fails, errno is set to ENOMEM.
 * Data are copy-assigned; extra data are copy-constructed if the array grows, and destructed if the array shrinks.
 * @param arr destination array; if NULL, then errno is set to EINVAL and the array is initialized to empty.
 * @param src source array; if NULL, or it overlaps with destination array, then errno is set to EINVAL and the array is initialized to empty.
 */
void algui_array_copy(ALGUI_ARRAY* arr, const ALGUI_ARRAY* src);


/**
 * Destroys an array and deallocates the memory it occupies.
 * After the call, any operations on the array are undefined, unless the array is initialized again.
 * Elements are destroyed before deallocation.
 * @param arr array to destroy; if null, errno is set to EINVAL.
 */
void algui_array_destruct(ALGUI_ARRAY* arr);


/**
 * Checks if the array is empty.
 * @param arr array to check; if null, then errno is set to EINVAL and true is returned.
 * @return true if the array is empty or there is an error, false otherwise.
 */
ALGUI_BOOL algui_array_is_empty(const ALGUI_ARRAY* arr);


/**
 * Returns pointer to the start of the array data.
 * The pointer is valid as long as the array's memory block is not moved by 'realloc'.
 * @param array to get the data of; if null, then errno is set to EINVAL and NULL is returned.
 * @return pointer to the array's start of memory block or NULL if the array is empty or there is an error.
 */
void* algui_array_get_data(const ALGUI_ARRAY* arr);


/**
 * Sets the array data.
 * If the data is null and size is 0, then the array is cleared.
 * @param arr array to set the data of; the new data are copied into the array. If null, then no change is done, and errno is set to EINVAL.
 * @param data pointer to data; if null and size > 0, no change is done and errno is set to EINVAL.
 * @param size number of elements in data; if 0 and data non null, no change is done and errno is set to EINVAL.
 */
void algui_array_set_data(ALGUI_ARRAY* arr, void* data, size_t size);


/**
 * Returns the size of the array.
 * @param arr array to get the size of; if null, errno is set to EINVAL and 0 is returned.
 * @return size of array, i.e. number of elements, or 0 on empty array or on error.
 */
size_t algui_array_get_size(const ALGUI_ARRAY* arr);


/**
 * Sets the size of an array, i.e. the number of elements.
 * If the array is extended, new elements are default constructed.
 * If the size is 0, the array is cleared.
 * If the array is shrunk, extra elements are destructed.
 * If reallocation fails, errno is set to ENOMEM.
 * @param arr array to set the size of; if null, no change happens and errno is set to EINVAL.
 * @param size number of elements in the array.
 */
void algui_array_set_size(ALGUI_ARRAY* arr, size_t size);


/**
 * Clears the array, i.e. sets the size to 0, destroys the data and frees the memory occupied by the array. 
 * @param arr array to clear; if null, errno is set to EINVAL.
 */
void algui_array_clear(ALGUI_ARRAY* arr);


/**
 * Returns the capacity of the array.
 * @param arr array to get the capacity of; if null, errno is set to EINVAL, and 0 is returned.
 * @return the capacity of the array, how many elements can fit in its allocated memory.
 */
size_t algui_array_get_capacity(const ALGUI_ARRAY* arr);


/**
 * Sets the capacity of the array.
 * If the new capacity is smaller than the size, then the extra elements are destroyed.
 * If the new capacity is 0, the array is cleared.
 * @param arr array to set the capacity of; if null, no change happens and errno is set to EINVAL.
 * @param capacity new capacity of the array. 
 */
void algui_array_set_capacity(ALGUI_ARRAY* arr, size_t capacity);


/**
 * Sets the capacity to be the same as the array size.
 * @param arr array to change; if null, no change happens and errno is set to EINVAL.
 */
void algui_array_set_capacity_to_size(ALGUI_ARRAY* arr);


/**
 * Sets the capacity of the array to be appropriate for the given size.
 * 
 * The capacity value is chosen by the following algorithm:
 * 
 *  - If the requested size is 0, then capacity is 0; the array data pointer is null, i.e. no memory is allocated for the array.
 * 
 *  - If the requested size is less than or equal to the value of macro ALGUI_ARRAY_SMALLEST_CAPACITY (which defaults to 1),
 *    then capacity is set to ALGUI_ARRAY_SMALLEST_CAPACITY.
 * 
 *  - If size is less than or equal to the value of macro ALGUI_ARRAY_MAX_POWER_OF_2_CAPACITY (which defaults to 2^20),
 *    then capacity is set to the power-of-2 value that is equal to or greater than the requested number of elements.
 * 
 *  - otherwise, capacity is a multiple of the value of macro ALGUI_ARRAY_CAPACITY_STEP_AFTER_POWER_OF_2 (which defaults to 2^20),
 *    big enough to hold the requested number of elements.
 * 
 * The above macros can be set during compile time. If they are not set, then they are set to their default values, mentioned above.
 * 
 * @param arr array to set the capacity of; if null, no change happens and errno is set to EINVAL.
 * @param size the size, i.e. number of elements, to use for capacity calculations. 
 * @param resize_array_if_needed if true, then the array is resized if new capacity is smaller than current size; otherwise, the array is not resized.
 */
void algui_array_set_capacity_for_size(ALGUI_ARRAY* arr, size_t size, ALGUI_BOOL resize_array_if_needed);


/**
 * Returns the array element at the specified index.
 * @param arr array to get the element of; if null, then errno is set to EINVAL and NULL is returned.
 * @param index index of requested element; if out of bounds, errno is set to EINVAL and NULL is returned.
 * @return pointer to element at the specified index or NULL if there is an error or the index is out of bounds.
 */
void* algui_array_get_element(const ALGUI_ARRAY* arr, size_t index);


/**
 * Inserts one or more elements into an array.
 * The array is extended as required by new size, i.e. if current capacity is exceeded, the data are reallocated with an extended capacity.
 * If data are empty, i.e. data is null and size is 0, no change happens.
 * Memmove is used to move the existing elements (therefore, elements should not have pointers to elements).
 * The new elements are copy-constructed from the given elements.
 * @param arr array to insert the elements to; if null, no change happens and errno is set to EINVAL.
 * @param index insertion index; it can be from 0 to current array size, inclusive (to allow insertion at array end); 
 *  if greater than current array size, then no change happens and errno is set to EINVAL.
 * @param data pointer to data; if null and size > 0, no change is done and errno is set to EINVAL.
 * @param size number of elements in data; if 0 and data non null, no change is done and errno is set to EINVAL.
 */
void algui_array_insert_elements(ALGUI_ARRAY* arr, size_t index, void* data, size_t size);


/**
 * Inserts one element in the array.
 * Shortcut for algui_array_insert_elements(arr, index, data, 1).
 * @param arr array to insert the element to; if null, no change happens and errno is set to EINVAL.
 * @param index insertion index; it can be from 0 to current array size, inclusive (to allow insertion at array end);
 *  if greater than current array size, then no change happens and errno is set to EINVAL.
 * @param elem pointer to element to insert; if null, no change is done and errno is set to EINVAL.
 */
void algui_array_insert_element(ALGUI_ARRAY* arr, size_t index, void* elem);


/**
 * Removes one or more elements from the array.
 * The removed elements are destructed.
 * The size is reduced and the array is potentially reallocated if capacity needs to become smaller.
 * @param arr array to remove elements from; if null, no change happens and errno is set to EINVAL.
 * @param index removal index; it can be from 0 to current array size, exclusive. If out of bounds, then no change happens and errno is set to EINVAL.
 * @param size number of elements to remove; if index + size greater than current array size, then no change happens and errno is set to EINVAL.
 */
void algui_array_remove_elements(ALGUI_ARRAY* arr, size_t index, size_t size);


/**
 * Removes one element from the array.
 * Shortcut for algui_array_remove_elements(arr, index, 1).
 * @param arr array to remove the element from; if null, no change happens and errno is set to EINVAL.
 * @param index removal index; it can be from 0 to current array size, exclusive. If out of bounds, then no change happens and errno is set to EINVAL.
 */
void algui_array_remove_element(ALGUI_ARRAY* arr, size_t index);


/**
 * Searches for an element into an array sequentially.
 * @param arr array to search; if null, errno is set to EINVAL and NULL is returned.
 * @param index index to start the search from; if out of bounds, errno is set to EINVAL and NULL is returned.
 * @param elem element to search for; if null, errno is set to EINVAL and NULL is returned.
 * @param compare the comparison function; if null, errno is set to EINVAL and NULL is returned.
 * @param context optional context; if not given, then the array is passed as context.
 * @return pointer to the element found or NULL if not found or there is an error.
 */
void* algui_array_find_element_sequential_search(const ALGUI_ARRAY* arr, size_t index, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context);


/**
 * Searches for an element into an array sequentially in reverse order.
 * @param arr array to search; if null, errno is set to EINVAL and NULL is returned.
 * @param index index to start the search from; if -1, then search starts from last element; if out of bounds, errno is set to EINVAL and NULL is returned.
 * @param elem element to search for; if null, errno is set to EINVAL and NULL is returned.
 * @param compare the comparison function; if null, errno is set to EINVAL and NULL is returned.
 * @param context optional context; if not given, then the array is passed as context.
 * @return pointer to the element found or NULL if not found or there is an error.
 */
void* algui_array_find_element_sequential_search_reverse(const ALGUI_ARRAY* arr, size_t index, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context);


/**
 * Searches a sorted array for an element using binary search.
 * @param arr array to search; if null, errno is set to EINVAL and NULL is returned.
 * @param elem element to search for; if null, errno is set to EINVAL and NULL is returned.
 * @param compare the comparison function; context is either the given context or the array itself; if null, errno is set to EINVAL and NULL is returned.
 * @param context optional context; if not given, then the array is passed as context.
 * @param comp_result final comparison result; if null, then errno is set to EINVAL and NULL is returned.
 *  if -1, then the result element is the element that is less than the searched element;
 *  if 1, then the result element is the element that is greater than the searched element;
 *  if 0, then the result element is the element that is equal to the searched element.
 * @return pointer to an element that is equal, less or greater than the searched element; if there is an error, NULL is returned.
 */
void* algui_array_find_element_binary_search(const ALGUI_ARRAY* arr, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context, int* comp_result);


/**
 * Computes the element index from an element pointer.
 * @param arr array to check against the element; if null, errno is set to EINVAL and -1 is returned.
 * @param elem pointer to element; it can point to the middle of an element; if null, errno is set to EINVAL and -1 is returned.
 */
size_t algui_array_get_element_index(const ALGUI_ARRAY* arr, const void* elem);


/**
 * Inserts an element into a sorted array, using binary search to locate the previous, equal or next element.
 * @param arr array to insert the element to; if null, no change happens and errno is set to EINVAL.
 * @param elem pointer to element to insert; if null, no change is done and errno is set to EINVAL.
 * @param compare the comparison function; context is either the given context or the array itself; if null, errno is set to EINVAL and NULL is returned.
 * @param context optional context; if not given, then the array is passed as context.
 * @param fail_if_exists if the array already contains an equal item, and this flag is set, then insertion fails, otherwise the element is copy-assigned.
 * @return the position into the array that the element was inserted into, or NULL if there is an error.
 */
void* algui_array_insert_element_sorted(ALGUI_ARRAY* arr, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context, ALGUI_BOOL fail_if_exists);


/**
 * Removes an element from a sorted array, using binary search to locate the equal element.
 * @param arr array to remove the element from; if null, no change happens and errno is set to EINVAL.
 * @param elem pointer to element to remove; if null, no change is done and errno is set to EINVAL.
 * @param compare the comparison function; context is either the given context or the array itself; if null, errno is set to EINVAL and NULL is returned.
 * @param context optional context; if not given, then the array is passed as context.
 * @return the position into the array that the element was inserted into, or NULL if there is an error.
 */
void* algui_array_remove_element_sorted(ALGUI_ARRAY* arr, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context);


/**
 * Executes a callback function for each element of the array.
 * @param arr array to invoke the callback for; if null, errno is set to EINVAL and 0 is returned.
 * @param index start index; if out of bounds, errno is set to EINVAL and 0 is returned.
 * @param size number of elements to invoke the callback for; if -1, size is set to the array size; if index + size is beyond array size, errno is set to EINVAL and 0 is returned.
 * @param callback the callback to invoke for each element; if null, errno is set to EINVAL and 0 is returned.
 * @param context optional context; if not given, then the array is passed as context.
 * @return what the callback returns or 0 if all the elements were examined or there was an error.
 */
uintptr_t algui_array_for_each_element(ALGUI_ARRAY* arr, size_t index, size_t size, uintptr_t(*callback)(void* elem, size_t index, void* context), void* context);


/**
 * Executes a callback function for each element of the array in reverse order.
 * @param arr array to invoke the callback for; if null, errno is set to EINVAL and 0 is returned.
 * @param index start index; if -1, it is set to the index of the last element; if out of bounds, errno is set to EINVAL and 0 is returned.
 * @param size number of elements to invoke the callback for; if -1, it is set to the array size; if index + size is beyond array size, errno is set to EINVAL and 0 is returned.
 * @param callback the callback to invoke for each element; if null, errno is set to EINVAL and 0 is returned.
 * @param context optional context; if not given, then the array is passed as context.
 * @return what the callback returns or 0 if all the elements were examined or there was an error.
 */
uintptr_t algui_array_for_each_element_reverse(ALGUI_ARRAY* arr, size_t index, size_t size, uintptr_t(*callback)(void* elem, size_t index, void* context), void* context);


/**
 * Sorts an array (or part of it), using quicksort.
 * @param array to sort; if null, no sorting happens and errno is set to EINVAL.
 * @param index start index; if -1, it is set to 0; if out of bounds, errno is set to EINVAL and 0 is returned.
 * @param size number of elements to sort; if -1, it is set to the array size; if index + size is beyond array size, errno is set to EINVAL and 0 is returned.
 * @param compare the comparison function; context is either the given context or the array itself; if null, errno is set to EINVAL and NULL is returned.
 * @param context optional context; if not given, then the array is passed as context.
 */
void algui_array_sort_elements(ALGUI_ARRAY* arr, size_t index, size_t size, int (*compare)(const void* a, const void* b, void* context), void* context);


#endif //ALGUI_ARRAY_H
