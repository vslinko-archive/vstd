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

#include <vstd/adt/list.h>
#include <vstd/test.h>

struct adt_list *list;

static void vstd_setup() {
    list = adt_list_alloc();
}

static void vstd_teardown() {
    adt_list_free(list);
}

vstd_test_unit(vstd_adt_list_alloc, 10000, {
    assert(list);
    assert(list->first == NULL);
    assert(list->last == NULL);
    assert(list->length == 0);
})

vstd_test_unit(vstd_adt_list_push, 10000, {
    static char *first = "first";
    static char *second = "second";
    static char *third = "third";

    adt_list_push(list, first);
    assert(list->first != NULL);
    assert(list->first->value == first);
    assert(list->first->next == NULL);
    assert(list->last == list->first);
    assert(list->length == 1);

    adt_list_push(list, second);
    assert(list->first != NULL);
    assert(list->first->value == first);
    assert(list->last != NULL);
    assert(list->last->value == second);
    assert(list->last->next == NULL);
    assert(list->first->next == list->last);
    assert(list->length == 2);

    adt_list_push(list, third);
    assert(list->first != NULL);
    assert(list->first->value == first);
    assert(list->last != NULL);
    assert(list->last->value == third);
    assert(list->last->next == NULL);
    assert(list->first->next->next == list->last);
    assert(list->length == 3);
})
