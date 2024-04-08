#include <string.h>
#include <memory.h>
#include "algui/test.h"
#include "algui/object.h"
#include "util.h"


typedef struct TEST_OBJECT {
    ALGUI_OBJECT object;
    int test_value;
} TEST_OBJECT;


static void init_test_object(TEST_OBJECT* obj) {
    algui_init_object(&obj->object);
    obj->test_value = 0;
}


static ALGUI_RESULT test_cleanup_handler(ALGUI_OBJECT* obj, void* data) {
    ++((TEST_OBJECT*)obj)->test_value;
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
        ALGUI_ENSURE_ERROR(algui_cleanup_object(&object) == ALGUI_TRUE, 0);
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


void test_object(ALGUI_TEST_STATISTICS* stats) {
    algui_do_test(stats, "algui_init_object", test_init_object, NULL);
    algui_do_test(stats, "algui_cleanup_object", test_cleanup_object, NULL);
    algui_do_test(stats, "algui_define_object_property", test_define_object_property, NULL);
    algui_do_test(stats, "algui_get_object_property", test_get_object_property, NULL);
}
