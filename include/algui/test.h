#ifndef ALGUI_TEST_H
#define ALGUI_TEST_H


#include <stdio.h>
#include <errno.h>
#include <stdbool.h>


/**
 * Macro that executes a specific action if the expression evaluates to false.
 * It prints a message to stderr about the file and line of the error.
 * @param EXPR expression to evaluate.
 * @param ACTION code to execute if the expression evaluates to false.
 */
#define ALGUI_ON_TEST_FAILURE(EXPR, ACTION) {\
    if (!(EXPR)) {\
        fprintf(stderr, "\033[0;91mFAILURE\033[0m\n    \033[0;97mERROR: %s, %i: %s\033[0m\n", __FILE__, __LINE__, #EXPR);\
        {ACTION;}\
    }\
}


/**
 * Macro that returns false if the given expresion evaluates to false.
 * It prints a message to stderr about the file and line of the error.
 * @param EXPR expression to evaluate.
 */
#define ALGUI_ENSURE(EXPR) ALGUI_ON_TEST_FAILURE(EXPR, return false)


/**
 * Ensures that the given expression, when executed, evaluates to true, and results into errno having the given value.
 * @param EXPR expression to evaluate.
 * @param ERRCODE error code to test with errno.
 */
#define ALGUI_ENSURE_EXPRESSION(EXPR, ERRCODE) {\
    errno = 0;\
    ALGUI_ENSURE((EXPR) && errno == (ERRCODE));\
}


/**
 * Ensures that the given statement, when executed, results into errno having the given value.
 * @param STM statement to evaluate.
 * @param ERRCODE error code to test with errno.
 */
#define ALGUI_ENSURE_STATEMENT(STM, ERRCODE) {\
    errno = 0;\
    {STM;}\
    ALGUI_ENSURE(errno == (ERRCODE));\
}


/**
 * Test test. 
 */
typedef struct ALGUI_TEST {
    ///total number of tests performed.
    unsigned test_count;

    ///number of successful tests.
    unsigned successful_test_count;

    ///number of failed tests.
    unsigned failed_test_count;
} ALGUI_TEST;


/**
 * Test function. 
 * @param context test context.
 * @return true for success, false for failure.
 */
typedef bool (*ALGUI_TEST_FUNCTION)(void* context);


/**
 * Initialiazes a test and a test structure.
 * On windows, it also enables virtual terminal for output, in order to show the colors.
 * @param test pointer to test test to initialize; shall not be null.
 * @return true if the test is not null, false otherwise.
 */
bool algui_init_test(ALGUI_TEST* test);


/**
 * Runs a test function.
 * @param test test to update.
 * @param name name of test.
 * @param func test function.
 * @param context test context.
 * @return true for success, false on failure or on invalid argument.
 */
bool algui_run_test_case(ALGUI_TEST* test, const char* name, ALGUI_TEST_FUNCTION func, void* context);


/**
 * Prints the test results in stdout.
 * @param test test with results to print.
 * @return true if the argument was not null, false otherwise.
 */
bool algui_print_test(const ALGUI_TEST* test);


#endif //ALGUI_TEST_H
