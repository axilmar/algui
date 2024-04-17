#include <errno.h>
#include <memory.h>
#include "algui/array.h"
#include "algui/algorithm.h"
#include "algui/stdlib.h"


#ifndef ALGUI_ARRAY_SMALLEST_CAPACITY
#define ALGUI_ARRAY_SMALLEST_CAPACITY                      1
#endif


#ifndef ALGUI_ARRAY_MAX_POWER_OF_2_CAPACITY
#define ALGUI_ARRAY_MAX_POWER_OF_2_CAPACITY                1048576
#endif


#ifndef ALGUI_ARRAY_CAPACITY_STEP_AFTER_POWER_OF_2
#define ALGUI_ARRAY_CAPACITY_STEP_AFTER_POWER_OF_2         1048576
#endif


//calculate capacity for size
static size_t calc_capacity(const size_t size) {
    //size is not checked for 0 because 0 leads to clearing the array,
    //which is a special case, and therefore handled locally in the caller functions.

    //select the power-or-2 capacity that is greater than the size up to a limit
    if (size <= ALGUI_ARRAY_MAX_POWER_OF_2_CAPACITY) {
        size_t capacity = ALGUI_ARRAY_SMALLEST_CAPACITY;
        while (capacity < size) {
            capacity <<= 1;
        }
        return capacity;
    }

    //after the limit, capacity increases by a step
    return ((size + ALGUI_ARRAY_CAPACITY_STEP_AFTER_POWER_OF_2 - 1) / ALGUI_ARRAY_CAPACITY_STEP_AFTER_POWER_OF_2) * ALGUI_ARRAY_CAPACITY_STEP_AFTER_POWER_OF_2;
}


//get element
static inline void* element(const ALGUI_ARRAY* arr, size_t index) {
    return arr->data.data + index * arr->elem_size;
}


//check if data and size are good
static inline ALGUI_BOOL valid_data(const void* data, size_t size) {
    return (data && size) || (!data && !size);
}


//check if data array outside of the array object
static inline ALGUI_BOOL data_outside_array(const ALGUI_ARRAY* arr, const void* data, size_t size) {
    return (char*)data + size <= arr->data.data || (char*)data >= arr->data.data + arr->data.size;
}


//check data against the array
static inline ALGUI_BOOL check_data(const ALGUI_ARRAY* arr, const void* data, size_t size) {
    return valid_data(data, size) && data_outside_array(arr, data, size);
}


//check data type of source
static inline ALGUI_BOOL check_type(const ALGUI_ARRAY* arr, const ALGUI_ARRAY* src) {
    return arr->elem_size == src->elem_size && arr->elem_vtable == src->elem_vtable;
}


//construct from data
static void construct_from_data(ALGUI_ARRAY* arr, size_t elem_size, const ALGUI_ELEMENT_VTABLE* elem_vtable, void* data, size_t size) {
    //setup element details
    arr->elem_size = elem_size;
    arr->elem_vtable = elem_vtable;

    //init empty array
    if (!size) {
        algui_byte_array_construct(&arr->data);
        arr->size = 0;
        arr->capacity = 0;
        return;
    }

    //calculate the initial capacity
    size_t capacity = calc_capacity(size);

    //init the byte array from data
    algui_byte_array_construct_from_data(&arr->data, data, elem_size * capacity);

    //check for allocation failure
    if (!arr->data.size) {
        arr->size = 0;
        arr->capacity = 0;
        return;
    }

    //save the size and capacity
    arr->size = size;
    arr->capacity = capacity;

    //copy the elements
    elem_vtable->copy_constructor(arr, arr->data.data, data, elem_size, size);
}


//clear array
static void clear(ALGUI_ARRAY* arr) {
    if (arr->size) {
        arr->elem_vtable->destructor(arr, arr->data.data, arr->elem_size, arr->size);
        algui_byte_array_clear(&arr->data);
        arr->size = 0;
        arr->capacity = 0;
    }
}


