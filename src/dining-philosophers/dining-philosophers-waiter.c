#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <memory.h>
#include <stdbool.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>
#include "error.h"

#define NO_PHILOSOPHERS 5
bool DEBUG;

sem_t WAITER;
sem_t CHOPSTICKS[NO_PHILOSOPHERS];

int leftChopstick(int i) {
    return i;
}

int rightChopstick(int i) {
    return (i + 1) % 5;
}

void *philosopherJob(void *arg) {
    int id = (int) (uintptr_t) arg;
    while (1) {
        usleep(500000);
        // Check if waiter allows you to join the table
        sem_wait(&WAITER);
        if(DEBUG) printf("  [D] Philosopher id: %i, TID=%ld joined the table\n", id, (long) pthread_self());
        // Acquire chopsticks
        sem_wait(&CHOPSTICKS[rightChopstick(id)]);
        sem_wait(&CHOPSTICKS[leftChopstick(id)]);
        if(DEBUG) printf("  [D] Philosopher id: %i, TID=%ld acquired chopsticks\n", id, (long) pthread_self());

        // Eat
        printf("Philosopher id: %i, TID=%ld is eating now\n", id, (long) pthread_self());
        usleep(500000);

        // Release chopsticks
        sem_post(&CHOPSTICKS[leftChopstick(id)]);
        sem_post(&CHOPSTICKS[rightChopstick(id)]);
        if(DEBUG) printf("  [D] Philosopher id: %i, TID=%ld released chopsticks\n", id, (long) pthread_self());
        sem_post(&WAITER);
        if(DEBUG) printf("  [D] Philosopher id: %i, TID=%ld left the table\n", id, (long) pthread_self());
    }
    return 0;
}

void parseArguments(int argc, char *argv[], int minNumberOfArguments,
                    bool *debug) {
    if (argc < minNumberOfArguments + 1) {
        throwError(
                "Not enough arguments. Usage: ./main [-d]");
    }
    int currentArgument = 1;
    if (argc == minNumberOfArguments + 2 && strcmp(argv[currentArgument++], "-d") == 0) {
        (*debug) = true;
    }
}

int main(int argc, char *argv[]) {
    parseArguments(argc, argv, 0, &DEBUG);
    printf("Dining-philosophers problem. Solution with waiter\n");
    printf("    NO_PHILO=%i, DEBUG=%i\n\n", NO_PHILOSOPHERS, DEBUG);

    // Initialize semaphores
    if (sem_init(&WAITER, 0, 4)) throwError("sem_init error"); // Waiter allows max 4 philosophers
    for (int i = 0; i < NO_PHILOSOPHERS; ++i) {
        if (sem_init(&CHOPSTICKS[i], 0, 1)) throwError("sem_init error");
    }

    // Initialize arrays of threads IDs
    pthread_t *philosophersThreadsIds = malloc(NO_PHILOSOPHERS * sizeof(pthread_t));
    if (philosophersThreadsIds == NULL) { cannotAllocateMemoryError(); }

    // Create philosophers threads
    for (int i = 0; i < NO_PHILOSOPHERS; ++i) {
        if (pthread_create(&philosophersThreadsIds[i], NULL, philosopherJob, (void *) (uintptr_t) i) != 0) {
            throwError("pthread_create error");
        }
    }

    // Wait for philosophers to finish
    for (int i = 0; i < NO_PHILOSOPHERS; ++i) {
        if (pthread_join(philosophersThreadsIds[i], NULL) != 0) {
            throwError("pthread_join error");
        }
    }

    free(philosophersThreadsIds);
    sem_destroy(&WAITER);
    for (int i = 0; i < NO_PHILOSOPHERS; ++i) {
        sem_destroy(&CHOPSTICKS[i]);
    }

    return 0;
}
