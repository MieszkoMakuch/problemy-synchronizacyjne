#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <semaphore.h>
#include "error.h"
#include "common.h"

int NO_READERS;
int NO_WRITERS;
int NO_READERS_READING = 0;     // How many readers need shared resources
int NO_WRITERS_WRITING = 0;     // How many writers need shared resources
bool VERBOSE;


sem_t resourceSemaphore, readerSemaphore, serviceSemaphore;

void *readerJob(void *arg) {
    while (1) {
        sem_wait(&serviceSemaphore);
        sem_wait(&readerSemaphore);
        NO_READERS_READING++;
        if (NO_READERS_READING == 1) {
            sem_wait(&resourceSemaphore);
        }
        sem_post(&serviceSemaphore);
        sem_post(&readerSemaphore);

        readMemory((int) (uintptr_t) arg, SHARED_ARRAY, SHARED_ARRAY_SIZE, RANGE, VERBOSE);

        sem_wait(&readerSemaphore);
        NO_READERS_READING--;
        if (NO_READERS_READING == 0) {
            sem_post(&resourceSemaphore);
        }
        sem_post(&readerSemaphore);
    }
    return 0;
}

void *writerJob(void *arg) {
    while (1) {
        sem_wait(&serviceSemaphore); // Wait in line to be serviced
        sem_wait(&resourceSemaphore);
        sem_post(&serviceSemaphore); // Allow next in line to be serviced

        writeMemory(SHARED_ARRAY, SHARED_ARRAY_SIZE, RANGE, VERBOSE);

        sem_post(&resourceSemaphore);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    parseArguments(argc, argv, 2, &NO_READERS, &NO_WRITERS, &VERBOSE);
    printf("Readers–writers problem: third readers-writers problem (no thread shall be allowed to starve) with semaphore synchronization.\n");
    printf("    NO_READERS=%i, NO_WRITERS=%i, VERBOSE=%i\n\n",
           NO_READERS, NO_WRITERS, VERBOSE);

    // Initialize semaphores
    if (sem_init(&resourceSemaphore, 0, 1)) throwError("sem_init error");
    if (sem_init(&serviceSemaphore, 0, 1)) throwError("sem_init error");
    if (sem_init(&readerSemaphore, 0, 1)) throwError("sem_init error");

    // Initialize arrays of threads IDs
    pthread_t *readersThreadsIds = malloc(NO_READERS * sizeof(pthread_t));
    if (readersThreadsIds == NULL) { cannotAllocateMemoryError(); }
    pthread_t *writersThreadsIds = malloc(NO_READERS * sizeof(pthread_t));
    if (writersThreadsIds == NULL) { cannotAllocateMemoryError(); }

    // Initialize shared memory (array) with random numbers
    srand(time(NULL));
    initSharedArray();

    // Create readers threads
    for (int i = 0; i < NO_READERS; ++i) {
        if (pthread_create(&readersThreadsIds[i], NULL, readerJob, (void *) (uintptr_t) i) != 0) {
            throwError("pthread_create error");
        }
    }
    // Create writers threads
    for (int i = 0; i < NO_WRITERS; ++i) {
        if (pthread_create(&writersThreadsIds[i], NULL, writerJob, NULL) != 0) {
            throwError("pthread_create error");
        }
    }

    // Wait for readers to finish
    for (int i = 0; i < NO_READERS; ++i) {
        if (pthread_join(readersThreadsIds[i], NULL) != 0) {
            throwError("pthread_join error");
        }
    }
    // Wait for writers to finish
    for (int i = 0; i < NO_WRITERS; ++i) {
        if (pthread_join(writersThreadsIds[i], NULL) != 0) {
            throwError("pthread_join error");
        }
    }

    free(readersThreadsIds);
    free(writersThreadsIds);
    sem_destroy(&resourceSemaphore);
    sem_destroy(&serviceSemaphore);
    sem_destroy(&readerSemaphore);
    return 0;
}
