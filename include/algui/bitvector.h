#ifndef ALG_BITVECTOR_H
#define ALG_BITVECTOR_H


#include <stdint.h>


typedef struct ALG_BITVECTOR {
    uintptr_t* words;
    int owner : 1;
    int count : sizeof(int) * 8 - 1;
} ALG_BITVECTOR;


/**
 * Initializes a bit vector.
 * @param vec bit vector to initialize.
 * @param words array of words that becomes the storage for the bitvector.
 * @param count number of words.
 * @param owner owner flag; if true, then when it is cleaned up, it frees the memory occupied by the bitvector.
 */
void alg_init_bitvector(ALG_BITVECTOR* vec, uintptr_t* words, int count, int owner);


/**
 * Cleans up the bitvector.
 * If the bitvector is the owner of the storage, then the storage is freed.
 * @param vec bit vector to cleanup.
 */
void alg_cleanup_bitvector(ALG_BITVECTOR* vec);


/**
 * Gets the size of a bitvector in bits.
 * @param vec vector to get the size of.
 * @return the size of the bit vector.
 */
int alg_get_bitvector_size(ALG_BITVECTOR* vec);


/**
 * Gets the value of the bit vector at the given bit index.
 * @param vec vector to get the value of.
 * @param index index of the bit.
 * @return 1 if the bit is set, 0 if the bit is not set or if the index is invalid.
 */
int alg_get_bitvector_bit(ALG_BITVECTOR* vec, int index);


/**
 * Tests if a bit vector contains one or more flags set.
 * @param index index of 1st bit.
 * @param ... rest of indexes; 0-terminated.
 * @return true if the bit vector contains any of the flags set.
 */
int alg_test_bitvector_bits(ALG_BITVECTOR* vec, int index1, ...);


/**
 * Sets the bit value at the specified index.
 * It will allocate a new buffer if the index is beyond the current size.
 * @param vec vector to set the bit value of.
 * @param index the bit index.
 * @param value the bit value.
 * @return non-zero on success, zero on memory allocation error.
 */
int alg_set_bitvector_bit(ALG_BITVECTOR* vec, int index, int value);


#endif //ALG_BITVECTOR_H
