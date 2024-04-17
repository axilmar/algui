#include <errno.h>
#include <memory.h>
#include <stdio.h>
#include "algui/algorithm.h"
#include "algui/stdlib.h"


#ifdef _WIN32


typedef struct {
    void* context;
    int (*compare)(const void* a, const void* b, void* context);
} QSORT_CONTEXT;


static int win32_sort_comparator(void* context, const void* a, const void* b) {
    QSORT_CONTEXT* qc = (QSORT_CONTEXT*)context;
    return qc->compare(a, b, qc->context);
}


#endif //_WIN32


//the actual binary search algorithm
static void* binary_search(void* context, size_t elem_size, void* begin, void* end, const void* elem, int (*compare)(const void* a, const void* b, void* context), int* last_comp_result) {
    char* lo = (char*)begin;
    char* hi = (char*)end - elem_size;

    for (;;) {
        //calculate mid element
        char* mid = (char*)(((uintptr_t)lo + (uintptr_t)hi) / 2);

        //do the comparison
        const int comp_result = compare(mid, elem, context);

        //mid is equal to elem; return mid element
        if (comp_result == 0) {
            *last_comp_result = 0;
            return mid;
        }

        //else mid is less than elem; continue with the lower part
        else if (comp_result < 0) {
            hi = mid - elem_size;
            if (hi < lo) {
                *last_comp_result = 1;
                return mid;
            }
        }

        //else mid is greater than elem; continue with the upper part
        else {
            lo = mid + elem_size;
            if (lo > hi) {
                *last_comp_result = -1;
                return mid;
            }
        }
    }

    //not found
    return NULL;
}


/**************************************************************************************************/


//sequential search
void* algui_find_element_sequential_search(size_t elem_size, void* data, size_t size, size_t index, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context) {
    //check the args
    if (elem_size == 0 || (!data && size) || (data && !size) || index >= size || !elem || !compare) {
        errno = EINVAL;
        return NULL;
    }

    //check if the data is empty
    if (!size) {
        return NULL;
    }

    //search
    for (size_t i = index; i < size; ++i) {
        void* data_elem = (char*)data + elem_size * i;
        if (compare(data_elem, elem, context) == 0) {
            return data_elem;
        }
    }

    //not found
    return NULL;
}


//reverse sequential search
void* algui_find_element_sequential_search_reverse(size_t elem_size, void* data, size_t size, size_t index, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context) {
    //if index is -1, then set it to to size - 1
    if (index == -1) {
        index = size - 1;
    }

    //check the args
    if (elem_size == 0 || (!data && size) || (data && !size) || index >= size || !elem || !compare) {
        errno = EINVAL;
        return NULL;
    }

    //check if the data is empty
    if (!size) {
        return NULL;
    }

    //search
    for (size_t i = index + 1; i > 0; --i) {
        void* data_elem = (char*)data + elem_size * (i - 1);
        if (compare(data_elem, elem, context) == 0) {
            return data_elem;
        }
    }

    //not found
    return NULL;
}


//binary search
void* algui_find_element_binary_search(size_t elem_size, void* data, size_t size, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context, int* comp_result) {
    //check the args
    if (elem_size == 0 || (!data && size) || (data && !size) || !elem || !compare || !comp_result) {
        errno = EINVAL;
        return NULL;
    }

    //check if the data is empty
    if (!size) {
        return NULL;
    }

    //search
    return binary_search(context, elem_size, data, (char*)data + elem_size * size, elem, compare, comp_result);
}


//get element index
size_t algui_get_element_index(size_t elem_size, const void* data, size_t size, const void* elem) {
    if (elem_size == 0 || !data || !elem) {
        errno = EINVAL;
        return -1;
    }

    //calculate the index
    size_t result = ((char*)elem - (char*)data) / elem_size;

    //check the result against the size
    if (result >= size) {
        return -1;
    }

    //success
    return result;
}


