#ifndef ALGUI_MAP_H
#define ALGUI_MAP_H


#include <stdint.h>
#include "array.h"


/**
 * A map of keys to values.
 * The map is 32-bit, i.e. it can hold up to UINT32_MAX elements.
 * The map is backed up by an array.
 * The map is lazy: when adding elements, the elements are added at the end of the table.
 * When the map is accessed, the map is sorted, if not sorted yet, and then the element is retrieved by its key using binary search.
 * If the same key is added multiple times, only the last one remains, the rest of the entries are deleted.
 */
typedef struct ALGUI_MAP {
    ALGUI_ARRAY array;
    ALGUI_COMPARATOR compare;
    ALGUI_DESTRUCTOR key_dtor;
    ALGUI_DESTRUCTOR value_dtor;
    uint32_t counter;
    uint32_t size;
    uint32_t sorted_count;
    uint32_t key_size : ALGUI_MAX_ELEMENT_SIZE_BITS;
    uint32_t value_size : ALGUI_MAX_ELEMENT_SIZE_BITS;
} ALGUI_MAP;


/**
 * Initializes a map.
 * @param map map to initialize; if null, then false is returned and errno is set to EINVAL.
 * @param key_size size of key, in bytes; if 0 or greater than ALGUI_MAX_ELEMENT_SIZE, then false is returned and errno is set to EINVAL.
 * @param value_size size of value, in bytes; 0 is allowed, if the map is to be treated as a set, inserting null values. If greater than ALGUI_MAX_ELEMENT_SIZE, then false is returned and errno is set to EINVAL.
 * @param compare the function that imposes an order in the map; if null, then false is returned and errno is set to EINVAL.
 * @param key_dtor optional key destructor.
 * @param value_dtor optional value destructor.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_init_map(ALGUI_MAP* map, uint32_t key_size, uint32_t value_size, ALGUI_COMPARATOR compare, ALGUI_DESTRUCTOR key_dtor, ALGUI_DESTRUCTOR value_dtor);


/**
 * cleans up a map.
 * @param map map to cleanup; if null, then false is returned and errno is set to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_cleanup_map(ALGUI_MAP* map);


/**
 * Checks if a map is empty.
 * @param map map to check is empty; if null, then false is returned and errno is set to EINVAL.
 * @return true on success and if the map is empty, false if the map is not empty or on error.
 */
ALGUI_BOOL algui_is_empty_map(ALGUI_MAP* map);


/**
 * Retrieves a map element value.
 * @param map map to get an element of; if null, then NULL is returned and errno is set to EINVAL.
 * @param key pointer to key; if null, then NULL is returned and errno is set to EINVAL.
 * @return pointer to map element value, NULL otherwise.
 */
void* algui_get_map_element(ALGUI_MAP* map, const void* key);


/**
 * Inserts an element into the map.
 * @param map the map to insert an element into; if null, then false is returned and errno is set to EINVAL.
 * @param key pointer to key; if null, then false is returned and errno is set to EINVAL.
 * @param value pointer to key; if null and value size is not 0 or if not null and value size is 0, then false is returned and errno is set to EINVAL.
 * @return pointer to value on success, NULL on error; if memory allocation fails, errno is set to ENOMEM.
 */
void* algui_set_map_element(ALGUI_MAP* map, const void* key, const void* value);


/**
 * Removes an array element.
 * @param map the map to remove an element from; if null, then false is returned and errno is set to EINVAL.
 * @param key pointer to key; if null, then false is returned and errno is set to EINVAL.
 * @return true if the key was found and removed, false otherwise.
 */
ALGUI_BOOL algui_remove_map_element(ALGUI_MAP* map, const void* key);


/**
 * Executes a callback for each element in the map.
 * @param map the map to iterate elements of; if null, then NULL is returned and errno is set to EINVAL.
 * @param func callback function; if it returns non-null, then the loop stops and the functions' result is returned; if null, then NULL is returned and errno is set to EINVAL.
 * @param data callback data.
 * @return what the function returns or NULL on error.
 */
uintptr_t algui_for_each_map_element(ALGUI_MAP* map, uintptr_t (*func)(const void* key, void* value, void* data), void* data);


/**
 * Executes a callback for each element in the map, in reverse element order.
 * @param map the map to iterate elements of; if null, then NULL is returned and errno is set to EINVAL.
 * @param func callback function; if it returns non-null, then the loop stops and the functions' result is returned; if null, then NULL is returned and errno is set to EINVAL.
 * @param data callback data.
 * @return what the function returns or NULL on error.
 */
uintptr_t algui_for_each_map_element_reverse(ALGUI_MAP* map, uintptr_t(*func)(const void* key, void* value, void* data), void* data);


/**
 * Sorts the map elements, removes duplicates, resets the most recent element counter, resizes the map array to the same size as the map.
 * @param map map to opimize; if null, then false is returned and errno is set to EINVAL.
 * @return true on success, false on error.
 */
ALGUI_BOOL algui_optimize_map(ALGUI_MAP* map);


#endif //ALGUI_MAP_H
