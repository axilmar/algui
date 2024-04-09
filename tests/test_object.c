#include <string.h>
#include <memory.h>
#include "algui/test.h"
#include "algui/object.h"
#include "util.h"


typedef long long long_long;
typedef unsigned char unsigned_char;
typedef unsigned short unsigned_short;
typedef unsigned int unsigned_int;
typedef unsigned long unsigned_long;
typedef unsigned long long unsigned_long_long;
typedef long double long_double;
typedef ALGUI_BOOL bool;


enum TEST_MESSAGES {
    MSG_TEST = ALGUI_MSG_LAST
};


typedef struct TEST_OBJECT {
    ALGUI_OBJECT object;
    int test_value;
} TEST_OBJECT;


typedef struct TEST_DATA {
    int value;
} TEST_DATA;


static void init_test_object(TEST_OBJECT* obj) {
    algui_init_object(&obj->object);
    obj->test_value = 0;
}


static ALGUI_RESULT test_cleanup_handler(ALGUI_OBJECT* obj, void* data) {
    ++((TEST_OBJECT*)obj)->test_value;
    return 0;
}


static ALGUI_RESULT test_handler(ALGUI_OBJECT* obj, void* data) {
    ((TEST_OBJECT*)obj)->test_value = ((TEST_DATA*)data)->value;
    return 1;
}


static ALGUI_RESULT test_handler1(ALGUI_OBJECT* obj, void* data) {
    ((TEST_OBJECT*)obj)->test_value = ((TEST_DATA*)data)->value;
    return 2;
}


static ALGUI_RESULT unknown_message_handler(ALGUI_OBJECT* obj, void* data) {
    ALGUI_MESSAGE* msg = (ALGUI_MESSAGE*)data;

    switch (msg->id) {
        case MSG_TEST:
            return test_handler(obj, msg->data);
    }

    return 0;
}


static ALGUI_BOOL test_value_get(ALGUI_OBJECT* obj, ALGUI_BUFFER* buf) {
    if (buf->size < sizeof(int)) {
        errno = EINVAL;
        return ALGUI_FALSE;
    }
    *(int*)buf->data = ((TEST_OBJECT*)obj)->test_value;
    return ALGUI_TRUE;
}


static ALGUI_BOOL test_value_set(ALGUI_OBJECT* obj, const ALGUI_BUFFER* buf) {
    ((TEST_OBJECT*)obj)->test_value = *(const int*)buf->data;
    return ALGUI_TRUE;
}


