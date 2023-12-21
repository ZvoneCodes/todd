.DEFAULT_GOAL := purge 

todo.o:
	cc -c todo.c -o todo.o

main.o:
	cc -c main.c -o main.o

purge:
	rm -f *.o
	make all

all: todo.o main.o
	cc todo.o main.o -o todd