//resize array to specify capacity
static ALGUI_BOOL set_capacity(ALGUI_ARRAY* arr, size_t capacity) {
    //calculate the byte size of the array
    const size_t byte_size = capacity * arr->elem_size;

    //set the byte size
    algui_byte_array_set_size(&arr->data, byte_size);

    //if byte size failed to be set
    if (arr->data.size != byte_size) {
        return ALGUI_FALSE;
    }

    //success; save capacity
    arr->capacity = capacity;
    return ALGUI_TRUE;
}


//reserve enough element space for the given size
static ALGUI_BOOL set_capacity_for_size(ALGUI_ARRAY* arr, size_t size) {
    //calculate new capacity for size
    size_t capacity = calc_capacity(size);

    //if there is no change, do nothing else
    if (capacity == arr->capacity) {
        return ALGUI_TRUE;
    }

    //set capacity
    return set_capacity(arr, capacity);
}


//set data
static void set_data(ALGUI_ARRAY* arr, void* data, size_t size) {
    //if size greater than capacity is requested
    if (size > arr->capacity) {
        if (!set_capacity_for_size(arr, size)) {
            return;
        }
        const size_t old_size = arr->size;
        arr->size = size;
        arr->elem_vtable->copy_assignment(arr, arr->data.data, data, arr->elem_size, old_size);
        arr->elem_vtable->copy_constructor(arr, element(arr, old_size), data, arr->elem_size, size - old_size);
    }

    //else if size is 0
    else if (!size) {
        clear(arr);
    }

    //else if size greater than existing size but not beyond capacity
    else if (size > arr->size) {
        const size_t old_size = arr->size;
        arr->size = size;
        arr->elem_vtable->copy_assignment(arr, arr->data.data, data, arr->elem_size, old_size);
        arr->elem_vtable->copy_constructor(arr, element(arr, old_size), data, arr->elem_size, size - old_size);
    }

    //else size is less than existing size
    else if (size < arr->size) {
        arr->elem_vtable->destructor(arr, element(arr, arr->size), arr->elem_size, arr->size - size);
        arr->elem_vtable->copy_assignment(arr, arr->data.data, data, arr->elem_size, size);
        set_capacity_for_size(arr, size);
        arr->size = size;
    }

    //else equal size
    else {
        arr->elem_vtable->copy_assignment(arr, arr->data.data, data, arr->elem_size, size);
    }
}


//set size
static void set_size(ALGUI_ARRAY* arr, size_t size) {
    //if size increases beyond capacity
    if (size > arr->capacity) {
        if (!set_capacity_for_size(arr, size)) {
            return;
        }
        const size_t old_size = arr->size;
        arr->size = size;
        arr->elem_vtable->default_constructor(arr, element(arr, arr->size), arr->elem_size, size - old_size);
    }

    //else if size is 0
    else if (!size) {
        clear(arr);
    }

    //else if size increases but not beyond capacity
    else if (size > arr->size) {
        const size_t old_size = arr->size;
        arr->size = size;
        arr->elem_vtable->default_constructor(arr, element(arr, arr->size), arr->elem_size, size - old_size);
    }

    //else if size decreases below current size
    else if (size < arr->size) {
        arr->elem_vtable->destructor(arr, element(arr, size), arr->elem_size, arr->size - size);
        set_capacity_for_size(arr, size);
        arr->size = size;
    }
}


/**************************************************************************************************/


//init array
void algui_array_construct(ALGUI_ARRAY* arr, size_t elem_size, const ALGUI_ELEMENT_VTABLE* elem_vtable) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return;
    }

    //elem size and vtable must be given, otherwise it is an error
    if (!elem_size || !elem_vtable) {
        errno = EINVAL;
        elem_size = 1;
        elem_vtable = &algui_null_element_vtable;
    }

    //init the array structure
    algui_byte_array_construct(&arr->data);
    arr->size = 0;
    arr->capacity = 0;
    arr->elem_size = elem_size;
    arr->elem_vtable = elem_vtable;
}


//init array from data
void algui_array_construct_from_data(ALGUI_ARRAY* arr, size_t elem_size, const ALGUI_ELEMENT_VTABLE* elem_vtable, void* data, size_t size) {
    if (!arr || !elem_size || !elem_vtable || !valid_data(data, size)) {
        algui_array_construct(arr, 0, NULL);
        return;
    }
    construct_from_data(arr, elem_size, elem_vtable, data, size);
}