static ALGUI_BOOL test_init_object(void* context) {
    //null object
    {
        ALGUI_ENSURE_ERROR(algui_init_object(NULL) == ALGUI_FALSE, EINVAL);
    }

    //non-null object
    {
        ALGUI_OBJECT object;
        ALGUI_ENSURE_ERROR(algui_init_object(&object) == ALGUI_TRUE, 0);
        algui_cleanup_map(&object.data);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_cleanup_object(void* context) {
    //null object
    {
        ALGUI_ENSURE_ERROR(algui_cleanup_object(NULL) == ALGUI_FALSE, EINVAL);
    }

    //non-null object, no cleanup handler
    {
        ALGUI_OBJECT object;
        algui_init_object(&object);
        ALGUI_ENSURE_ERROR(algui_cleanup_object(&object) == ALGUI_TRUE, EINVAL);
    }

    //no-null object, cleanup handler
    {
        TEST_OBJECT object;
        init_test_object(&object);
        algui_set_object_message_handler(&object.object, ALGUI_MSG_CLEANUP, test_cleanup_handler, NULL, NULL);
        ALGUI_ENSURE_ERROR(algui_cleanup_object(&object.object) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(object.test_value == 1);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_define_object_property(void* context) {
    //null object
    {
        ALGUI_PROPERTY_DEFINITION pd;
        ALGUI_ENSURE_ERROR(algui_define_object_property(NULL, ALGUI_PROP_ID, &pd, NULL, NULL) == ALGUI_FALSE, EINVAL);
    }

    //null property
    {
        ALGUI_OBJECT object;
        ALGUI_ENSURE_ERROR(algui_define_object_property(&object, ALGUI_PROP_ID, NULL, NULL, NULL) == ALGUI_FALSE, EINVAL);
    }

    //invalid property type
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = 200;

        ALGUI_ENSURE_ERROR(algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, NULL, NULL) == ALGUI_FALSE, EINVAL);

        algui_cleanup_object(&test.object);
    }

    //accessor property, null get, null set, null initial value
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_ACCESSOR;
        pd.accessor.get = NULL;
        pd.accessor.set = NULL;

        ALGUI_ENSURE_ERROR(algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, NULL, NULL) == ALGUI_FALSE, EINVAL);

        algui_cleanup_object(&test.object);
    }

    //accessor property, non null get, null set, null initial value
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_ACCESSOR;
        pd.accessor.get = test_value_get;
        pd.accessor.set = NULL;

        ALGUI_ENSURE_ERROR(algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, NULL, NULL) == ALGUI_TRUE, 0);

        algui_cleanup_object(&test.object);
    }

    //accessor property, non null get, null set, non-null initial value
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_ACCESSOR;
        pd.accessor.get = test_value_get;
        pd.accessor.set = NULL;

        ALGUI_BUFFER initial_value;
        int value = 5;
        algui_init_buffer(&initial_value, &value, sizeof(value), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, &initial_value, NULL) == ALGUI_FALSE, EINVAL);

        algui_cleanup_object(&test.object);
    }

    //accessor property, null get, non null set, null initial value
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_ACCESSOR;
        pd.accessor.get = NULL;
        pd.accessor.set = test_value_set;

        ALGUI_ENSURE_ERROR(algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, NULL, NULL) == ALGUI_TRUE, 0);

        algui_cleanup_object(&test.object);
    }

    //accessor property, null get, non null set, non null initial value
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_ACCESSOR;
        pd.accessor.get = NULL;
        pd.accessor.set = test_value_set;

        ALGUI_BUFFER initial_value;
        int value = 5;
        algui_init_buffer(&initial_value, &value, sizeof(value), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, &initial_value, NULL) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(test.test_value == 5);

        algui_cleanup_object(&test.object);
    }

    //accessor property, non null get, non null set, null initial value
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_ACCESSOR;
        pd.accessor.get = test_value_get;
        pd.accessor.set = test_value_set;

        ALGUI_ENSURE_ERROR(algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, NULL, NULL) == ALGUI_TRUE, 0);

        algui_cleanup_object(&test.object);
    }

    //embedded property
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        memset(&pd.value, 0, sizeof(pd.value));
        pd.value.size = sizeof(int);

        ALGUI_BUFFER initial_value;
        int value = 5;
        algui_init_buffer(&initial_value, &value, sizeof(value), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, &initial_value, NULL) == ALGUI_TRUE, 0);

        algui_cleanup_object(&test.object);
    }

    //statically allocated property
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        memset(&pd.value, 0, sizeof(pd.value));

        ALGUI_BUFFER initial_value;
        int value[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        algui_init_buffer(&initial_value, value, sizeof(value), ALGUI_FALSE);
        pd.value.size = sizeof(value);

        ALGUI_ENSURE_ERROR(algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, &initial_value, NULL) == ALGUI_TRUE, 0);

        algui_cleanup_object(&test.object);
    }

    //dynamically allocated property
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        memset(&pd.value, 0, sizeof(pd.value));
        pd.value.size = 0;

        ALGUI_BUFFER initial_value;
        const char* value = "the quick brown fox jumps over the lazy dog.";
        algui_init_buffer(&initial_value, (void*)value, strlen(value), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, &initial_value, NULL) == ALGUI_TRUE, 0);

        algui_cleanup_object(&test.object);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_delete_object_property(void* context) {
    //null object
    {
        ALGUI_ENSURE_ERROR(algui_delete_object_property(NULL, ALGUI_PROP_ID, NULL) == ALGUI_FALSE, EINVAL);
    }

    //non-existent property
    {
        ALGUI_OBJECT object;
        algui_init_object(&object);
        ALGUI_ENSURE_ERROR(algui_delete_object_property(&object, ALGUI_PROP_ID, NULL) == ALGUI_FALSE, EINVAL);
        algui_cleanup_object(&object);
    }

    //existing property
    {
        ALGUI_OBJECT object;
        algui_init_object(&object);

        ALGUI_PROPERTY_DEFINITION pd;
        memset(&pd, 0, sizeof(pd));
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        pd.value.size = sizeof(int);
        algui_define_object_property(&object, ALGUI_PROP_ID, &pd, NULL, NULL);

        ALGUI_ENSURE_ERROR(algui_delete_object_property(&object, ALGUI_PROP_ID, NULL) == ALGUI_TRUE, 0);

        algui_cleanup_object(&object);
    }

    //existing property with access token
    {
        ALGUI_OBJECT object;
        algui_init_object(&object);

        ALGUI_PROPERTY_DEFINITION pd;
        memset(&pd, 0, sizeof(pd));
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        pd.value.size = sizeof(int);
        ALGUI_BUFFER access_token;
        char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, access_token_data, sizeof(access_token_data), ALGUI_FALSE);
        algui_define_object_property(&object, ALGUI_PROP_ID, &pd, NULL, &access_token);

        ALGUI_ENSURE_ERROR(algui_delete_object_property(&object, ALGUI_PROP_ID, NULL) == ALGUI_FALSE, EINVAL);
        ALGUI_ENSURE_ERROR(algui_delete_object_property(&object, ALGUI_PROP_ID, &access_token) == ALGUI_TRUE, 0);

        algui_cleanup_object(&object);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_get_object_property(void* context) {
    //null object
    {
        ALGUI_BUFFER value = ALGUI_BUFFER_INIT;
        ALGUI_ENSURE_ERROR(algui_get_object_property(NULL, ALGUI_PROP_ID, &value, NULL, NULL) == ALGUI_FALSE, EINVAL);
    }

    //null buffer
    {
        TEST_OBJECT test;
        init_test_object(&test);
        ALGUI_ENSURE_ERROR(algui_get_object_property(&test.object, ALGUI_PROP_ID, NULL, NULL, NULL) == ALGUI_FALSE, EINVAL);
        algui_cleanup_object(&test.object);
    }

    //invalid property
    {
        TEST_OBJECT test;
        init_test_object(&test);
        ALGUI_BUFFER value = ALGUI_BUFFER_INIT;
        ALGUI_ENSURE_ERROR(algui_get_object_property(&test.object, ALGUI_PROP_ID, &value, NULL, NULL) == ALGUI_FALSE, EINVAL);
        algui_cleanup_object(&test.object);
    }

    //invalid property, default value
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_BUFFER result_value;
        int result_value_data = 0;
        algui_init_buffer(&result_value, &result_value_data, sizeof(result_value_data), ALGUI_FALSE);

        ALGUI_BUFFER default_value;
        int default_value_data = 15;
        algui_init_buffer(&default_value, &default_value_data, sizeof(default_value_data), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_get_object_property(&test.object, ALGUI_PROP_ID, &result_value, &default_value, NULL) == ALGUI_TRUE, EINVAL);
        ALGUI_ENSURE(result_value_data == 15);

        algui_cleanup_object(&test.object);
    }

    //get accessor property, invalid buffer size
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_ACCESSOR;
        pd.accessor.get = test_value_get;
        pd.accessor.set = test_value_set;
        ALGUI_BUFFER initial_value;
        int test_value = 5;
        algui_init_buffer(&initial_value, &test_value, sizeof(test_value), ALGUI_FALSE);
        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, &initial_value, NULL);

        ALGUI_BUFFER prop_value;
        algui_init_buffer(&prop_value, NULL, sizeof(test_value)/2, ALGUI_TRUE);
        ALGUI_ENSURE_ERROR(algui_get_object_property(&test.object, ALGUI_PROP_ID, &prop_value, NULL, NULL) == ALGUI_FALSE, EINVAL);

        algui_cleanup_buffer(&prop_value);
        algui_cleanup_object(&test.object);
    }

    //get accessor property, valid buffer size
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_ACCESSOR;
        pd.accessor.get = test_value_get;
        pd.accessor.set = test_value_set;
        ALGUI_BUFFER initial_value;
        int test_value = 5;
        algui_init_buffer(&initial_value, &test_value, sizeof(test_value), ALGUI_FALSE);
        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, &initial_value, NULL);

        ALGUI_BUFFER prop_value;
        algui_init_buffer(&prop_value, NULL, sizeof(test_value), ALGUI_TRUE);
        ALGUI_ENSURE_ERROR(algui_get_object_property(&test.object, ALGUI_PROP_ID, &prop_value, NULL, NULL) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(*(int*)prop_value.data == test_value);

        algui_cleanup_buffer(&prop_value);
        algui_cleanup_object(&test.object);
    }

    //get accessor property, null getter, null default value
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_ACCESSOR;
        pd.accessor.get = NULL;
        pd.accessor.set = test_value_set;
        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, NULL, NULL);

        ALGUI_BUFFER prop_value;
        algui_init_buffer(&prop_value, NULL, sizeof(int), ALGUI_TRUE);
        ALGUI_ENSURE_ERROR(algui_get_object_property(&test.object, ALGUI_PROP_ID, &prop_value, NULL, NULL) == ALGUI_FALSE, EINVAL);

        algui_cleanup_buffer(&prop_value);
        algui_cleanup_object(&test.object);
    }

    //get accessor property, null getter, non-null default value
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_ACCESSOR;
        pd.accessor.get = NULL;
        pd.accessor.set = test_value_set;

        ALGUI_BUFFER default_value;
        int default_value_data = 15;
        algui_init_buffer(&default_value, &default_value_data, sizeof(default_value_data), ALGUI_FALSE);

        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, &default_value, NULL);

        ALGUI_BUFFER prop_value;
        int prop_value_data = 0;
        algui_init_buffer(&prop_value, &prop_value_data, sizeof(int), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_get_object_property(&test.object, ALGUI_PROP_ID, &prop_value, &default_value, NULL) == ALGUI_TRUE, EINVAL);
        ALGUI_ENSURE(prop_value_data == default_value_data);

        algui_cleanup_object(&test.object);
    }

    //embedded property
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        memset(&pd.value, 0, sizeof(pd.value));
        pd.value.size = sizeof(int);

        ALGUI_BUFFER initial_value;
        int initial_value_data = 5;
        algui_init_buffer(&initial_value, &initial_value_data, sizeof(initial_value_data), ALGUI_FALSE);

        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, &initial_value, NULL);

        ALGUI_BUFFER prop_value;
        int prop_value_data = 0;
        algui_init_buffer(&prop_value, &prop_value_data, sizeof(int), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_get_object_property(&test.object, ALGUI_PROP_ID, &prop_value, NULL, NULL) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(prop_value_data == initial_value_data);

        algui_cleanup_object(&test.object);
    }

    //statically allocated property
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        memset(&pd.value, 0, sizeof(pd.value));

        ALGUI_BUFFER initial_value;
        int initial_value_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        algui_init_buffer(&initial_value, &initial_value_data, sizeof(initial_value_data), ALGUI_FALSE);

        pd.value.size = sizeof(initial_value_data);
        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, &initial_value, NULL);

        ALGUI_BUFFER prop_value;
        int prop_value_data[sizeof(initial_value_data) / sizeof(int)];
        algui_init_buffer(&prop_value, &prop_value_data, sizeof(initial_value_data), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_get_object_property(&test.object, ALGUI_PROP_ID, &prop_value, NULL, NULL) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(memcmp(prop_value_data, initial_value_data, sizeof(initial_value_data)) == 0);

        algui_cleanup_object(&test.object);
    }

    //dynamically allocated property
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        memset(&pd.value, 0, sizeof(pd.value));
        pd.value.size = 0;

        ALGUI_BUFFER initial_value;
        const char initial_value_data[] = "the quick brown fox jumps over the lazy dog.";
        algui_init_buffer(&initial_value, (void*)initial_value_data, sizeof(initial_value_data), ALGUI_FALSE);

        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, &initial_value, NULL);

        ALGUI_BUFFER prop_value = ALGUI_BUFFER_INIT;

        ALGUI_ENSURE_ERROR(algui_get_object_property(&test.object, ALGUI_PROP_ID, &prop_value, NULL, NULL) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(prop_value.size == initial_value.size);
        ALGUI_ENSURE(algui_memcmp_buffers(&prop_value, &initial_value) == 0);

        algui_cleanup_object(&test.object);
    }

    //invalid access with null access token
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        memset(&pd.value, 0, sizeof(pd.value));
        pd.value.size = sizeof(int);

        ALGUI_BUFFER initial_value;
        int initial_value_data = 5;
        algui_init_buffer(&initial_value, &initial_value_data, sizeof(initial_value_data), ALGUI_FALSE);

        ALGUI_BUFFER access_token;
        const char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, (void*)access_token_data, sizeof(access_token_data), ALGUI_FALSE);

        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, &initial_value, &access_token);

        ALGUI_BUFFER prop_value;
        int prop_value_data = 0;
        algui_init_buffer(&prop_value, &prop_value_data, sizeof(int), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_get_object_property(&test.object, ALGUI_PROP_ID, &prop_value, NULL, NULL) == ALGUI_FALSE, EINVAL);

        algui_cleanup_object(&test.object);
    }

    //invalid access with null access token and default value
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        memset(&pd.value, 0, sizeof(pd.value));
        pd.value.size = sizeof(int);

        ALGUI_BUFFER initial_value;
        int initial_value_data = 5;
        algui_init_buffer(&initial_value, &initial_value_data, sizeof(initial_value_data), ALGUI_FALSE);

        ALGUI_BUFFER access_token;
        const char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, (void*)access_token_data, sizeof(access_token_data), ALGUI_FALSE);

        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, &initial_value, &access_token);

        ALGUI_BUFFER prop_value;
        int prop_value_data = 0;
        algui_init_buffer(&prop_value, &prop_value_data, sizeof(int), ALGUI_FALSE);

        ALGUI_BUFFER default_value;
        int default_value_data = 15;
        algui_init_buffer(&default_value, &default_value_data, sizeof(default_value_data), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_get_object_property(&test.object, ALGUI_PROP_ID, &prop_value, &default_value, NULL) == ALGUI_TRUE, EINVAL);
        ALGUI_ENSURE(prop_value_data == default_value_data);

        algui_cleanup_object(&test.object);
    }

    //invalid access with invalid access token
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        memset(&pd.value, 0, sizeof(pd.value));
        pd.value.size = sizeof(int);

        ALGUI_BUFFER initial_value;
        int initial_value_data = 5;
        algui_init_buffer(&initial_value, &initial_value_data, sizeof(initial_value_data), ALGUI_FALSE);

        ALGUI_BUFFER access_token;
        const char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, (void*)access_token_data, sizeof(access_token_data), ALGUI_FALSE);

        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, &initial_value, &access_token);

        ALGUI_BUFFER prop_value;
        int prop_value_data = 0;
        algui_init_buffer(&prop_value, &prop_value_data, sizeof(int), ALGUI_FALSE);

        ALGUI_BUFFER get_access_token;
        const char get_access_token_data[] = "I found the secret password!";
        algui_init_buffer(&get_access_token, (void*)get_access_token_data, sizeof(get_access_token_data), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_get_object_property(&test.object, ALGUI_PROP_ID, &prop_value, NULL, &get_access_token) == ALGUI_FALSE, EINVAL);

        algui_cleanup_object(&test.object);
    }

    //invalid access with invalid access token and default value
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        memset(&pd.value, 0, sizeof(pd.value));
        pd.value.size = sizeof(int);

        ALGUI_BUFFER initial_value;
        int initial_value_data = 5;
        algui_init_buffer(&initial_value, &initial_value_data, sizeof(initial_value_data), ALGUI_FALSE);

        ALGUI_BUFFER access_token;
        const char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, (void*)access_token_data, sizeof(access_token_data), ALGUI_FALSE);

        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, &initial_value, &access_token);

        ALGUI_BUFFER prop_value;
        int prop_value_data = 0;
        algui_init_buffer(&prop_value, &prop_value_data, sizeof(int), ALGUI_FALSE);

        ALGUI_BUFFER default_value;
        int default_value_data = 15;
        algui_init_buffer(&default_value, &default_value_data, sizeof(default_value_data), ALGUI_FALSE);

        ALGUI_BUFFER get_access_token;
        const char get_access_token_data[] = "I found the secret password!";
        algui_init_buffer(&get_access_token, (void*)get_access_token_data, sizeof(get_access_token_data), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_get_object_property(&test.object, ALGUI_PROP_ID, &prop_value, &default_value, &get_access_token) == ALGUI_TRUE, EINVAL);
        ALGUI_ENSURE(prop_value_data == default_value_data);

        algui_cleanup_object(&test.object);
    }

    //valid access without default value
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        memset(&pd.value, 0, sizeof(pd.value));
        pd.value.size = sizeof(int);

        ALGUI_BUFFER initial_value;
        int initial_value_data = 5;
        algui_init_buffer(&initial_value, &initial_value_data, sizeof(initial_value_data), ALGUI_FALSE);

        ALGUI_BUFFER access_token;
        const char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, (void*)access_token_data, sizeof(access_token_data), ALGUI_FALSE);

        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, &initial_value, &access_token);

        ALGUI_BUFFER prop_value;
        int prop_value_data = 0;
        algui_init_buffer(&prop_value, &prop_value_data, sizeof(int), ALGUI_FALSE);

        ALGUI_BUFFER get_access_token;
        const char get_access_token_data[] = "secret_password";
        algui_init_buffer(&get_access_token, (void*)get_access_token_data, sizeof(get_access_token_data), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_get_object_property(&test.object, ALGUI_PROP_ID, &prop_value, NULL, &get_access_token) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(prop_value_data == initial_value_data);

        algui_cleanup_object(&test.object);
    }

    //valid access with default value
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        memset(&pd.value, 0, sizeof(pd.value));
        pd.value.size = sizeof(int);

        ALGUI_BUFFER initial_value;
        int initial_value_data = 5;
        algui_init_buffer(&initial_value, &initial_value_data, sizeof(initial_value_data), ALGUI_FALSE);

        ALGUI_BUFFER access_token;
        const char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, (void*)access_token_data, sizeof(access_token_data), ALGUI_FALSE);

        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, &initial_value, &access_token);

        ALGUI_BUFFER prop_value;
        int prop_value_data = 0;
        algui_init_buffer(&prop_value, &prop_value_data, sizeof(int), ALGUI_FALSE);

        ALGUI_BUFFER default_value;
        int default_value_data = 15;
        algui_init_buffer(&default_value, &default_value_data, sizeof(default_value_data), ALGUI_FALSE);

        ALGUI_BUFFER get_access_token;
        const char get_access_token_data[] = "secret_password";
        algui_init_buffer(&get_access_token, (void*)get_access_token_data, sizeof(get_access_token_data), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_get_object_property(&test.object, ALGUI_PROP_ID, &prop_value, &default_value, &get_access_token) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(prop_value_data == initial_value_data);

        algui_cleanup_object(&test.object);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_set_object_property(void* context) {
    //null object
    {
        ALGUI_BUFFER value = ALGUI_BUFFER_INIT;
        ALGUI_ENSURE_ERROR(algui_set_object_property(NULL, ALGUI_PROP_ID, &value, NULL) == ALGUI_FALSE, EINVAL);
    }

    //null buffer
    {
        TEST_OBJECT test;
        init_test_object(&test);
        ALGUI_ENSURE_ERROR(algui_set_object_property(&test.object, ALGUI_PROP_ID, NULL, NULL) == ALGUI_FALSE, EINVAL);
        algui_cleanup_object(&test.object);
    }

    //set invalid property
    {
        TEST_OBJECT test;
        init_test_object(&test);
        ALGUI_BUFFER value = ALGUI_BUFFER_INIT;
        ALGUI_ENSURE_ERROR(algui_set_object_property(&test.object, ALGUI_PROP_ID, &value, NULL) == ALGUI_FALSE, EINVAL);
        algui_cleanup_object(&test.object);
    }

    //set accessor property
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        memset(&pd, 0, sizeof(pd));
        pd.type = ALGUI_PROPERTY_TYPE_ACCESSOR;
        pd.accessor.get = test_value_get;
        pd.accessor.set = test_value_set;
        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, NULL, NULL);

        ALGUI_BUFFER value;
        int value_data = 5;
        algui_init_buffer(&value, &value_data, sizeof(value_data), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_set_object_property(&test.object, ALGUI_PROP_ID, &value, NULL) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(test.test_value == value_data);

        algui_cleanup_object(&test.object);
    }

    //set embedded property
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        memset(&pd, 0, sizeof(pd));
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        pd.value.size = sizeof(int);
        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, NULL, NULL);

        ALGUI_BUFFER value;
        int value_data = 5;
        algui_init_buffer(&value, &value_data, sizeof(value_data), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_set_object_property(&test.object, ALGUI_PROP_ID, &value, NULL) == ALGUI_TRUE, 0);

        ALGUI_BUFFER value1;
        int value1_data = 0;
        algui_init_buffer(&value1, &value1_data, sizeof(value1_data), ALGUI_FALSE);
        algui_get_object_property(&test.object, ALGUI_PROP_ID, &value1, NULL, NULL);
        ALGUI_ENSURE(value1_data == value_data);

        algui_cleanup_object(&test.object);
    }

    //static heap property
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        memset(&pd, 0, sizeof(pd));
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        pd.value.size = sizeof(int) * 10;
        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, NULL, NULL);

        ALGUI_BUFFER value;
        int value_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        algui_init_buffer(&value, &value_data, sizeof(value_data), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_set_object_property(&test.object, ALGUI_PROP_ID, &value, NULL) == ALGUI_TRUE, 0);

        ALGUI_BUFFER value1;
        int value1_data[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        algui_init_buffer(&value1, &value1_data, sizeof(value1_data), ALGUI_FALSE);
        algui_get_object_property(&test.object, ALGUI_PROP_ID, &value1, NULL, NULL);
        ALGUI_ENSURE(memcmp(value1_data, value_data, sizeof(value_data)) == 0);

        algui_cleanup_object(&test.object);
    }

    //dynamic heap property
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        memset(&pd, 0, sizeof(pd));
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        pd.value.size = 0;
        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, NULL, NULL);

        ALGUI_BUFFER value;
        char value_data[] = "the quick brown fox jumps over a lazy dog";
        algui_init_buffer(&value, &value_data, sizeof(value_data), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_set_object_property(&test.object, ALGUI_PROP_ID, &value, NULL) == ALGUI_TRUE, 0);

        ALGUI_BUFFER value1 = ALGUI_BUFFER_INIT;
        algui_get_object_property(&test.object, ALGUI_PROP_ID, &value1, NULL, NULL);
        ALGUI_ENSURE(algui_memcmp_buffers(&value1, &value) == 0);
        algui_cleanup_buffer(&value1);

        algui_cleanup_object(&test.object);
    }

    //set protected property with null access token
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        memset(&pd, 0, sizeof(pd));
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        pd.value.size = sizeof(int);
        ALGUI_BUFFER access_token;
        char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, access_token_data, sizeof(access_token_data), ALGUI_FALSE);
        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, NULL, &access_token);

        ALGUI_BUFFER value;
        int value_data = 5;
        algui_init_buffer(&value, &value_data, sizeof(value_data), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_set_object_property(&test.object, ALGUI_PROP_ID, &value, NULL) == ALGUI_FALSE, EINVAL);

        algui_cleanup_object(&test.object);
    }

    //set protected property with invalid access token
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        memset(&pd, 0, sizeof(pd));
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        pd.value.size = sizeof(int);
        ALGUI_BUFFER access_token;
        char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, access_token_data, sizeof(access_token_data), ALGUI_FALSE);
        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, NULL, &access_token);

        ALGUI_BUFFER value;
        int value_data = 5;
        algui_init_buffer(&value, &value_data, sizeof(value_data), ALGUI_FALSE);
        ALGUI_BUFFER set_access_token;
        char set_access_token_data[] = "I found the secret password!";
        algui_init_buffer(&set_access_token, set_access_token_data, sizeof(set_access_token_data), ALGUI_FALSE);
        ALGUI_ENSURE_ERROR(algui_set_object_property(&test.object, ALGUI_PROP_ID, &value, &set_access_token) == ALGUI_FALSE, EINVAL);

        algui_cleanup_object(&test.object);
    }

    //set protected property with valid access token
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_PROPERTY_DEFINITION pd;
        memset(&pd, 0, sizeof(pd));
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        pd.value.size = sizeof(int);
        ALGUI_BUFFER access_token;
        char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, access_token_data, sizeof(access_token_data), ALGUI_FALSE);
        algui_define_object_property(&test.object, ALGUI_PROP_ID, &pd, NULL, &access_token);

        ALGUI_BUFFER value;
        int value_data = 5;
        algui_init_buffer(&value, &value_data, sizeof(value_data), ALGUI_FALSE);
        ALGUI_BUFFER set_access_token;
        char set_access_token_data[] = "secret_password";
        algui_init_buffer(&set_access_token, set_access_token_data, sizeof(set_access_token_data), ALGUI_FALSE);
        ALGUI_ENSURE_ERROR(algui_set_object_property(&test.object, ALGUI_PROP_ID, &value, &set_access_token) == ALGUI_TRUE, 0);

        ALGUI_BUFFER value1;
        int value1_data = 0;
        algui_init_buffer(&value1, &value1_data, sizeof(value1_data), ALGUI_FALSE);
        algui_get_object_property(&test.object, ALGUI_PROP_ID, &value1, NULL, &set_access_token);
        ALGUI_ENSURE(value1_data == value_data);

        algui_cleanup_object(&test.object);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_get_object_message_handler(void* context) {
    //null object
    {
        ALGUI_ENSURE_ERROR(algui_get_object_message_handler(NULL, MSG_TEST, NULL) == NULL, EINVAL);
    }

    //valid object, invalid handler
    {
        TEST_OBJECT test;
        init_test_object(&test);
        ALGUI_ENSURE_ERROR(algui_get_object_message_handler(&test.object, MSG_TEST, NULL) == NULL, EINVAL);
        algui_cleanup_object(&test.object);
    }

    //valid object, valid handler
    {
        TEST_OBJECT test;
        init_test_object(&test);
        algui_set_object_message_handler(&test.object, MSG_TEST, test_handler, NULL, NULL);
        ALGUI_ENSURE_ERROR(algui_get_object_message_handler(&test.object, MSG_TEST, NULL) == test_handler, 0);
        algui_cleanup_object(&test.object);
    }

    //valid object, valid protected handler, null access token
    {
        TEST_OBJECT test;
        init_test_object(&test);
        
        ALGUI_BUFFER access_token;
        char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, access_token_data, sizeof(access_token_data), ALGUI_FALSE);
        algui_set_object_message_handler(&test.object, MSG_TEST, test_handler, &access_token, NULL);

        ALGUI_ENSURE_ERROR(algui_get_object_message_handler(&test.object, MSG_TEST, NULL) == NULL, EINVAL);

        algui_cleanup_object(&test.object);
    }

    //valid object, valid protected handler, invalid access token
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_BUFFER access_token;
        char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, access_token_data, sizeof(access_token_data), ALGUI_FALSE);
        algui_set_object_message_handler(&test.object, MSG_TEST, test_handler, &access_token, NULL);

        ALGUI_BUFFER get_access_token;
        char get_access_token_data[] = "I found the secret password!";
        algui_init_buffer(&get_access_token, get_access_token_data, sizeof(get_access_token_data), ALGUI_FALSE);
        ALGUI_ENSURE_ERROR(algui_get_object_message_handler(&test.object, MSG_TEST, &get_access_token) == NULL, EINVAL);

        algui_cleanup_object(&test.object);
    }

    //valid object, valid protected handler, valid access token
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_BUFFER access_token;
        char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, access_token_data, sizeof(access_token_data), ALGUI_FALSE);
        algui_set_object_message_handler(&test.object, MSG_TEST, test_handler, &access_token, NULL);

        ALGUI_BUFFER get_access_token;
        char get_access_token_data[] = "secret_password";
        algui_init_buffer(&get_access_token, get_access_token_data, sizeof(get_access_token_data), ALGUI_FALSE);
        ALGUI_ENSURE_ERROR(algui_get_object_message_handler(&test.object, MSG_TEST, &get_access_token) == test_handler, 0);

        algui_cleanup_object(&test.object);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_set_object_message_handler(void* context) {
    //null object
    {
        ALGUI_ENSURE_ERROR(algui_set_object_message_handler(NULL, MSG_TEST, test_handler, NULL, NULL) == ALGUI_FALSE, EINVAL);
    }

    //valid object, invalid handler
    {
        TEST_OBJECT test;
        init_test_object(&test);
        ALGUI_ENSURE_ERROR(algui_set_object_message_handler(&test.object, MSG_TEST, NULL, NULL, NULL) == ALGUI_FALSE, EINVAL);
        algui_cleanup_object(&test.object);
    }

    //valid object, valid handler
    {
        TEST_OBJECT test;
        init_test_object(&test);
        ALGUI_ENSURE_ERROR(algui_set_object_message_handler(&test.object, MSG_TEST, test_handler, NULL, NULL) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(algui_get_object_message_handler(&test.object, MSG_TEST, NULL) == test_handler);
        algui_cleanup_object(&test.object);
    }

    //valid object, valid protected handler, null access token
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_BUFFER access_token;
        char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, access_token_data, sizeof(access_token_data), ALGUI_FALSE);
        ALGUI_ENSURE_ERROR(algui_set_object_message_handler(&test.object, MSG_TEST, test_handler, &access_token, NULL) == ALGUI_TRUE, 0);

        ALGUI_ENSURE_ERROR(algui_get_object_message_handler(&test.object, MSG_TEST, NULL) == NULL, EINVAL);
        ALGUI_ENSURE_ERROR(algui_set_object_message_handler(&test.object, MSG_TEST, test_handler1, NULL, NULL) == ALGUI_FALSE, EINVAL);

        algui_cleanup_object(&test.object);
    }

    //valid object, valid protected handler, invalid access token
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_BUFFER access_token;
        char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, access_token_data, sizeof(access_token_data), ALGUI_FALSE);
        ALGUI_ENSURE_ERROR(algui_set_object_message_handler(&test.object, MSG_TEST, test_handler, &access_token, NULL) == ALGUI_TRUE, 0);

        ALGUI_BUFFER get_access_token;
        char get_access_token_data[] = "I found the secret password!";
        algui_init_buffer(&get_access_token, get_access_token_data, sizeof(get_access_token_data), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_get_object_message_handler(&test.object, MSG_TEST, &get_access_token) == NULL, EINVAL);
        ALGUI_ENSURE_ERROR(algui_set_object_message_handler(&test.object, MSG_TEST, test_handler1, &get_access_token, NULL) == ALGUI_FALSE, EINVAL);

        algui_cleanup_object(&test.object);
    }

    //valid object, valid protected handler, valid access token
    {
        TEST_OBJECT test;
        init_test_object(&test);

        ALGUI_BUFFER access_token;
        char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, access_token_data, sizeof(access_token_data), ALGUI_FALSE);
        ALGUI_ENSURE_ERROR(algui_set_object_message_handler(&test.object, MSG_TEST, test_handler, &access_token, NULL) == ALGUI_TRUE, 0);

        ALGUI_BUFFER get_access_token;
        char get_access_token_data[] = "secret_password";
        algui_init_buffer(&get_access_token, get_access_token_data, sizeof(get_access_token_data), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_get_object_message_handler(&test.object, MSG_TEST, &get_access_token) == test_handler, 0);

        ALGUI_ENSURE_ERROR(algui_set_object_message_handler(&test.object, MSG_TEST, test_handler1, &get_access_token, NULL) == ALGUI_TRUE, 0);
        ALGUI_ENSURE_ERROR(algui_get_object_message_handler(&test.object, MSG_TEST, &get_access_token) == test_handler1, 0);

        algui_cleanup_object(&test.object);
    }

    //valid object, prev handler
    {
        TEST_OBJECT test;
        init_test_object(&test);
        
        ALGUI_ENSURE_ERROR(algui_set_object_message_handler(&test.object, MSG_TEST, test_handler, NULL, NULL) == ALGUI_TRUE, 0);        
        ALGUI_ENSURE(algui_get_object_message_handler(&test.object, MSG_TEST, NULL) == test_handler);

        ALGUI_OBJECT_MESSAGE_HANDLER prev_handler;
        ALGUI_ENSURE_ERROR(algui_set_object_message_handler(&test.object, MSG_TEST, test_handler1, NULL, &prev_handler) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(algui_get_object_message_handler(&test.object, MSG_TEST, NULL) == test_handler1);
        ALGUI_ENSURE(prev_handler == test_handler);

        algui_cleanup_object(&test.object);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_do_object_message(void* context) {
    //null object
    {
        TEST_DATA td = { 5 };
        ALGUI_ENSURE_ERROR(algui_do_object_message(NULL, MSG_TEST, &td, NULL) == 0, EINVAL);
    }

    //valid object, invalid message handler
    {
        TEST_OBJECT test;
        init_test_object(&test);

        TEST_DATA td = { 5 };
        ALGUI_ENSURE_ERROR(algui_do_object_message(&test.object, MSG_TEST, &td, NULL) == 0, EINVAL);

        algui_cleanup_object(&test.object);
    }

    //valid object, valid message handler
    {
        TEST_OBJECT test;
        init_test_object(&test);
        algui_set_object_message_handler(&test.object, MSG_TEST, test_handler, NULL, NULL);

        TEST_DATA td = { 5 };
        ALGUI_ENSURE_ERROR(algui_do_object_message(&test.object, MSG_TEST, &td, NULL) == 1, 0);
        ALGUI_ENSURE(test.test_value == td.value);

        algui_cleanup_object(&test.object);
    }

    //valid object, valid protected message handler, null access token
    {
        TEST_OBJECT test;
        init_test_object(&test);
        ALGUI_BUFFER access_token;
        char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, access_token_data, sizeof(access_token_data), ALGUI_FALSE);
        algui_set_object_message_handler(&test.object, MSG_TEST, test_handler, &access_token, NULL);

        TEST_DATA td = { 5 };
        ALGUI_ENSURE_ERROR(algui_do_object_message(&test.object, MSG_TEST, &td, NULL) == 0, EINVAL);

        algui_cleanup_object(&test.object);
    }

    //valid object, valid protected message handler, invalid access token
    {
        TEST_OBJECT test;
        init_test_object(&test);
        ALGUI_BUFFER access_token;
        char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, access_token_data, sizeof(access_token_data), ALGUI_FALSE);
        algui_set_object_message_handler(&test.object, MSG_TEST, test_handler, &access_token, NULL);

        TEST_DATA td = { 5 };
        ALGUI_BUFFER get_access_token;
        char get_access_token_data[] = "I found the secret password!";
        algui_init_buffer(&get_access_token, get_access_token_data, sizeof(get_access_token_data), ALGUI_FALSE);
        ALGUI_ENSURE_ERROR(algui_do_object_message(&test.object, MSG_TEST, &td, &get_access_token) == 0, EINVAL);

        algui_cleanup_object(&test.object);
    }

    //valid object, valid protected message handler, valid access token
    {
        TEST_OBJECT test;
        init_test_object(&test);
        ALGUI_BUFFER access_token;
        char access_token_data[] = "secret_password";
        algui_init_buffer(&access_token, access_token_data, sizeof(access_token_data), ALGUI_FALSE);
        algui_set_object_message_handler(&test.object, MSG_TEST, test_handler, &access_token, NULL);

        TEST_DATA td = { 5 };
        ALGUI_BUFFER get_access_token;
        char get_access_token_data[] = "secret_password";
        algui_init_buffer(&get_access_token, get_access_token_data, sizeof(get_access_token_data), ALGUI_FALSE);
        ALGUI_ENSURE_ERROR(algui_do_object_message(&test.object, MSG_TEST, &td, &get_access_token) == 1, 0);
        ALGUI_ENSURE(test.test_value == td.value);

        algui_cleanup_object(&test.object);
    }

    //do unknown message
    {
        TEST_OBJECT test;
        init_test_object(&test);
        algui_set_object_message_handler(&test.object, ALGUI_MSG_UNKNOWN, unknown_message_handler, NULL, NULL);

        TEST_DATA td = { 5 };
        ALGUI_ENSURE_ERROR(algui_do_object_message(&test.object, MSG_TEST, &td, NULL) == 1, 0);
    }

    return ALGUI_TRUE;
}


