#include <errno.h>
#include <stdlib.h>
#include <memory.h>
#include "algui/map.h"
#include "algui/array_util.h"


//element header
typedef struct ELEMENT_HEADER {
    ALGUI_MAP* map;
    uint32_t counter;
} ELEMENT_HEADER;


//get the pointer to the key
static inline char* get_key(const ELEMENT_HEADER* elem) {
    return (char*)(elem + 1);
}


//get the pointer to the value
static inline char* get_value(const ALGUI_MAP* map, const ELEMENT_HEADER* elem) {
    return (char*)(elem + 1) + map->key_size;
}


//returns pointer to previous element
static inline ELEMENT_HEADER* get_prev_element(ALGUI_MAP* map, const ELEMENT_HEADER* elem) {
    return (ELEMENT_HEADER*)((char*)elem - map->array.element_size);
}


//returns pointer to next element
static inline ELEMENT_HEADER* get_next_element(ALGUI_MAP* map, const ELEMENT_HEADER* elem) {
    return (ELEMENT_HEADER*)((char*)elem + map->array.element_size);
}


//compute the map element size
static inline uint32_t compute_element_size(uint32_t key_size, uint32_t value_size) {
    return (uint32_t)sizeof(ELEMENT_HEADER) + key_size + value_size;
}


//returns true if map is sorted
static inline ALGUI_BOOL is_sorted(const ALGUI_MAP* map) {
    return map->sorted_count == map->size || map->size == 1;
}


//the map comparator for sorting
static int sort_comparator(const void* a, const void* b) {
    //get the element headers
    const ELEMENT_HEADER* const ha = (const ELEMENT_HEADER*)a;
    const ELEMENT_HEADER* const hb = (const ELEMENT_HEADER*)b;

    //get the keys; they are right after the headers
    const void* const ka = ha + 1;
    const void* const kb = hb + 1;

    //compare the keys
    const int comp = ha->map->compare(ka, kb);

    //if the keys are not equal, return the result of the comparison
    if (comp) {
        return comp;
    }

    //the keys are equal, therefore compare counters; later is greater
    return ha->counter < hb->counter;
}


//grows the map array
static ALGUI_BOOL grow_array(ALGUI_MAP* map, uint32_t new_size) {
    //reallocate data
    char* new_data = realloc(map->array.data, map->array.element_size * new_size);

    //check for reallocation failure
    if (new_data == NULL) {
        errno = ENOMEM;
        return ALGUI_FALSE;
    }

    //save the new array data and size
    map->array.data = new_data;
    map->array.size = new_size;

    //success
    return ALGUI_TRUE;
}


//resizes the array so as that it contains the exact number of elements the map contains
static void shrink_array_to_fit_map(ALGUI_MAP* map) {
    //the difference of array size to map size must exceed a certain limit
    if (map->array.size - map->size < 32) {
        return;
    }

    char* new_data = realloc(map->array.data, map->array.element_size * map->size);

    //check reallocation failure;
    //the error is not critical, the previous array size still exists
    if (new_data == NULL) {
        return;
    }

    //save the new array data
    map->array.data = new_data;
    map->array.size = map->size;
}


//sorts the map
static void sort_map(ALGUI_MAP* map) {
    //sort the array
    qsort(map->array.data, map->size, map->array.element_size, sort_comparator);

    //the whole map is now sorted
    map->sorted_count = map->size;
}


//sorts the map if it is unsorted
static void sort_unsorted_map(ALGUI_MAP* map) {
    if (is_sorted(map)) {
        return;
    }
    sort_map(map);
}


//the map comparator for searching
static int search_comparator(const void* a, const void* b) {
    //get the element header for a; b points to the key directly
    const ELEMENT_HEADER* const ha = (const ELEMENT_HEADER*)a;

    //get the key for a; it is after the header
    const void* const ka = ha + 1;

    //compare the keys
    return ha->map->compare(ka, b);
}


