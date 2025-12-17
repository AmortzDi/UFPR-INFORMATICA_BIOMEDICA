#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>


static int existe_vertice(int id, grafo G){
  if(!G || id < 0 || id > MAX_ID) return 0;
  return G->id_para_indice[id] != -1;
}
//---------------------------------------------------------
// getters:

int vertice_id(vertice v) {
  return v->id;
};
lista fronteira(vertice v) {
    lista l = cria_lista();
    grafo G = v->grafo;
    int indice = G->id_para_indice[v->id];
    if (indice != -1) {
        for (int j = 0; j < G->num_v_atual; j++) {
            if (G->matriz[indice][j] != NULL) {
                empilha(G->matriz[indice][j], l);
            }
        }
    }
    return l;
}
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
    lista l = cria_lista();
    for (int i = 0; i < G->num_v_atual; i++) {
        empilha(G->vertices[i], l);
    }
    return l;
}
lista arestas(grafo G) {
    lista l = cria_lista();
    for (int i = 0; i < G->num_v_atual; i++) {
        for (int j = i; j < G->num_v_atual; j++) {
            if (G->matriz[i][j] != NULL) {
                empilha(G->matriz[i][j], l);
            }
        }
    }
    return l;
}

//---------------------------------------------------------
// funcoes para construcao/desconstrucao do grafo:

// cria grafo vazio e o retorna
grafo cria_grafo() {
  grafo G = (grafo) malloc(sizeof(t_grafo));
  if (!G) exit(_ERRO_MALLOC_);
  
  G->num_v_atual = 0;
  for(int i = 0; i < MAX_VERTICES; i++){
    G->vertices[i] = NULL;
    G->indice_para_id[i] = -1;
    for(int j = 0; j < MAX_VERTICES;j++)
      G->matriz[i][j] = NULL;
  }

  for(int i = 0; i <= MAX_ID;i++) G->id_para_indice[i] = -1;

  return G;
}

// destroi grafo G (desaloca toda a memoria)
void destroi_grafo(grafo G) {
  if(!G) return;

  //libera as aresta
  for(int i = 0; i < G->num_v_atual; i++){
    for(int j = i; j < G->num_v_atual; j++)
      if(G->matriz[i][j] != NULL) free(G->matriz[i][j]);
  }

  for(int i = 0; i < G->num_v_atual;i++)
    if(G->vertices[i] != NULL) free(G->vertices[i]);

  free(G);
}

// cria novo vertice com id <id> e adiciona ao grafo G
void adiciona_vertice(int id, grafo G) {
   if (G->num_v_atual >= MAX_VERTICES || id > MAX_ID || id < 0) return;
  if (existe_vertice(id, G)) return;

  int indice = G->num_v_atual;
  G->id_para_indice[id] = indice;
  G->indice_para_id[indice] = id;
  
  vertice v = (vertice) malloc(sizeof(t_vertice));
  if(!v)exit(_ERRO_MALLOC_); 
  v->id = id;
  v->grafo = G;
  G->vertices[indice] = v;

  G->num_v_atual++;
  
}

// cria aresta com id <id> incidente a vertices com
// ids <u_id> e <v_id> e adiciona ao grafo G
void adiciona_aresta(int id, int u_id, int v_id, grafo G) {
  if (!existe_vertice(u_id, G) || !existe_vertice(v_id, G)) return;

  int u_idx = G->id_para_indice[u_id];
  int v_idx = G->id_para_indice[v_id];

  //verificar se os vertices existem
  if (G->matriz[u_idx][v_idx] != NULL) return; // Aresta já existe

  aresta e = (aresta) malloc(sizeof(t_aresta));
  if(!e)
    exit(_ERRO_MALLOC_);
  e->id = id;
  e->u = G->vertices[u_idx];
  e->v = G->vertices[v_idx];

 G->matriz[u_idx][v_idx] = e;
    if (u_idx != v_idx) G->matriz[v_idx][u_idx] = e;
  
}

void remove_vertice(int id, grafo G) {
    if (!existe_vertice(id, G)) return;

    int idx_remover = G->id_para_indice[id];
    int idx_ultimo = G->num_v_atual - 1;

    // Libera arestas conectadas ao vértice
    for (int i = 0; i < G->num_v_atual; i++) {
        aresta e = G->matriz[idx_remover][i];
        if (e != NULL) {
            vertice outro_v = (e->u->id == id) ? e->v : e->u;
            int outro_idx = G->id_para_indice[outro_v->id];
            G->matriz[idx_remover][outro_idx] = NULL;
            G->matriz[outro_idx][idx_remover] = NULL;
            free(e);
        }
    }
    
    free(G->vertices[idx_remover]);
    G->id_para_indice[id] = -1;

    // "Swap and Pop": Move o último vértice para a posição do removido
    if (idx_remover != idx_ultimo) {
        vertice v_ultimo = G->vertices[idx_ultimo];
        int id_ultimo = v_ultimo->id;
        G->vertices[idx_remover] = v_ultimo;
        G->id_para_indice[id_ultimo] = idx_remover;
        G->indice_para_id[idx_remover] = id_ultimo;

        for (int i = 0; i < G->num_v_atual; i++) {
            G->matriz[idx_remover][i] = G->matriz[idx_ultimo][i];
            G->matriz[i][idx_remover] = G->matriz[i][idx_ultimo];
        }
    }
    
    // Limpa a posição antiga do último vértice
    G->vertices[idx_ultimo] = NULL;
    G->indice_para_id[idx_ultimo] = -1;
    for (int i = 0; i < G->num_v_atual; i++) {
        G->matriz[idx_ultimo][i] = NULL;
        G->matriz[i][idx_ultimo] = NULL;
    }
    
    G->num_v_atual --;
}


