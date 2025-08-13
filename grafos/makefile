parametrosCompilacao = -Wall -g
nomePrograma = aula1

all: $(nomePrograma)

$(nomePrograma): aula1.o lista.o
	gcc -o $(nomePrograma) $^ $(parametrosCompilacao)


aula1.o: aula1.c aula1.h
	gcc -c aula1.c $(parametrosCompilacao)


lista.o: lista.c lista.h
	gcc -c lista.c $(parametrosCompilacao)

clean:
	rm -f *.o *.vc $(nomePrograma)