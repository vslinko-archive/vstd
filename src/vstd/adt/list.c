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

#include <assert.h>
#include <stdlib.h>
#include "list.h"

struct adt_list *adt_list_alloc() {
    struct adt_list *list = malloc(sizeof(struct adt_list));
    assert(list);
    list->first = NULL;
    list->last = NULL;
    list->length = 0;
    return list;
}

struct adt_list_item *adt_list_push(struct adt_list *list, void *value) {
    struct adt_list_item *item = malloc(sizeof(struct adt_list_item));
    assert(item);
    item->value = value;
    item->next = NULL;

    if (list->first == NULL) {
        list->first = item;
        list->last = item;
    } else {
        list->last->next = item;
        list->last = item;
    }

    list->length++;

    return item;
}

void adt_list_free(struct adt_list *list) {
    if (list->first != NULL) {
        struct adt_list_item *item = list->first;
        struct adt_list_item *next = NULL;
        do {
            next = item->next;
            free(item);
            item = next;
        } while (next != NULL);
    }

    free(list);
}
