#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tabela.h"
/* funcao de hash para a tabela 2 */

// Funcao de hash de string
static unsigned long hash_string(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash * 32) + hash) + c;
    return hash;
}

/* funcao de hash para a tabela 1 */
int h1(const char *chave) {
    return hash_string(chave) % TAM_MAX;
}

static unsigned long hash_sd(const char * str){
    unsigned long hash = 0;
    int c;
    while ((c = *str++))
        hash = c + (hash * 64) + (hash * 65536) - hash; // Outro algoritmo comum
    return hash;
}
//impressão digital numérica para uma string
int h2(const char *chave) {
   return hash_sd(chave) % TAM_MAX;
}


/* funcao de busca em ambas as tabelas */
int busca(struct tabela_hash *T1, struct tabela_hash *T2, const char *nome) {
    /* busca primeiro na tabela 1 */
    int i1 = h1(nome);

    //se string sao iguais
    if (T1[i1].estado == OCUPADO && strcmp(T1[i1].nome, nome) == 0) {
        return T1[i1].id_vertice;  
    }

    // se nao estiver na tabela 1 busca na tabela 2 
    int i = h2(nome);
    if (T2[i].estado == OCUPADO && strcmp(T2[i].nome, nome) == 0) { 
        return T2[i].id_vertice;
    }
  
    //retorna -1 se nao encontrar a nome sendo buscada 
    return -1;
}


/* funcao para inserir na tabela 1 ou na tabela 2 */
void insere(struct tabela_hash *T1, struct tabela_hash *T2, const char *nome, int id_vertice) {    

    if(busca(T1,T2,nome)!= -1){
        return; //ja existe
    }

    struct tabela_hash item_atual;
    strncpy(item_atual.nome, nome, TAM_NOME - 1);
    item_atual.nome[TAM_NOME - 1] = '\0';
    item_atual.id_vertice = id_vertice;
    item_atual.estado = OCUPADO;

    for(int ten = 0; ten < TENT_MAX; ten++){
        // Tenta inserir na Tabela 1
        int pos1 = h1(item_atual.nome);
        //Se o slot esta vazio ou removido
        if(T1[pos1].estado != OCUPADO){
            T1[pos1] = item_atual;
            return;
        }

        // Se a posicao esta ocupada, expulsa o item antigo
        struct tabela_hash item_expulso = T1[pos1];
        T1[pos1] = item_atual;
        item_atual = item_expulso;

        // Tenta inserir o item expulso na Tabela 2
        int pos2 = h2(item_atual.nome);
        if(T2[pos2].estado != OCUPADO){
            T2[pos2] = item_atual;
            return;
        }

        // Se a posicao 2 também esta ocupada, expulsa e continua o loop
        item_expulso = T2[pos2];
        T2[pos2] = item_atual;
        item_atual = item_expulso;

    }
    
}

/* funcao para remover uma chave de uma das tabelas */
void remover(struct tabela_hash *T1, struct tabela_hash *T2, const char *nome) {
    //Verifica a tabela 1
    int i1 = h1(nome);
    if(T1[i1].estado == OCUPADO && strcmp(T1[i1].nome, nome) == 0){
        T1[i1].estado = REMOVIDO;
        return;
    }

    //verifica a tabela 2
    int i2 = h2(nome);
    if(T2[i2].estado == OCUPADO && strcmp(T2[i2].nome,nome) == 0){
        T2[i2].estado = REMOVIDO;
        return;
    }
}

static int compara_nomes(const void *a,const void *b){
    struct tabela_hash *item_a = (struct tabela_hash *)a;
    struct tabela_hash *item_b = (struct tabela_hash *)b;
    return strcmp(item_a->nome, item_b->nome);
}
/* funcao para imprimir as tabelas de forma ordenada */
void imprime(struct tabela_hash *T1, struct tabela_hash *T2) {
    struct tabela_hash temp[TAM_MAX * 2];
    int cont = 0;
   
   // Coleta todos os itens válidos  de ambas as tabelas
   for(int i = 0; i < TAM_MAX; i++){
    if(T1[i].estado == OCUPADO) temp[cont++] = T1[i];
    
    if(T2[i].estado == OCUPADO) temp[cont++] = T2[i];
   }

   qsort(temp,cont,sizeof(struct tabela_hash),compara_nomes);

   printf("--- Conteúdo da Tabela Hash ---\n");
    for (int i = 0; i < cont; i++) {
        int pos_h1 = h1(temp[i].nome);
        
        // Verifica em qual tabela o item está realmente armazenad
        if (T1[pos_h1].estado == OCUPADO && strcmp(T1[pos_h1].nome, temp[i].nome) == 0) 
            printf("ID: %d, Nome: %s, Local: T1[%d]\n", temp[i].id_vertice, temp[i].nome, pos_h1);
        else {
            int pos_h2 = h2(temp[i].nome);
            printf("ID: %d, Nome: %s, Local: T2[%d]\n", temp[i].id_vertice, temp[i].nome, pos_h2);
        }
    }
    printf("-----------------------------\n");
}
   
