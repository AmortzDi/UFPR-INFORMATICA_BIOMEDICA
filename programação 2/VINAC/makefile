parametrosCompilacao = -Wall -g
nomePrograma = vinac

all: $(nomePrograma)

$(nomePrograma): vina.o vinac.o lz.o lista.o
	gcc -o $(nomePrograma) $^ $(parametrosCompilacao)

vina.o: vina.c vinac.h lz.h
	gcc -c vina.c $(parametrosCompilacao)

vinac.o: vinac.c vinac.h
	gcc -c vinac.c $(parametrosCompilacao)

lz.o: lz.c lz.h
	gcc -c lz.c $(parametrosCompilacao)

lista.o: lista.c lista.h
	gcc -c lista.c $(parametrosCompilacao)

clean:
	rm -f *.o *.vc $(nomePrograma)