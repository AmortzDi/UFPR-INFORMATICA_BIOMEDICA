CC = gcc

CFLAGS = -g -Wall

myavl: main.o arvore_avl.o
	$(CC) -o myavl main.o arvore_avl.o

arvore_avl.o: arvore_avl.c myavl.h
	$(CC) -c $(CFLAGS) arvore_avl.c

main.o: main.c myavl.h
	$(CC) -c $(CFLAGS) main.c

clean:
	rm -f *.o myavl

