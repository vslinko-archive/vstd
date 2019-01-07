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
#include "queue.h"

#include <stdlib.h>

static struct vstd_object_pool* queue_pool;

static void vstd_queue_reset(struct vstd_queue* queue) {
  queue->_list = NULL;
}

struct vstd_queue* vstd_queue_alloc(void) {
  if (!queue_pool) {
    queue_pool = vstd_object_pool_alloc(
      8,
      sizeof(struct vstd_queue),
      (vstd_object_pool_reset_fn*) &vstd_queue_reset
    );
  }

  struct vstd_queue* queue = vstd_object_pool_get(queue_pool);
  queue->_list = vstd_list_alloc();
  return queue;
}

unsigned int vstd_queue_size(struct vstd_queue* queue) {
  return queue->_list->length;
}

void vstd_queue_push(struct vstd_queue* queue, void* value) {
  vstd_list_push(queue->_list, value);
}

void* vstd_queue_pop(struct vstd_queue* queue) {
  return vstd_list_unshift(queue->_list);
}

void vstd_queue_free(struct vstd_queue* queue) {
  vstd_list_free(queue->_list);
  vstd_object_pool_return(queue_pool, queue);
}
