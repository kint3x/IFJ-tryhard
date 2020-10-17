CFLAGS=-std=c99 -Wall -pedantic -g
BIN=project
CC=gcc
RM=rm -f

ALL: scaner.o main.o dynstring.o
	$(CC) $(CFLAGS) -o $(BIN) str.o stable.o ilist.o scaner.o parser.o interpret.o main.o

clean:
	$(RM) *.o $(BIN)
