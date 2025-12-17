#ifndef _DETECTAR_CICLO_
#define _DETECTAR_CICLO_
#include "grafo.h"
#include <stdbool.h>

#define NAO_VISITADO -1
#define MAX_VERTICES 5000

void imprime_ordenacao_topologica(grafo G, char* mapa_inverso[],int total_vertices);
bool GRAPHcycle(grafo G, char* mapa_inverso[]);
void destro_vet(char* mapa_inverso[],int total_vertices);

#endif
