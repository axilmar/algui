#include <string.h>
#include <stdint.h>
#include "algui/comparator.h"


int algui_int_comparator(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}


int algui_uint32_comparator(const void* a, const void* b) {
    return *(uint32_t*)a - *(uint32_t*)b;
}


int algui_string_comparator(const void* a, const void* b) {
    return a == b ? 0 : strcmp((const char*)a, (const char*)b);
}
