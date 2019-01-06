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

struct object_pool* pool;

void reset_int(int* l);
void reset_int(int* l) {
  *l = 0;
}

static void vstd_setup() {
  pool = create_object_pool(1, sizeof(int), (object_pool_reset_fn*) &reset_int);
}

static void vstd_teardown() {
  free_object_pool(pool);
}

vstd_test_unit(create_object_pool, 10000, {
    assert(pool->allocations_size == 1);
    assert((*pool->allocations)->size == 1);
    assert(pool->size == 1);
    assert(pool->item_size == sizeof(int));
    assert(pool->row_size == sizeof(int) + sizeof(char));
    assert(pool->reset_fn == (object_pool_reset_fn*) &reset_int);
})

vstd_test_unit(get_object_from_pool, 10000, {
    int *i = get_object_from_pool(pool);

    assert(pool->size == 2);
    assert(*i == 0);

    return_object_to_pool(pool, i);
})

vstd_test_unit(return_object_to_pool, 10000, {
    int *i = get_object_from_pool(pool);
    *i = 1;

    return_object_to_pool(pool, i);

    assert(*i == 0);
})