//construct array from size
void algui_array_construct_from_size(ALGUI_ARRAY* arr, size_t elem_size, const ALGUI_ELEMENT_VTABLE* elem_vtable, size_t size) {
    //check the args
    if (!arr || !elem_size || !elem_vtable) {
        algui_array_construct(arr, 0, NULL);
        return;
    }

    //setup element details
    arr->elem_size = elem_size;
    arr->elem_vtable = elem_vtable;

    //init empty array
    if (!size) {
        algui_byte_array_construct(&arr->data);
        arr->size = 0;
        arr->capacity = 0;
        return;
    }

    //calculate the initial capacity
    size_t capacity = calc_capacity(size);

    //init the byte array from data
    algui_byte_array_construct_from_size(&arr->data, elem_size * capacity);

    //check for allocation failure
    if (!arr->data.size) {
        arr->size = 0;
        arr->capacity = 0;
        return;
    }

    //save the size and capacity
    arr->size = size;
    arr->capacity = capacity;

    //initialize the elements
    elem_vtable->default_constructor(arr, arr->data.data, elem_size, size);
}


//copy construct array
void algui_array_construct_copy(ALGUI_ARRAY* arr, const ALGUI_ARRAY* src) {
    if (!arr || !src || !valid_data(src->data.data, src->data.size)) {
        algui_array_construct(arr, 0, NULL);
        return;
    }
    construct_from_data(arr, src->elem_size, src->elem_vtable, src->data.data, src->size);
}


//copy array
void algui_array_copy(ALGUI_ARRAY* arr, const ALGUI_ARRAY* src) {
    if (!arr || !src || !check_type(arr, src) || !check_data(arr, src->data.data, src->data.size)) {
        errno = EINVAL;
        return;
    }
    set_data(arr, src->data.data, src->size);
}


//destruct array
void algui_array_destruct(ALGUI_ARRAY* arr) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return;
    }

    //destroy data if non-empty
    if (arr->size) {
        arr->elem_vtable->destructor(arr, arr->data.data, arr->elem_size, arr->size);
        algui_byte_array_destruct(&arr->data);
    }
}


//check if array is empty
ALGUI_BOOL algui_array_is_empty(const ALGUI_ARRAY* arr) {
    if (!arr) {
        errno = EINVAL;
        return ALGUI_TRUE;
    }
    return !arr->size;
}


//get data
void* algui_array_get_data(const ALGUI_ARRAY* arr) {
    if (!arr) {
        errno = EINVAL;
        return NULL;
    }
    return arr->data.data;
}


//set data
void algui_array_set_data(ALGUI_ARRAY* arr, void* data, size_t size) {
    if (!arr || !check_data(arr, data, size)) {
        errno = EINVAL;
        return;
    }
    set_data(arr, data, size);
}


//returns the current size
size_t algui_array_get_size(const ALGUI_ARRAY* arr) {
    if (!arr) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }
    return arr->size;
}


//resizes the array
void algui_array_set_size(ALGUI_ARRAY* arr, size_t size) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return;
    }

    //set the size
    set_size(arr, size);
}


//clears the array
void algui_array_clear(ALGUI_ARRAY* arr) {
    if (!arr) {
        errno = EINVAL;
        return;
    }
    clear(arr);
}


//get capacity
size_t algui_array_get_capacity(const ALGUI_ARRAY* arr) {
    if (!arr) {
        errno = EINVAL;
        return 0;
    }
    return arr->capacity;
}


//set capacity
void algui_array_set_capacity(ALGUI_ARRAY* arr, size_t capacity) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return;
    }

    //if more capacity
    if (capacity > arr->capacity) {
        set_capacity(arr, capacity);
    }

    //else if 0 capacity
    else if (!capacity) {
        clear(arr);
    }

    //else if capacity smaller than size
    else if (capacity < arr->size) {
        arr->elem_vtable->destructor(arr, element(arr, capacity), arr->elem_size, arr->size - capacity);
        set_capacity(arr, capacity);
        arr->size = capacity;
    }

    //else if smaller capacity but greater than size
    else if (capacity < arr->capacity) {
        set_capacity(arr, capacity);
    }
}


