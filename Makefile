.DEFAULT_GOAL := all

CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99 -g

todo.o:
	$(CC) $(CFLAGS) -c engine/todo.c -o todo.o

main.o:
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f *.o
	rm -f todd
	rm /usr/local/bin/todd || true

todd: todo.o main.o
	$(CC) $(CFLAGS) todo.o main.o -lncurses -o todd

all: todd

install: todd
	cp todd /usr/local/bin/todd
