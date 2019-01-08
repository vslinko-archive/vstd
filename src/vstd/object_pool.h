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

#ifndef VSTD_OBJECT_POOL_H
#define VSTD_OBJECT_POOL_H

#include <stdlib.h>

struct vstd_object_pool_memory_allocation {
    unsigned long size;
    void *items;
};

struct vstd_object_pool {
    unsigned long allocations_size;
    struct vstd_object_pool_memory_allocation **allocations;

    unsigned long size;

    unsigned long next_a;
    unsigned long next_i;

    size_t item_size;
    size_t row_size;
};

struct vstd_object_pool *vstd_object_pool_alloc(unsigned long initial_pool_size,
                                                size_t item_size);

void *vstd_object_pool_get(struct vstd_object_pool *pool);

void vstd_object_pool_return(struct vstd_object_pool *pool, void **item);

void vstd_object_pool_free(struct vstd_object_pool *pool);

#endif
