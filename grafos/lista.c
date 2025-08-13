#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>

#include "lista.h"



struct lista *lista_cria() {
    struct lista *lista = (struct lista*)malloc(sizeof(struct lista));
    if (lista == NULL) {
        return NULL;
    }
    lista->ini = NULL;
    lista->ptr = NULL;
    lista->tamanho = 0;
    
    return lista;
}

void lista_destroi(struct lista **lista) {
    if (lista == NULL || *lista == NULL) {
        return;
    }
    struct nodo *atual = (*lista)->ini;
    while (atual != NULL) {
        struct nodo *prox = atual->prox;
        free(atual);
        atual = prox;
    }
    free(*lista);
    *lista = NULL;
}

int lista_vazia(struct lista *lista) {
    return lista->tamanho == 0;
}

int lista_tamanho(struct lista *lista) {
    return lista->tamanho;
}

int lista_insere_inicio (struct lista *lista, int chave){
   if (lista == NULL) {
        return 0;
    }
    struct nodo *novo = (struct nodo*)malloc(sizeof(struct nodo));
    if (novo == NULL) {
        return 0;
    }
    novo->chave = chave;
    novo->prox = lista->ini;
    lista->ini = novo;
    lista->tamanho++;
    return 1;
}

int lista_insere_fim(struct lista *lista, int chave) {
    if (lista == NULL) {
        return 0;
    }
    struct nodo *novo = (struct nodo*)malloc(sizeof(struct nodo));
    if (novo == NULL) {
        return 0;
    }
    novo->chave = chave;
    novo->prox = NULL;

    if (lista->ini == NULL) {
        lista->ini = novo;
    } else {
        struct nodo *atual = lista->ini;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novo;
    }
    lista->tamanho++;
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

int lista_pertence(struct lista *lista, int chave) {
    struct nodo *atual = lista->ini;
    while (atual != NULL) {
        if (atual->chave == chave) {
            return 1;
        }
        atual = atual->prox;
    }
    return 0;
}

int lista_remove_inicio(struct lista *lista, int *chave) {
    if (lista == NULL || lista->ini == NULL) {
        return 0;
    }
    struct nodo *remover = lista->ini;
    *chave = remover->chave;
    lista->ini = lista->ini->prox;
    free(remover);
    lista->tamanho--;
    return 1;
}

int lista_remove_fim(struct lista *lista, int *chave) {
    if (lista == NULL || lista->ini == NULL) {
        return 0;
    }
    struct nodo *atual = lista->ini;
    struct nodo *anterior = NULL;

    while (atual->prox != NULL) {
        anterior = atual;
        atual = atual->prox;
    }

    if (anterior == NULL) {
        lista->ini = NULL;
    } else {
        anterior->prox = NULL;
    }

    *chave = atual->chave;
    free(atual);
    lista->tamanho--;
    return 1;
}

int lista_remove_ordenado(struct lista *lista, int chave) {
    if (lista == NULL || lista->ini == NULL) {
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

    free(aux);
    lista->tamanho--;
    return 1;
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
        return 1;
    }
    return 0;
}