/* Copyright (c) 2014 Vyacheslav Slinko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "../object_pool.h"
#include "../test.h"

static struct vstd_object_pool *pool;

static void setup() {
    pool = vstd_object_pool_alloc(1, sizeof(int));
    tassert(pool);
}

static void teardown() {
    vstd_object_pool_free(pool);
}

static void test_vstd_object_pool_alloc() {
    tassert(pool->allocations_size == 1);
    tassert((*pool->allocations)->size == 1);
    tassert(pool->size == 1);
    tassert(pool->item_size == sizeof(int));
    tassert(pool->row_size == sizeof(int) + sizeof(char));
}
VSTD_TEST_REGISTER_UNIT(test_vstd_object_pool_alloc, 10000, setup, teardown)

static void test_vstd_object_pool_get() {
    int *i;

    i = vstd_object_pool_get(pool);
    tassert(i);

    tassert(pool->size == 2);

    vstd_object_pool_return(pool, (void **) &i);
}
VSTD_TEST_REGISTER_UNIT(test_vstd_object_pool_get, 10000, setup, teardown)

static void test_vstd_object_pool_return() {
    int *i;

    i = vstd_object_pool_get(pool);
    tassert(i);
    *i = 1;

    vstd_object_pool_return(pool, (void **) &i);
    tassert(i == NULL);
}
VSTD_TEST_REGISTER_UNIT(test_vstd_object_pool_return, 10000, setup, teardown)