//find lowest index with the same key
static uint32_t find_same_key_lowest_index(ALGUI_MAP* map, const void* key, uint32_t index) {
    for (; index > 0; --index) {
        ELEMENT_HEADER* prev_elem = ALGUI_GET_ARRAY_ELEMENT_UTIL(ELEMENT_HEADER, map->array.data, map->array.size, index - 1);
        if (map->compare(prev_elem, key) != 0) {
            break;
        }
    }
    return index;
}


//removes elements from map
static void remove_map_elements(ALGUI_MAP* map, uint32_t index, uint32_t count) {
    algui_remove_array_elements(&map->array, index, count);
    map->size -= count;
    if (index < map->sorted_count) {
        map->sorted_count -= map->sorted_count - index;
    }
    shrink_array_to_fit_map(map);
}


//remove older entries for the given key at the given index on a sorted map; returns adjusted index
static uint32_t remove_older_entries(ALGUI_MAP* map, const void* key, uint32_t index) {
    //if there are no previous entries, do nothing else
    if (index == 0) {
        return index;
    }

    //find first index of previous entries with same key    
    const uint32_t prev_index = find_same_key_lowest_index(map, key, index);

    //calculate number of entries with same key to remove
    const uint32_t same_key_count = index - prev_index;

    //if there are previous entries with the same key, remove those entries
    if (same_key_count > 0) {
        remove_map_elements(map, prev_index, same_key_count);
    }

    //return the adjusted index
    return prev_index;
}


//destroys the key and value of an element
static void elem_dtor(void* elem) {
    ELEMENT_HEADER* h = (ELEMENT_HEADER*)elem;
    ALGUI_MAP* map = h->map;

    //destroy the key
    if (map->key_dtor) {
        char* key = get_key(h);
        map->key_dtor(key);
    }

    //destroy the value
    if (map->value_dtor) {
        char* value = get_value(map, h);
        map->value_dtor(value);
    }
}


//get extended size of array
static uint32_t calc_new_size(uint32_t size) {
    //if size is small, start with a minimum specific size
    if (size < 32) {
        return 32;
    }

    //if curr size is below a certain limit, try to allocate more based on power-of-2
    if (size <= 2048) {
        size *= 2;
        uint32_t new_size = 1;
        do {
            new_size *= 2;
        } while (new_size < size);
        return new_size;
    }

    //increment the size by a fixed amount of data
    return size + 4096;
}


