#ifndef LEITOR_REDE_H
#define LEITOR_REDE_H

#include "grafo.h"
#define NAO_ENCONTRADO -1

enum tipo{REACAO,METABOLITO};

grafo le_rede_metabolica_de_arquivo(const char *caminho_arquivo, char* mapa_inverso[], int* contador_vertices_ptr);

#endif 