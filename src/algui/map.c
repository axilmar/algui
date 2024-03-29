#include <errno.h>
#include <stdlib.h>
#include <memory.h>
#include "algui/map.h"
#include "algui/array_util.h"


//element header
typedef struct ELEMENT_HEADER {
    ALGUI_COMPARATOR compare;
    size_t counter;
} ELEMENT_HEADER;


//compute the map element size
static size_t compute_element_size(size_t key_size, size_t value_size) {
    return sizeof(ELEMENT_HEADER) + key_size + value_size;
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
    const int comp = ha->compare(ka, kb);

    //if the keys are not equal, return the result of the comparison
    if (comp) {
        return comp;
    }

    //the keys are equal, therefore compare counters; later is greater
    return ha->counter < hb->counter;
}


//sort the map, if not sorted
static void sort_map(ALGUI_MAP* map) {
    //if sorted, do nothing else
    if (map->sorted) {
        return;
    }

    //sort the array
    qsort(map->array.data, map->size, map->array.element_size, sort_comparator);

    //resize the array to match the map size
    algui_set_array_size(&map->array, map->size);

    //the map is sorted
    map->sorted = ALGUI_TRUE;
}


//the map comparator for searching
static int search_comparator(const void* a, const void* b) {
    //get the element header for a; b points to the key directly
    const ELEMENT_HEADER* const ha = (const ELEMENT_HEADER*)a;

    //get the key for a; it is after the header
    const void* const ka = ha + 1;

    //compare the keys
    return ha->compare(ka, b);
}


//find index of previous entry with the same key
static size_t find_lowest_same_index(ALGUI_MAP* map, const void* key, size_t index) {
    for (; index > 0; --index) {
        ELEMENT_HEADER* prev_elem = ALGUI_GET_ARRAY_ELEMENT_UTIL(ELEMENT_HEADER, map->array.data, map->array.size, index - 1);
        if (map->compare(prev_elem + 1, key) != 0) {
            break;
        }
    }
    return index;
}


//remove older entries for the given key at the given index on a sorted map; returns adjusted index
static size_t remove_older_entries(ALGUI_MAP* map, const void* key, size_t index) {
    //if there are no previous entries, do nothing else
    if (index == 0) {
        return index;
    }

    //find first index of previous entries with same key    
    const size_t prev_index = find_lowest_same_index(map, key, index);

    const size_t same_key_count = index - prev_index;

    //if there are previous entries with the same key, remove those entries
    if (same_key_count > 0) {
        algui_remove_array_elements(&map->array, prev_index, same_key_count);
    }

    //adjust the map size by the removed entries count
    map->size -= same_key_count;

    //return the adjusted index
    return prev_index;
}


//init map
ALGUI_BOOL algui_init_map(ALGUI_MAP* map, size_t key_size, size_t value_size, ALGUI_COMPARATOR compare) {
    //check the map
    if (map == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the key size
    if (key_size == 0) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //check the compare function
    if (compare == NULL) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }

    //compute map element size
    const size_t element_size = compute_element_size(key_size, value_size);

    //setup the map
    algui_init_array(&map->array, element_size, 0);
    map->compare = compare;
    map->counter = 0;
    map->key_size = key_size;
    map->size = 0;
    map->value_size = value_size;
    map->sorted = ALGUI_TRUE;
    map->bucket_size = 32;

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

    //cleanup the map
    algui_cleanup_array(&map->array);
    map->size = 0;
    map->counter = 0;
    map->sorted = ALGUI_TRUE;

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

    //sort the map, if needed
    sort_map(map);

    //find the entry, based on the map search comparator
    size_t index = algui_find_array_element_index_binary_search_util(map->array.data, map->size, map->array.element_size, key, search_comparator);

    //if not found
    if (index == ALGUI_INVALID_INDEX) {
        return NULL;
    }

    //remove older entries with the same key
    index = remove_older_entries(map, key, index);

    //locate the entry
    const ELEMENT_HEADER* const elem = ALGUI_GET_ARRAY_ELEMENT_UTIL(ELEMENT_HEADER, map->array.data, map->array.element_size, index);

    //return the value
    char* const value = (char*)(elem + 1) + map->key_size;
    return value;

}


//set element
ALGUI_BOOL algui_set_map_element(ALGUI_MAP* map, const void* key, const void* value) {
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

    //check the value, but only if map value size is greater than zero
    if ((map->value_size > 0 && value == NULL) || (map->value_size == 0 && value != NULL)) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }
    
    //make room into the array if needed; on error, return false
    if (map->size == map->array.size) {
        if (algui_set_array_size(&map->array, map->size + map->bucket_size) == ALGUI_FALSE) {
            return ALGUI_FALSE;
        }
    }

    //get the array element pointer
    ELEMENT_HEADER* elem = ALGUI_GET_ARRAY_ELEMENT_UTIL(ELEMENT_HEADER, map->array.data, map->array.element_size, map->size);

    //set the element header
    elem->compare = map->compare;
    elem->counter = map->counter;

    //copy the key
    memmove(elem + 1, key, map->key_size);

    //copy the value
    memmove((char*)(elem + 1) + map->key_size, value, map->value_size);

    //setup the map after the insertion
    ++map->counter;
    ++map->size;
    map->sorted = ALGUI_FALSE;

    //success
    return ALGUI_TRUE;
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
    sort_map(map);

    //find the entry, based on the map search comparator
    const size_t index = algui_find_array_element_index_binary_search_util(map->array.data, map->size, map->array.element_size, key, search_comparator);

    //if not found
    if (index == ALGUI_INVALID_INDEX) {
        return ALGUI_FALSE;
    }

    //find prev entry index
    const size_t prev_index = find_lowest_same_index(map, key, index);

    //find same key count; include the found element
    const size_t same_key_count = index + 1 - prev_index;

    //remove the entries
    algui_remove_array_elements(&map->array, prev_index, same_key_count);

    //adjust the map size by the removed entries count
    map->size -= same_key_count;

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
    sort_map(map);

    //loop
    for (size_t i = 0; i < map->size; ) {
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
    sort_map(map);

    //loop
    for (size_t i = map->size; i > 0; ) {
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
