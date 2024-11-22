#include <stdio.h>
#include <stdlib.h>
#include "fila.h"
/* 
 * Cria e retorna uma nova fila.
 * Retorna NULL em caso de erro de alocação.
*/
struct fila *fila_cria (){
    struct fila *fila;

    if(!(fila = malloc(sizeof(struct fila))))
        return NULL;

    fila->ini = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;

    return fila;
}

/* Desaloca toda memoria da fila e faz fila receber NULL. */
void fila_destroi (struct fila **fila){
    struct nodo *aux_remove = (*fila)->ini;
    struct nodo *atual;

    while(aux_remove != NULL){
        atual = aux_remove->prox;
        free(aux_remove);
        aux_remove = atual;

    }

    free(*fila);
    *fila = NULL;
}

/* 
 * Insere dado na fila (politica FIFO). Retorna 1
 * em caso de sucesso e 0 em caso de falha.
*/
int enqueue (struct fila *fila, int dado){
    struct nodo *novo;

    if(!(novo = malloc(sizeof(struct nodo))))
        return 0;

    novo->chave = dado;
    novo->prox = NULL;

    if(fila->fim == NULL){
        fila->ini = novo;
        fila->fim = novo;
    }else{
        fila->fim->prox = novo;
        fila->fim = novo;
    }

    fila->tamanho++;
    return 1;
}

/* 
 * Remove dado da fila (politica FIFO) e retorna o elemento 
 * no parametro dado. A funcao retorna 1 em caso de 
 * sucesso e 0 no caso da fila estar vazia.
*/
int dequeue (struct fila *fila, int *dado){
    struct nodo *remove = fila->ini;

    if(fila->ini == NULL)
        return 0;

    *dado = remove->chave;
    fila->ini = remove->prox;

    if(fila->ini == NULL)
        fila->fim = NULL;

    free(remove);
    fila->tamanho--;

    return 1;
}
 
/* Retorna o numero de elementos da pilha, que pode ser 0. */
int fila_tamanho (struct fila *fila){
    return fila->tamanho;
}

/* Retorna 1 se fila vazia, 0 em caso contrario. */ 
int fila_vazia (struct fila *fila){
    if (fila->tamanho == 0)
        return 1;
    return 0;
}