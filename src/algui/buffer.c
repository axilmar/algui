#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "algui/buffer.h"


//initialize a buffer from data.
ALGUI_CONST_BUFFER algui_create_const_buffer(const void* data, size_t buffer_size) {
    //check the args
    if ((data == NULL && buffer_size > 0) || (data != NULL && buffer_size == 0)) {
        errno = EINVAL;
        ALGUI_CONST_BUFFER empty_buffer = { NULL, 0 };
        return empty_buffer;
    }

    //success
    ALGUI_CONST_BUFFER result = { data, buffer_size };
    return result;
}


//init buffer from string
ALGUI_CONST_BUFFER algui_create_const_buffer_from_string(const char* str) {
    return str != NULL ? algui_create_const_buffer(str, strlen(str)) : algui_create_const_buffer(NULL, 0);
}


//convert buffer to buffer
ALGUI_CONST_BUFFER algui_create_const_buffer_from_buffer(const ALGUI_BUFFER* buffer) {
    //check the args
    if (buffer == NULL) {
        errno = EINVAL;
        ALGUI_CONST_BUFFER empty_buffer = { NULL, 0 };
        return empty_buffer;
    }

    //success
    ALGUI_CONST_BUFFER result = { buffer->data, buffer->size };
    return result;
}


//init buffer
ALGUI_BOOL algui_init_buffer(ALGUI_BUFFER* buffer, size_t buffer_size) {
    //check the buffer
    if (!buffer) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //allocate memory
    if (buffer_size > 0) {
        buffer->data = malloc(buffer_size);

        //if allocation failed
        if (buffer->data == NULL) {
            buffer->size = 0;
            errno = ENOMEM;
            return ALGUI_FALSE;
        }
    }
    else {
        buffer->data = NULL;
    }

    //success
    buffer->size = buffer_size;
    return ALGUI_TRUE;
}


//cleanup a buffer
ALGUI_BOOL algui_cleanup_buffer(ALGUI_BUFFER* buffer) {
    //check the buffer
    if (!algui_is_valid_buffer(buffer)) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //free the buffer data if there are not null
    if (buffer->data != NULL) {
        free(buffer->data);
        buffer->data = NULL;
        buffer->size = 0;
    }

    //success
    return ALGUI_TRUE;
}


//check if a buffer is valid
ALGUI_BOOL algui_is_valid_const_buffer(ALGUI_CONST_BUFFER buffer) {
    return ((buffer.data == NULL && buffer.size == 0) || (buffer.data != NULL && buffer.size > 0));
}


//check if a buffer is valid
ALGUI_BOOL algui_is_valid_buffer(const ALGUI_BUFFER* buffer) {
    if (buffer == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }
    return ((buffer->data == NULL && buffer->size == 0) || (buffer->data != NULL && buffer->size > 0));
}


//check if const buffer is empty
ALGUI_BOOL algui_is_empty_const_buffer(ALGUI_CONST_BUFFER buffer) {
    return buffer.size == 0;
}


//check if a buffer is empty
ALGUI_BOOL algui_is_empty_buffer(const ALGUI_BUFFER* buffer) {
    if (buffer == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }
    return buffer->size == 0;
}


//copy buffer
ALGUI_BOOL algui_copy_const_buffer(ALGUI_BUFFER* const dst, const ALGUI_CONST_BUFFER src) {
    //check the buffers; the destination must have sufficient space
    if (!algui_is_valid_buffer(dst) || !algui_is_valid_const_buffer(src) || dst->size < src.size) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //copy the data from source to destination
    memmove(dst->data, src.data, src.size);
    
    return ALGUI_TRUE;
}


//copy buffer
ALGUI_BOOL algui_copy_buffer(ALGUI_BUFFER* const dst, const ALGUI_BUFFER* src) {
    //check the buffers; the destination must have sufficient space
    if (!algui_is_valid_buffer(dst) || !algui_is_valid_buffer(src) || dst->size < src->size) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //copy the data from source to destination
    memmove(dst->data, src->data, src->size);

    return ALGUI_TRUE;
}


//set buffer size
ALGUI_BOOL algui_set_buffer_size(ALGUI_BUFFER* buffer, size_t size) {
    //check the buffer
    if (!algui_is_valid_buffer(buffer)) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //if there is no change, do nothing
    if (size == buffer->size) {
        return ALGUI_TRUE;
    }

    //if some size is specified, reallocate the data
    if (size > 0) {
        //reallocate
        void* data = realloc(buffer->data, size);
        
        //if reallocation failed
        if (data == NULL) {
            errno = ENOMEM;
            return ALGUI_FALSE;
        }
        
        //set the buffer
        buffer->data = data;
        buffer->size = size;
    }

    //else if size is 0, reset the buffer
    else {
        free(buffer->data);
        buffer->data = NULL;
        buffer->size = 0;
    }

    return ALGUI_TRUE;
}