//sorts the map, removes duplicate elements, fixes the counters.
static void fix_map(ALGUI_MAP* map) {
    //sort the map so as that elements are grouped by key
    sort_unsorted_map(map);

    //start with the first element
    ELEMENT_HEADER* first_elem = (ELEMENT_HEADER*)map->array.data;

    //shift position is the first element
    ELEMENT_HEADER* shift_pos = first_elem;

    //loop until last element
    const ELEMENT_HEADER* const end_elem = ALGUI_GET_ARRAY_ELEMENT_UTIL(ELEMENT_HEADER, map->array.data, map->array.element_size, map->size);

    //loop without destructor
    if (map->array.dtor == NULL) {
        for (ELEMENT_HEADER* elem = get_next_element(map, first_elem); elem < end_elem; elem = get_next_element(map, elem)) {

            //compare the keys of current element and first element; 
            //if they are the different, a new entry is found
            if (map->compare(get_key(first_elem), get_key(elem)) != 0) {

                //get the previous element, which is the only element to remain in the map from that group
                ELEMENT_HEADER* prev_elem = get_prev_element(map, elem);

                //set its counter to 0, since it will be only version of the element remaining
                prev_elem->counter = 0;

                //move the element that will remain in the map into the current shift position
                memmove(shift_pos, prev_elem, map->array.element_size);

                //compute the next shift position
                shift_pos = get_next_element(map, shift_pos);

                //first element of the group is the current element
                first_elem = elem;
            }
        }

        //handle the last element
        ELEMENT_HEADER* last_elem = get_prev_element(map, end_elem);

        //set its counter to 0, since it will be only version of the element remaining
        last_elem->counter = 0;

        //move the last element into the current shift position
        memmove(shift_pos, last_elem, map->array.element_size);

        //compute the next shift position
        shift_pos = get_next_element(map, shift_pos);
    }

    //else loop with destructor
    else {
        for (ELEMENT_HEADER* elem = get_next_element(map, first_elem); elem < end_elem; elem = get_next_element(map, elem)) {

            //compare the keys of current element and first element; 
            //if they are the different, a new entry is found
            if (map->compare(get_key(first_elem), get_key(elem)) != 0) {

                //get the previous element, which is the only element to remain in the map from that group
                ELEMENT_HEADER* prev_elem = get_prev_element(map, elem);

                //set its counter to 0, since it will be only version of the element remaining
                prev_elem->counter = 0;

                //destroy redundant elements in reverse array order
                if (prev_elem > first_elem) {
                    for (ELEMENT_HEADER* temp_elem = get_prev_element(map, prev_elem); temp_elem >= first_elem; temp_elem = get_prev_element(map, temp_elem)) {
                        elem_dtor(temp_elem);
                    }
                }

                //move the element that will remain in the map into the current shift position
                memmove(shift_pos, prev_elem, map->array.element_size);

                //compute the next shift position
                shift_pos = get_next_element(map, shift_pos);

                //first element of the group is the current element
                first_elem = elem;
            }
        }

        //handle the last element
        ELEMENT_HEADER* last_elem = get_prev_element(map, end_elem);

        //set its counter to 0, since it will be only version of the element remaining
        last_elem->counter = 0;

        //destroy redundant elements in reverse array order
        if (last_elem > first_elem) {
            for (ELEMENT_HEADER* temp_elem = get_prev_element(map, last_elem); temp_elem >= first_elem; temp_elem = get_prev_element(map, temp_elem)) {
                elem_dtor(temp_elem);
            }
        }

        //move the last element into the current shift position
        memmove(shift_pos, last_elem, map->array.element_size);

        //compute the next shift position
        shift_pos = get_next_element(map, shift_pos);
    }

    //recompute the map size from the last shift position
    map->size = ((char*)shift_pos - (char*)map->array.data) / map->array.element_size;

    //the current counter for the map is 0; next elements will get counter 1
    map->counter = 1;

    //reallocate the array to have the same size as the map
    shrink_array_to_fit_map(map);
}


/*************************************************************************************************/


