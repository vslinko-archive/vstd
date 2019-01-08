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

#include "../string.h"
#include "../test.h"

#define assert_string_memory_size(char_size) \
    assert(vstd_string_used_memory(string) \
           == (sizeof(size_t) * 3 + sizeof(vstd_string_t) * char_size))

static vstd_string_t *string;

static void setup() {
    string = vstd_string_alloc(2);
}

static void teardown() {
    vstd_string_free(string);
}

static void test_vstd_string_alloc() {
    assert(string);
    assert(string[0] == 0);
    assert(string[1] == 0);
    assert(string[2] == 0);
}
VSTD_TEST_REGISTER_UNIT(test_vstd_string_alloc, 10000, setup, teardown)

static void test_vstd_string_length() {
    assert(vstd_string_length(string) == 0);
    vstd_string_append_character(string, 1);
    assert(vstd_string_length(string) == 1);
}
VSTD_TEST_REGISTER_UNIT(test_vstd_string_length, 10000, setup, teardown)

static void test_vstd_string_used_memory() {
    assert_string_memory_size(3);
}
VSTD_TEST_REGISTER_UNIT(test_vstd_string_used_memory, 10000, setup, teardown)

static void test_vstd_string_set() {
    vstd_string_set(string, "a");
    assert(vstd_string_length(string) == 1);
    assert_string_memory_size(3);
    assert(string[0] == 'a');
    assert(string[1] == 0);
    assert(string[2] == 0);

    vstd_string_set(string, "abc");
    assert(vstd_string_length(string) == 3);
    assert_string_memory_size(5);
    assert(string[0] == 'a');
    assert(string[1] == 'b');
    assert(string[2] == 'c');
    assert(string[3] == 0);
    assert(string[4] == 0);

    vstd_string_set(string, "d");
    assert(vstd_string_length(string) == 1);
    assert_string_memory_size(5);
    assert(string[0] == 'd');
    assert(string[1] == 0);
    assert(string[2] == 0);
    assert(string[3] == 0);
    assert(string[4] == 0);
}
VSTD_TEST_REGISTER_UNIT(test_vstd_string_set, 10000, setup, teardown)

static void test_vstd_string_append_character() {
    vstd_string_append_character(string, 1);
    assert(vstd_string_length(string) == 1);
    assert_string_memory_size(3);
    assert(string[0] == 1);
    assert(string[1] == 0);
    assert(string[2] == 0);

    vstd_string_append_character(string, 2);
    assert(vstd_string_length(string) == 2);
    assert_string_memory_size(3);
    assert(string[0] == 1);
    assert(string[1] == 2);
    assert(string[2] == 0);

    vstd_string_append_character(string, 3);
    assert(vstd_string_length(string) == 3);
    assert_string_memory_size(5);
    assert(string[0] == 1);
    assert(string[1] == 2);
    assert(string[2] == 3);
    assert(string[3] == 0);
    assert(string[4] == 0);
}
VSTD_TEST_REGISTER_UNIT(test_vstd_string_append_character, 10000, setup, teardown)
