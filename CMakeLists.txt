cmake_minimum_required(VERSION 3.0.0)

# Project name/type
project(lac C)

# Debug mode arguments
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG_INIT} -Wall -g -D DEBUG")

# Release mode arguments
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_DEBUG_INIT} -Wall -g -O3")

# The path to gcc compiler
set(CMAKE_C_COMPILER "/usr/bin/gcc")

file(GLOB COMMON_SRC
        "common/*.h"
        "common/*.c")

file(GLOB LEX
        "lex/*.h"
        "lex/*.c")

file(GLOB UTILS
        "utils/*.h"
        "utils/*.c")

file(GLOB RUNTIME
        "runtime/*.h"
        "runtime/*.c")

file(GLOB DEV
        "dev/*.h"
        "dev/*.c")

message("C compiler: ${CMAKE_C_COMPILER}")

# Test units

#add_executable(test_analex tests/test_analex.c ${LEX} ${COMMON_SRC} ${UTILS})
#add_executable(test_queue tests/test_queue.c ${COMMON_SRC})
#add_executable(test_stack tests/test_stack.c ${COMMON_SRC})
#add_executable(test_calculate tests/test_calculate.c ${UTILS} ${COMMON_SRC})
#add_executable(test_symbol_table tests/test_symbol_table.c ${UTILS} ${COMMON_SRC} ${RUNTIME})
#add_executable(test_compile tests/test_compile.c ${UTILS} ${COMMON_SRC} ${RUNTIME})
#add_executable(test_interpret tests/test_interpret.c ${LEX} ${UTILS} ${COMMON_SRC} ${RUNTIME})
#add_executable(test_strtable tests/test_strtable.c ${LEX} ${UTILS} ${COMMON_SRC} ${RUNTIME})

add_executable(projet1 demo/projet1-analex.c ${LEX} ${COMMON_SRC} ${UTILS})
add_executable(projet2 demo/projet2-calculate.c ${UTILS} ${COMMON_SRC})
add_executable(projet3 demo/projet3-symtable.c ${LEX} ${UTILS} ${COMMON_SRC} ${RUNTIME})
add_executable(projet4 demo/projet4-lac.c ${LEX} ${UTILS} ${COMMON_SRC} ${RUNTIME})
add_executable(lac lac.c ${LEX} ${UTILS} ${COMMON_SRC} ${RUNTIME})
add_executable(color tools/color.c ${LEX} ${COMMON_SRC} ${UTILS})

add_executable(dev_hash ${DEV} ${LEX} ${UTILS} ${COMMON_SRC} ${RUNTIME})