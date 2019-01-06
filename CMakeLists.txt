cmake_minimum_required(VERSION 3.13)
include_directories(src)
add_executable(
  vstd-test
  src/vstd/__tests__/object_pool_test.c
  src/vstd/object_pool.c
  src/vstd/adt/__tests__/list_test.c
  src/vstd/adt/list.c
  src/vstd/__tests__/string_test.c
  src/vstd/string.c
  src/vstd/test.c
  src/vstd/test_runner.c
)
target_compile_options(
  vstd-test
  PRIVATE -g
  PRIVATE -O0
)
target_compile_options(
  vstd-test
  PRIVATE -Wall
  PRIVATE -Wextra
  PRIVATE -std=c11
  PRIVATE -pedantic
  PRIVATE -Wmissing-prototypes
  PRIVATE -Wstrict-prototypes
  PRIVATE -Wold-style-definition
)
