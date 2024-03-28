cmake_minimum_required(VERSION 3.27)
project(Tema_1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Tema_1
        lists_functions.c
        lists_functions.h
        lists_structs.c
        lists_structs.h
        main.c
        allocator_functions.c
        allocator_functions.h
        datatype_functions.c
        datatype_functions.h)
