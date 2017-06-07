cmake_minimum_required(VERSION 3.7)
project(dining_philosophers)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES-waiter
        dining-philosophers-waiter.c
        error.c
        error.h)

add_executable(dining-philosophers-waiter ${SOURCE_FILES-waiter})

set(SOURCE_FILES-resource-hierarchy
        dining-philosophers-resource-hierarchy.c
        error.c
        error.h)

add_executable(dining-philosophers-resource-hierarchy ${SOURCE_FILES-resource-hierarchy})