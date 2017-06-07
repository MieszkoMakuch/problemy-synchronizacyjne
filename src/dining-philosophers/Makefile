CFLAGS= -Wall -std=gnu99 -pthread -pedantic
CC=gcc
DEP = error.c error.h

all: compile

compile:
	$(CC) $(CFLAGS) dining-philosophers-waiter.c $(DEP) -o dining-philosophers-waiter
	$(CC) $(CFLAGS) dining-philosophers-resource-hierarchy.c $(DEP) -o dining-philosophers-resource-hierarchy

clean:
	rm -f dining-philosophers-waiter
