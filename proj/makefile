CFLAGS=-std=c99 -Wall -pedantic -g
BIN=project
CC=gcc
RM=rm -f

ALL: scaner.o main.o dynstring.o parser.o stack.o expr.o
	$(CC) $(CFLAGS) -o $(BIN) scaner.o parser.o main.o dynstring.o stack.o expr.o

clean:
	$(RM) *.o $(BIN)
