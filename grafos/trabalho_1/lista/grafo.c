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
lista fronteira(vertice v) {
  return v->fronteira;
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

// destroi grafo G (desaloca toda a memoria)
void destroi_grafo(grafo G) {
  while(!vazio(arestas(G))){
    aresta e = (aresta)desempilha(arestas(G));
    free(e);
  }
  free(arestas(G));

  while (!vazio(vertices(G))){
    vertice v = (vertice)desempilha(vertices(G));
    //libera a lista de fronteira
    while (!vazio(fronteira(v))){
      desempilha(fronteira(v));
    }
    free(fronteira(v));
    free(v);
  }

  free(vertices(G));
  free(G);
}

// cria novo vertice com id <id> e adiciona ao grafo G
void adiciona_vertice(int id, grafo G) {
  vertice v = (vertice) malloc(sizeof(t_vertice));
  if(!v)
    exit(_ERRO_MALLOC_);
  v->id = id;
  v->fronteira = cria_lista();
  empilha(v,vertices(G));

}

// remove vertice com id <id> do grafo G e o destroi
// [deve remover e destruir tambem as arestas incidentes]
void remove_vertice(int id, grafo G) {
  vertice v = (vertice) remove_chave(id,vertices(G), _pega_id_vertice);
  if(!v)
    return;

  while(!vazio(fronteira(v))){
    aresta e = (aresta)desempilha(fronteira(v));

    //identidica o outro vertice da aresta
    vertice outro_v = (vertice_u(e) == v) ? vertice_v(e) : vertice_u(e);
    
    //remove a aresta da fronteira do outro vertice e da lita principal de arestas
    remove_chave(aresta_id(e),fronteira(outro_v), _pega_id_aresta);
    remove_chave(aresta_id(e), arestas(G), _pega_id_aresta);

    free(e);
  }
  free(fronteira(v));
  free(v);
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
  empilha(e, fronteira(u));
  empilha(e, fronteira(v));
}

// remove aresta com id <id> do grafo G e a destroi
void remove_aresta(int id, grafo G) {
  aresta e = (aresta) remove_chave(id, arestas(G), _pega_id_aresta);
  if (!e)
    return; // Aresta não encontrada

  remove_chave(id, fronteira(vertice_u(e)), _pega_id_aresta);
  remove_chave(id, fronteira(vertice_v(e)), _pega_id_aresta);
  
  free(e);
}

//---------------------------------------------------------
// funcoes para operacoes com o grafo pronto:

// calcula e devolve o grau do vertice v
int grau(vertice v) {
  int d_v = 0;
  for (no n = primeiro_no(fronteira(v)); n; n = proximo(n))
    ++d_v;
  return d_v;
}

// imprime o grafo G
void imprime_grafo(grafo G) {
  if (!G) {
    printf("Erro: Grafo nulo.\n");
    return;
  }

  // --- CABEÇALHO E SUMÁRIO ---
  printf("\n---[ IMPRESSÃO DO GRAFO ]---\n");
  // Supondo que você tenha uma função 'tamanho_lista' na sua biblioteca de lista
  printf("| Vértices: %-4d | Arestas: %-4d |\n",
         tamanho_lista(vertices(G)),
         tamanho_lista(arestas(G)));
  printf("------------------------------\n");
  printf("FORMATO: Vértice ID [grau]:\n");


  // --- LISTA DE VÉRTICES ---
  // A impressão de cada vértice já mostra suas arestas,
  // então não precisamos de uma lista de arestas separada.
  if(vazio(vertices(G))){
      printf("O grafo não possui vértices.\n");
  } else {
      imprime_lista(vertices(G), (void_f_obj) imprime_vertice);
  }
  
  // --- RODAPÉ ---
  printf("------------------------------\n\n");
}

// Em grafo.c

// imprime o vertice v
void imprime_vertice(vertice v) {
  // Cabeçalho do vértice
  printf("  Vértice %d [grau: %d]:\n", vertice_id(v), grau(v));

  // Verifica se o vértice não tem arestas
  if (vazio(fronteira(v))) {
    printf("    -> (Nenhuma aresta)\n");
  } else {
    // Itera pela fronteira para imprimir cada aresta em uma nova linha
    // (Isso nos dá mais controle sobre a formatação do que usar imprime_lista)
    for (no n = primeiro_no(fronteira(v)); n; n = proximo(n)) {
      printf("    -> "); // Adiciona indentação para clareza
      imprime_aresta((aresta)conteudo(n));
      printf("\n");
    }
  }
}


void remove_vertices_ausentes(grafo G, grafo GX) {
    // Pega o primeiro nó da lista de vértices do grafo original
    no n = primeiro_no(vertices(G));

    // Itera pela lista de vértices de G
    do {
        // Pega o vértice atual
        vertice v_em_G = (vertice)conteudo(n);
        int id_atual = vertice_id(v_em_G);

        // IMPORTANTE: Pega o próximo nó ANTES de qualquer remoção,
        // pois a remoção pode invalidar o nó atual 'n'.
        no proximo_n = proximo(n);

        // Verifica se o vértice com este ID existe no subgrafo GX
        vertice v_em_GX = (vertice)busca_chave(id_atual, vertices(GX), _pega_id_vertice);

        // Se a busca retornar NULL, o vértice não existe em GX e deve ser removido de G
        if (v_em_GX == NULL) {
            // A função remove_vertice já cuida de remover o vértice da lista
            // de vértices e também todas as suas arestas incidentes.
            remove_vertice(id_atual, G);
        }

        // Avança para o próximo nó que foi salvo anteriormente
        n = proximo_n;
    }while (n);
    
}

// imprime a aresta e
void imprime_aresta(aresta e) {
  // Formato mais limpo e direto
  printf("Aresta %d: {%d, %d} [Peso: %d]",
         aresta_id(e),
         vertice_id(vertice_u(e)),
         vertice_id(vertice_v(e)),
         e->peso);
}


static void atribui_pesos_grau(grafo G) {
    no n = primeiro_no(arestas(G));
    while (n) {
        aresta e = (aresta)conteudo(n);
        // O peso é a soma dos graus dos vértices da aresta
        e->peso = grau(vertice_u(e)) + grau(vertice_v(e));
        n = proximo(n);
    }
}

static int existe_vertice(int id_procurado, grafo G) {
    vertice v_encontrado = (vertice) busca_chave(id_procurado, vertices(G), _pega_id_vertice);
    return (v_encontrado != NULL);
}

static int existe_aresta(int u_id, int v_id, grafo G) {
    vertice u = (vertice) busca_chave(u_id, vertices(G), _pega_id_vertice);
    if (!u) return 0; // Vértice u não existe

    // Itera pela fronteira  de u
    for (no n = primeiro_no(fronteira(u)); n; n = proximo(n)) {
        aresta e = (aresta) conteudo(n);
        
        // Pega o outro vértice da aresta
        vertice vizinho = (vertice_u(e) == u) ? vertice_v(e) : vertice_u(e);
        
        // Compara o id do vizinho com o v_id procurado
        if (vertice_id(vizinho) == v_id) {
            return 1; // Encontrou a aresta
        }
    }
    return 0; 
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
grafo ler_subgrafo(grafo G) {
    grafo GX = cria_grafo();
    int u_id, v_id;
    int id_aresta = 1;

    printf("\nDigite o subgrafo (par u v), termine com 0 0:\n");

    while (scanf("%d %d", &u_id, &v_id) == 2 && (u_id != 0 || v_id != 0)) {
        // Primeiro, verifica se os vértices existem no grafo original G
        vertice vert_u_orig = (vertice) busca_chave(u_id, vertices(G), _pega_id_vertice);
        vertice vert_v_orig = (vertice) busca_chave(v_id, vertices(G), _pega_id_vertice);

        if (vert_u_orig && vert_v_orig) {
            // Vértices existem em G. Agora adiciona em GX (se não existirem lá)
            if (!busca_chave(u_id, vertices(GX), _pega_id_vertice))
                adiciona_vertice(u_id, GX);
            if (!busca_chave(v_id, vertices(GX), _pega_id_vertice))
                adiciona_vertice(v_id, GX);

            adiciona_aresta(id_aresta++, u_id, v_id, GX);
        } else {
            // Vértices não existem em G, chama a mensagem de erro
            mensagem(u_id, v_id, vert_u_orig, vert_v_orig);
        }
    }
    atribui_pesos_grau(GX);
    return GX;
}

void ler_pare(grafo G) {
  int id = 1;
  int u_id, v_id;


  while (scanf("%d %d", &u_id, &v_id) == 2 && (u_id != 0 || v_id != 0))  {
    vertice vert_u = (vertice) busca_chave(u_id, vertices(G), _pega_id_vertice);
    vertice vert_v = (vertice) busca_chave(v_id, vertices(G), _pega_id_vertice);

    if (vert_u && vert_v) {
      // Ambos os vértices existem, então a aresta é adicionada.
      adiciona_aresta(id++, u_id, v_id, G);
    } else {
      // Chamada corrigida para a nova função mensagem
      mensagem(u_id, v_id, vert_u, vert_v);
    }
  }
  atribui_pesos_grau(G);
}