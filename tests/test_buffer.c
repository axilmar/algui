#include "algui/test.h"
#include "algui/buffer.h"
#include "util.h"


static ALGUI_BOOL test_init_buffer(void* context) {
    //null buffer
    {
        ALGUI_ENSURE_ERROR(algui_init_buffer(NULL, NULL, 0, ALGUI_TRUE) == ALGUI_FALSE, EINVAL);
    }

    //empty buffer
    {
        ALGUI_BUFFER buffer;
        algui_init_buffer(&buffer, NULL, 0, ALGUI_TRUE);
        ALGUI_ENSURE(buffer.data == NULL);
        ALGUI_ENSURE(buffer.owner == 1);
        ALGUI_ENSURE(buffer.size == 0);
    }

    //external data buffer
    {
        ALGUI_BUFFER buffer;
        char data[4];
        algui_init_buffer(&buffer, data, sizeof(data), ALGUI_FALSE);
        ALGUI_ENSURE(buffer.data == data);
        ALGUI_ENSURE(buffer.owner == 0);
        ALGUI_ENSURE(buffer.size == sizeof(data));
    }

    //owned data buffer
    {
        ALGUI_BUFFER buffer;
        algui_init_buffer(&buffer, NULL, 4, ALGUI_TRUE);
        ALGUI_ENSURE(buffer.data != NULL);
        ALGUI_ENSURE(buffer.owner == 1);
        ALGUI_ENSURE(buffer.size == 4);
        algui_cleanup_buffer(&buffer);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_cleanup_buffer(void* context) {
    //null buffer
    {
        ALGUI_ENSURE_ERROR(algui_cleanup_buffer(NULL) == ALGUI_FALSE, EINVAL);
    }

    //empty buffer
    {
        ALGUI_BUFFER buffer;
        algui_init_buffer(&buffer, NULL, 0, ALGUI_TRUE);
        algui_cleanup_buffer(&buffer);
        ALGUI_ENSURE(buffer.data == NULL);
        ALGUI_ENSURE(buffer.owner == 1);
        ALGUI_ENSURE(buffer.size == 0);
    }

    //external data buffer
    {
        ALGUI_BUFFER buffer;
        char data[4];
        algui_init_buffer(&buffer, data, sizeof(data), ALGUI_FALSE);
        ALGUI_ENSURE(buffer.data == data);
        ALGUI_ENSURE(buffer.owner == 0);
        ALGUI_ENSURE(buffer.size == sizeof(data));
        algui_cleanup_buffer(&buffer);
        ALGUI_ENSURE(buffer.data == NULL);
        ALGUI_ENSURE(buffer.owner == 1);
        ALGUI_ENSURE(buffer.size == 0);
    }

    //owned data buffer
    {
        ALGUI_BUFFER buffer;
        algui_init_buffer(&buffer, NULL, 4, ALGUI_TRUE);
        ALGUI_ENSURE(buffer.data != NULL);
        ALGUI_ENSURE(buffer.owner == 1);
        ALGUI_ENSURE(buffer.size == 4);
        algui_cleanup_buffer(&buffer);
        ALGUI_ENSURE(buffer.data == NULL);
        ALGUI_ENSURE(buffer.owner == 1);
        ALGUI_ENSURE(buffer.size == 0);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_is_empty_buffer(void* context) {
    //null buffer
    {
        ALGUI_ENSURE_ERROR(algui_is_empty_buffer(NULL) == ALGUI_FALSE, EINVAL);
    }

    //empty buffer
    {
        ALGUI_BUFFER buffer;
        algui_init_buffer(&buffer, NULL, 0, ALGUI_TRUE);
        ALGUI_ENSURE(algui_is_empty_buffer(&buffer) == ALGUI_TRUE);
    }

    //external data buffer
    {
        ALGUI_BUFFER buffer;
        char data[4];
        algui_init_buffer(&buffer, data, sizeof(data), ALGUI_FALSE);
        ALGUI_ENSURE(algui_is_empty_buffer(&buffer) == ALGUI_FALSE);
        algui_cleanup_buffer(&buffer);
        ALGUI_ENSURE(algui_is_empty_buffer(&buffer) == ALGUI_TRUE);
    }

    //owned data buffer
    {
        ALGUI_BUFFER buffer;
        algui_init_buffer(&buffer, NULL, 4, ALGUI_TRUE);
        ALGUI_ENSURE(algui_is_empty_buffer(&buffer) == ALGUI_FALSE);
        algui_cleanup_buffer(&buffer);
        ALGUI_ENSURE(algui_is_empty_buffer(&buffer) == ALGUI_TRUE);
        
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_copy_buffer(void* context) {
    //null dst
    {
        ALGUI_BUFFER src;
        ALGUI_ENSURE_ERROR(algui_copy_buffer(NULL, &src) == ALGUI_FALSE, EINVAL);
    }

    //null src
    {
        ALGUI_BUFFER dst;
        ALGUI_ENSURE_ERROR(algui_copy_buffer(&dst, NULL) == ALGUI_FALSE, EINVAL);
    }

    //empty to empty
    {
        ALGUI_BUFFER src;
        algui_init_buffer(&src, NULL, 0, ALGUI_TRUE);

        ALGUI_BUFFER dst;
        algui_init_buffer(&dst, NULL, 0, ALGUI_TRUE);

        ALGUI_ENSURE_ERROR(algui_copy_buffer(&dst, &src) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(dst.data == NULL);
        ALGUI_ENSURE(dst.owner == 1);
        ALGUI_ENSURE(dst.size == 0);

        algui_cleanup_buffer(&src);
        algui_cleanup_buffer(&dst);
    }

    //empty to external
    {
        ALGUI_BUFFER src;
        algui_init_buffer(&src, NULL, 0, ALGUI_TRUE);

        ALGUI_BUFFER dst;
        char dst_data[4];
        algui_init_buffer(&dst, dst_data, sizeof(dst_data), ALGUI_FALSE);
        const ALGUI_BUFFER org_dst = dst;

        ALGUI_ENSURE_ERROR(algui_copy_buffer(&dst, &src) == ALGUI_FALSE, EINVAL);
        ALGUI_ENSURE(dst.data == org_dst.data);
        ALGUI_ENSURE(dst.owner == org_dst.owner);
        ALGUI_ENSURE(dst.size == org_dst.size);

        algui_cleanup_buffer(&src);
        algui_cleanup_buffer(&dst);
    }

    //empty to owned
    {
        ALGUI_BUFFER src;
        algui_init_buffer(&src, NULL, 0, ALGUI_TRUE);

        ALGUI_BUFFER dst;
        algui_init_buffer(&dst, NULL, 4, ALGUI_TRUE);

        ALGUI_ENSURE_ERROR(algui_copy_buffer(&dst, &src) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(dst.data == NULL);
        ALGUI_ENSURE(dst.owner == 1);
        ALGUI_ENSURE(dst.size == 0);

        algui_cleanup_buffer(&src);
        algui_cleanup_buffer(&dst);
    }

    //external to empty
    {
        ALGUI_BUFFER src;
        char src_data[4];
        algui_init_buffer(&src, src_data, sizeof(src_data), ALGUI_FALSE);

        ALGUI_BUFFER dst;
        algui_init_buffer(&dst, NULL, 0, ALGUI_TRUE);

        ALGUI_ENSURE_ERROR(algui_copy_buffer(&dst, &src) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(dst.data != NULL);
        ALGUI_ENSURE(dst.data != src.data);
        ALGUI_ENSURE(dst.owner == 1);
        ALGUI_ENSURE(dst.size == src.size);

        algui_cleanup_buffer(&src);
        algui_cleanup_buffer(&dst);
    }

    //external to external, different size
    {
        ALGUI_BUFFER src;
        char src_data[4];
        algui_init_buffer(&src, src_data, sizeof(src_data), ALGUI_FALSE);

        ALGUI_BUFFER dst;
        char dst_data[8];
        algui_init_buffer(&dst, dst_data, sizeof(dst_data), ALGUI_FALSE);
        const ALGUI_BUFFER org_dst = dst;

        ALGUI_ENSURE_ERROR(algui_copy_buffer(&dst, &src) == ALGUI_FALSE, EINVAL);
        ALGUI_ENSURE(dst.data == org_dst.data);
        ALGUI_ENSURE(dst.owner == org_dst.owner);
        ALGUI_ENSURE(dst.size == org_dst.size);

        algui_cleanup_buffer(&src);
        algui_cleanup_buffer(&dst);
    }

    //external to external, same size
    {
        ALGUI_BUFFER src;
        char src_data[4] = { 1, 2, 3, 4 };
        algui_init_buffer(&src, src_data, sizeof(src_data), ALGUI_FALSE);

        ALGUI_BUFFER dst;
        char dst_data[4];
        algui_init_buffer(&dst, dst_data, sizeof(dst_data), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_copy_buffer(&dst, &src) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(dst.data != NULL);
        ALGUI_ENSURE(dst.data != src.data);
        ALGUI_ENSURE(dst.owner == 0);
        ALGUI_ENSURE(dst.size == src.size);
        for (size_t i = 0; i < sizeof(src_data); ++i) {
            ALGUI_ENSURE(dst.data[i] == src_data[i]);
        }

        algui_cleanup_buffer(&src);
        algui_cleanup_buffer(&dst);
    }

    //external to owned
    {
        ALGUI_BUFFER src;
        char src_data[4] = { 1, 2, 3, 4 };
        algui_init_buffer(&src, src_data, sizeof(src_data), ALGUI_FALSE);

        ALGUI_BUFFER dst;
        algui_init_buffer(&dst, NULL, 8, ALGUI_TRUE);

        ALGUI_ENSURE_ERROR(algui_copy_buffer(&dst, &src) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(dst.data != NULL);
        ALGUI_ENSURE(dst.data != src.data);
        ALGUI_ENSURE(dst.owner == 1);
        ALGUI_ENSURE(dst.size == src.size);
        for (size_t i = 0; i < sizeof(src_data); ++i) {
            ALGUI_ENSURE(dst.data[i] == src_data[i]);
        }

        algui_cleanup_buffer(&src);
        algui_cleanup_buffer(&dst);
    }

    //owned to empty
    {
        ALGUI_BUFFER src;
        algui_init_buffer(&src, NULL, 4, ALGUI_TRUE);
        for (size_t i = 0; i < src.size; ++i) {
            src.data[i] = (char)(i + 1);
        }

        ALGUI_BUFFER dst;
        algui_init_buffer(&dst, NULL, 0, ALGUI_TRUE);

        ALGUI_ENSURE_ERROR(algui_copy_buffer(&dst, &src) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(dst.data != NULL);
        ALGUI_ENSURE(dst.data != src.data);
        ALGUI_ENSURE(dst.owner == 1);
        ALGUI_ENSURE(dst.size == src.size);

        algui_cleanup_buffer(&src);
        algui_cleanup_buffer(&dst);
    }

    //owned to external, different size
    {
        ALGUI_BUFFER src;
        algui_init_buffer(&src, NULL, 4, ALGUI_TRUE);
        for (size_t i = 0; i < src.size; ++i) {
            src.data[i] = (char)(i + 1);
        }

        ALGUI_BUFFER dst;
        char dst_data[8];
        algui_init_buffer(&dst, dst_data, sizeof(dst_data), ALGUI_FALSE);
        const ALGUI_BUFFER org_dst = dst;

        ALGUI_ENSURE_ERROR(algui_copy_buffer(&dst, &src) == ALGUI_FALSE, EINVAL);
        ALGUI_ENSURE(dst.data == org_dst.data);
        ALGUI_ENSURE(dst.owner == org_dst.owner);
        ALGUI_ENSURE(dst.size == org_dst.size);

        algui_cleanup_buffer(&src);
        algui_cleanup_buffer(&dst);
    }

    //owned to external, same size
    {
        ALGUI_BUFFER src;
        algui_init_buffer(&src, NULL, 4, ALGUI_TRUE);
        for (size_t i = 0; i < src.size; ++i) {
            src.data[i] = (char)(i + 1);
        }

        ALGUI_BUFFER dst;
        char dst_data[4];
        algui_init_buffer(&dst, dst_data, sizeof(dst_data), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_copy_buffer(&dst, &src) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(dst.data != NULL);
        ALGUI_ENSURE(dst.data != src.data);
        ALGUI_ENSURE(dst.owner == 0);
        ALGUI_ENSURE(dst.size == src.size);

        algui_cleanup_buffer(&src);
        algui_cleanup_buffer(&dst);
    }

    //owned to owned
    {
        ALGUI_BUFFER src;
        algui_init_buffer(&src, NULL, 4, ALGUI_TRUE);
        for (size_t i = 0; i < src.size; ++i) {
            src.data[i] = (char)(i + 1);
        }

        ALGUI_BUFFER dst;
        algui_init_buffer(&dst, NULL, 8, ALGUI_TRUE);

        ALGUI_ENSURE_ERROR(algui_copy_buffer(&dst, &src) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(dst.data != NULL);
        ALGUI_ENSURE(dst.data != src.data);
        ALGUI_ENSURE(dst.owner == 1);
        ALGUI_ENSURE(dst.size == src.size);

        algui_cleanup_buffer(&src);
        algui_cleanup_buffer(&dst);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_set_buffer_size(void* context) {
    //resize external buffer
    {
        ALGUI_BUFFER buffer;
        char data[4];
        algui_init_buffer(&buffer, data, sizeof(data), ALGUI_FALSE);
        ALGUI_ENSURE_ERROR(algui_set_buffer_size(&buffer, 10) == ALGUI_FALSE, EINVAL);
        ALGUI_ENSURE(buffer.data == data);
        ALGUI_ENSURE(buffer.size == sizeof(data));
    }

    //resize empty owned buffer
    {
        ALGUI_BUFFER buffer;
        algui_init_buffer(&buffer, NULL, 0, ALGUI_TRUE);
        ALGUI_ENSURE_ERROR(algui_set_buffer_size(&buffer, 10) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(buffer.size == 10);
        algui_cleanup_buffer(&buffer);
    }

    //increase size
    {
        ALGUI_BUFFER buffer;
        algui_init_buffer(&buffer, NULL, 5, ALGUI_TRUE);
        ALGUI_ENSURE_ERROR(algui_set_buffer_size(&buffer, 10) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(buffer.size == 10);
        algui_cleanup_buffer(&buffer);
    }

    //increase size to invalid big size
    {
        ALGUI_BUFFER buffer;
        algui_init_buffer(&buffer, NULL, 5, ALGUI_TRUE);
        ALGUI_ENSURE_ERROR(algui_set_buffer_size(&buffer, UINT_MAX) == ALGUI_FALSE, ENOMEM);
        ALGUI_ENSURE(buffer.size == 5);
        algui_cleanup_buffer(&buffer);
    }

    //decrease size
    {
        ALGUI_BUFFER buffer;
        algui_init_buffer(&buffer, NULL, 15, ALGUI_TRUE);
        ALGUI_ENSURE_ERROR(algui_set_buffer_size(&buffer, 10) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(buffer.size == 10);
        algui_cleanup_buffer(&buffer);
    }

    //decrease size to 0
    {
        ALGUI_BUFFER buffer;
        algui_init_buffer(&buffer, NULL, 15, ALGUI_TRUE);
        ALGUI_ENSURE_ERROR(algui_set_buffer_size(&buffer, 0) == ALGUI_TRUE, 0);
        ALGUI_ENSURE(algui_is_empty_buffer(&buffer));
        algui_cleanup_buffer(&buffer);
    }

    return ALGUI_TRUE;
}


static ALGUI_BOOL test_memcmp_buffers(void* context) {
    //null a
    {
        ALGUI_BUFFER b = ALGUI_BUFFER_INIT;
        ALGUI_ENSURE_ERROR(algui_memcmp_buffers(NULL, &b) == -1, EINVAL);
    }

    //null b
    {
        ALGUI_BUFFER a = ALGUI_BUFFER_INIT;
        ALGUI_ENSURE_ERROR(algui_memcmp_buffers(&a, NULL) == 1, EINVAL);
    }

    //compare empty buffer with empty buffer
    {
        ALGUI_BUFFER a = ALGUI_BUFFER_INIT;
        ALGUI_BUFFER b = ALGUI_BUFFER_INIT;
        ALGUI_ENSURE_ERROR(algui_memcmp_buffers(&a, &b) == 0, 0);
    }

    //compare empty buffer with non empty buffer
    {
        ALGUI_BUFFER a = ALGUI_BUFFER_INIT;

        ALGUI_BUFFER b;
        algui_init_buffer(&b, NULL, 4, ALGUI_TRUE);

        ALGUI_ENSURE_ERROR(algui_memcmp_buffers(&a, &b) == -1, 0);
        algui_cleanup_buffer(&b);
    }

    //compare non empty buffer with empty buffer
    {
        ALGUI_BUFFER a;
        algui_init_buffer(&a, NULL, 4, ALGUI_TRUE);
        ALGUI_BUFFER b = ALGUI_BUFFER_INIT;
        ALGUI_ENSURE_ERROR(algui_memcmp_buffers(&a, &b) == 1, 0);
        algui_cleanup_buffer(&a);
    }

    //compare non empty buffers where a < b and sizeof(a) < sizeof(b)
    {
        ALGUI_BUFFER a;
        char data1[] = {'a', 'b', 'c'};
        algui_init_buffer(&a, data1, sizeof(data1), ALGUI_FALSE);

        ALGUI_BUFFER b;
        char data2[] = { 'a', 'c', 'd', 'e' };
        algui_init_buffer(&b, data2, sizeof(data2), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_memcmp_buffers(&a, &b) == -1, 0);
    }

    //compare non empty buffers where a < b and sizeof(a) == sizeof(b)
    {
        ALGUI_BUFFER a;
        char data1[] = { 'a', 'b', 'c' };
        algui_init_buffer(&a, data1, sizeof(data1), ALGUI_FALSE);

        ALGUI_BUFFER b;
        char data2[] = { 'a', 'c', 'd' };
        algui_init_buffer(&b, data2, sizeof(data2), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_memcmp_buffers(&a, &b) == -1, 0);
    }

    //compare non empty buffers where a < b and sizeof(a) > sizeof(b)
    {
        ALGUI_BUFFER a;
        char data1[] = { 'a', 'b', 'c', 'd'};
        algui_init_buffer(&a, data1, sizeof(data1), ALGUI_FALSE);

        ALGUI_BUFFER b;
        char data2[] = { 'a', 'c', 'd' };
        algui_init_buffer(&b, data2, sizeof(data2), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_memcmp_buffers(&a, &b) == -1, 0);
    }

    //compare non empty buffers where a == b and sizeof(a) < sizeof(b)
    {
        ALGUI_BUFFER a;
        char data1[] = { 'a', 'b', 'c' };
        algui_init_buffer(&a, data1, sizeof(data1), ALGUI_FALSE);

        ALGUI_BUFFER b;
        char data2[] = { 'a', 'b', 'c', 'd' };
        algui_init_buffer(&b, data2, sizeof(data2), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_memcmp_buffers(&a, &b) == -1, 0);
    }

    //compare non empty buffers where a == b and sizeof(a) == sizeof(b)
    {
        ALGUI_BUFFER a;
        char data1[] = { 'a', 'b', 'c', 'd' };
        algui_init_buffer(&a, data1, sizeof(data1), ALGUI_FALSE);

        ALGUI_BUFFER b;
        char data2[] = { 'a', 'b', 'c', 'd' };
        algui_init_buffer(&b, data2, sizeof(data2), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_memcmp_buffers(&a, &b) == 0, 0);
    }

    //compare non empty buffers where a == b and sizeof(a) > sizeof(b)
    {
        ALGUI_BUFFER a;
        char data1[] = { 'a', 'b', 'c', 'd', 'e'};
        algui_init_buffer(&a, data1, sizeof(data1), ALGUI_FALSE);

        ALGUI_BUFFER b;
        char data2[] = { 'a', 'b', 'c', 'd' };
        algui_init_buffer(&b, data2, sizeof(data2), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_memcmp_buffers(&a, &b) == 1, 0);
    }

    //compare non empty buffers where a > b and sizeof(a) < sizeof(b)
    {
        ALGUI_BUFFER a;
        char data1[] = { 'a', 'c', 'd' };
        algui_init_buffer(&a, data1, sizeof(data1), ALGUI_FALSE);

        ALGUI_BUFFER b;
        char data2[] = { 'a', 'b', 'c', 'd' };
        algui_init_buffer(&b, data2, sizeof(data2), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_memcmp_buffers(&a, &b) == 1, 0);
    }

    //compare non empty buffers where a > b and sizeof(a) == sizeof(b)
    {
        ALGUI_BUFFER a;
        char data1[] = { 'a', 'c', 'd', 'e'};
        algui_init_buffer(&a, data1, sizeof(data1), ALGUI_FALSE);

        ALGUI_BUFFER b;
        char data2[] = { 'a', 'b', 'c', 'd' };
        algui_init_buffer(&b, data2, sizeof(data2), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_memcmp_buffers(&a, &b) == 1, 0);
    }

    //compare non empty buffers where a > b and sizeof(a) > sizeof(b)
    {
        ALGUI_BUFFER a;
        char data1[] = { 'a', 'c', 'd', 'e', 'f' };
        algui_init_buffer(&a, data1, sizeof(data1), ALGUI_FALSE);

        ALGUI_BUFFER b;
        char data2[] = { 'a', 'b', 'c', 'd' };
        algui_init_buffer(&b, data2, sizeof(data2), ALGUI_FALSE);

        ALGUI_ENSURE_ERROR(algui_memcmp_buffers(&a, &b) == 1, 0);
    }

    return ALGUI_TRUE;
}


void run_tests_buffer(ALGUI_TEST_STATISTICS* stats) {
    algui_do_test(stats, "algui_init_buffer", test_init_buffer, NULL);
    algui_do_test(stats, "algui_cleanup_buffer", test_cleanup_buffer, NULL);
    algui_do_test(stats, "algui_is_empty_buffer", test_is_empty_buffer, NULL);
    algui_do_test(stats, "algui_copy_buffer", test_copy_buffer, NULL);
    algui_do_test(stats, "algui_set_buffer_size", test_set_buffer_size, NULL);
    algui_do_test(stats, "algui_memcmp_buffers", test_memcmp_buffers, NULL);
}
