//
// Created by Mieszko Makuch on 24.05.2017.
//

#ifndef ZAD1_COMMON_H
#define ZAD1_COMMON_H

#include <stdbool.h>

#define SHARED_ARRAY_SIZE 500
#define RANGE 100
int SHARED_ARRAY[SHARED_ARRAY_SIZE];

void readMemory(int divider, int *sharedArray, int sharedArraySize, int range, int verbose);

void writeMemory(int *sharedArray, int sharedArraySize, int range, int verbose);

void initSharedArray();

void parseArguments(int argc, char *argv[], int minNumberOfArguments,
                    int *noReaders, int *noWriters, bool *verbose);

#endif //ZAD1_COMMON_H
