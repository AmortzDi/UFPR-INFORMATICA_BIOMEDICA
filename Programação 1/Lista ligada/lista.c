#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>

#include "lista.h"


struct lista *lista_cria (){
    struct lista *lista;

    if(!(lista = malloc(sizeof(struct lista)))){
        printf("ERRO AO CRIAR LISTA\n");
        return NULL;
    }

    lista->ini = NULL;
    lista->ptr = NULL;
    lista->tamanho = 0;

    return lista;
}

void lista_destroi(struct lista **lista) {
    if (lista == NULL || *lista == NULL)
        return;

    struct nodo *aux;
    struct nodo *p = (*lista)->ini;

    while (p != NULL) {
        aux = p;
        p = p->prox;
        free(aux);
    }

    free(*lista);
    *lista = NULL;
}

int lista_insere_inicio(struct lista *lista, int chave) {
    struct nodo *novo;

    if (!(novo = malloc(sizeof(struct nodo)))) {
        return 0;
    }

    novo->chave = chave;
    novo->prox = lista->ini;
    lista->ini = novo;
    (lista->tamanho)++;

    return 1;
}
int lista_insere_fim(struct lista *lista, int chave) {
    struct nodo *novo;

    if (!(novo = malloc(sizeof(struct nodo)))) {
        return 0;
    }

    novo->chave = chave;
    novo->prox = NULL;

    if (lista->ini == NULL) {
        lista->ini = novo;
    } else {
        struct nodo *aux = lista->ini;
        while (aux->prox != NULL)
            aux = aux->prox;
        aux->prox = novo;
    }

    (lista->tamanho)++;
    return 1;
}

int lista_insere_ordenado(struct lista *lista, int chave) {
    if (lista == NULL) {
        return 0;
    }

    struct nodo *novo = (struct nodo*)malloc(sizeof(struct nodo));
    if (novo == NULL) {
        return 0;
    }
    novo->chave = chave;

    if (lista->ini == NULL || lista->ini->chave >= chave) {
        novo->prox = lista->ini;
        lista->ini = novo;
    } else {
        struct nodo *atual = lista->ini;
        while (atual->prox != NULL && atual->prox->chave < chave) {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        atual->prox = novo;
    }

    lista->tamanho++;
    return 1;
}

int lista_remove_inicio(struct lista *lista, int *chave) {
    if (lista_vazia(lista)) {
        return 0;
    }

    struct nodo *aux = lista->ini;
    *chave = aux->chave;
    lista->ini = aux->prox;
    lista->tamanho--;
    free(aux);

    return 1;
}
int lista_remove_fim(struct lista *lista, int *chave) {
    if (lista_vazia(lista)) {
        return 0;
    }

    struct nodo *aux = lista->ini;
    struct nodo *anterior = NULL;

    while (aux->prox != NULL) {
        anterior = aux;
        aux = aux->prox;
    }
    
    if (anterior == NULL) {
        lista->ini = NULL;
    } else {
        anterior->prox = NULL;
    }

    *chave = aux->chave;
    lista->tamanho--;
    free(aux);

    return 1;
}

int lista_remove_ordenado(struct lista *lista, int chave) {
    if (lista_vazia(lista)) {
        return 0;
    }

    struct nodo *aux = lista->ini;
    struct nodo *anterior = NULL;

    while (aux != NULL && aux->chave != chave) {
        anterior = aux;
        aux = aux->prox;
    }
   
    if (aux == NULL) {
        return 0;
    }
    
    if (anterior == NULL) {
        lista->ini = aux->prox;
    } else {
        anterior->prox = aux->prox;
    }

    lista->tamanho--;
    free(aux);

    return 1;    
}

int lista_vazia(struct lista *lista) {
    return (lista == NULL || lista->ini == NULL);
}

int lista_tamanho(struct lista *lista) {
    if (lista_vazia(lista)) {
        return 0;
    }
    return lista->tamanho;
}

int lista_pertence(struct lista *lista, int chave) {
    if (lista_vazia(lista)) {
        return 0;
    }
    
    struct nodo *aux = lista->ini;

    while (aux != NULL && aux->chave != chave) {
        aux = aux->prox;
    }

    if (aux != NULL) {
        return 1;
    }

    return 0;
}

/*Função para inicializar o iterador*/
void lista_inicia_iterador(struct lista *lista) {
    lista->ptr = lista->ini;
}

/*Função para incrementar o iterador*/
int lista_incrementa_iterador(struct lista *lista, int *chave) {
    if (lista->ptr != NULL) {
        *chave = lista->ptr->chave;
        lista->ptr = lista->ptr->prox;
        return (lista->ptr != NULL) ? 1 : 0;
    }
    return 0;
}