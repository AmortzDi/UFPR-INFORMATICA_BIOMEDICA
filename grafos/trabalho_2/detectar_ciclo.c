#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

#include "leitor_rede.h"
#include "grafo.h"
#include "lista.h"
#include"detectar_ciclo.h"

static bool dfsRhcy(grafo G, vertice v, char* mapa_inverso[], int *tempo_dfs){

    v->pre = (*tempo_dfs)++;

    for(no n = primeiro_no(fronteira_saida(v));n;n = proximo(n)){
        aresta e = (aresta)conteudo(n);
        vertice vizinho = vertice_v(e);

        if(vizinho->pre == NAO_VISITADO){
            if(dfsRhcy(G,vizinho,mapa_inverso,tempo_dfs)) 
                return true;
        }else if(vizinho->pos == NAO_VISITADO) return true;
    }

    v->pos = (*tempo_dfs)++;

    return false;
}

bool GRAPHcycle(grafo G, char* mapa_inverso[]){
    int tempo_dfs = 0;

    for(no n = primeiro_no(vertices(G));n;n = proximo(n)){
        vertice v = (vertice)conteudo(n);
        v->pre = NAO_VISITADO;
        v->pos = NAO_VISITADO;
    }

    for(no n = primeiro_no(vertices(G));n;n = proximo(n)){
        vertice v = (vertice)conteudo(n);
        if(v->pre == NAO_VISITADO)
            if(dfsRhcy(G,v,mapa_inverso,&tempo_dfs)) return true;
    }

    return false;
}

static int compara_pos(const void *a, const void *b){
    vertice vA = *(const vertice*)a;
    vertice vB = *(const vertice*)b;
    return vB->pos - vA->pos;
}

void imprime_ordenacao_topologica(grafo G, char* mapa_inverso[],int total_vertices){
    if(total_vertices == 0) return;

    vertice vetor_vertices[total_vertices];
    int i = 0;
    for(no n = primeiro_no(vertices(G));n;n = proximo(n))
        vetor_vertices[i++] = (vertice)conteudo(n);

    qsort(vetor_vertices,total_vertices,sizeof(vertice),compara_pos);

    for(i = 0; i < total_vertices;i++){
        vertice v = vetor_vertices[i];
        if(vertice_tipo(v) == METABOLITO){
            printf("%s ", mapa_inverso[vertice_id(v)]);
        }
    }
    printf("\n");
}
void destro_vet(char* mapa_inverso[],int total_vertices){
    for (int i = 0; i < total_vertices; i++) {
        free(mapa_inverso[i]);
    }
}