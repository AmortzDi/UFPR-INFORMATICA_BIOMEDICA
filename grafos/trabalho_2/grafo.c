#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>

//funcoes auxiliares para busca em listas 
static int _pega_id_vertice(obj v) {
  return vertice_id((vertice)v);
}
static int _pega_id_aresta(obj e) {
  return aresta_id((aresta)e);
}
//---------------------------------------------------------
// getters:

int vertice_id(vertice v) {
  return v->id;
};

// CORREÇÃO: A função estava com o nome "vertice_tip" em vez de "vertice_tipo"
int vertice_tipo(vertice v){
  return v->tipo;
}

lista fronteira_saida(vertice v) {
  return v->fronteira_saida;
};
lista fronteira_entrada(vertice v) {
  return v->fronteira_entreda;
};
int aresta_id(aresta e) {
  return e->id;
};
vertice vertice_u(aresta e) {
  return e->u;
};
vertice vertice_v(aresta e) {
  return e->v;
};
lista vertices(grafo G) {
  return G->vertices;
};
lista arestas(grafo G) {
  return G->arestas;
};

//---------------------------------------------------------
// funcoes para construcao/desconstrucao do grafo:

// cria grafo vazio e o retorna
grafo cria_grafo() {
  grafo G = (grafo) malloc(sizeof(t_grafo));
  if (!G)
    exit(_ERRO_MALLOC_);
  G->vertices = cria_lista();
  G->arestas = cria_lista();
  return G;
}

// cria novo vertice com id <id> e adiciona ao grafo G
void adiciona_vertice(int id, int tipo, grafo G) {
  vertice v = (vertice) malloc(sizeof(t_vertice));
  if(!v)
    exit(_ERRO_MALLOC_);
  v->id = id;
  v->tipo = tipo;
  v->fronteira_entreda = cria_lista();
  v->fronteira_saida = cria_lista();

  v->pre = INFINITO;
  v->pos = INFINITO;
  v->pai = NULL;

  empilha(v,vertices(G));

}


// cria aresta com id <id> incidente a vertices com
// ids <u_id> e <v_id> e adiciona ao grafo G
void adiciona_aresta(int id, int u_id, int v_id, grafo G) {
  vertice u = (vertice) busca_chave(u_id,vertices(G),_pega_id_vertice);
  vertice v = (vertice) busca_chave(v_id,vertices(G),_pega_id_vertice);

  if(!u || !v)
    return;

  aresta e = (aresta) malloc(sizeof(t_aresta));
  if(!e)
    exit(_ERRO_MALLOC_);
  e->id = id;
  e->u = u;
  e->v = v;

  empilha(e, arestas(G));
  empilha(e, fronteira_saida(u));
  empilha(e, fronteira_entrada(v));
}

// remove aresta com id <id> do grafo G e a destroi
void remove_aresta(int id, grafo G) {
  aresta e = (aresta) remove_chave(id, arestas(G), _pega_id_aresta);
  if (!e)
    return; // Aresta não encontrada

  // Remove a referência da aresta da fronteira de saída da origem
  remove_chave(id, fronteira_saida(vertice_u(e)), _pega_id_aresta);
  // Remove a referência da aresta da fronteira de entrada do destino
  remove_chave(id, fronteira_entrada(vertice_v(e)), _pega_id_aresta);
  
  free(e);
}

void remove_vertice(int id, grafo G){
  vertice v = (vertice)remove_chave(id,vertices(G), _pega_id_vertice);
  if(!v)
    return;

  while(!vazio(fronteira_saida(v))){
    aresta e_saindo = desempilha(fronteira_saida(v));
    remove_aresta(e_saindo->id, G);
  }

  while(!vazio(fronteira_entrada(v))){
    aresta e_entrando = desempilha(fronteira_entrada(v));
    remove_aresta(e_entrando->id, G);
  }

  free(fronteira_saida(v));
  free(fronteira_entrada(v));
  free(v);
}
// destroi grafo G (desaloca toda a memoria)
void destroi_grafo(grafo G) {

  if(!G) return;
 
  while(!vazio(vertices(G))){
    vertice primeiro = topo(vertices(G));
    remove_vertice(primeiro->id,G);
  }

  free(arestas(G));
  free(vertices(G));
  free(G);
}
//---------------------------------------------------------
// funcoes para operacoes com o grafo pronto:

static void imprime_aresta_saida(obj e_obj) {
    aresta e = (aresta)e_obj;
    printf("(%d -> %d) ", vertice_id(vertice_u(e)), vertice_id(vertice_v(e)));
}

static void imprime_vertice_completo(obj v_obj) {
    vertice v = (vertice)v_obj;
    printf("  Vértice %-4d (Tipo: %d) | Sai para: ", vertice_id(v), vertice_tipo(v));
    
    if (vazio(fronteira_saida(v))) {
        printf("(nenhum)\n");
    } else {
        imprime_lista(fronteira_saida(v), imprime_aresta_saida);
        printf("\n");
    }
}

void imprime_grafo(grafo G) {
    if (!G) {
        printf("Erro: Grafo nulo.\n");
        return;
    }
    printf("\n---[ IMPRESSÃO DO GRAFO DIRECIONADO ]---\n");
    printf("| Vértices: %-4d | Arestas: %-4d |\n",
           tamanho_lista(vertices(G)), tamanho_lista(arestas(G)));
    printf("-----------------------------------------\n");
    if (vazio(vertices(G))) {
        printf("O grafo está vazio.\n");
    } else {
        imprime_lista(vertices(G), imprime_vertice_completo);
    }
    printf("-----------------------------------------\n\n");
}