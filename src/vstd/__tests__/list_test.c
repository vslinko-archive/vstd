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
    tassert(list);
}

static void teardown() {
    vstd_list_free(list);
    vstd_list_free_object_pool();
}

static void test_vstd_list_alloc(void) {
    tassert(list);
    tassert(list->first == NULL);
    tassert(list->last == NULL);
    tassert(list->length == 0);
}
VSTD_TEST_REGISTER_UNIT(test_vstd_list_alloc, 10000, setup, teardown)

static void test_vstd_list_push() {
    static char *first, *second, *third;

    first = "first";
    second = "second";
    third = "third";

    tassert(vstd_list_push(list, first));
    tassert(list->first != NULL);
    tassert(list->first->value == first);
    tassert(list->first->next == NULL);
    tassert(list->last == list->first);
    tassert(list->length == 1);

    tassert(vstd_list_push(list, second));
    tassert(list->first != NULL);
    tassert(list->first->value == first);
    tassert(list->last != NULL);
    tassert(list->last->value == second);
    tassert(list->last->next == NULL);
    tassert(list->first->next == list->last);
    tassert(list->length == 2);

    tassert(vstd_list_push(list, third));
    tassert(list->first != NULL);
    tassert(list->first->value == first);
    tassert(list->last != NULL);
    tassert(list->last->value == third);
    tassert(list->last->next == NULL);
    tassert(list->first->next->next == list->last);
    tassert(list->length == 3);
}
VSTD_TEST_REGISTER_UNIT(test_vstd_list_push, 10000, setup, teardown)

#define BENCHMARK_VSTD_LIST_PUSH_ITERATIONS 30000000
static void benchmark_vstd_list_push() {
    struct vstd_list *list;
    int i;

    list = vstd_list_alloc();
    tassert(list);

    for (i = 0; i < BENCHMARK_VSTD_LIST_PUSH_ITERATIONS; i++) {
        tassert(vstd_list_push(list, "a"));
    }

    for (i = 0; i < BENCHMARK_VSTD_LIST_PUSH_ITERATIONS; i++) {
        tassert(vstd_list_unshift(list));
    }

    tassert(vstd_list_unshift(list) == NULL);

    vstd_list_free(list);
}
VSTD_TEST_REGISTER_BENCHMARK(benchmark_vstd_list_push, 1.0, NULL, NULL)
