#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"leitor_rede.h"
#include"tabela.h"
#include"grafo.h"



/**
 * Função auxiliar para obter o ID de um vértice.
 *
 * Se o vértice já existe na tabela hash, retorna seu ID.
 * Se não, cria um novo vértice no grafo, o insere na tabela hash,
 * e retorna o novo ID.
 */
static int criar_vertice(const char *nome, int tipo, grafo G, 
                        struct tabela_hash T1[], struct tabela_hash T2[],
                        int *prox_id, char *mapa[]){
    int id = busca(T1,T2,nome);

    //vertice nao existe, precisa ser criado
    if(id == NAO_ENCONTRADO){
        id = *prox_id;
        insere(T1,T2,nome,id);
        adiciona_vertice(id, tipo, G);

        mapa[id] = strdup(nome);
        if(mapa[id] == NULL){
            fprintf(stderr, "Erro de alocação de memória para o mapa inverso.\n");
            exit(EXIT_FAILURE);
        }
        (*prox_id)++;
    }
    return id;
}
grafo le_rede_metabolica_de_arquivo(const char *caminho_arquivo, char* mapa_inverso[], int* contador_vertices_ptr) {
    FILE *arq_met = fopen(caminho_arquivo, "r");
    if(!arq_met){
        perror("Erro ao abrir o arquivo da rede metabólica");
        return NULL;
    }

    grafo G = cria_grafo();
    struct tabela_hash T1[TAM_MAX] = {0};
    struct tabela_hash T2[TAM_MAX] = {0};
    int prox_id_vertice = 0;
    int prox_id_aresta = 0;

    char linha[1024];
    while(fgets(linha, sizeof(linha), arq_met)){
        char nome_reacao[TAM_NOME];
        char resto_linha[1000];

        /* %s: Lê a primeira "palavra" da linha (R00214) e a armazena em nome_reacao.
        *  %[^\n]: Lê todo o resto da linha até o final e armazena em resto_linha.
        */
        if(sscanf(linha, "%s %[^\n]",nome_reacao,resto_linha) != 2) continue;

        int id_reacao = criar_vertice(nome_reacao, REACAO, G, T1, T2, &prox_id_vertice, mapa_inverso);
        
        char* parte_substratos = strtok(resto_linha, "->");
        char* parte_produtos = strtok(NULL, "->");
        if (!parte_substratos || !parte_produtos) continue;

        char* token = strtok(parte_substratos," +");
        while (token != NULL){
            if (strlen(token) > 0) {
                int id_substrato = criar_vertice(token, METABOLITO, G, T1, T2, &prox_id_vertice, mapa_inverso);
                adiciona_aresta(prox_id_aresta++, id_substrato, id_reacao, G);
            }
            token = strtok(NULL, " +");
        }

        token = strtok(parte_produtos, " +");
        while (token != NULL) {
            if (strlen(token) > 0) {
                int id_produto = criar_vertice(token, METABOLITO, G, T1, T2, &prox_id_vertice, mapa_inverso);
                adiciona_aresta(prox_id_aresta++, id_reacao, id_produto, G);
            }
            token = strtok(NULL, " +");
        }
    }
    
    fclose(arq_met);
    *contador_vertices_ptr = prox_id_vertice;
    return G;
}