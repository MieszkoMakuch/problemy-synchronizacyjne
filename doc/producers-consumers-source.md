# Problem producentów i konsumentów

## Użycie

Przejdź do folderu z kodem źródłowym, następnie:

```bash
make
./producers-consumers NO_PRODUCERS NO_CONSUMERS [-d]
```

Parametr `-d` - debug pozwala wypisywać szczegółowe informacje o przebiegu algorytmu.


## Kod źródłowy

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <semaphore.h>
#include <unistd.h>
#include "error.h"

#define RANGE 100 // Range of produced items (integers)
#define BUFFER_SIZE 50
int BUFFER[BUFFER_SIZE];

int NO_PRODUCERS;
int NO_CONSUMERS;
bool DEBUG;

pthread_mutex_t bufferMutex = PTHREAD_MUTEX_INITIALIZER;
sem_t filledSpaceSemaphore;
sem_t leftSpaceSemaphore;

int semaphoreGetVal(sem_t *sem){
    int sval;
    sem_getvalue(sem, &sval);
    return sval;
}

void putItem(int item){
    int index = semaphoreGetVal(&filledSpaceSemaphore);
    if(index > BUFFER_SIZE){throwError("putItem error filledSpaceSemaphore > BUFFER_SIZE");}
    BUFFER[index] = item;
    printf("Producer TID=%ld put item=%i to index=%i.\n", (long) pthread_self(), item, index);
}

void *producerJob(void *arg) {
    while (1) {
        // Produce item
        int item = rand() % RANGE;

        if(DEBUG){
            printf("Producer TID=%ld is trying to decrement leftSpaceSemaphore=%i.\n",
                   (long) pthread_self(), semaphoreGetVal(&leftSpaceSemaphore));
        }
        sem_wait(&leftSpaceSemaphore);
        pthread_mutex_lock(&bufferMutex);

        // Put item into buffer
        putItem(item);

        pthread_mutex_unlock(&bufferMutex);
        sem_post(&filledSpaceSemaphore);
    }
    return 0;
}

int removeItem(){
    int indexToRemove = semaphoreGetVal(&filledSpaceSemaphore);
    if(indexToRemove < 0){throwError("removeItem error filledSpaceSemaphore < 0");}
    int item = BUFFER[indexToRemove];
    printf("Consumer TID=%ld removed item=%i from index=%i.\n", (long) pthread_self(), item, indexToRemove);
    return item;
}

void *consumerJob(void *arg) {
    while (1) {
        if(DEBUG){
            printf("Consumer TID=%ld is trying to decrement filledSpaceSemaphore=%i.\n",
                   (long) pthread_self(), semaphoreGetVal(&filledSpaceSemaphore));
        }
        sem_wait(&filledSpaceSemaphore);
        pthread_mutex_lock(&bufferMutex);

        // Remove item from buffer
        int item = removeItem();

        pthread_mutex_unlock(&bufferMutex);
        sem_post(&leftSpaceSemaphore);

        // Consume item
        item += item;
    }
    return 0;
}

void parseArguments(int argc, char *argv[], int minNumberOfArguments,
                    int *noProducers, int *noConsumers, bool *debug) {
    if (argc < minNumberOfArguments + 1) {
        throwError(
                "Not enough arguments. Usage: ./main NO_PRODUCERS NO_CONSUMERS [-d]");
    }
    int currentArgument = 1;
    (*noProducers) = atoi(argv[currentArgument++]);
    (*noConsumers) = atoi(argv[currentArgument++]);
    if (argc == minNumberOfArguments + 2 && strcmp(argv[currentArgument++], "-d") == 0) {
        (*debug) = true;
    }
}

int main(int argc, char *argv[]) {
    parseArguments(argc, argv, 2, &NO_PRODUCERS, &NO_CONSUMERS, &DEBUG);
    printf("Producers-consumers problem.\n");
    printf("    NO_PRODUCERS=%i, NO_CONSUMERS=%i, DEBUG=%i\n\n",
           NO_PRODUCERS, NO_CONSUMERS, DEBUG);

    // Initialize semaphores
    if (sem_init(&filledSpaceSemaphore, 0, 0)) throwError("sem_init error");
    if (sem_init(&leftSpaceSemaphore, 0, BUFFER_SIZE)) throwError("sem_init error");

    // Initialize arrays of threads IDs
    pthread_t *producersThreadsIds = malloc(NO_PRODUCERS * sizeof(pthread_t));
    if (producersThreadsIds == NULL) { cannotAllocateMemoryError(); }
    pthread_t *consumersThreadsIds = malloc(NO_PRODUCERS * sizeof(pthread_t));
    if (consumersThreadsIds == NULL) { cannotAllocateMemoryError(); }

    // Create producers threads
    for (int i = 0; i < NO_PRODUCERS; ++i) {
        if (pthread_create(&producersThreadsIds[i], NULL, producerJob, NULL) != 0) {
            throwError("pthread_create error");
        }
    }
    // Create consumers threads
    for (int i = 0; i < NO_CONSUMERS; ++i) {
        if (pthread_create(&consumersThreadsIds[i], NULL, consumerJob, NULL) != 0) {
            throwError("pthread_create error");
        }
    }

    // Wait for producers to finish
    for (int i = 0; i < NO_PRODUCERS; ++i) {
        if (pthread_join(producersThreadsIds[i], NULL) != 0) {
            throwError("pthread_join error");
        }
    }
    // Wait for consumers to finish
    for (int i = 0; i < NO_CONSUMERS; ++i) {
        if (pthread_join(consumersThreadsIds[i], NULL) != 0) {
            throwError("pthread_join error");
        }
    }

    free(producersThreadsIds);
    free(consumersThreadsIds);
    pthread_mutex_destroy(&bufferMutex);
    sem_destroy(&filledSpaceSemaphore);
    sem_destroy(&leftSpaceSemaphore);


    return 0;
}


```