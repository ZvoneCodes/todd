.DEFAULT_GOAL := purge 

todo.o:
	gcc -c engine/todo.c -o todo.o

main.o:
	gcc -c main.c -o main.o

purge:
	rm -f *.o
	make all

all: todo.o main.o
	gcc todo.o main.o -o todd
