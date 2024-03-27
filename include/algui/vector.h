#ifndef ALG_VECTOR_H
#define ALG_VECTOR_H


/**
 * A vector (dynamic array).
 */
typedef struct ALG_VECTOR {
    void* data;
    int size;
    int capacity;
    int elem_size;
} ALG_VECTOR;


/**
 * Initializes a vector.
 * @param vec vector to initialize.
 * @param elem_size element size.
 * @param size initial number of elements.
 * @param elements initial elements.
 * @param capacity initial capacity; if 0, then it is set equal to size.
 * @return non-zero on success, zero on allocation failure.
 */
int alg_init_vector(ALG_VECTOR* vec, int elem_size, int size, void* elements, int capacity);


/**
 * Frees the memory occupied by a vector.
 * @param vec vector to cleanup.
 */
void alg_cleanup_vector(ALG_VECTOR* vec);


/**
 * Returns the element size of a vector.
 * @return the element size of a vector.
 */
int alg_get_vector_element_size(ALG_VECTOR* vec);


/**
 * Returns the number of elements of a vector.
 * @param vec vector to get the size of.
 * @return number of elements.
 */
int alg_get_vector_size(ALG_VECTOR* vec);


/**
 * Sets the vector size.
 * If extra capacity is requested, then the vector memory is reallocated with realloc.
 * @param vec vector to resize.
 * @param size new size.
 * @return non-zero on success, zero on reallocation failure.
 */
int alg_set_vector_size(ALG_VECTOR* vec, int size);


/**
 * Sets the vector size to zero.
 * @param vec vector to clear.
 */
void alg_clear_vector(ALG_VECTOR* vec);


/**
 * Returns the vector capacity.
 * @param vec the vector to get the capacity of.
 * @return the vector's capacity.
 */
int alg_get_vector_capacity(ALG_VECTOR* vec);


/**
 * Sets the capacity of the vector.
 * If the capacity gets smaller than the size, the vector is resized to match the capacity.
 * @param vector vector to set the capacity of.
 * @param capacity the new capacity.
 * @return non-zero on success, zero on reallocation failure.
 */
int alg_set_vector_capacity(ALG_VECTOR* vec, int capacity);


/**
 * Returns the element at the specific index.
 * @param vec the vector to get the index of.
 * @return the element at the specific index or null if the index is invalid.
 */
void* alg_get_vector_element(ALG_VECTOR* vec, int index);


/**
 * Copies the given element into the vector at the specific index.
 * @param vec the vector to get the index of.
 * @param element the element to copy.
 * @return non-zero on success, zero if index is invalid.
 */
int alg_set_vector_element(ALG_VECTOR* vec, int index, void* elem);


/**
 * Resizes the vector to hold the given index, then copies the given element into the vector at the specific index.
 * @param vec the vector to get the index of.
 * @param element the element to copy.
 * @return non-zero on success, zero if index is invalid.
 */
int alg_resize_and_set_vector_element(ALG_VECTOR* vec, int index, void* elem);


#endif //ALG_VECTOR_H
