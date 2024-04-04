#ifndef ALGUI_BUFFER_H
#define ALGUI_BUFFER_H


#include <stdint.h>
#include "bool.h"


/**
 * A buffer is a constant view on a buffer.
 * All members are const, and it is passed by value.
 */
typedef struct ALGUI_CONST_BUFFER {
    ///data.
    const char* const data;

    ///number of bytes the buffer holds.
    const size_t size;
} ALGUI_CONST_BUFFER;


/**
 * A mutable buffer.
 */
typedef struct ALGUI_BUFFER {
    ///data.
    char* data;

    ///number of bytes the buffer holds.
    size_t size;
} ALGUI_BUFFER;


/**
 * Creates a const buffer.
 * @param data pointer to data; if null and size > 0, an empty buffer is returned and errno is set to EINVAL.
 * @param buffer_size number of bytes the buffer points to; if 0 and data is not null, an empty buffer is returned and errno is set to EINVAL.
 * #return a buffer.
 */
ALGUI_CONST_BUFFER algui_create_const_buffer(const void* data, size_t buffer_size);


/**
 * Creates a const buffer from a null-terminated string.
 * @param str string to initialize the buffer from.
 * @return the buffer; if the string ptr is null, then an empty buffer is returned.
 */
ALGUI_CONST_BUFFER algui_create_const_buffer_from_string(const char* str);


/**
 * Creates a const buffer from a mutable buffer.
 * @param buffer mutable buffer to create a const buffer from; if null, then an empty buffer is returned and errno is set to EINVAL.
 * @return the buffer; if the string ptr is null, then an empty buffer is returned.
 */
ALGUI_CONST_BUFFER algui_create_const_buffer_from_buffer(const ALGUI_BUFFER* buffer);


/**
 * Initializes a buffer.
 * @param buffer buffer to initialize; if null, false is returned and errno is set to EINVAL.
 * @param buffer_size number of bytes to allocate; if 0, an empty buffer is returned. If memory allocation fails, errno is set to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_init_buffer(ALGUI_BUFFER* buffer, size_t buffer_size);


/**
 * Cleans up a buffer; deallocates the memory the buffer contains.
 * @param buffer buffer to cleanup; if null, false is returned and errno is set to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_cleanup_buffer(ALGUI_BUFFER* buffer);


/**
 * Checks if a const buffer is valid, i.e. if data is null and size is 0 or data is not null and size is not 0.
 * @param buffer buffer to check.
 * @return true if the buffer is valid, false otherwise.
 */
ALGUI_BOOL algui_is_valid_const_buffer(ALGUI_CONST_BUFFER buffer);


/**
 * Checks if a buffer is valid, i.e. if data is null and size is 0 or data is not null and size is not 0.
 * @param buffer buffer to check; if null, then false is returned and errno is set to EINVAL.
 * @return true if the buffer is valid, false otherwise.
 */
ALGUI_BOOL algui_is_valid_buffer(const ALGUI_BUFFER* buffer);


/**
 * Checks if a const buffer is empty.
 * @param buffer buffer to check.
 * @return true if the buffer is empty, false otherwise.
 */
ALGUI_BOOL algui_is_empty_const_buffer(ALGUI_CONST_BUFFER buffer);


/**
 * Checks if a buffer is empty.
 * @param buffer buffer to check; if null, then false is returned and errno is set to EINVAL.
 * @return true if the buffer is empty, false otherwise.
 */
ALGUI_BOOL algui_is_empty_buffer(const ALGUI_BUFFER* buffer);


/**
 * Copies the contents of a const buffer into a mutable buffer.
 * @param src source const buffer.
 * @param dst destination buffer; if null, or it does not have enough size, then false is returned and errno is set to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_copy_const_buffer(ALGUI_BUFFER* const dst, ALGUI_CONST_BUFFER src);


/**
 * Copies the contents of a mutable buffer into another mutable buffer.
 * @param src source buffer; if null, then false is returned and errno is set to EINVAL.
 * @param dst destination buffer; if null, or it does not have enough size, then false is returned and errno is set to EINVAL; it can be the same as the source buffer.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_copy_buffer(ALGUI_BUFFER* const dst, const ALGUI_BUFFER* src);


/**
 * Resizes a buffer.
 * @param buffer buffer to resize; if null, then false is returned and errno is set to EINVAL.
 * @param size new size; if 0, then the buffer is cleared.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_set_buffer_size(ALGUI_BUFFER* buffer, size_t size);


#endif //ALGUI_BUFFER_H
