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
#include <string.h>
#include "string.h"

#define VSTD_STRING_PREFIX_SIZE (sizeof(size_t) * 3)

#define vstd_string_get_memory(string) \
    (((void *) string) - VSTD_STRING_PREFIX_SIZE)

/* String memory structure:
 * |      size_t      |      size_t      |      size_t      |   vstd_string_t  ...
 * ^                  ^                  ^                  ^
 * chunk_size         allocated_chunks   string_length      string
 */
vstd_string_t *vstd_string_alloc(size_t chunk_size) {
    void *string_memory = malloc(VSTD_STRING_PREFIX_SIZE + sizeof(vstd_string_t) * (chunk_size + 1));
    assert(string_memory);

    size_t *string_prefix = (size_t *) string_memory;
    string_prefix[0] = chunk_size;
    string_prefix[1] = 1;
    string_prefix[2] = 0;

    vstd_string_t *string = (vstd_string_t *) string_memory + VSTD_STRING_PREFIX_SIZE;
    memset(string, 0, chunk_size + 1);

    return string;
}

size_t vstd_string_length(const vstd_string_t *string) {
    size_t *string_prefix = (size_t *) vstd_string_get_memory(string);
    return string_prefix[2];
}

size_t vstd_string_used_memory(const vstd_string_t *string) {
    size_t *string_prefix = (size_t *) vstd_string_get_memory(string);

    size_t chunk_size = string_prefix[0];
    size_t allocated_chunks = string_prefix[1];

    return VSTD_STRING_PREFIX_SIZE + sizeof(vstd_string_t) * (allocated_chunks * chunk_size + 1);
}

void vstd_string_set(vstd_string_t *string, const char *new_string) {
    void *string_memory = vstd_string_get_memory(string);
    size_t *string_prefix = (size_t *) string_memory;

    size_t new_string_length = strlen(new_string);
    size_t chunk_size = string_prefix[0];
    size_t allocated_chunks = string_prefix[1];
    size_t allocated_length = allocated_chunks * chunk_size;

    if (new_string_length > allocated_length) {
        string_prefix[1] = allocated_chunks = new_string_length / chunk_size + 1;
        allocated_length = allocated_chunks * chunk_size;
        string_memory = realloc(string_memory, sizeof(vstd_string_t) * (allocated_chunks * chunk_size + 1));
        assert(string_memory);
    }

    memcpy(string, new_string, new_string_length);
    memset(string + new_string_length, 0, allocated_length - new_string_length + 1);
    string_prefix[2] = new_string_length;
}

void vstd_string_append_character(vstd_string_t *string, char character) {
    void *string_memory = vstd_string_get_memory(string);
    size_t *string_prefix = (size_t *) string_memory;

    size_t chunk_size = string_prefix[0];
    size_t string_length = string_prefix[2];

    if (string_length > 0 && string_length % chunk_size == 0) {
        string_memory = realloc(string_memory, VSTD_STRING_PREFIX_SIZE + sizeof(vstd_string_t) * (string_length + chunk_size + 1));
        assert(string_memory);
        memset(string + string_length, 0, chunk_size + 1);
        string_prefix[1]++;
    }

    string[string_length] = character;
    string_prefix[2]++;
}

void vstd_string_free(vstd_string_t *string) {
    free(vstd_string_get_memory(string));
}
