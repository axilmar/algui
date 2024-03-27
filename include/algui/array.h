#ifndef ALGUI_ARRAY_H
#define ALGUI_ARRAY_H


#include "bool.h"
#include "index.h"
#include "comparator.h"


typedef struct ALGUI_ARRAY {
    char* data;
    size_t element_size;
    size_t size;
} ALGUI_ARRAY;


ALGUI_BOOL algui_init_array(ALGUI_ARRAY* array, size_t element_size, size_t size);


ALGUI_BOOL algui_cleanup_array(ALGUI_ARRAY* array);


size_t algui_get_array_element_size(const ALGUI_ARRAY* array);


size_t algui_get_array_size(const ALGUI_ARRAY* array);


ALGUI_BOOL algui_set_array_size(ALGUI_ARRAY* array, size_t size);


void* algui_get_array_element(const ALGUI_ARRAY* array, size_t index);


ALGUI_BOOL algui_set_array_element(ALGUI_ARRAY* array, size_t index, void* element);


size_t algui_get_array_element_index(const ALGUI_ARRAY* array, const void* element);


size_t algui_find_array_element_index(const ALGUI_ARRAY* array, size_t start_index, const void* element, ALGUI_COMPARATOR compare);


size_t algui_find_array_element_index_reverse(const ALGUI_ARRAY* array, size_t start_index, void* element, ALGUI_COMPARATOR compare);


void* algui_find_array_element(const ALGUI_ARRAY* array, size_t start_index, const void* element, ALGUI_COMPARATOR compare);


void* algui_find_array_element_reverse(const ALGUI_ARRAY* array, size_t start_index, void* element, ALGUI_COMPARATOR compare);


ALGUI_BOOL algui_insert_array_elements(ALGUI_ARRAY* array, size_t index, const void* element, size_t count);


ALGUI_BOOL algui_prepend_array_elements(ALGUI_ARRAY* array, const void* element, size_t count);


ALGUI_BOOL algui_append_array_elements(ALGUI_ARRAY* array, const void* element, size_t count);


ALGUI_BOOL algui_insert_array_element(ALGUI_ARRAY* array, size_t index, const void* element);


ALGUI_BOOL algui_prepend_array_element(ALGUI_ARRAY* array, const void* element);


ALGUI_BOOL algui_append_array_element(ALGUI_ARRAY* array, const void* element);


ALGUI_BOOL algui_remove_array_elements(ALGUI_ARRAY* array, size_t index, size_t count);


ALGUI_BOOL algui_remove_array_element(ALGUI_ARRAY* array, size_t index);


ALGUI_BOOL algui_qsort_array(ALGUI_ARRAY* array, ALGUI_COMPARATOR compare);


#endif //ALGUI_ARRAY_H
