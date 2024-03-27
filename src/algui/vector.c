#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "algui/vector.h"


//init vector
int alg_init_vector(ALG_VECTOR* vec, int elem_size, int size, void* elements, int capacity) {
    assert(vec);
    assert(elem_size > 0);
    assert(size >= 0);
    assert(capacity >= 0);

    //set the vector
    vec->elem_size = elem_size;
    vec->size = size;
    vec->capacity = capacity > size ? capacity : size;

    //allocate data
    if (vec->capacity > 0) {
        //allocate memory 
        vec->data = malloc(elem_size * vec->capacity);

        //if allocation failed
        if (!vec->data) {
            vec->size = 0;
            vec->capacity = 0;
            return 0;
        }

        //copy elements, if given
        if (elements) {
            memcpy(vec->data, elements, elem_size * size);
        }
    }

    //no data
    else {
        vec->data = NULL;
    }

    //success
    return 1;
}


//Frees the memory occupied by a vector.
void alg_cleanup_vector(ALG_VECTOR* vec) {
    assert(vec);
    if (vec->data) {
        free(vec->data);
        vec->data = NULL;
        vec->capacity = 0;
        vec->size = 0;
    }
}


//Returns the element size of a vector.
int alg_get_vector_element_size(ALG_VECTOR* vec) {
    assert(vec);
    return vec->elem_size;
}


//Returns the number of elements of a vector.
int alg_get_vector_size(ALG_VECTOR* vec) {
    assert(vec);
    return vec->size;
}


//Sets the vector size.
int alg_set_vector_size(ALG_VECTOR* vec, int size) {
    assert(vec);

    //if no change, do nothing
    if (size == vec->size) {
        return 1;
    }

    //check the size
    if (size < 0) {
        return 0;
    }

    //if the size is less than the capacity, simply set the size member;
    //this also accounts for clearing the vector
    if (size <= vec->capacity) {
        vec->size = size;
        return 1;
    }

    int capacity = vec->capacity;

    //set minimum capacity
    if (capacity == 0) {
        capacity = 16;
    }

    //grow the capacity
    while (capacity <= 1024 && capacity <= size) {
        capacity *= 2;
    }
    while (capacity <= size) {
        capacity += 1024;
    }

    //reallocate the memory of the vector
    void* new_data = realloc(vec->data, vec->elem_size * capacity);
    
    //if reallocation failed
    if (!new_data) {
        return 0;
    }

    //save the new data
    vec->data = new_data;
    vec->size = size;
    vec->capacity = capacity;

    //success
    return 1;
}


//Sets the vector size to zero.
void alg_clear_vector(ALG_VECTOR* vec) {
    alg_set_vector_size(vec, 0);
}


//Returns the vector capacity.
int alg_get_vector_capacity(ALG_VECTOR* vec) {
    assert(vec);
    return vec->capacity;
}


//Sets the capacity of the vector.
int alg_set_vector_capacity(ALG_VECTOR* vec, int capacity) {
    assert(vec);

    //check for change
    if (capacity == vec->capacity) {
        return 1;
    }

    //if capacity is negative
    if (capacity < 0) {
        return 0;
    }

    //if capacity is 0, totally clear the vector
    if (capacity == 0) {
        free(vec->data);
        vec->data = NULL;
        vec->size = 0;
        vec->capacity = 0;
        return 1;
    }

    //reallocate the data
    void* new_data = realloc(vec->data, vec->elem_size * capacity);

    //if reallocation failed
    if (!new_data) {
        return 0;
    }

    //save the new data
    vec->data = new_data;
    vec->size = capacity < vec->size ? capacity : vec->size;
    vec->capacity = capacity;

    //success
    return 1;
}


//Returns the element at the specific index.
void* alg_get_vector_element(ALG_VECTOR* vec, int index) {
    assert(vec);
    return index >= 0 && index < vec->size ? (char*)vec->data + index * vec->elem_size : NULL;
}


//Copies the given element into the vector at the specific index.
int alg_set_vector_element(ALG_VECTOR* vec, int index, void* elem) {
    assert(vec);
    if (index >= 0 && index < vec->size) {
        memcpy((char*)vec->data + index * vec->elem_size, elem, vec->elem_size);
        return 1;
    }
    return 0;
}


//resize and set element
int alg_resize_and_set_vector_element(ALG_VECTOR* vec, int index, void* elem) {
    assert(vec);
    assert(index >= 0);

    //resize, if needed
    if (index >= vec->size) {
        alg_set_vector_size(vec, index + 1);
    }

    //set element
    return alg_set_vector_element(vec, index, elem);
}
