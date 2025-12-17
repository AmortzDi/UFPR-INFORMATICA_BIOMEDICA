#include <stdio.h>
#include <stdlib.h>

#include "leitor_rede.h"
#include "grafo.h"
#include "lista.h"
#include"detectar_ciclo.h"


int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.met>\n", argv[0]);
        return 1;
    }

    char* mapa_inverso[MAX_VERTICES];
    int total_vertices = 0;
    grafo G = le_rede_metabolica_de_arquivo(argv[1], mapa_inverso, &total_vertices);

    if (G == NULL) return 1;

    if (GRAPHcycle(G, mapa_inverso)) 
        printf("rede cíclica\n");
    else 
        imprime_ordenacao_topologica(G, mapa_inverso, total_vertices);
    
    destro_vet(mapa_inverso,total_vertices);
    destroi_grafo(G);

    return 0;
}

