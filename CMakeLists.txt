cmake_minimum_required(VERSION 3.7)
project(problemy_synchronizacyjne)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/dining-philosophers/dining-philosophers-resource-hierarchy.c
        src/dining-philosophers/dining-philosophers-waiter.c
        src/dining-philosophers/error.c
        src/dining-philosophers/error.h
        src/producers-consumers/error.c
        src/producers-consumers/error.h
        src/producers-consumers/producers-consumers.c
        src/readers-writers/common.c
        src/readers-writers/common.h
        src/readers-writers/error.c
        src/readers-writers/error.h
        src/readers-writers/rwp1_mutex.c
        src/readers-writers/rwp1_semaphore.c
        src/readers-writers/rwp2_mutex.c
        src/readers-writers/rwp2_semaphore.c
        src/readers-writers/rwp3_mutex.c
        src/readers-writers/rwp3_semaphore.c)

add_executable(problemy_synchronizacyjne ${SOURCE_FILES})