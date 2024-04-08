#ifndef ALGUI_MINMAX_H
#define ALGUI_MINMAX_H


/**
 * Macro for getting the minimum of two values.
 * @param A first value.
 * @param B second value.
 * @return the minimum between the two.
 */
#define ALGUI_MIN(A, B) ((A) < (B) ? (A) : (B))


 /**
  * Macro for getting the maximum of two values.
  * @param A first value.
  * @param B second value.
  * @return the maximum between the two.
  */
#define ALGUI_MAX(A, B) ((A) > (B) ? (A) : (B))


#endif //ALGUI_MINMAX_H
