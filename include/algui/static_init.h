#ifndef ALG_STATIC_INIT_H
#define ALG_STATIC_INIT_H


/**
 * Static code initialization.
 * Not thread safe.
 * @param CODE code to run once.
 */
#define ALG_STATIC_INIT(CODE) {\
    static int flag = 0;\
    if (!flag) {\
        flag = 1;\
        { CODE }\
    }\
}


#endif //ALG_STATIC_INIT_H
