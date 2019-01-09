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

#define tassert(e) \
    ((void) ((e) ? 0 : __tassert(#e, __FILE__, __LINE__)))

#define __tassert(e, file, line) \
    (printf(" FAILED\n\t%s:%u: %s\n", file, line, e), abort(), 0)

#define VSTD_TEST_REGISTER_UNIT(fn, run_count, setup_fn, teardown_fn) \
    struct vstd_test vstd_test_##fn = { \
        VSTD_TEST_UNIT, #fn, setup_fn, fn, teardown_fn, run_count, 0 \
    }; \
    void vstd_test_##fn##_register(void) __attribute__((constructor)); \
    void vstd_test_##fn##_register(void) { vstd_test_register(&vstd_test_##fn); }

#define VSTD_TEST_REGISTER_BENCHMARK(fn, max_time, setup_fn, teardown_fn) \
    struct vstd_test vstd_test_##fn = { \
        VSTD_TEST_BENCHMARK, #fn, setup_fn, fn, teardown_fn, 0, max_time \
    }; \
    void vstd_test_##fn##_register(void) __attribute__((constructor)); \
    void vstd_test_##fn##_register(void) { vstd_test_register(&vstd_test_##fn); }

#define VSTD_TEST_REGISTER_ABORT(fn, setup_fn, teardown_fn) \
    struct vstd_test vstd_test_##fn = { \
        VSTD_TEST_ABORT, #fn, setup_fn, fn, teardown_fn, 0, 0 \
    }; \
    void vstd_test_##fn##_register(void) __attribute__((constructor)); \
    void vstd_test_##fn##_register(void) { vstd_test_register(&vstd_test_##fn); }

enum vstd_test_type {
    VSTD_TEST_UNIT,
    VSTD_TEST_BENCHMARK,
    VSTD_TEST_ABORT
};

typedef void vstd_test_function(void);

struct vstd_test {
    enum vstd_test_type type;
    char *name;
    vstd_test_function *setup;
    vstd_test_function *function;
    vstd_test_function *teardown;
    int run_count;
    double max_time;
};

void vstd_test_register(struct vstd_test *test);

void vstd_test_runner(int argc, char **argv);

#endif
