parametrosCompilacao=-Wall -g #-Wshadow
nomePrograma=analise

all: $(nomePrograma)

$(nomePrograma): analise.o leitor_rede.o  lista.o grafo.o tabela.o detectar_ciclo.o
	gcc -o $(nomePrograma) analise.o leitor_rede.o lista.o grafo.o tabela.o detectar_ciclo.o $(parametrosCompilacao)

analise.o: analise.c
	gcc -c analise.c $(parametrosCompilacao)

lista.o:lista.h lista.c
	gcc -c lista.c $(parametrosCompilacao)

grafo.o:grafo.h grafo.c
	gcc -c grafo.c $(parametrosCompilacao)

leitor_rede.o: leitor_rede.h leitor_rede.c
	gcc -c leitor_rede.c $(parametrosCompilacao)

tabela.o:tabela.h tabela.c
	gcc -c tabela.c $(parametrosCompilacao)

detectar_ciclo.o:detectar_ciclo.h detectar_ciclo.c
	gcc -c detectar_ciclo.c $(parametrosCompilacao)

clean:
	rm -f *.o  $(nomePrograma)