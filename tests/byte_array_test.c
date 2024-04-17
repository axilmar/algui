#include <memory.h>
#include "algui/test.h"
#include "algui/byte_array.h"
#include "algui/stdlib.h"


static ALGUI_BOOL test_algui_byte_array_construct(void* context) {
    //null array
    {
        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct(NULL), EINVAL);
    }

    //non null array
    {
        ALGUI_BYTE_ARRAY ba;
        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct(&ba), 0);
        ALGUI_ENSURE(ba.data == NULL);
        ALGUI_ENSURE(ba.size == 0);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_algui_byte_array_construct_from_data(void* context) {
    //null array
    {
        char data[1];
        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct_from_data(NULL, data, sizeof(data)), EINVAL);
    }

    //null data, size > 0
    {
        ALGUI_BYTE_ARRAY ba;
        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct_from_data(&ba, NULL, 1), EINVAL);
        ALGUI_ENSURE(ba.data == NULL);
        ALGUI_ENSURE(ba.size == 0);
    }

    //non null data, size == 0
    {
        ALGUI_BYTE_ARRAY ba;
        char data[1];
        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct_from_data(&ba, data, 0), EINVAL);
        ALGUI_ENSURE(ba.data == NULL);
        ALGUI_ENSURE(ba.size == 0);
    }

    //enomem
    {
        ALGUI_BYTE_ARRAY ba;
        char data[1];
        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct_from_data(&ba, data, SIZE_MAX), ENOMEM);
        ALGUI_ENSURE(ba.data == NULL);
        ALGUI_ENSURE(ba.size == 0);
    }

    //zero size
    {
        ALGUI_BYTE_ARRAY ba;
        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct_from_data(&ba, NULL, 0), 0);
        ALGUI_ENSURE(ba.data == NULL);
        ALGUI_ENSURE(ba.size == 0);
    }

    //non-zero size
    {
        ALGUI_BYTE_ARRAY ba;
        char data[] = "the quick brown fox jumps over the lazy dog";
        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct_from_data(&ba, data, sizeof(data)), 0);
        ALGUI_ENSURE(ba.data != NULL);
        ALGUI_ENSURE(ba.data != data);
        ALGUI_ENSURE(ba.size == sizeof(data));
        ALGUI_ENSURE(memcmp(ba.data, data, sizeof(data)) == 0);
        free(ba.data);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_algui_byte_array_construct_from_size(void* context) {
    //null array
    {
        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct_from_size(NULL, 1), EINVAL);
    }

    //empty
    {
        ALGUI_BYTE_ARRAY ba;
        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct_from_size(&ba, 0), 0);
        ALGUI_ENSURE(ba.data == NULL);
        ALGUI_ENSURE(ba.size == 0);
    }

    //enomem
    {
        ALGUI_BYTE_ARRAY ba;
        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct_from_size(&ba, SIZE_MAX), ENOMEM);
        ALGUI_ENSURE(ba.data == NULL);
        ALGUI_ENSURE(ba.size == 0);
    }

    //zero size
    {
        ALGUI_BYTE_ARRAY ba;
        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct_from_size(&ba, 0), 0);
        ALGUI_ENSURE(ba.data == NULL);
        ALGUI_ENSURE(ba.size == 0);
    }

    //non-zero size
    {
        ALGUI_BYTE_ARRAY ba;
        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct_from_size(&ba, 32), 0);
        ALGUI_ENSURE(ba.data != NULL);
        ALGUI_ENSURE(ba.size == 32);
        free(ba.data);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_algui_byte_array_construct_copy(void* context) {
    //null destination array
    {
        ALGUI_BYTE_ARRAY src;
        algui_byte_array_construct(&src);
        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct_copy(NULL, &src), EINVAL);
    }

    //null source array
    {
        ALGUI_BYTE_ARRAY dst;
        algui_byte_array_construct(&dst);
        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct_copy(&dst, NULL), EINVAL);
    }

    //source array same as destination
    {
        ALGUI_BYTE_ARRAY dst;
        algui_byte_array_construct(&dst);
        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct_copy(&dst, &dst), EINVAL);
    }

    //empty source array
    {
        ALGUI_BYTE_ARRAY dst, src;
        algui_byte_array_construct(&src);
        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct_copy(&dst, &src), 0);
        ALGUI_ENSURE(dst.data == NULL);
        ALGUI_ENSURE(dst.size == 0);
    }

    //non empty source array
    {
        ALGUI_BYTE_ARRAY dst, src;

        char data[] = "the quick brown fox jumps over the lazy dog";
        algui_byte_array_construct_from_data(&src, data, sizeof(data));

        ALGUI_ENSURE_STATEMENT(algui_byte_array_construct_copy(&dst, &src), 0);
        ALGUI_ENSURE(dst.data != src.data);
        ALGUI_ENSURE(dst.size == src.size);
        ALGUI_ENSURE(memcmp(dst.data, src.data, sizeof(data)) == 0);

        free(src.data);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_algui_byte_array_copy(void* context) {
    //null destination array
    {
        ALGUI_BYTE_ARRAY src;
        algui_byte_array_construct(&src);
        ALGUI_ENSURE_STATEMENT(algui_byte_array_copy(NULL, &src), EINVAL);
    }

    //null source array
    {
        ALGUI_BYTE_ARRAY dst;
        algui_byte_array_construct(&dst);
        ALGUI_ENSURE_STATEMENT(algui_byte_array_copy(&dst, NULL), EINVAL);
    }

    //source array same as destination
    {
        ALGUI_BYTE_ARRAY dst;
        algui_byte_array_construct(&dst);
        ALGUI_ENSURE_STATEMENT(algui_byte_array_copy(&dst, &dst), 0);
        ALGUI_ENSURE(dst.data == NULL);
        ALGUI_ENSURE(dst.size == 0);
    }

    //empty source array
    {
        ALGUI_BYTE_ARRAY dst, src;
        algui_byte_array_construct(&src);
        algui_byte_array_construct(&dst);
        ALGUI_ENSURE_STATEMENT(algui_byte_array_copy(&dst, &src), 0);
        ALGUI_ENSURE(dst.data == NULL);
        ALGUI_ENSURE(dst.size == 0);
    }

    //non empty source array to empty source array
    {
        ALGUI_BYTE_ARRAY dst, src;

        char data[] = "the quick brown fox jumps over the lazy dog";
        algui_byte_array_construct_from_data(&src, data, sizeof(data));

        algui_byte_array_construct(&dst);

        ALGUI_ENSURE_STATEMENT(algui_byte_array_copy(&dst, &src), 0);
        ALGUI_ENSURE(dst.data != src.data);
        ALGUI_ENSURE(dst.size == src.size);
        ALGUI_ENSURE(memcmp(dst.data, src.data, sizeof(data)) == 0);

        free(src.data);
        free(dst.data);
    }

    //non empty source array to non-empty source array
    {
        ALGUI_BYTE_ARRAY dst, src;

        char data[] = "the quick brown fox jumps over the lazy dog";
        algui_byte_array_construct_from_data(&src, data, sizeof(data));

        char dst_data[] = "destination data";
        algui_byte_array_construct_from_data(&dst, dst_data, sizeof(dst_data));

        ALGUI_ENSURE_STATEMENT(algui_byte_array_copy(&dst, &src), 0);
        ALGUI_ENSURE(dst.data != src.data);
        ALGUI_ENSURE(dst.size == src.size);
        ALGUI_ENSURE(memcmp(dst.data, src.data, sizeof(data)) == 0);

        free(src.data);
        free(dst.data);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_algui_byte_array_destruct(void* context) {
    //null array
    {
        ALGUI_ENSURE_STATEMENT(algui_byte_array_destruct(NULL), EINVAL);
    }

    //empty array
    {
        ALGUI_BYTE_ARRAY ba;
        algui_byte_array_construct(&ba);
        ALGUI_ENSURE_STATEMENT(algui_byte_array_destruct(&ba), 0);
        ALGUI_ENSURE(algui_free_ptr == ba.data);
    }

    return ALGUI_TRUE;
}


void byte_array_test(ALGUI_TEST* test) {
    algui_test_run(test, "algui_byte_array_construct", test_algui_byte_array_construct, NULL);
    algui_test_run(test, "algui_byte_array_construct_from_data", test_algui_byte_array_construct_from_data, NULL);
    algui_test_run(test, "algui_byte_array_construct_copy", test_algui_byte_array_construct_copy, NULL);
    algui_test_run(test, "algui_byte_array_copy", test_algui_byte_array_copy, NULL);
    algui_test_run(test, "algui_byte_array_destruct", test_algui_byte_array_destruct, NULL);
}
