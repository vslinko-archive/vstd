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

#include "test.h"

#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>

struct test_runner_options {
    char* test_name;
    bool memory_dump;
};

static struct test_runner_options test_runner_options = {
    .test_name = NULL,
    .memory_dump = false
};

static int tests_length = 0;
static struct vstd_test** tests;

static jmp_buf env;
static bool catch_abort = false;
static int stderr_copy = -1;

static void parse_test_runner_arguments(int argc, char** argv) {
    char key = 0;

    for (int i = 1; i < argc; i++) {
        if (key == 't') {
            test_runner_options.test_name = argv[i];
            key = 0;
        } else if (strcmp(argv[i], "--memory-dump") == 0) {
            test_runner_options.memory_dump = true;
        } else if (strcmp(argv[i], "--test") == 0) {
            key = 't';
        }
    }
}

static long get_max_memory_usage() {
    long max_memory_usage;
    struct rusage* memory = malloc(sizeof(struct rusage));
    assert(memory);
    getrusage(RUSAGE_SELF, memory);
    max_memory_usage = memory->ru_maxrss;
    free(memory);
    return max_memory_usage;
}

static void abort_handler(int signo) {
    if (signo == SIGABRT && catch_abort) {
        longjmp(env, signo);
    }
}

static void unit_test_runner(struct vstd_test* test) {
    if (test->run_count == 0) {
        return;
    }

    printf("Running unit test `%s':", test->name);

    /* Run test once to reach max memory usage */
    test->function();

    long memory_limit = get_max_memory_usage() * 1.2;

    for (int i = 1; i < test->run_count; i++) {
        test->function();
        assert(get_max_memory_usage() <= memory_limit);
    }

    printf(" DONE\n");
}

static void benchmark_test_runner(struct vstd_test* test) {
    if (test->max_time == 0) {
        return;
    }

    printf("Running benchmark test `%s':", test->name);

    clock_t start_time = clock();

    test->function();

    double seconds_spend = ((double) (clock() - start_time)) / CLOCKS_PER_SEC;

    if (seconds_spend > test->max_time) {
        printf(" FAILED (%fs > %fs)\n", seconds_spend, test->max_time);
    } else {
        printf(" DONE (%fs)\n", seconds_spend);
    }
}

static void abort_test_runner(struct vstd_test* test) {
    printf("Running abort test `%s':", test->name);

    if (stderr_copy == -1) {
        stderr_copy = dup(2);
        close(2);
    }

    int jump_value = setjmp(env);

    if (jump_value == 0) {
        catch_abort = true;
        test->function();
    }

    catch_abort = false;

    dup2(stderr_copy, 2);
    close(stderr_copy);
    stderr_copy = -1;

    if (jump_value == SIGABRT) {
        printf(" DONE\n");
    } else {
        printf(" FAILED\n");
        abort();
    }
}

static void test_runner(struct vstd_test* test) {
    switch (test->type) {
        case VSTD_TEST_UNIT:
            unit_test_runner(test);
            break;

        case VSTD_TEST_BENCHMARK:
            benchmark_test_runner(test);
            break;

        case VSTD_TEST_ABORT:
            abort_test_runner(test);
            break;
    }
}

static void memory_dump_test_runner(struct vstd_test* test) {
    printf("Running memory test `%s':\n", test->name);

    /* Run test once to reach max memory usage */
    test->function();

    double start_memory = get_max_memory_usage();

    while (true) {
        sleep(1);
        test->function();
        double current_memory = get_max_memory_usage();
        double diff = current_memory / start_memory * 100;
        printf("Maximum memory usage: %.0f %.2f%%\n", current_memory, diff);
    }
}

void vstd_test_register(struct vstd_test* test) {
    if (tests_length == 0) {
        tests = malloc(sizeof(struct test_case*));
    } else {
        tests = realloc(tests, sizeof(struct test_case*) * (tests_length + 1));
    }
    assert(tests);

    tests[tests_length++] = test;
}

void vstd_test_runner(int argc, char** argv) {
    signal(SIGABRT, abort_handler);

    parse_test_runner_arguments(argc, argv);

    struct vstd_test* test = NULL;

    if (test_runner_options.test_name != NULL) {
        for (int i = 0; i < tests_length; i++) {
            if (strcmp(tests[i]->name, test_runner_options.test_name) == 0) {
                test = tests[i];
                break;
            }
        }

        if (test == NULL) {
            fprintf(stderr, "Unknown test \"%s\"\n", test_runner_options.test_name);
            exit(EXIT_FAILURE);
        }
    }

    if (test_runner_options.memory_dump) {
        if (test == NULL) {
            fprintf(stderr, "You should select test for memory test runner\n");
            exit(EXIT_FAILURE);
        }

        memory_dump_test_runner(test);
    }

    if (test == NULL) {
        for (int i = 0; i < tests_length; i++) {
            test_runner(tests[i]);
        }
    } else {
        test_runner(test);
    }
}