//init map
ALGUI_BOOL algui_init_map(ALGUI_MAP* map, uint32_t key_size, uint32_t value_size, ALGUI_COMPARATOR compare, ALGUI_DESTRUCTOR key_dtor, ALGUI_DESTRUCTOR value_dtor) {
    //check the map
    if (map == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the key size
    if (key_size == 0 || key_size > ALGUI_MAX_ELEMENT_SIZE) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the value size
    if (value_size > ALGUI_MAX_ELEMENT_SIZE) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the compare function
    if (compare == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //compute map element size
    const uint32_t element_size = compute_element_size(key_size, value_size);

    //setup the map
    algui_init_array(&map->array, element_size, 0, key_dtor || value_dtor ? elem_dtor : NULL);
    map->compare = compare;
    map->key_dtor = key_dtor;
    map->value_dtor = value_dtor;
    map->counter = 0;
    map->size = 0;
    map->sorted_count = 0;
    map->key_size = key_size;
    map->value_size = value_size;

    //success
    return ALGUI_TRUE;
}


//cleanup map
ALGUI_BOOL algui_cleanup_map(ALGUI_MAP* map) {
    //check the map
    if (map == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //if the map is empty, do nothing else
    if (map->size == 0) {
        return ALGUI_TRUE;
    }

    //cleanup the array; make sure excess elements are not cleaned up, since they are not initialized
    map->array.size = map->size;
    algui_cleanup_array(&map->array);

    //cleanup the map
    map->counter = 0;
    map->size = 0;
    map->sorted_count = 0;

    //success
    return ALGUI_TRUE;
}


//check if map is empty
ALGUI_BOOL algui_is_empty_map(ALGUI_MAP* map) {
    //check the map
    if (map == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //the map is empty if it has no entries
    return map->size == 0;
}


//get element
void* algui_get_map_element(ALGUI_MAP* map, const void* key) {
    uint32_t found_index;

    //check the map
    if (map == NULL) {
        errno = EINVAL;
        return NULL;
    }

    //check the key
    if (key == NULL) {
        errno = EINVAL;
        return NULL;
    }

    //if the map is empty, do nothing else
    if (map->size == 0) {
        return NULL;
    }

    //if the map is already sorted, use binary search to locate the entry
    if (is_sorted(map)) {
        found_index = algui_find_array_element_index_binary_search_util(map->array.data, map->size, map->array.element_size, key, search_comparator);
    }

    //else the map is unsorted
    else {
        //search the already sorted part of the map
        if (map->sorted_count > 0) {
            found_index = algui_find_array_element_index_binary_search_util(map->array.data, map->sorted_count, map->array.element_size, key, search_comparator);
            if (found_index != ALGUI_INVALID_INDEX) {
                goto FOUND;
            }
        }

        //if the unsorted part is small, then search it in reverse order in order to find the most recent entry
        if (map->size - map->sorted_count <= 32) {
            for (uint32_t index = map->size; index > map->sorted_count; --index) {
                const ELEMENT_HEADER* elem = ALGUI_GET_ARRAY_ELEMENT_UTIL(ELEMENT_HEADER, map->array.data, map->array.element_size, index - 1);
                if (map->compare(elem + 1, key) == 0) {
                    return get_value(map, elem);
                }
            }
        }

        //nothing from the above worked, so sort the map and search the whole of it
        sort_map(map);
        found_index = algui_find_array_element_index_binary_search_util(map->array.data, map->size, map->array.element_size, key, search_comparator);
    }

    //if not found
    if (found_index == ALGUI_INVALID_INDEX) {
        return NULL;
    }

    FOUND:

    //remove older entries with the same key
    found_index = remove_older_entries(map, key, found_index);

    //locate the entry
    const ELEMENT_HEADER* const elem = ALGUI_GET_ARRAY_ELEMENT_UTIL(ELEMENT_HEADER, map->array.data, map->array.element_size, found_index);

    //return the value
    return get_value(map, elem);
}


//set element
void* algui_set_map_element(ALGUI_MAP* map, const void* key, const void* value) {
    //check the map
    if (map == NULL) {
        errno = EINVAL;
        return NULL;
    }

    //check the key
    if (key == NULL) {
        errno = EINVAL;
        return NULL;
    }

    //check the value against the value size; null values are permitted only when the value size is 0
    if ((map->value_size > 0 && value == NULL) || (map->value_size == 0 && value != NULL)) {
        errno = EINVAL;
        return NULL;
    }

    //if the maximum size or maximum counter is reached, then fix the map; if the map cannot be fixed, abort.
    if (map->size == UINT32_MAX || map->counter == UINT32_MAX) {
        fix_map(map);
        if (map->size == UINT32_MAX || map->counter == UINT32_MAX) {
            errno = ENOMEM;
            return NULL;
        }
    }
    
    //make room into the array if needed; on error, return false
    if (map->size == map->array.size) {
        const uint32_t new_size = calc_new_size(map->size);
        if (grow_array(map, new_size) == ALGUI_FALSE) {
            return NULL;
        }
    }

    //get the array element pointer
    ELEMENT_HEADER* elem = ALGUI_GET_ARRAY_ELEMENT_UTIL(ELEMENT_HEADER, map->array.data, map->array.element_size, map->size);

    //set the element header
    elem->map = map;
    elem->counter = map->counter;

    //copy the key
    memmove(elem + 1, key, map->key_size);

    //copy the value
    char* elem_value = get_value(map, elem);
    memmove(elem_value, value, map->value_size);

    //setup the map after the insertion
    ++map->counter;
    ++map->size;

    //success
    return elem_value;
}


//delete element
ALGUI_BOOL algui_remove_map_element(ALGUI_MAP* map, const void* key) {
    //check the map
    if (map == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the key
    if (key == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //if the map is empty, return
    if (map->size == 0) {
        return ALGUI_TRUE;
    }

    //sort the map, if need, so as that we can find the entry
    sort_unsorted_map(map);

    //find the entry, based on the map search comparator
    const uint32_t index = algui_find_array_element_index_binary_search_util(map->array.data, map->size, map->array.element_size, key, search_comparator);

    //if not found
    if (index == ALGUI_INVALID_INDEX) {
        return ALGUI_FALSE;
    }

    //find prev entry index
    const uint32_t prev_index = find_same_key_lowest_index(map, key, index);

    //find same key count; include the found element
    const uint32_t same_key_count = index + 1 - prev_index;

    //remove the entries
    remove_map_elements(map, prev_index, same_key_count);

    //success
    return ALGUI_TRUE;
}


//execute function for each map element
uintptr_t algui_for_each_map_element(ALGUI_MAP* map, uintptr_t(*func)(const void* key, void* value, void* data), void* data) {
    //check the map
    if (map == NULL) {
        errno = EINVAL;
        return (uintptr_t)NULL;
    }

    //check the function
    if (func == NULL) {
        errno = EINVAL;
        return (uintptr_t)NULL;
    }

    //sort the map, if needed
    sort_unsorted_map(map);

    //loop
    for (uint32_t i = 0; i < map->size; ) {
        //get element
        ELEMENT_HEADER* eh = ALGUI_GET_ARRAY_ELEMENT_UTIL(ELEMENT_HEADER, map->array.data, map->array.element_size, i);

        //get key
        char* key = (char*)(eh + 1);

        //get value
        char* value = key + map->key_size;
        
        //invoke the func
        const uintptr_t res = func(key, value, data);

        //if it returns a result, then stop
        if (res != (uintptr_t)NULL) {
            return res;
        }

        //skip next elements with same key
        for (++i; i < map->size; ++i) {
            //get other element
            ELEMENT_HEADER* eh1 = ALGUI_GET_ARRAY_ELEMENT_UTIL(ELEMENT_HEADER, map->array.data, map->array.element_size, i);

            //get other key
            const char* key1 = (char*)(eh1 + 1);

            //if keys are not the same, then break
            if (map->compare(key, key1) != 0) {
                break;
            }
        }
    }

    //the loop didn't stop at any element
    return (uintptr_t)NULL;
}


//execute function for each map element in reverse order
uintptr_t algui_for_each_map_element_reverse(ALGUI_MAP* map, uintptr_t(*func)(const void* key, void* value, void* data), void* data) {
    //check the map
    if (map == NULL) {
        errno = EINVAL;
        return (uintptr_t)NULL;
    }

    //check the function
    if (func == NULL) {
        errno = EINVAL;
        return (uintptr_t)NULL;
    }

    //sort the map, if needed
    sort_unsorted_map(map);

    //loop
    for (uint32_t i = map->size; i > 0; ) {
        //get element
        ELEMENT_HEADER* eh = ALGUI_GET_ARRAY_ELEMENT_UTIL(ELEMENT_HEADER, map->array.data, map->array.element_size, i - 1);

        //get key
        char* key = (char*)(eh + 1);

        //get value
        char* value = key + map->key_size;

        //invoke the func
        const uintptr_t res = func(key, value, data);

        //if it returns a result, then stop
        if (res != (uintptr_t)NULL) {
            return res;
        }

        //skip prev elements with same key
        for (--i; i > 0; --i) {
            //get other element
            ELEMENT_HEADER* eh1 = ALGUI_GET_ARRAY_ELEMENT_UTIL(ELEMENT_HEADER, map->array.data, map->array.element_size, i - 1);

            //get other key
            const char* key1 = (char*)(eh1 + 1);

            //if keys are not the same, then break
            if (map->compare(key, key1) != 0) {
                break;
            }
        }
    }

    //the loop didn't stop at any element
    return (uintptr_t)NULL;
}


//optimize map
ALGUI_BOOL algui_optimize_map(ALGUI_MAP* map) {
    //check the map
    if (map == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    fix_map(map);

    //success
    return ALGUI_TRUE;
}
