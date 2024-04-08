#ifndef ALGUI_BUFFER_H
#define ALGUI_BUFFER_H


#include <stdint.h>
#include "bool.h"


/**
 * Macro that initializes a buffer to empty.
 */
#define ALGUI_BUFFER_INIT { NULL, 1, 0 }


/**
 * A buffer.
 */
typedef struct ALGUI_BUFFER {
    ///data.
    char* data;

    ///if the buffer owns the memory block.
    size_t owner : 1;

    ///number of bytes the buffer holds.
    size_t size : sizeof(size_t) * 8 - 1;
} ALGUI_BUFFER;


/**
 * Initializes a buffer.
 * @param buffer buffer to initialize; if null, false is returned and errno is set to EINVAL.
 * @param data pointer to data.
 * @param buffer_size number of bytes to allocate, if data is null, or number of bytes the data pointers points to. If memory allocation fails, errno is set to EINVAL.
 * @param own_data if true, the buffer gets a copy of the data, if the data are specified; else the buffer contains external data.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_init_buffer(ALGUI_BUFFER* buffer, void* data, size_t buffer_size, ALGUI_BOOL own_data);


/**
 * Cleans up a buffer; deallocates the memory the buffer contains.
 * @param buffer buffer to cleanup; if null, false is returned and errno is set to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_cleanup_buffer(ALGUI_BUFFER* buffer);


/**
 * Checks if a buffer is empty.
 * @param buffer buffer to check; if null, then false is returned and errno is set to EINVAL.
 * @return true if the buffer is empty, false otherwise.
 */
ALGUI_BOOL algui_is_empty_buffer(const ALGUI_BUFFER* buffer);


/**
 * Copies the contents of a mutable buffer into another mutable buffer.
 * @param src source buffer; if null, then false is returned and errno is set to EINVAL.
 * @param dst destination buffer; if null, or it does not have enough size, then false is returned and errno is set to EINVAL; it can be the same as the source buffer.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_copy_buffer(ALGUI_BUFFER* dst, const ALGUI_BUFFER* src);


/**
 * Resizes a buffer.
 * @param buffer buffer to resize; if null, then false is returned and errno is set to EINVAL.
 * @param size new size; if 0, then the buffer is cleared.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_set_buffer_size(ALGUI_BUFFER* buffer, size_t size);


/**
 * Compares two buffers via memcmp.
 * @param a first buffer; if null, then the result is -1 and errno is set to EINVAL.
 * @param b second buffer; if null, then the result is 1 and errno is set to EINVAL.
 * @return negative int if a < b, 0 if a == b, positive int if a > b.
 */
int algui_memcmp_buffers(const ALGUI_BUFFER* a, const ALGUI_BUFFER* b);


#endif //ALGUI_BUFFER_H
