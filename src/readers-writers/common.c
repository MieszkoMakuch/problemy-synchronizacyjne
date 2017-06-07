//
// Created by Mieszko Makuch on 24.05.2017.
//

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <memory.h>
#include "common.h"
#include "error.h"

void readMemory(int divider, int *sharedArray, int sharedArraySize, int range, int verbose) {
    while (divider == 0) divider = rand() % range;
    printf("Reader TID=%ld is looking for numbers divisible by %i\n", (long) pthread_self(), divider);
    int divisibleCounter = 0;
    for (int i = 0; i < sharedArraySize; ++i) {
        if (sharedArray[i] % divider == 0) {
            divisibleCounter++;
            if (verbose) printf("(index=%i, value=%i), ", i, sharedArray[i]);
        }
    }
    printf("Reader TID=%ld found %i numbers divisible by %i.\n", (long) pthread_self(), divisibleCounter, divider);
}

void writeMemory(int *sharedArray, int sharedArraySize, int range, int verbose) {
    int noModifications = rand() % sharedArraySize;
    printf("Writer TID=%ld is modifying %i indexes in shared array. ", (long) pthread_self(), noModifications);
    for (int i = 0; i < noModifications; ++i) {
        int indexToModify = rand() % sharedArraySize;
        int newValue = rand() % range;
        sharedArray[indexToModify] = newValue;
        if (verbose) { printf("(i=%i, newValue=%i), ", indexToModify, newValue); }
    }
    printf("\n");
}

void initSharedArray() {
    for (int i = 0; i < SHARED_ARRAY_SIZE; ++i) {
        SHARED_ARRAY[i] = rand() % RANGE;
    }
    printf("Shared array initialized: \n\t");
    for (int i = 0; i < SHARED_ARRAY_SIZE; ++i) {
        printf("%i, ", SHARED_ARRAY[i]);
    }
    printf("\n\n");
}

void parseArguments(int argc, char *argv[], int minNumberOfArguments,
                    int *noReaders, int *noWriters, bool *verbose) {
    if (argc < minNumberOfArguments + 1) {
        throwError(
                "Not enough arguments. Usage: ./main NO_READERS NO_WRITERS [-i]");
    }
    int currentArgument = 1;
    (*noReaders) = atoi(argv[currentArgument++]);
    (*noWriters) = atoi(argv[currentArgument++]);
    if (argc == minNumberOfArguments + 2 && strcmp(argv[currentArgument++], "-i") == 0) {
        (*verbose) = true;
    }
}