//sets the capacity equal to size
void algui_array_set_capacity_to_size(ALGUI_ARRAY* arr) {
    if (!arr) {
        errno = EINVAL;
        return;
    }
    set_capacity(arr, arr->size);
}


//reserve capacity
void algui_array_set_capacity_for_size(ALGUI_ARRAY* arr, size_t size, ALGUI_BOOL resize_array_if_needed) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return;
    }

    //if requested size is greater than or equal to the current size, 
    //then set capacity for size, no element is affected 
    if (size >= arr->size) {
        set_capacity_for_size(arr, size);
        return;
    }

    //if size is 0, optionally clear the array
    if (!size) {
        if (resize_array_if_needed) {
            clear(arr);
        }
        return;
    }

    //requested size is greater than 0 and smaller than the current size

    //calculate new capacity
    size_t new_capacity = calc_capacity(size);

    //if new capacity extends beyond current size, set the capacity, no element is affected
    if (new_capacity >= arr->size) {
        set_capacity(arr, new_capacity);
        return;
    }

    //new capacity smaller than current size, resize the array;
    //the capacity will be set to the appropriate value
    if (resize_array_if_needed) {
        set_size(arr, size);
    }
}


//get element
void* algui_array_get_element(const ALGUI_ARRAY* arr, size_t index) {
    //check the args
    if (!arr || index >= arr->size) {
        errno = EINVAL;
        return NULL;
    }

    //get element
    return element(arr, index);
}


//insert elements
void algui_array_insert_elements(ALGUI_ARRAY* arr, size_t index, void* data, size_t size) {
    //check the args
    if (!arr || index > arr->size || !check_data(arr, data, size)) {
        errno = EINVAL;
        return;
    }

    //if size is 0, then do nothing
    if (!size) {
        return;
    }

    //new array size
    const size_t new_size = arr->size + size;

    //if size exceeds the capacity, make more space
    if (new_size > arr->capacity) {
        if (!set_capacity_for_size(arr, new_size)) {
            return;
        }
    }

    //set the new size
    const size_t old_size = arr->size;
    arr->size = new_size;

    //move the data to make room for the inserted elements
    if (index < old_size) {
        memmove(element(arr, index + size), element(arr, index), (old_size - index) * arr->elem_size);
    }

    //construct the new elements
    arr->elem_vtable->copy_constructor(arr, element(arr, index), data, arr->elem_size, size);
}


//insert one element
void algui_array_insert_element(ALGUI_ARRAY* arr, size_t index, void* elem) {
    algui_array_insert_elements(arr, index, elem, 1);
}


//remove elements
void algui_array_remove_elements(ALGUI_ARRAY* arr, size_t index, size_t size) {
    const size_t end_index = index + size;

    //check the args
    if (!arr || index >= arr->size || end_index > arr->size) {
        errno = EINVAL;
        return;
    }

    //clear the whole array
    if (index == 0 && size == arr->size) {
        clear(arr);
        return;
    }

    //destroy the elements at the given range
    arr->elem_vtable->destructor(arr, element(arr, index), arr->elem_size, size);

    //move the rest of the data into the removed range
    if (end_index < arr->size) {
        memmove(element(arr, index), element(arr, end_index), (arr->size - end_index) * arr->elem_size);
    }

    //compact the array, if needed
    set_capacity_for_size(arr, arr->size);

    //adjust the size
    arr->size -= size;
}


//remove one element
void algui_array_remove_element(ALGUI_ARRAY* arr, size_t index) {
    algui_array_remove_elements(arr, index, 1);
}


//sequential search
void* algui_array_find_element_sequential_search(const ALGUI_ARRAY* arr, size_t index, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return NULL;
    }

    //search
    return algui_find_element_sequential_search(arr->elem_size, arr->data.data, arr->size, index, elem, compare, context ? context : arr);
}


