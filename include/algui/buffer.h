#ifndef ALGUI_BUFFER_H
#define ALGUI_BUFFER_H


#include <stdint.h>
#include "bool.h"


typedef struct ALGUI_CONST_BUFFER {
    const void* const data;
    const size_t size;
} ALGUI_CONST_BUFFER;


typedef struct ALGUI_BUFFER {
    void* data;
    size_t size;
} ALGUI_BUFFER;


ALGUI_BOOL algui_init_buffer(ALGUI_BUFFER* buffer, size_t buffer_size);


ALGUI_BOOL algui_cleanup_buffer(ALGUI_BUFFER* const buffer);


ALGUI_BOOL algui_is_valid_const_buffer(const ALGUI_CONST_BUFFER* const buffer);


ALGUI_BOOL algui_is_valid_buffer(const ALGUI_BUFFER* const buffer);


ALGUI_BOOL algui_copy_const_buffer(ALGUI_BUFFER* const dst, const ALGUI_CONST_BUFFER* const src);


ALGUI_BOOL algui_copy_buffer(ALGUI_BUFFER* const dst, const ALGUI_BUFFER* const src);


ALGUI_BOOL algui_set_buffer_size(ALGUI_BUFFER* const buffer, size_t size);


#endif //ALGUI_BUFFER_H
