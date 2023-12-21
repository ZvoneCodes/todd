.DEFAULT_GOAL := purge 

llist.o:
	cc -c engine/llist.c -o llist.o

todo.o:
	cc -c engine/todo.c -o todo.o

main.o:
	cc -c main.c -o main.o

purge:
	rm -f *.o
	make all

all: todo.o main.o llist.o
	cc todo.o main.o llist.o -o todd
