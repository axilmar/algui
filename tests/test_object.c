#include "algui/test.h"
#include "algui/messages.h"
#include "algui/object.h"
#include "algui/util.h"


#define MSG_GET_TEST_VALUE 10000
#define MSG_SET_TEST_VALUE 10001


typedef struct TEST_OBJECT {
    ALGUI_OBJECT object;
    int test_value;
} TEST_OBJECT;


static ALGUI_OBJECT* freed_object = NULL;


static bool test_proc(ALGUI_OBJECT* obj, int id, void* data) {
    switch (id) {
        case ALGUI_MSG_MALLOC:
            ALGUI_PTR_VAL(void*, data) = ALGUI_NEW(TEST_OBJECT);
            return true;

        case ALGUI_MSG_FREE:
            freed_object = obj;
            break;

        case ALGUI_MSG_INIT:
            ALGUI_PTR(TEST_OBJECT, obj)->test_value = 0;
            return true;

        case ALGUI_MSG_CLEANUP:
            ALGUI_PTR(TEST_OBJECT, obj)->test_value = -1;
            return true;

        case MSG_GET_TEST_VALUE:
            ALGUI_PTR_VAL(int, data) = ALGUI_PTR(TEST_OBJECT, obj)->test_value;
            return true;

        case MSG_SET_TEST_VALUE:
            ALGUI_PTR(TEST_OBJECT, obj)->test_value = ALGUI_PTR_VAL(int, data);
            return true;
    }

    return algui_object_proc(obj, id, data);
}


static bool test_algui_object_proc(void* context) {
    //free null object
    {
        ALGUI_ENSURE_EXPRESSION(!algui_object_proc(NULL, ALGUI_MSG_FREE, NULL), EINVAL);
    }

    //free non-null object
    {
        ALGUI_OBJECT* obj = ALGUI_NEW(ALGUI_OBJECT);
        ALGUI_ENSURE_EXPRESSION(algui_object_proc(obj, ALGUI_MSG_FREE, NULL), 0);
    }

    return true;
}


static bool test_algui_do_message(void* context) {
    int v = 0;

    //null object
    {
        ALGUI_ENSURE_EXPRESSION(!algui_do_message(NULL, MSG_SET_TEST_VALUE, &v), EINVAL);
    }

    //non-null object
    {
        TEST_OBJECT obj = { { test_proc }, 0 };
        v = 55;

        //set value
        ALGUI_ENSURE_EXPRESSION(algui_do_message(&obj.object, MSG_SET_TEST_VALUE, &v), 0);
        ALGUI_ENSURE(obj.test_value == 55);

        //get value
        v = 0;
        ALGUI_ENSURE_EXPRESSION(algui_do_message(&obj.object, MSG_GET_TEST_VALUE, &v), 0);
        ALGUI_ENSURE(v == 55);
    }

    return true;
}


static bool test_algui_init_object(void* context) {
    //null object
    {
        ALGUI_ENSURE_EXPRESSION(!algui_init_object(NULL, test_proc), EINVAL);
    }

    //null proc
    {
        TEST_OBJECT obj;
        ALGUI_ENSURE_EXPRESSION(!algui_init_object(&obj.object, NULL), EINVAL);
    }

    //success
    {
        TEST_OBJECT obj;
        ALGUI_ENSURE_EXPRESSION(algui_init_object(&obj.object, test_proc), 0);
    }

    return true;
}


static bool test_algui_cleanup_object(void* context) {
    //null object
    {
        ALGUI_ENSURE_EXPRESSION(!algui_cleanup_object(NULL), EINVAL);
    }

    //success
    {
        TEST_OBJECT obj;

        //init
        algui_init_object(&obj.object, test_proc);
        obj.test_value = 55;

        //cleanup
        ALGUI_ENSURE_EXPRESSION(algui_cleanup_object(&obj.object), 0);
        ALGUI_ENSURE(obj.test_value == -1);
    }

    return true;
}


static bool test_algui_create_object(void* context) {
    //null proc
    {
        ALGUI_ENSURE_EXPRESSION(!algui_create_object(NULL), EINVAL);
    }

    //success
    {
        ALGUI_OBJECT *obj;
        ALGUI_ENSURE_EXPRESSION(obj = algui_create_object(test_proc), 0);
        free(obj);
    }

    return true;
}


static bool test_algui_destroy_object(void* context) {
    //null proc
    {
        ALGUI_ENSURE_EXPRESSION(!algui_destroy_object(NULL), EINVAL);
    }

    //success
    {
        ALGUI_OBJECT* obj = algui_create_object(test_proc);
        ALGUI_ENSURE_EXPRESSION(algui_destroy_object(obj), 0);
        ALGUI_ENSURE(freed_object == obj);
    }

    return true;
}


void test_object(ALGUI_TEST* test) {
    algui_run_test_case(test, "algui_object_proc", test_algui_object_proc, NULL);
    algui_run_test_case(test, "algui_do_message", test_algui_do_message, NULL);
    algui_run_test_case(test, "algui_init_object", test_algui_init_object, NULL);
    algui_run_test_case(test, "algui_cleanup_object", test_algui_cleanup_object, NULL);
    algui_run_test_case(test, "algui_create_object", test_algui_create_object, NULL);
    algui_run_test_case(test, "algui_destroy_object", test_algui_destroy_object, NULL);
}
