#ifndef ALGUI_MAP_H
#define ALGUI_MAP_H


#include "array.h"


typedef struct ALGUI_MAP {
    ALGUI_ARRAY array;
    size_t size;
    size_t counter;
    ALGUI_BOOL sorted;
    size_t key_size;
    size_t value_size;
    size_t bucket_size;
    ALGUI_COMPARATOR compare;
} ALGUI_MAP;


ALGUI_BOOL algui_init_map(ALGUI_MAP* map, size_t key_size, size_t value_size, ALGUI_COMPARATOR compare);


ALGUI_BOOL algui_cleanup_map(ALGUI_MAP* map);


ALGUI_BOOL algui_is_empty_map(ALGUI_MAP* map);


void* algui_get_map_element(ALGUI_MAP* map, const void* key);


ALGUI_BOOL algui_set_map_element(ALGUI_MAP* map, const void* key, const void* value);


ALGUI_BOOL algui_delete_map_element(ALGUI_MAP* map, const void* key);


uintptr_t algui_for_each_map_element(ALGUI_MAP* map, uintptr_t (*func)(const void* key, void* value, void* data), void* data);


#endif //ALGUI_MAP_H
