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

#include "object_pool.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

static struct object_pool_memory_allocation* create_object_pool_memory_allocation(
  struct object_pool* pool,
  unsigned long size
);
static struct object_pool_memory_allocation* create_object_pool_memory_allocation(
  struct object_pool* pool,
  unsigned long size
) {
    struct object_pool_memory_allocation* allocation = malloc(
      sizeof(struct object_pool_memory_allocation)
    );
    allocation->size = size;
    allocation->items = malloc(pool->row_size * allocation->size);

    for (unsigned long i = 0; i < allocation->size; i++) {
        void* item = (char*) allocation->items + i * pool->row_size;
        bool* used = (bool*) ((char*) item + pool->item_size);
        pool->reset_fn(item);
        *used = false;
    }

    return allocation;
}

struct object_pool* create_object_pool(
  unsigned long initial_pool_size,
  size_t item_size,
  object_pool_reset_fn* reset_fn
) {
    struct object_pool* pool = malloc(sizeof(struct object_pool));
    pool->size = initial_pool_size;
    pool->item_size = item_size;
    pool->row_size = pool->item_size + sizeof(char);
    pool->reset_fn = reset_fn;
    pool->next_a = 0;
    pool->next_i = 0;

    pool->allocations_size = 1;
    pool->allocations = malloc(sizeof(struct object_pool_memory_allocation*));
    pool->allocations[0] = create_object_pool_memory_allocation(pool, pool->size);

    return pool;
}

void* get_object_from_pool(struct object_pool* pool) {
    struct object_pool_memory_allocation* allocation = pool->allocations[pool->next_a];
    void* item = (char*) allocation->items + pool->next_i * pool->row_size;
    bool* used = (bool*) ((char*) item + pool->item_size);
    assert(*used == false);
    *used = true;

    for (unsigned long a = pool->next_a; a < pool->allocations_size; a++) {
        struct object_pool_memory_allocation* allocation = pool->allocations[a];
        unsigned long min_i = a == pool->next_a ? pool->next_i : 0;

        for (unsigned long i = min_i; i < allocation->size; i++) {
            void* next_item = (char*) allocation->items + i * pool->row_size;
            bool* next_used = (bool*) ((char*) next_item + pool->item_size);
            if (*next_used == false) {
                pool->next_a = a;
                pool->next_i = i;
                return item;
            }
        }
    }

    for (unsigned long a = 0; a <= pool->next_a; a++) {
        struct object_pool_memory_allocation* allocation = pool->allocations[a];
        unsigned long max_i = a == pool->next_a ? pool->next_i : allocation->size;

        for (unsigned long i = 0; i < max_i; i++) {
            void* next_item = (char*) allocation->items + i * pool->row_size;
            bool* next_used = (bool*) ((char*) next_item + pool->item_size);
            if (*next_used == false) {
                pool->next_a = a;
                pool->next_i = i;
                return item;
            }
        }
    }

    pool->allocations_size++;
    pool->allocations = realloc(
      pool->allocations,
      sizeof(struct object_pool_memory_allocation*) * pool->allocations_size
    );
    pool->allocations[pool->allocations_size - 1] = create_object_pool_memory_allocation(
      pool,
      pool->size
    );
    pool->size = pool->size * 2;
    pool->next_a = pool->allocations_size - 1;
    pool->next_i = 0;

    return item;
}

void return_object_to_pool(struct object_pool* pool, void* item) {
    bool* used = (bool*) ((char*) item + pool->item_size);
    pool->reset_fn(item);
    *used = false;
}

void free_object_pool(struct object_pool* pool) {
    for (unsigned long i = 0; i < pool->allocations_size; i++) {
        struct object_pool_memory_allocation* allocation = pool->allocations[i];
        free(allocation->items);
        free(allocation);
    }
    free(pool->allocations);
    free(pool);
}
