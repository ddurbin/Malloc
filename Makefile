CC = gcc
CCFLAGS = -Wall -g
ARKEYS = rv
TARGET = malloc

all: malloc
malloc: main.o
	$(CC) $(CCFLAGS) -o malloc main.o
main.o: main.c
	$(CC) $(CCFLAGS) -c main.c              
clean:
	rm -f sl *.o malloc
