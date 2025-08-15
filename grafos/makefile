parametrosCompilacao = -Wall -g
nomePrograma = aula1

all: $(nomePrograma)

$(nomePrograma): aula1.o lista.o main.o
	gcc -o $(nomePrograma) $^ $(parametrosCompilacao)

main.o: main.c aula1.h lista.h
	gcc -c main.c $(parametrosCompilacao)

aula1.o: aula1.c aula1.h
	gcc -c aula1.c $(parametrosCompilacao)


lista.o: lista.c lista.h
	gcc -c lista.c $(parametrosCompilacao)

clean:
	rm -f *.o *.vc $(nomePrograma)