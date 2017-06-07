CFLAGS= -Wall -std=gnu99 -pthread -pedantic
CC=gcc
DEP = common.c common.h error.c error.h

all: compile

compile:
	$(CC) $(CFLAGS) rwp1_mutex.c $(DEP) -o rwp1_mutex
	$(CC) $(CFLAGS) rwp2_mutex.c $(DEP) -o rwp2_mutex
	$(CC) $(CFLAGS) rwp3_mutex.c $(DEP) -o rwp3_mutex
	$(CC) $(CFLAGS) rwp1_semaphore.c $(DEP) -o rwp1_semaphore
	$(CC) $(CFLAGS) rwp2_semaphore.c $(DEP) -o rwp2_semaphore
	$(CC) $(CFLAGS) rwp3_semaphore.c $(DEP) -o rwp3_semaphore

clean:
	rm -f rwp1_mutex rwp2_mutex rwp3_mutex rwp1_semaphore rwp2_semaphore
