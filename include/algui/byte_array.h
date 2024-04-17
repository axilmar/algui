#ifndef ALGUI_BYTE_ARRAY_H
#define ALGUI_BYTE_ARRAY_H


#include <stdint.h>
#include "bool.h"


/**
 * A byte array.
 */
typedef struct {
    char* data;
    size_t size;
} ALGUI_BYTE_ARRAY;


/**
 * Initializes a byte array to default state, i.e. empty.
 * @param arr array to initialize; if null, errno is set to EINVAL.
 */
void algui_byte_array_construct(ALGUI_BYTE_ARRAY* arr);


/**
 * Initializes a byte array from data.
 * @param arr array to initialize; if null, errno is set to EINVAL.
 * @param data source data; they are copied into the array. If null or not null and size > 0, errno is set to EINVAL.
 * @param size size of data. If 0 and data is not null, errno is set to EINVAL; if size is too big, errno is set to ENOMEM.
 */
void algui_byte_array_construct_from_data(ALGUI_BYTE_ARRAY* arr, const char* data, size_t size);


/**
 * Initializes a byte array from size.
 * @param arr array to initialize; if null, errno is set to EINVAL.
 * @param size size of data. If 0 and data is not null, errno is set to EINVAL; if size is too big, errno is set to ENOMEM.
 */
void algui_byte_array_construct_from_size(ALGUI_BYTE_ARRAY* arr, size_t size);


/**
 * Initializes a byte array from another byte array.
 * @param arr array to initialize; if null, errno is set to EINVAL.
 * @param src; source byte array. if null, errno is set to EINVAL.
 */
void algui_byte_array_construct_copy(ALGUI_BYTE_ARRAY* arr, const ALGUI_BYTE_ARRAY* src);


/**
 * Copies an array into another array.
 * @param arr destination array; if NULL, errno is set to EINVAL.
 * @param src source array; if null, errno is set to EINVAL.
 */
void algui_byte_array_copy(ALGUI_BYTE_ARRAY* arr, const ALGUI_BYTE_ARRAY* src);


/**
 * Frees the memory occupied by the given byte array.
 * After the call, any operations on the array are undefined, unless the array is initialized again.
 * @param arr array to initialize; if null, errno is set to EINVAL.
 */
void algui_byte_array_destruct(ALGUI_BYTE_ARRAY* arr);


/**
 * Checks if byte array is empty.
 * @param arr array to check; if null, errno is set to EINVAL.
 * @return true if empty, false if not empty or on error.
 */
ALGUI_BOOL algui_byte_array_is_empty(const ALGUI_BYTE_ARRAY* arr);


/**
 * Returns pointer to the start of the internal buffer of a byte array.
 * @param arr array to get the data of; if null, errno is set to EINVAL.
 * @return pointer to array data start or null if the array is empty or there is an error.
 */
char* algui_byte_array_get_data(const ALGUI_BYTE_ARRAY* arr);


/**
 * Copies the given data into the array.
 * @param arr array to set the data of; if null, errno is set to EINVAL.
 * @param data source data; they are copied into the array. If null or not null and size > 0, errno is set to EINVAL.
 * @param size size of data. If 0 and data is not null, errno is set to EINVAL; if size is too big, errno is set to ENOMEM.
 */
void algui_byte_array_set_data(ALGUI_BYTE_ARRAY* arr, const void* data, size_t size);


/**
 * Returns the size of an array.
 * @param arr array to get the size of; if null, errno is set to EINVAL.
 * @return array size on success, 0 if the array is empty or there is an error.
 */
size_t algui_byte_array_get_size(const ALGUI_BYTE_ARRAY* arr);


/**
 * Resizes an array.
 * @param arr array to resize; if null, errno is set to EINVAL.
 * @param size new array size; if 0, the array is cleared; if size is too big, errno is set to ENOMEM.
 */
void algui_byte_array_set_size(ALGUI_BYTE_ARRAY* arr, size_t size);


/**
 * Clears a byte array.
 * The memory the array has is deallocated.
 * @param arr array to clear; if null, errno is set to EINVAL.
 */
void algui_byte_array_clear(ALGUI_BYTE_ARRAY* arr);


/**
 * Returns a pointer to the middle of the array, based on the given index.
 * @param arr array to get an element of; if null, errno is set to EINVAL.
 * @param index index of byte; if out of bounds, errno is set to EINVAL.
 * @return pointer to array byte at the given index, or null if the array is empty or there is an error.
 */
char* algui_byte_array_get_element(const ALGUI_BYTE_ARRAY* arr, size_t index);


#endif //ALGUI_BYTE_ARRAY_H
