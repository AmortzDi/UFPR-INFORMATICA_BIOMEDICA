CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm
SRC = main.c arvore_rb.c
OBJ = main.o arvore_rb.o
EXEC = myrb

# Regra para gerar o executável
all: $(EXEC)

# Como compilar o executável
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ) $(LDFLAGS)

# Como compilar o main.o
main.o: main.c arvore_rb.h
	$(CC) $(CFLAGS) -c main.c

# Como compilar o arvore_rb.o
arvore_rb.o: arvore_rb.c arvore_rb.h
	$(CC) $(CFLAGS) -c arvore_rb.c

# Regra para limpar os arquivos compilados
clean:
	rm -f $(OBJ) $(EXEC)

