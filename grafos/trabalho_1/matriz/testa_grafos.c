

#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>


// Em testa_grafos.c (ou um novo arquivo testa_ponderado.c)
int main() {
  
  // cria grafo vazio
  grafo G = cria_grafo();
  
  // le n e adiciona vertices 1..n
  int n;
  printf("Digite o numero de vertices: ");
  scanf("%d", &n); // qtd vertices
  
  for (int i = 1; i <= n; ++i)
    adiciona_vertice(i, G);
  
  printf("Digite as arestas (par u v), termine com 0 0:\n");
  // le pares u v e adiciona arestas {u,v}
  ler_pare(G);
  printf("\n--- Grafo Ponderado ---");

  imprime_grafo(G);
 
  grafo GX = ler_subgrafo(G);
  printf("\n--- Subgrafo  G[X] ---");
  imprime_grafo(GX);
   //  REMOÇÃO DOS VÉRTICES EM G QUE NÃO ESTÃO EM GX
  printf("\n---Removendo de G os vertices que nao estao no subgrafo ---\n");
  remove_vertices_ausentes(G, GX);
  
  // IMPRESSÃO E FINALIZAÇÃO
  printf("\n--- Grafo Original G (apos a remocao) ---");
  imprime_grafo(G);


  destroi_grafo(GX);
  destroi_grafo(G);


  return 0;
}