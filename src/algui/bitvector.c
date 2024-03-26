#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "algui/bitvector.h"


//4096 bits
static const int LIMIT = 4096 / (sizeof(uintptr_t) * 8);


//computes a new word count
static int compute_new_word_count(int curr_count, int max_count) {
    //can't double 0, so set it to 1 if it is 0
    if (curr_count == 0) {
        curr_count = 1;
    }

    //while below limit, double it
    while (curr_count < max_count && curr_count < LIMIT) {
        curr_count *= 2;
    }

    //above limit, increment it
    while (curr_count < max_count) {
        curr_count += LIMIT;
    }

    return curr_count;
}


//manage storage for bit vector
static int manage_storage(ALG_BITVECTOR* vec, int word_addr) {
    //if word address is less than count, we are ok
    if (word_addr < vec->count) {
        return 1;
    }

    //compute the new size, in words
    int new_word_count = compute_new_word_count(vec->count, word_addr + 1);

    //if it owns the memory, then reallocate it
    if (vec->owner) {
        uintptr_t* new_words = realloc(vec->words, new_word_count * sizeof(uintptr_t));
        if (!new_words) {
            return 0;
        }
        memset(new_words + vec->count, 0, (new_word_count - vec->count) * sizeof(uintptr_t));
        vec->words = new_words;
    }

    //else copy the memory to an owned buffer
    else {
        uintptr_t* new_words = (uintptr_t*)malloc(new_word_count * sizeof(uintptr_t));
        if (!new_words) {
            return 0;
        }
        memcpy(new_words, vec->words, vec->count * sizeof(uintptr_t));
        memset(new_words + vec->count, 0, (new_word_count - vec->count) * sizeof(uintptr_t));
        vec->words = new_words;
        vec->count = new_word_count;
        vec->owner = 1;
    }
    return 1;
}


//init bit vector
void alg_init_bitvector(ALG_BITVECTOR* vec, uintptr_t* words, int count, int owner) {
    assert(vec);
    assert((words == NULL && count == 0) || (words != NULL && count > 0));
    vec->words = words;
    vec->count = count;
    vec->owner = owner;
}


//Cleans up the bitvector.
void alg_cleanup_bitvector(ALG_BITVECTOR* vec) {
    assert(vec);
    if (vec->owner) {
        free(vec->words);
    }
}


//returns the bitvector size
int alg_get_bitvector_size(ALG_BITVECTOR* vec) {
    assert(vec);
    return vec->count * (sizeof(uintptr_t) * 8);
}


//Gets the value of the bit vector at the given bit index.
int alg_get_bitvector_bit(ALG_BITVECTOR* vec, int index) {
    assert(vec);
    assert(index >= 0);
    const int word_addr = index / (sizeof(uintptr_t) * 8);
    if (word_addr < vec->count) {
        const int bit_addr = 1 << (index % (sizeof(uintptr_t) * 8));
        return (vec->words[word_addr] & bit_addr) != 0;
    }
    return 0;
}


//test multiple bits
int alg_test_bitvector_bits(ALG_BITVECTOR* vec, int index1, ...) {
    //test first bit
    if (alg_get_bitvector_bit(vec, index1)) {
        return 1;
    }

    //test the rest of the bits
    va_list indexes;
    va_start(indexes, index1);
    for (;;) {
        int index = va_arg(indexes, int);
        if (!index) {
            break;
        }
        if (alg_get_bitvector_bit(vec, index)) {
            return 1;
        }
    }
    va_end(indexes);
    
    //no flag set
    return 0;
}


//Sets the bit value at the specified index.
int alg_set_bitvector_bit(ALG_BITVECTOR* vec, int index, int value) {
    assert(vec);
    assert(index >= 0);
    const int word_addr = index / (sizeof(uintptr_t) * 8);
    const int bit_addr = 1 << (index % (sizeof(uintptr_t) * 8));
    if (manage_storage(vec, word_addr)) {
        vec->words[word_addr] = value ? (vec->words[word_addr] | bit_addr) : (vec->words[word_addr] & ~bit_addr);
        return 1;
    }
    return 0;
}
