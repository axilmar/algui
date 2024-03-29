#ifndef ALGUI_TEST_H
#define ALGUI_TEST_H


#include <stdio.h>
#include "bool.h"


/**
 * Macro that returns false if the given expresion evaluates to false.
 * It prints a message to stderr about the file and line of the error.
 * @param EXPR expression to evaluate.
 */
#define ALGUI_ENSURE(EXPR) {\
    if (!(EXPR)) {\
        fprintf(stderr, "\033[0;91mFAILURE\033[0m\n    \033[0;97mERROR: %s, %i: %s\033[0m\n", __FILE__, __LINE__, #EXPR);\
        return ALGUI_FALSE;\
    }\
}


/**
 * Test statistics. 
 */
typedef struct ALGUI_TEST_STATISTICS {
    ///total number of tests performed.
    unsigned test_count;

    ///number of successful tests.
    unsigned successful_test_count;

    ///number of failed tests.
    unsigned failed_test_count;
} ALGUI_TEST_STATISTICS;


/**
 * Test function. 
 * @param data user data.
 * @return true for success, false for failure.
 */
typedef ALGUI_BOOL(*ALGUI_TEST_FUNCTION)(void* data);


/**
 * Initialiazes a test and a test structure.
 * On windows, it also enables virtual terminal for output, in order to show the colors.
 * @param stats pointer to test structure to initialize; shall not be null.
 * @return true if the stats is not null, false otherwise.
 */
ALGUI_BOOL algui_init_test(ALGUI_TEST_STATISTICS* stats);


/**
 * Runs a test function.
 * @param stats statistics to update.
 * @param name name of test.
 * @param func test function.
 * @param data user data to pass to the test function.
 * @return true for success, false on failure or on invalid argument.
 */
ALGUI_BOOL algui_do_test(ALGUI_TEST_STATISTICS* stats, const char* name, ALGUI_TEST_FUNCTION func, void* data);


/**
 * Prints the test statistics in stdout.
 * @param stats stats to print.
 * @return true if the argument was not null, false otherwise.
 */
ALGUI_BOOL algui_print_test_statistics(const ALGUI_TEST_STATISTICS* stats);


#endif //ALGUI_TEST_H
