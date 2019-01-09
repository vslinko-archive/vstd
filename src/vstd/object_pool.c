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

#include "./object_pool.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

static struct vstd_object_pool_memory_allocation *
vstd_object_pool_memory_alloc(struct vstd_object_pool *pool,
                              unsigned long size) {
    struct vstd_object_pool_memory_allocation *allocation;
    unsigned long i;
    void *item;
    bool *used;

    allocation = malloc(sizeof(struct vstd_object_pool_memory_allocation));

    if (!allocation) {
        return NULL;
    }

    allocation->size = size;
    allocation->items = malloc(pool->row_size * allocation->size);

    if (!allocation->items) {
        free(allocation);
        return NULL;
    }

    for (i = 0; i < allocation->size; i++) {
        item = (char *) allocation->items + i * pool->row_size;
        used = (bool *) ((char *) item + pool->item_size);
        *used = false;
    }

    return allocation;
}

struct vstd_object_pool *vstd_object_pool_alloc(unsigned long initial_pool_size,
                                                size_t item_size) {
    struct vstd_object_pool *pool;

    pool = malloc(sizeof(struct vstd_object_pool));

    if (!pool) {
        return NULL;
    }

    pool->size = initial_pool_size;
    pool->item_size = item_size;
    pool->row_size = pool->item_size + sizeof(char);
    pool->next_a = 0;
    pool->next_i = 0;

    pool->allocations_size = 1;
    pool->allocations = malloc(sizeof(struct object_pool_memory_allocation *));

    if (!pool->allocations) {
        free(pool);
        return NULL;
    }

    pool->allocations[0] = vstd_object_pool_memory_alloc(pool, pool->size);

    if (!pool->allocations[0]) {
        free(pool->allocations);
        free(pool);
        return NULL;
    }

    return pool;
}

void *vstd_object_pool_get(struct vstd_object_pool *pool) {
    struct vstd_object_pool_memory_allocation *allocation;
    struct vstd_object_pool_memory_allocation **new_allocations;
    void *item, *next_item;
    bool *used, *next_used;
    unsigned long a, i, min_i, max_i;

    allocation = pool->allocations[pool->next_a];
    item = (char *) allocation->items + pool->next_i * pool->row_size;
    used = (bool *) ((char *) item + pool->item_size);
    assert(*used == false);
    *used = true;

    for (a = pool->next_a; a < pool->allocations_size; a++) {
        allocation = pool->allocations[a];
        min_i = a == pool->next_a ? pool->next_i : 0;

        for (i = min_i; i < allocation->size; i++) {
            next_item = (char *) allocation->items + i * pool->row_size;
            next_used = (bool *) ((char *) next_item + pool->item_size);
            if (*next_used == false) {
                pool->next_a = a;
                pool->next_i = i;
                return item;
            }
        }
    }

    for (a = 0; a <= pool->next_a; a++) {
        allocation = pool->allocations[a];
        max_i = a == pool->next_a ? pool->next_i : allocation->size;

        for (i = 0; i < max_i; i++) {
            next_item = (char *) allocation->items + i * pool->row_size;
            next_used = (bool *) ((char *) next_item + pool->item_size);
            if (*next_used == false) {
                pool->next_a = a;
                pool->next_i = i;
                return item;
            }
        }
    }

    new_allocations = realloc(pool->allocations,
                              sizeof(struct vstd_object_pool_memory_allocation *) * pool->allocations_size + 1);

    if (!new_allocations) {
        return NULL;
    }

    pool->allocations = new_allocations;
    pool->allocations[pool->allocations_size] = vstd_object_pool_memory_alloc(pool, pool->size);

    if (!pool->allocations[pool->allocations_size]) {
        /* I don't want to rollback realloc because I don't want to handle another error.
         * It's shouldn't be a problem to leave it as it is.
         */
        return NULL;
    }

    pool->size = pool->size * 2;
    pool->next_a = pool->allocations_size;
    pool->next_i = 0;

    pool->allocations_size++;

    return item;
}

void vstd_object_pool_return(struct vstd_object_pool *pool, void *item) {
    bool *used;

    used = (bool *) ((char *) item + pool->item_size);
    *used = false;
}

void vstd_object_pool_free(struct vstd_object_pool *pool) {
    struct vstd_object_pool_memory_allocation *allocation;
    unsigned long i;

    for (i = 0; i < pool->allocations_size; i++) {
        allocation = pool->allocations[i];
        free(allocation->items);
        free(allocation);
    }

    free(pool->allocations);
    free(pool);
}