#define TYPED_PROPERTY_TEST(TYPE)\
{\
    ALGUI_OBJECT obj;\
    algui_init_object(&obj);\
    ALGUI_ENSURE_ERROR(algui_get_##TYPE##_object_property(&obj, ALGUI_PROP_ID, 10, NULL) == 10, EINVAL);\
    ALGUI_PROPERTY_DEFINITION pd;\
    memset(&pd, 0, sizeof(pd));\
    pd.type = ALGUI_PROPERTY_TYPE_VALUE;\
    pd.value.size = sizeof(TYPE);\
    ALGUI_ENSURE_ERROR(algui_define_##TYPE##_object_property(&obj, ALGUI_PROP_ID, &pd, 5, NULL) == ALGUI_TRUE, 0);\
    ALGUI_ENSURE_ERROR(algui_get_##TYPE##_object_property(&obj, ALGUI_PROP_ID, 10, NULL) == 5, 0);\
    ALGUI_ENSURE_ERROR(algui_set_##TYPE##_object_property(&obj, ALGUI_PROP_ID, 15, NULL) == ALGUI_TRUE, 0);\
    ALGUI_ENSURE_ERROR(algui_get_##TYPE##_object_property(&obj, ALGUI_PROP_ID, 10, NULL) == 15, 0);\
    algui_delete_object_property(&obj, ALGUI_PROP_ID, NULL);\
    ALGUI_ENSURE_ERROR(algui_define_##TYPE##_object_property(&obj, ALGUI_PROP_ID, &pd, 5, "secret_password") == ALGUI_TRUE, 0);\
    ALGUI_ENSURE_ERROR(algui_get_##TYPE##_object_property(&obj, ALGUI_PROP_ID, 10, NULL) == 10, EINVAL);\
    ALGUI_ENSURE_ERROR(algui_get_##TYPE##_object_property(&obj, ALGUI_PROP_ID, 10, "secret_password") == 5, 0);\
    ALGUI_ENSURE_ERROR(algui_set_##TYPE##_object_property(&obj, ALGUI_PROP_ID, 15, NULL) == ALGUI_FALSE, EINVAL);\
    ALGUI_ENSURE_ERROR(algui_set_##TYPE##_object_property(&obj, ALGUI_PROP_ID, 15, "secret_password") == ALGUI_TRUE, 0);\
    ALGUI_ENSURE_ERROR(algui_get_##TYPE##_object_property(&obj, ALGUI_PROP_ID, 10, "secret_password") == 15, 0);\
    algui_cleanup_object(&obj);\
}


static ALGUI_BOOL test_string_property(ALGUI_OBJECT* object, int id, const char* test_value, const char* access_token) {
    char* value = algui_get_string_object_property(object, id, NULL, access_token);
    const ALGUI_BOOL result = value != NULL && strcmp(value, test_value) == 0;
    free(value);
    return result;
}


static ALGUI_BOOL test_typed_object_property_functions(void* context) {
    TYPED_PROPERTY_TEST(char);
    TYPED_PROPERTY_TEST(short);
    TYPED_PROPERTY_TEST(int);
    TYPED_PROPERTY_TEST(long);
    TYPED_PROPERTY_TEST(long_long);
    TYPED_PROPERTY_TEST(unsigned_char);
    TYPED_PROPERTY_TEST(unsigned_short);
    TYPED_PROPERTY_TEST(unsigned_int);
    TYPED_PROPERTY_TEST(unsigned_long);
    TYPED_PROPERTY_TEST(unsigned_long_long);
    TYPED_PROPERTY_TEST(float);
    TYPED_PROPERTY_TEST(double);
    TYPED_PROPERTY_TEST(long_double);
    TYPED_PROPERTY_TEST(bool);
    TYPED_PROPERTY_TEST(int8_t);
    TYPED_PROPERTY_TEST(int16_t);
    TYPED_PROPERTY_TEST(int32_t);
    TYPED_PROPERTY_TEST(int64_t);
    TYPED_PROPERTY_TEST(uint8_t);
    TYPED_PROPERTY_TEST(uint16_t);
    TYPED_PROPERTY_TEST(uint32_t);
    TYPED_PROPERTY_TEST(uint64_t);

    //string
    {
        ALGUI_OBJECT obj;
        algui_init_object(&obj);

        //get default value if the property does not exist
        ALGUI_ENSURE_ERROR(algui_get_string_object_property(&obj, ALGUI_PROP_ID, NULL, NULL) == NULL, EINVAL);

        //define property
        ALGUI_PROPERTY_DEFINITION pd;
        memset(&pd, 0, sizeof(pd));
        pd.type = ALGUI_PROPERTY_TYPE_VALUE;
        ALGUI_ENSURE_ERROR(algui_define_string_object_property(&obj, ALGUI_PROP_ID, &pd, "test", NULL) == ALGUI_TRUE, 0);

        //get initial value
        ALGUI_ENSURE_ERROR(test_string_property(&obj, ALGUI_PROP_ID, "test", NULL) == ALGUI_TRUE, 0);

        //set new value
        ALGUI_ENSURE_ERROR(algui_set_string_object_property(&obj, ALGUI_PROP_ID, "new_test", NULL) == ALGUI_TRUE, 0);
        ALGUI_ENSURE_ERROR(test_string_property(&obj, ALGUI_PROP_ID, "new_test", NULL) == ALGUI_TRUE, 0);

        //reinstall the property, this time with an access token
        algui_delete_object_property(&obj, ALGUI_PROP_ID, NULL);
        ALGUI_ENSURE_ERROR(algui_define_string_object_property(&obj, ALGUI_PROP_ID, &pd, "5", "secret_password") == ALGUI_TRUE, 0);

        //get initial value
        ALGUI_ENSURE_ERROR(algui_get_string_object_property(&obj, ALGUI_PROP_ID, NULL, NULL) == NULL, EINVAL);
        ALGUI_ENSURE_ERROR(test_string_property(&obj, ALGUI_PROP_ID, "5", "secret_password") == ALGUI_TRUE, 0);

        //set new value
        ALGUI_ENSURE_ERROR(algui_set_string_object_property(&obj, ALGUI_PROP_ID, "15", NULL) == ALGUI_FALSE, EINVAL);
        ALGUI_ENSURE_ERROR(algui_set_string_object_property(&obj, ALGUI_PROP_ID, "15", "secret_password") == ALGUI_TRUE, 0);
        ALGUI_ENSURE_ERROR(test_string_property(&obj, ALGUI_PROP_ID, "15", "secret_password") == ALGUI_TRUE, 0);

        algui_cleanup_object(&obj);
    }

    return ALGUI_TRUE;
}


void test_object(ALGUI_TEST_STATISTICS* stats) {
    algui_do_test(stats, "algui_init_object", test_init_object, NULL);
    algui_do_test(stats, "algui_cleanup_object", test_cleanup_object, NULL);
    algui_do_test(stats, "algui_define_object_property", test_define_object_property, NULL);
    algui_do_test(stats, "algui_delete_object_property", test_delete_object_property, NULL);
    algui_do_test(stats, "algui_set_object_property", test_set_object_property, NULL);
    algui_do_test(stats, "algui_get_object_message_handler", test_get_object_message_handler, NULL);
    algui_do_test(stats, "algui_set_object_message_handler", test_set_object_message_handler, NULL);
    algui_do_test(stats, "algui_do_object_message", test_do_object_message, NULL);
    algui_do_test(stats, "typed object property functions", test_typed_object_property_functions, NULL);
}
