CFLAGS= -Wall -std=gnu99 -pthread -pedantic
CC=gcc
DEP = error.c error.h

all: compile

compile:
	$(CC) $(CFLAGS) producers-consumers.c $(DEP) -o producers-consumers

clean:
	rm -f producers-consumers