//insert sorted
void* algui_insert_element_sorted(const ALGUI_ELEMENT_VTABLE* elem_vtable, size_t elem_size, void* data, size_t size, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context, ALGUI_BOOL fail_if_exists) {
    //check the args
    if (elem_size == 0 || !data || !elem || !compare || !elem_vtable) {
        errno = EINVAL;
        return NULL;
    }

    //if size of array is 0, then insert the element at the first position of the array
    if (size == 0) {
        elem_vtable->copy_constructor(context, data, elem, elem_size, 1);
        return data;
    }

    //insert in non-empty array

    //find relative position for insertion
    char* end_pos = (char*)data + elem_size * size;
    int comp_result;
    char* pos = (char*)binary_search(context, elem_size, data, end_pos, elem, compare, &comp_result);

    //insert relative to the found position
    switch (comp_result) {
        //*elem > *pos; insert elem after pos
        case -1:
            pos += elem_size;
            break;

        //*elem == *pos; replace element
        case 0:
            if (fail_if_exists) {
                return NULL;
            }
            elem_vtable->copy_assignment(context, pos, elem, elem_size, 1);
            return pos;

        //*elem < *pos; insert elem at pos
        case 1:
            break;

        //shall never happen
        default:
            printf("ALGUI binary search: invalid comp_result: %i\n", comp_result);
            return NULL;
    }

    //make room for the new element if needed
    if (pos < end_pos) {
        memmove(pos + elem_size, pos, end_pos - pos);
    }

    //copy-construct the new element
    elem_vtable->copy_constructor(context, pos, elem, elem_size, 1);

    return pos;
}


//remove sorted
void* algui_remove_element_sorted(const ALGUI_ELEMENT_VTABLE* elem_vtable, size_t elem_size, void* data, size_t size, const void* elem, int (*compare)(const void* a, const void* b, void* context), void* context) {
    //check the args
    if (elem_size == 0 || (!data && size) || (data && !size) || !elem || !compare) {
        errno = EINVAL;
        return NULL;
    }

    //check if the data is empty
    if (!size) {
        return NULL;
    }

    //locate the element
    char* end_pos = (char*)data + elem_size * size;
    int comp_result;
    char* pos = (char*)binary_search(context, elem_size, data, end_pos, elem, compare, &comp_result);

    //if comp_result not 0, then the element is not found
    if (comp_result != 0) {
        return NULL;
    }

    //destroy the element in place
    elem_vtable->destructor(context, pos, elem_size, 1);

    //move the data to remove the element
    if (pos + elem_size < end_pos) {
        memmove(pos, pos + elem_size, end_pos - pos + elem_size);
    }

    //success
    return pos;
}


//for each
uintptr_t algui_for_each_element(size_t elem_size, void* data, size_t size, uintptr_t(*callback)(void* elem, size_t index, void* context), void* context) {
    //check the args
    if (!elem_size || (!data && size) || (data && !size) || !callback) {
        errno = EINVAL;
        return 0;
    }

    //if size is 0, return 0
    if (!size) {
        return 0;
    }

    //iterate
    char* elem = (char*)data;
    const char* const end = elem + elem_size * size;
    for (size_t index = 0; elem < end; elem += elem_size, ++index) {
        const uintptr_t result = callback(elem, index, context);
        if (result) {
            return result;
        }
    }

    //didn't find anything interesting
    return 0;
}


//for each reverse
uintptr_t algui_for_each_element_reverse(size_t elem_size, void* data, size_t size, uintptr_t(*callback)(void* elem, size_t index, void* context), void* context) {
    //check the args
    if (!elem_size || (!data && size) || (data && !size) || !callback) {
        errno = EINVAL;
        return 0;
    }

    //if size is 0, return 0
    if (!size) {
        return 0;
    }

    //iterate
    size_t index = size - 1;
    char* elem = (char*)data + elem_size * index;
    for (; elem >= (char*)data; elem -= elem_size, --index) {
        const uintptr_t result = callback(elem, index, context);
        if (result) {
            return result;
        }
    }

    //didn't find anything interesting
    return 0;
}


//quick sort
void algui_qsort_elements(size_t elem_size, void* data, size_t size, int (*compare)(const void* a, const void* b, void* context), void* context) {
    //check the args
    if (!elem_size || (!data && size) || (data && !size) || !compare) {
        errno = EINVAL;
        return;
    }

    //on win32, create a local context which includes the original context and function, due to reversed parameters
    #ifdef _WIN32
    QSORT_CONTEXT qc = { context, compare };
    qsort_s(data, size, elem_size, win32_sort_comparator, &qc);
    #else
    qsort_s(data, size, elem_size, compare, context);
    #endif
}
