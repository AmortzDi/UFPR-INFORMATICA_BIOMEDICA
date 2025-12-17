#ifndef _GRAFO_
#define _GRAFO_

#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 1000
#define _ERRO_MALLOC 1
#define MAX_ID (MAX_VERTICES) //maximo de id permetindo para um vertice

typedef struct t_vertice *vertice;
typedef struct t_aresta *aresta;
typedef struct t_grafo *grafo;

typedef struct t_vertice {
	int id;
	grafo grafo;
} t_vertice;

typedef struct t_aresta {
	int id;
	vertice u, v;
	int peso;
} t_aresta;

typedef struct t_grafo {
	aresta matriz[MAX_VERTICES][MAX_VERTICES];
	
	//armazena os ponteiros para os vertices onde o incede vai ser o mesmo da matriz
	vertice vertices[MAX_VERTICES];
	int num_v_atual; // numero de vertices ja foram adicionados

	int id_para_indice[MAX_VERTICES + 1]; 
    int indice_para_id[MAX_VERTICES];

} t_grafo ;

//---------------------------------------------------------
// getters:

int vertice_id(vertice v);
lista fronteira(vertice v);
int aresta_id(aresta e);
vertice vertice_u(aresta e);
vertice vertice_v(aresta e);
lista vertices(grafo G);
lista arestas(grafo G);

//---------------------------------------------------------
// funcoes para construcao/desconstrucao do grafo:

// cria grafo vazio e o retorna
grafo cria_grafo();

// destroi grafo G (desaloca toda a memoria)
void destroi_grafo(grafo G);

// cria novo vertice com id <id> e adiciona ao grafo G
void adiciona_vertice(int id, grafo G);

// remove vertice com id <id> do grafo G e o destroi
// [deve remover e destruir tambem as arestas incidentes]
void remove_vertice(int id, grafo G);

// cria aresta com id <id> tincidente a vertices com
// ids <u_id> e <v_id> e adiciona ao grafo G
void adiciona_aresta(int id, int u_id, int v_id, grafo G);

// remove aresta com id <id> do grafo G e a destroi
void remove_aresta(int id, grafo G);

//---------------------------------------------------------
// funcoes para operacoes com o grafo pronto:

// calcula e devolve o grau do vertice v
int grau(vertice v);

// imprime o grafo G
void imprime_grafo(grafo G);

// imprime o vertice v
void imprime_vertice(vertice v);

// imprime a aresta e
void imprime_aresta(aresta e);


grafo ler_subgrafo(grafo G);
void ler_pare(grafo G);
void remove_vertices_ausentes(grafo G, grafo GX);

#endif
