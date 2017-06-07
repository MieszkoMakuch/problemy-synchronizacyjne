cmake_minimum_required(VERSION 3.7)
project(producers_consumers)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        error.c
        error.h
        producers-consumers.c)

add_executable(producers_consumers ${SOURCE_FILES})