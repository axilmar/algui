#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "algui/buffer.h"
#include "algui/minmax.h"


//init empty buffer
static void init_empty_buffer(ALGUI_BUFFER* buffer) {
    buffer->data = NULL;
    buffer->owner = 1;
    buffer->size = 0;
}


//init buffer
ALGUI_BOOL algui_init_buffer(ALGUI_BUFFER* buffer, void* data, size_t buffer_size, ALGUI_BOOL owns_data) {
    //check the buffer
    if (!buffer) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //if external data is specified
    if (!owns_data) {
        //check data against the buffer size; they must be consistent
        if ((data == NULL && buffer_size > 0) || (data != NULL && buffer_size == 0)) {
            init_empty_buffer(buffer);
            errno = EINVAL;
            return ALGUI_FALSE;
        }

        //setup buffer from external data
        buffer->data = data;
        buffer->owner = 0;
        buffer->size = buffer_size;

        //success
        return ALGUI_TRUE;
    }

    //allocate own data

    //if no data is specified
    if (buffer_size == 0) {
        init_empty_buffer(buffer);

        //if data is not null, then there is an error, since the buffer size is 0
        if (data != NULL) {
            errno = EINVAL;
            return ALGUI_FALSE;
        }

        //success
        return ALGUI_TRUE;
    }

    //own data
    char* own_data = (char*)malloc(buffer_size);

    //check for allocation failure
    if (own_data == NULL) {
        init_empty_buffer(buffer);
        errno = ENOMEM;
        return ALGUI_FALSE;
    }

    //if external data are given, copy them to own buffer
    if (data != NULL) {
        memcpy(own_data, data, buffer_size);
    }

    //set buffer with own data
    buffer->data = own_data;
    buffer->owner = 1;
    buffer->size = buffer_size;

    //success
    return ALGUI_TRUE;
}


//cleanup a buffer
ALGUI_BOOL algui_cleanup_buffer(ALGUI_BUFFER* buffer) {
    //check the buffer
    if (buffer == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //free owned data
    if (buffer->owner) {
        if (buffer->data != NULL) {
            free(buffer->data);
            buffer->data = NULL;
            buffer->size = 0;
        }
    }

    //else reset buffer
    else if (buffer->data) {
        buffer->data = NULL;
        buffer->owner = 1;
        buffer->size = 0;
    }

    //success
    return ALGUI_TRUE;
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
ALGUI_BOOL algui_copy_buffer(ALGUI_BUFFER* dst, const ALGUI_BUFFER* src) {
    //check the dst buffer
    if (dst == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the src buffer
    if (src == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //if the destination buffer is not an owner
    if (!dst->owner) {
        //the destination must have the same size as the source
        if (dst->size != src->size) {
            errno = EINVAL;
            return ALGUI_FALSE;
        }

        //copy the data from source to destination
        memmove(dst->data, src->data, src->size);

        //success
        return ALGUI_TRUE;
    }

    //owning destination buffer

    //else if the source is not empty, copy its data to destination
    if (src->size > 0) {
        //reallocate the data, if size changes
        if (src->size != dst->size) {
            char* new_data = (char*)realloc(dst->data, src->size);

            //check for reallocation failure
            if (new_data == NULL) {
                errno = ENOMEM;
                return ALGUI_FALSE;
            }

            //set the destination data and size
            dst->data = new_data;
            dst->size = src->size;
        }

        //copy the data from source to destination
        memmove(dst->data, src->data, src->size);
    }

    //else the source is empty, clear the destination
    else {
        free(dst->data);
        dst->data = NULL;
        dst->size = 0;
    }

    return ALGUI_TRUE;
}


//set buffer size
ALGUI_BOOL algui_set_buffer_size(ALGUI_BUFFER* buffer, size_t size) {
    //check the buffer
    if (buffer == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //if the buffer does not own the memory, it is an error
    if (!buffer->owner) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //owning destination buffer

    //do nothing if size does not change
    if (size == buffer->size) {
        return ALGUI_TRUE;
    }

    //reallocate data if size is not 0
    if (size > 0) {
        char* new_data = (char*)realloc(buffer->data, size);
        if (new_data == NULL) {
            errno = ENOMEM;
            return ALGUI_FALSE;
        }
        buffer->data = new_data;
        buffer->size = size;
    }

    //else clear the buffer
    else {
        free(buffer->data);
        buffer->data = NULL;
        buffer->size = 0;
    }

    return ALGUI_TRUE;
}


//compare buffers
int algui_memcmp_buffers(const ALGUI_BUFFER* a, const ALGUI_BUFFER* b) {
    //check a
    if (a == NULL) {
        errno = EINVAL;
        return -1;
    }

    //check b
    if (b == NULL) {
        errno = EINVAL;
        return 1;
    }

    //if pointers are the same, including both being null, return 0
    if (a->data == b->data) {
        return 0;
    }

    //if a is null, return -1
    if (a->data == NULL) {
        return -1;
    }

    //if b is null, return 1
    if (b->data == NULL) {
        return 1;
    }

    //compare buffers up to smaller size
    size_t size = ALGUI_MIN(a->size, b->size);
    int comp = memcmp(a->data, b->data, size);
    
    //if not equal, then return result
    if (comp != 0) {
        return comp;
    }

    //since they are equal, use size to determine ordering
    return a->size < b->size ? -1 : a->size > b->size ? 1 : 0;
}
