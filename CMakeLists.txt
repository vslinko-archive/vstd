cmake_minimum_required(VERSION 3.13)

include_directories(src)

add_library(
  vstd
  SHARED
  src/vstd/object_pool.c
  src/vstd/queue.c
  src/vstd/list.c
  src/vstd/string.c
  src/vstd/test.c
)
target_compile_options(
  vstd
  PRIVATE -O2
)
target_compile_options(
  vstd
  PRIVATE -Wall
  PRIVATE -Wextra
  PRIVATE -std=c89
  PRIVATE -pedantic
  PRIVATE -Wmissing-prototypes
  PRIVATE -Wstrict-prototypes
  PRIVATE -Wold-style-definition
)

add_executable(
  vstd-test
  src/vstd/main_test.c
  src/vstd/__tests__/object_pool_test.c
  src/vstd/object_pool.c
  src/vstd/__tests__/queue_test.c
  src/vstd/queue.c
  src/vstd/__tests__/list_test.c
  src/vstd/list.c
  src/vstd/__tests__/string_test.c
  src/vstd/string.c
  src/vstd/test.c
)
target_compile_options(
  vstd-test
  PRIVATE -O2
)
target_compile_options(
  vstd-test
  PRIVATE -Wall
  PRIVATE -Wextra
  PRIVATE -std=c89
  PRIVATE -pedantic
  PRIVATE -Wmissing-prototypes
  PRIVATE -Wstrict-prototypes
  PRIVATE -Wold-style-definition
)
