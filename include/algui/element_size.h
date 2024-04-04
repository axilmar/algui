#ifndef ALGUI_ELEMENT_SIZE_H
#define ALGUI_ELEMENT_SIZE_H


/**
 * Number of bits for element size.
 */
#ifndef ALGUI_MAX_ELEMENT_SIZE_BITS
#define ALGUI_MAX_ELEMENT_SIZE_BITS 12
#endif


/**
 * Max element size.
 */
#define ALGUI_MAX_ELEMENT_SIZE (1 << ALGUI_MAX_ELEMENT_SIZE_BITS)


#endif //ALGUI_ELEMENT_SIZE_H
