#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include "algui/byte_array.h"


//construct from data
static void construct_from_data(ALGUI_BYTE_ARRAY* arr, const char* data, size_t size) {
    //init empty array
    if (!size) {
        arr->data = NULL;
        arr->size = 0;
        return;
    }

    //allocate memory for data duplication
    char* array_data = malloc(size);

    //check for malloc failure
    if (!array_data) {
        errno = ENOMEM;
        arr->data = NULL;
        arr->size = 0;
        return;
    }

    //copy the data
    memcpy(array_data, data, size);

    //init the array
    arr->data = array_data;
    arr->size = size;
}


//clear array
static void clear(ALGUI_BYTE_ARRAY* arr) {
    if (arr->size) {
        free(arr->data);
        arr->data = NULL;
        arr->size = 0;
    }
}


//copies the given data into the array.
static void copy_data(ALGUI_BYTE_ARRAY* arr, const void* data, size_t size) {
    //init empty array
    if (!size) {
        clear(arr);
        return;
    }

    //allocate memory for data duplication
    char* array_data = realloc(arr->data, size);

    //check for realloc failure
    if (!array_data) {
        errno = ENOMEM;
        return;
    }

    //copy the data
    memcpy(array_data, data, size);

    //init the array
    arr->data = array_data;
    arr->size = size;
}


/**************************************************************************************************/


//initializes a byte array to default state, i.e. empty.
void algui_byte_array_construct(ALGUI_BYTE_ARRAY* arr) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return;
    }

    //init the array
    arr->data = NULL;
    arr->size = 0;
}


//initializes a byte array from data.
void algui_byte_array_construct_from_data(ALGUI_BYTE_ARRAY* arr, const char* data, size_t size) {
    //check the args
    if (!arr || data == arr->data || (!data && size) || (data && !size)) {
        errno = EINVAL;
        arr->data = NULL;
        arr->size = 0;
        return;
    }

    construct_from_data(arr, data, size);
}


//initializes a byte array from size.
void algui_byte_array_construct_from_size(ALGUI_BYTE_ARRAY* arr, size_t size) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        arr->data = NULL;
        arr->size = 0;
        return;
    }

    //init empty array
    if (!size) {
        arr->data = NULL;
        arr->size = 0;
        return;
    }

    //allocate memory for data
    char* array_data = malloc(size);

    //check for malloc failure
    if (!array_data) {
        errno = ENOMEM;
        arr->data = NULL;
        arr->size = 0;
        return;
    }

    //init the array
    arr->data = array_data;
    arr->size = size;
}


//initializes a byte array from another byte array.
void algui_byte_array_construct_copy(ALGUI_BYTE_ARRAY* arr, const ALGUI_BYTE_ARRAY* src) {
    //check the args
    if (!arr || !src || src == arr) {
        errno = EINVAL;
        arr->data = NULL;
        arr->size = 0;
        return;
    }

    //init data
    construct_from_data(arr, src->data, src->size);
}


//copies an array into another array.
void algui_byte_array_copy(ALGUI_BYTE_ARRAY* arr, const ALGUI_BYTE_ARRAY* src) {
    //check the args
    if (!arr || !src || src == arr) {
        errno = EINVAL;
        return;
    }

    copy_data(arr, src->data, src->size);
}


//frees the memory occupied by the given byte array.
void algui_byte_array_destruct(ALGUI_BYTE_ARRAY* arr) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return;
    }

    //deallocate memory
    free(arr->data);
}


//checks if byte array is empty.
ALGUI_BOOL algui_byte_array_is_empty(const ALGUI_BYTE_ARRAY* arr) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }
    
    //an array is empty if its size is 0
    return !arr->size;
}


//returns pointer to the start of the internal buffer of a byte array.
char* algui_byte_array_get_data(const ALGUI_BYTE_ARRAY* arr) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return NULL;
    }

    return arr->data;
}


//copies the given data into the array.
void algui_byte_array_set_data(ALGUI_BYTE_ARRAY* arr, const void* data, size_t size) {
    //check the args
    if (!arr || data == arr->data || (!data && size) || (data && !size)) {
        errno = EINVAL;
        return;
    }

    copy_data(arr, data, size);
}


//returns the size of an array.
size_t algui_byte_array_get_size(const ALGUI_BYTE_ARRAY* arr) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return 0;
    }

    return arr->size;
}


//resizes an array.
void algui_byte_array_set_size(ALGUI_BYTE_ARRAY* arr, size_t size) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return;
    }

    //check for change
    if (size == arr->size) {
        return;
    }

    //check for clear
    if (!size) {
        clear(arr);
        return;
    }

    //reallocate memory to new size
    char* array_data = realloc(arr->data, size);

    //check for realloc failure
    if (!array_data) {
        errno = ENOMEM;
        return;
    }

    //setup the array
    arr->data = array_data;
    arr->size = size;
}


//clears a byte array.
void algui_byte_array_clear(ALGUI_BYTE_ARRAY* arr) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return;
    }

    //clear
    clear(arr);
}


//returns a pointer to the middle of the array, based on the given index.
char* algui_byte_array_get_element(const ALGUI_BYTE_ARRAY* arr, size_t index) {
    //check the args
    if (!arr || index >= arr->size) {
        errno = EINVAL;
        return NULL;
    }

    return arr->data + index;
}
