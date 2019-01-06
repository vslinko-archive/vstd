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

#include <vstd/object_pool.h>
#include <vstd/test.h>

struct vstd_object_pool* pool;

void reset_int(int* l);
void reset_int(int* l) {
  *l = 0;
}

static void vstd_setup() {
  pool = vstd_object_pool_alloc(1, sizeof(int), (vstd_object_pool_reset_fn*) &reset_int);
}

static void vstd_teardown() {
  vstd_object_pool_free(pool);
}

vstd_test_unit(vstd_object_pool_alloc, 10000, {
    assert(pool->allocations_size == 1);
    assert((*pool->allocations)->size == 1);
    assert(pool->size == 1);
    assert(pool->item_size == sizeof(int));
    assert(pool->row_size == sizeof(int) + sizeof(char));
    assert(pool->reset_fn == (vstd_object_pool_reset_fn*) &reset_int);
})

vstd_test_unit(vstd_object_pool_get, 10000, {
    int *i = vstd_object_pool_get(pool);

    assert(pool->size == 2);
    assert(*i == 0);

    vstd_object_pool_return(pool, i);
})

vstd_test_unit(vstd_object_pool_return, 10000, {
    int *i = vstd_object_pool_get(pool);
    *i = 1;

    vstd_object_pool_return(pool, i);

    assert(*i == 0);
})
