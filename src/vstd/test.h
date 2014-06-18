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

#ifndef VSTD_TEST_H
#define VSTD_TEST_H

#include <stdio.h>
#include <stdlib.h>

#define assert(e) \
    ((void) ((e) ? 0 : __assert(#e, __FILE__, __LINE__)))

#define __assert(e, file, line) \
    ((void) printf(" FAILED\n\t%s:%u: %s\n", file, line, e), abort())

#define vstd_test_definition(name, run_count, block) \
    void vstd_test_function_##name() { \
        vstd_setup(); \
        block \
        vstd_teardown(); \
    } \
    struct vstd_test vstd_test_##name = {#name, vstd_test_function_##name, run_count}; \
    void vstd_test_##name##_register(void) __attribute__((constructor)); \
    void vstd_test_##name##_register(void) { \
        vstd_test_register(&vstd_test_##name); \
    }

typedef void vstd_test_function(void);

struct vstd_test {
    char *name;
    vstd_test_function *function;
    int run_count;
};

void vstd_test_register(struct vstd_test *test);

void vstd_test_runner(int argc, char **argv);

#endif