void remove_vertices_ausentes(grafo G, grafo GX) {
    // Itera do final para o início para segurança durante a remoção
    for (int i = G->num_v_atual - 1; i >= 0; i--) {
        int id_atual_em_G = G->indice_para_id[i];
        if (!existe_vertice(id_atual_em_G, GX)) {
            remove_vertice(id_atual_em_G, G);
        }
    }
}
//---------------------------------------------------------
// funcoes para operacoes com o grafo pronto:

// calcula e devolve o grau do vertice v
int grau(vertice v) {
    grafo G = v->grafo;
    int indice = G->id_para_indice[v->id];
    if (indice == -1) return 0;

    int cont = 0;
    for (int j = 0; j < G->num_v_atual; ++j) {
        if (G->matriz[indice][j] != NULL) {
            cont++;
            // Laço na diagonal adiciona +1 ao grau (totalizando 2)
            if (indice == j) {
                cont++;
            }
        }
    }
    return cont;
}

// Em grafo.c

void imprime_vertice(vertice v) {
    printf("  Vértice %d [grau: %d]:\n", vertice_id(v), grau(v));
    
    grafo G = v->grafo;
    int indice = G->id_para_indice[v->id];
    int arestas_encontradas = 0;

    if (indice != -1) {
        for (int j = G->num_v_atual - 1; j >= 0; j--) {
            aresta e = G->matriz[indice][j];
            if (e != NULL) {
                printf("    -> ");
                imprime_aresta(e);
                printf("\n");
                arestas_encontradas++;
            }
        }
    }

    if (arestas_encontradas == 0) {
        printf("    -> (Nenhuma aresta)\n");
    }
}

void imprime_grafo(grafo G) {
    if (!G) { printf("Erro: Grafo nulo.\n"); return; }
    
    int num_arestas = 0;
    for (int i = 0; i < G->num_v_atual; i++) {
        for (int j = i; j < G->num_v_atual; j++) { // j=i para incluir laços
            if (G->matriz[i][j] != NULL) num_arestas++;
        }
    }

    printf("\n---[ IMPRESSÃO DO GRAFO (MATRIZ) ]---\n");
    printf("| Vértices: %-4d | Arestas: %-4d |\n", G->num_v_atual, num_arestas);
    printf("-----------------------------------\n");
    printf("FORMATO: Vértice ID [grau]:\n");
    
    if (G->num_v_atual == 0) {
        printf("O grafo não possui vértices.\n");
    } else {
        for (int i = G->num_v_atual - 1; i >= 0; i--) {
            imprime_vertice(G->vertices[i]);
        }
    }
    printf("-----------------------------------\n\n");
}
// Em grafo.c

// imprime a aresta e
void imprime_aresta(aresta e) {
    printf("Aresta %d: {%d, %d} [Peso: %d]",
           aresta_id(e), vertice_id(vertice_u(e)), vertice_id(vertice_v(e)), e->peso);
}
static void atribui_pesos_grau(grafo G) {
   for (int i = 0; i < G->num_v_atual; i++) {
        for (int j = i; j < G->num_v_atual; j++) {
            aresta e = G->matriz[i][j];
            if (e != NULL) {
                e->peso = grau(e->u) + grau(e->v);
            }
        }
    }
}

static void mensagem(int u_id, int v_id, vertice vert_u, vertice vert_v) {
    // Um ou ambos os vértices não existem. Informa o usuário.
    // Usa os IDs para imprimir, pois são sempre valores válidos.
    printf("Aviso: Aresta (%d, %d) ignorada. ", u_id, v_id);

    // Usa os ponteiros para a lógica, verificando se são NULL.
    if (!vert_u && !vert_v) 
        printf("Ambos os vertices nao existem.\n");
    else if (!vert_u)
        printf("O vertice %d nao existe.\n", u_id);
    else 
        printf("O vertice %d nao existe.\n", v_id);
}
void ler_pare(grafo G) {
    int u, v, id = 1;
    while (scanf("%d %d", &u, &v) == 2 && (u != 0 || v != 0)) {
        // A função 'existe_vertice' é mais eficiente aqui do que buscar o ponteiro
        int u_existe = existe_vertice(u, G);
        int v_existe = existe_vertice(v, G);

        if (u_existe && v_existe) {
            adiciona_aresta(id++, u, v, G);
        } else {
            // Passa NULL para os vértices que não existem para a função mensagem
            mensagem(u, v, u_existe ? G->vertices[G->id_para_indice[u]] : NULL, 
                           v_existe ? G->vertices[G->id_para_indice[v]] : NULL);
        }
    }
    atribui_pesos_grau(G);
}
grafo ler_subgrafo(grafo G) {
    grafo GX = cria_grafo();
    int u, v, id = 1;
    printf("\nDigite as arestas do subgrafo (par u v), termine com 0 0:\n");
    while (scanf("%d %d", &u, &v) == 2 && (u != 0 || v != 0)) {
        int u_existe = existe_vertice(u, G);
        int v_existe = existe_vertice(v, G);

        if (u_existe && v_existe) {
            if (!existe_vertice(u, GX)) adiciona_vertice(u, GX);
            if (!existe_vertice(v, GX)) adiciona_vertice(v, GX);
            adiciona_aresta(id++, u, v, GX);
        } else {
             mensagem(u, v, u_existe ? G->vertices[G->id_para_indice[u]] : NULL, 
                           v_existe ? G->vertices[G->id_para_indice[v]] : NULL);
        }
    }
    atribui_pesos_grau(GX);
    return GX;
}