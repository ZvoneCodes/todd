.DEFAULT_GOAL := purge 

CC=gcc

todo.o:
	$(CC) -c engine/todo.c -o todo.o

main.o:
	$(CC) -c main.c -o main.o

purge:
	rm -f *.o
	make all

all: todo.o main.o
	$(CC) todo.o main.o -o todd
