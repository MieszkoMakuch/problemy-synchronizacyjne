# Problem ucztujących filozofów

Rozwiązanie z kelnerem / arbitrem

## Użycie

Przejdź do folderu z kodem źródłowym, następnie:

```bash
make
./dining-philosophers-waiter [-d]
```

Parametr `-d` - debug pozwala wypisywać szczegółowe informacje o przebiegu algorytmu.

Przykładowy wynik, bez parametru `-d` (częściowy):

```bash
./dining-philosophers-waiter

Dining-philosophers problem. Solution with waiter
    NO_PHILO=5, DEBUG=0

Philosopher id: 0, TID=139930960656128 is eating now
Philosopher id: 1, TID=139930952263424 is eating now
Philosopher id: 4, TID=139930927085312 is eating now
Philosopher id: 2, TID=139930943870720 is eating now
Philosopher id: 0, TID=139930960656128 is eating now
Philosopher id: 3, TID=139930935478016 is eating now
Philosopher id: 1, TID=139930952263424 is eating now
Philosopher id: 2, TID=139930943870720 is eating now
Philosopher id: 4, TID=139930927085312 is eating now
Philosopher id: 0, TID=139930960656128 is eating now
Philosopher id: 3, TID=139930935478016 is eating now


```

## Kod źródłowy

```cpp
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


```