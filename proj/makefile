CFLAGS=-std=c99 -Wall -pedantic -g
BIN=project
CC=gcc
RM=rm -f

ALL: scaner.o main.o dynstring.o parser.o
	$(CC) $(CFLAGS) -o $(BIN) scaner.o parser.o main.o dynstring.o

clean:
	$(RM) *.o $(BIN)
