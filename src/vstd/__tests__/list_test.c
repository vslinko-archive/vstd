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

#include "../list.h"
#include "../test.h"

static struct vstd_list *list;

static void setup() {
    list = vstd_list_alloc();
}

static void teardown() {
    vstd_list_free(list);
    vstd_list_free_object_pool();
}

static void test_vstd_list_alloc(void) {
    assert(list);
    assert(list->first == NULL);
    assert(list->last == NULL);
    assert(list->length == 0);
}
VSTD_TEST_REGISTER_UNIT(test_vstd_list_alloc, 10000, setup, teardown)

static void test_vstd_list_push() {
    static char *first, *second, *third;

    first = "first";
    second = "second";
    third = "third";

    vstd_list_push(list, first);
    assert(list->first != NULL);
    assert(list->first->value == first);
    assert(list->first->next == NULL);
    assert(list->last == list->first);
    assert(list->length == 1);

    vstd_list_push(list, second);
    assert(list->first != NULL);
    assert(list->first->value == first);
    assert(list->last != NULL);
    assert(list->last->value == second);
    assert(list->last->next == NULL);
    assert(list->first->next == list->last);
    assert(list->length == 2);

    vstd_list_push(list, third);
    assert(list->first != NULL);
    assert(list->first->value == first);
    assert(list->last != NULL);
    assert(list->last->value == third);
    assert(list->last->next == NULL);
    assert(list->first->next->next == list->last);
    assert(list->length == 3);
}
VSTD_TEST_REGISTER_UNIT(test_vstd_list_push, 10000, setup, teardown)

static void benchmark_vstd_list_push() {
    struct vstd_list *list;
    int i;

    list = vstd_list_alloc();

    for (i = 0; i < 1000000; i++) {
        vstd_list_push(list, "a");
    }

    vstd_list_free(list);
}
VSTD_TEST_REGISTER_BENCHMARK(benchmark_vstd_list_push, 0.15, NULL, NULL)
