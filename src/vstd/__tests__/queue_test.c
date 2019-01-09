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

#include "../queue.h"
#include <string.h>
#include "../test.h"

static struct vstd_queue *queue;
static char *first, *second;

static void setup() {
    queue = vstd_queue_alloc();
    assert(queue);

    first = malloc(sizeof(char) * 6);
    assert(first);
    strcpy(first, "first");

    second = malloc(sizeof(char) * 7);
    assert(second);
    strcpy(second, "second");
}

static void teardown() {
    vstd_queue_free(queue);
    free(first);
    free(second);
    vstd_list_free_object_pool();
}

static void test_vstd_queue_alloc() {
    assert(vstd_queue_size(queue) == 0);
}
VSTD_TEST_REGISTER_UNIT(test_vstd_queue_alloc, 10000, setup, teardown)

static void test_vstd_queue_push() {
    assert(vstd_queue_push(queue, first));
    assert(vstd_queue_size(queue) == 1);
    assert(vstd_queue_push(queue, second));
    assert(vstd_queue_size(queue) == 2);
}
VSTD_TEST_REGISTER_UNIT(test_vstd_queue_push, 10000, setup, teardown)

static void test_vstd_queue_pop() {
    char *popped;

    assert(vstd_queue_push(queue, first));
    assert(vstd_queue_push(queue, second));

    popped = vstd_queue_pop(queue);
    assert(popped == first);
    assert(vstd_queue_size(queue) == 1);

    popped = vstd_queue_pop(queue);
    assert(popped == second);
    assert(vstd_queue_size(queue) == 0);
}
VSTD_TEST_REGISTER_UNIT(test_vstd_queue_pop, 10000, setup, teardown)