//sequential search reverse
void* algui_array_find_element_sequential_search_reverse(const ALGUI_ARRAY* arr, size_t index, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return NULL;
    }

    //search
    return algui_find_element_sequential_search_reverse(arr->elem_size, arr->data.data, arr->size, index, elem, compare, context ? context : arr);
}


//binary search
void* algui_array_find_element_binary_search(const ALGUI_ARRAY* arr, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context, int* comp_result) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return NULL;
    }

    //search
    return algui_find_element_binary_search(arr->elem_size, arr->data.data, arr->size, elem, compare, context ? context : arr, comp_result);
}


//get the element index
size_t algui_array_get_element_index(const ALGUI_ARRAY* arr, const void* elem) {
    //check args
    if (!arr) {
        errno = EINVAL;
        return -1;
    }

    //get the element index
    return algui_get_element_index(arr->elem_size, arr->data.data, arr->size, elem);
}


//insert sorted
void* algui_array_insert_element_sorted(ALGUI_ARRAY* arr, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context, ALGUI_BOOL fail_if_exists) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return NULL;
    }

    //ensure capacity is enough for one more element
    if (arr->size + 1 > arr->capacity) {
        set_capacity_for_size(arr, arr->size + 1);
        if (arr->size + 1 > arr->capacity) {
            return NULL;
        }
    }

    //insert one
    void* result = algui_insert_element_sorted(arr->elem_vtable, arr->elem_size, arr->data.data, arr->size, elem, compare, context ? context : arr, fail_if_exists);

    //on error
    if (!result) {
        return NULL;
    }

    //success; one element was added
    ++arr->size;
    return result;
}


//remove sorted
void* algui_array_remove_element_sorted(ALGUI_ARRAY* arr, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return NULL;
    }

    //remove one
    void* result = algui_remove_element_sorted(arr->elem_vtable, arr->elem_size, arr->data.data, arr->size, elem, compare, context ? context : arr);

    //on error
    if (!result) {
        return NULL;
    }

    //success; one element was removed; recalculate address after setting capacity because realloc may move the block
    const size_t index = ((char*)result - arr->data.data) / arr->elem_size;
    set_capacity_for_size(arr, --arr->size);
    return arr->data.data + arr->elem_size * index;
}


//for each
uintptr_t algui_array_for_each_element(ALGUI_ARRAY* arr, size_t index, size_t size, uintptr_t(*callback)(void* elem, size_t index, void* context), void* context) {
    //check the array
    if (!arr) {
        errno = EINVAL;
        return 0;
    }

    //set array size, if not given
    if (size == -1) {
        size = arr->size;
    }

    //check index and size
    if (index >= arr->size || index + size > arr->size) {
        errno = EINVAL;
        return 0;
    }

    //for each
    return algui_for_each_element(arr->elem_size, element(arr, index), size, callback, context ? context : arr);
}


//for each reverse
uintptr_t algui_array_for_each_element_reverse(ALGUI_ARRAY* arr, size_t index, size_t size, uintptr_t(*callback)(void* elem, size_t index, void* context), void* context) {
    //check the array
    if (!arr) {
        errno = EINVAL;
        return 0;
    }

    //set index, if not given
    if (index == -1) {
        index = arr->size - 1;
    }

    //set array size, if not given
    if (size == -1) {
        size = arr->size;
    }
        
    //check index and size
    if (index >= arr->size || index + size > arr->size) {
        errno = EINVAL;
        return 0;
    }

    //for each reverse
    return algui_for_each_element_reverse(arr->elem_size, element(arr, index), size, callback, context ? context : arr);
}


//sort array
void algui_array_sort_elements(ALGUI_ARRAY* arr, size_t index, size_t size, int (*compare)(const void* a, const void* b, void* context), void* context) {
    //check the args
    if (!arr) {
        errno = EINVAL;
        return;
    }

    //set index, if not given
    if (index == -1) {
        index = 0;
    }

    //set array size, if not given
    if (size == -1) {
        size = arr->size;
    }

    //check index and size
    if (index >= arr->size || index + size > arr->size) {
        errno = EINVAL;
        return;
    }

    //sort
    algui_qsort_elements(arr->elem_size, element(arr, index), size, compare, context ? context : arr);
}
