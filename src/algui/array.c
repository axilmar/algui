#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "algui/array.h"


//init array
ALGUI_BOOL algui_init_array(ALGUI_ARRAY* array, size_t element_size, size_t size) {
    //the array cannot be null
    if (array == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //the element size cannot be 0
    if (element_size == 0) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    char* data;

    //if size greater than 0, allocate memory
    if (size > 0) {
        data = malloc(element_size * size);

        //handle allocation failure
        if (data == NULL) {
            errno = ENOMEM;
            return ALGUI_FALSE;
        }
    }
    
    //else size is 0, therefore data is null
    else {
        data = NULL;
    }

    //set up the array struct
    array->data = data;
    array->element_size = element_size;
    array->size = size;

    //success
    return ALGUI_TRUE;
}


//cleanup array
ALGUI_BOOL algui_cleanup_array(ALGUI_ARRAY* array) {
    //the array cannot be null
    if (array == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //free the data
    if (array->data != NULL) {
        free(array->data);
        array->data = NULL;
        array->size = 0;
    }

    //success
    return ALGUI_TRUE;
}


//get the element size
size_t algui_get_array_element_size(const ALGUI_ARRAY* array) {
    //the array cannot be null
    if (array == NULL) {
        errno = EINVAL;
        return ALGUI_INVALID_INDEX;
    }

    //return the element size
    return array->element_size;
}


//get the array size
size_t algui_get_array_size(const ALGUI_ARRAY* array) {
    //the array cannot be null
    if (array == NULL) {
        errno = EINVAL;
        return ALGUI_INVALID_INDEX;
    }

    //return the size
    return array->size;
}


//set the array size
ALGUI_BOOL algui_set_array_size(ALGUI_ARRAY* array, size_t size) {
    //the array cannot be null
    if (array == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //if size does not change, do nothing
    if (size == array->size) {
        return ALGUI_TRUE;
    }

    void* new_data;

    //if size is greater than 0, reallocate the data
    if (size > 0) {
        new_data = realloc(array->data, array->element_size * size);

        //handle reallocation failure
        if (!new_data) {
            errno = ENOMEM;
            return ALGUI_FALSE;
        }
    }

    //else size is 0, free the data
    else {
        free(array->data);
        new_data = NULL;
    }

    //setup the structure
    array->data = new_data;
    array->size = size;

    //success
    return ALGUI_TRUE;
}


//get array element
void* algui_get_array_element(const ALGUI_ARRAY* array, size_t index) {
    //the array cannot be null
    if (array == NULL) {
        errno = EINVAL;
        return NULL;
    }

    //if index is invalid, return null
    if (index >= array->size) {
        errno = EINVAL;
        return NULL;
    }

    //return element at specific position
    return array->data + array->element_size * index;
}


//set array element
ALGUI_BOOL algui_set_array_element(ALGUI_ARRAY* array, size_t index, void* element) {
    //the array cannot be null
    if (array == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //if index is invalid, return false
    if (index >= array->size) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //if the element is null, return false
    if (element == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //copy the element
    memcpy(array->data + array->element_size * index, element, array->element_size);

    //success
    return ALGUI_TRUE;
}


//get index of array element
size_t algui_get_array_element_index(const ALGUI_ARRAY* array, const void* element) {
    //the array cannot be null
    if (array == NULL) {
        errno = EINVAL;
        return ALGUI_INVALID_INDEX;
    }

    //if array element is beyond the array, return -1
    if ((const char*)element < array->data || (const char*)element >= array->data + array->element_size * array->size) {
        errno = EINVAL;
        return ALGUI_INVALID_INDEX;
    }

    //find the actual index of the element from division of element offset by element size
    return ((const char*)element - array->data) / array->element_size;
}


//locate an element, starting from the given index.
size_t algui_find_array_element_index(const ALGUI_ARRAY* array, size_t start_index, const void* element, ALGUI_COMPARATOR compare) {
    //the array cannot be null
    if (array == NULL) {
        errno = EINVAL;
        return ALGUI_INVALID_INDEX;
    }

    //the start index cannot be beyond the array size
    if (start_index >= array->size) {
        errno = EINVAL;
        return ALGUI_INVALID_INDEX;
    }

    //the element cannot be null
    if (element == NULL) {
        errno = EINVAL;
        return ALGUI_INVALID_INDEX;
    }

    const char* array_end = array->data + array->element_size * array->size;

    //find element by comparator or by memcmp
    if (compare != NULL) {
        for (const char* array_element = array->data + array->element_size * start_index; array_element < array_end; array_element += array->element_size, ++start_index) {
            if (compare(array_element, element) == 0) {
                return start_index;
            }
        }
    }
    else {
        for (const char* array_element = array->data + array->element_size * start_index; array_element < array_end; array_element += array->element_size, ++start_index) {
            if (memcmp(array_element, element, array->element_size) == 0) {
                return start_index;
            }
        }
    }

    //not found
    return ALGUI_INVALID_INDEX;
}


//find index of element searching from end
size_t algui_find_array_element_index_reverse(const ALGUI_ARRAY* array, size_t start_index, void* element, ALGUI_COMPARATOR compare) {
    //the array cannot be null
    if (array == NULL) {
        errno = EINVAL;
        return ALGUI_INVALID_INDEX;
    }

    //the element cannot be null
    if (element == NULL) {
        errno = EINVAL;
        return ALGUI_INVALID_INDEX;
    }

    //if start index is -1, then set it to last element
    if (start_index == ALGUI_INVALID_INDEX) {
        start_index = array->size - 1;
    }

    //check start index
    if (start_index >= array->size) {
        errno = EINVAL;
        return ALGUI_INVALID_INDEX;
    }

    //find element by comparator or by memcmp
    if (compare != NULL) {
        for (const char* array_element = array->data + array->element_size * start_index; array_element >= array->data; array_element -= array->element_size, --start_index) {
            if (compare(array_element, element) == 0) {
                return start_index;
            }
        }
    }
    else {
        for (const char* array_element = array->data + array->element_size * start_index; array_element >= array->data; array_element -= array->element_size, --start_index) {
            if (memcmp(array_element, element, array->element_size) == 0) {
                return start_index;
            }
        }
    }

    //not found
    return ALGUI_INVALID_INDEX;
}


//find array element
void* algui_find_array_element(const ALGUI_ARRAY* array, size_t start_index, const void* element, ALGUI_COMPARATOR compare) {
    const size_t index = algui_find_array_element_index(array, start_index, element, compare);
    return algui_get_array_element(array, index);
}


//find array element in reverse order
void* algui_find_array_element_reverse(const ALGUI_ARRAY* array, size_t start_index, void* element, ALGUI_COMPARATOR compare) {
    const size_t index = algui_find_array_element_index_reverse(array, start_index, element, compare);
    return algui_get_array_element(array, index);
}


//insert array elements
ALGUI_BOOL algui_insert_array_elements(ALGUI_ARRAY* array, size_t index, const void* element, size_t count) {
    //the array cannot be null
    if (array == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the count
    if (count == 0) {
        return ALGUI_TRUE;
    }

    //the element cannot be null
    if (element == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //if index is -1, then index is end of array
    if (index == ALGUI_INVALID_INDEX) {
        index = array->size;
    }

    //else check the index
    else if (index > array->size) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }
    
    //allocate room for the new data
    char* new_data = realloc(array->data, array->element_size * (array->size + count));

    //handle reallocation failure
    if (!new_data) {
        errno = ENOMEM;
        return ALGUI_FALSE;
    }

    //insertion position
    char* const ins_pos = new_data + array->element_size * index;

    //make room for the new data
    memmove(ins_pos + array->element_size * count, ins_pos, array->element_size * (array->size - index));

    //copy the elements
    memcpy(ins_pos, element, array->element_size * count);

    //save the new data
    array->data = new_data;
    array->size += count;

    //success
    return ALGUI_TRUE;
}


//add elements at start
ALGUI_BOOL algui_prepend_array_elements(ALGUI_ARRAY* array, const void* element, size_t count) {
    return algui_insert_array_elements(array, 0, element, count);
}


//add elements at end
ALGUI_BOOL algui_append_array_elements(ALGUI_ARRAY* array, const void* element, size_t count) {
    return algui_insert_array_elements(array, ALGUI_INVALID_INDEX, element, count);
}


//insert one element
ALGUI_BOOL algui_insert_array_element(ALGUI_ARRAY* array, size_t index, const void* element) {
    return algui_insert_array_elements(array, 0, element, 1);
}


//prepend one element
ALGUI_BOOL algui_prepend_array_element(ALGUI_ARRAY* array, const void* element) {
    return algui_prepend_array_elements(array, element, 1);
}


//append one element
ALGUI_BOOL algui_append_array_element(ALGUI_ARRAY* array, const void* element) {
    return algui_append_array_elements(array, element, 1);
}


//removes one or more array elements
ALGUI_BOOL algui_remove_array_elements(ALGUI_ARRAY* array, size_t index, size_t count) {
    //the array cannot be null
    if (array == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the index and index + count; must be within the array
    if (index >= array->size || index + count > array->size) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //if count is 0, do nothing else
    if (count == 0) {
        return ALGUI_TRUE;
    }

    char* rem_pos = array->data + array->element_size * index;

    //move the data
    memmove(rem_pos, rem_pos + array->element_size * count, array->element_size * (array->size - index - count));

    //shrink the array
    array->size -= count;
    if (array->size > 0) {
        array->data = realloc(array->data, array->element_size * array->size);
    }
    else {
        free(array->data);
        array->data = NULL;
    }

    //success
    return ALGUI_TRUE;
}


//removes one element
ALGUI_BOOL algui_remove_array_element(ALGUI_ARRAY* array, size_t index) {
    return algui_remove_array_elements(array, index, 1);
}


//sort the array
ALGUI_BOOL algui_qsort_array(ALGUI_ARRAY* array, ALGUI_COMPARATOR compare) {
    //the array cannot be null
    if (array == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //the compare function cannot be null
    if (compare == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //if there are no data to sort, return true
    if (array->size == 0) {
        return ALGUI_TRUE;
    }

    //sort the array
    qsort(array->data, array->size, array->element_size, compare);
   
    //success
    return ALGUI_TRUE;
}
