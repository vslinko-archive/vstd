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

#include <string.h>
#include <vstd/queue.h>
#include <vstd/test.h>

struct vstd_queue* queue;
static char* first;
static char* second;

static void vstd_setup() {
  queue = vstd_queue_alloc();

  first = malloc(sizeof(char) * 6);
  strcpy(first, "first");

  second = malloc(sizeof(char) * 7);
  strcpy(second, "second");
}

static void vstd_teardown() {
  vstd_queue_free(queue);
  free(first);
  free(second);
  vstd_list_free_object_pool();
}

vstd_test_unit(vstd_queue_alloc, 10000, {
  assert(vstd_queue_size(queue) == 0);
})

vstd_test_unit(vstd_queue_push, 10000, {
  vstd_queue_push(queue, first);
  assert(vstd_queue_size(queue) == 1);
  vstd_queue_push(queue, second);
  assert(vstd_queue_size(queue) == 2);
})

vstd_test_unit(vstd_queue_pop, 10000, {
  char* popped = NULL;

  vstd_queue_push(queue, first);
  vstd_queue_push(queue, second);

  popped = vstd_queue_pop(queue);
  assert(popped == first);
  assert(vstd_queue_size(queue) == 1);

  popped = vstd_queue_pop(queue);
  assert(popped == second);
  assert(vstd_queue_size(queue) == 0);
})
