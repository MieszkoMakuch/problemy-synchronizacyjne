//
// Created by Mieszko Makuch on 30.03.2017.
//

#include <stdio.h>
#include <stdlib.h>

void throwError(const char message[]) {
    // perror - print a system error message
    perror(message);
    exit(EXIT_FAILURE);
}

void cannotAllocateMemoryError() {
    throwError("Cannot allocate memory");
}