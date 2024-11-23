CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm
SRC = main.c cuckoo.c
OBJ = main.o cuckoo.o
EXEC = myht

# Regra para gerar o executável
all: $(EXEC)

# Como compilar o executável
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ) $(LDFLAGS)

# Como compilar o main.o
main.o: main.c cuckoo.h
	$(CC) $(CFLAGS) -c main.c

# Como compilar o cuckoo.o
cuckoo.o: cuckoo.c cuckoo.h
	$(CC) $(CFLAGS) -c cuckoo.c

# Regra para limpar os arquivos compilados
clean:
	rm -f $(OBJ) $(EXEC)

