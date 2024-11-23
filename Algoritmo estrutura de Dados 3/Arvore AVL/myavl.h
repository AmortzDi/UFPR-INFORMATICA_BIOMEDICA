#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* estrutura de um no da arvore */
struct no {
    int chave; /* valor do no */
    struct no *filho_d, *filho_e, *pai;/* filho esquerdo */
    int altura; /* nivel do no */
};

/* estrutura de uma arvore */
struct arvore {
    struct no *raiz; /* raiz da arvore */
    int altura_arvo; /* nivel da arvore */
};

/* funcao para adicionar um no na arvore */
/* funcao para inicializar a arvore */
void cria_arvore(struct arvore *t);

/* funcao para retirar um no na arvore */
/* Retorna o no removido */
void remove_no(struct arvore *t, int chave);

/* funcao para inserir um no na arvore */
void insere_nodo(struct arvore *T, int z);

/* funcao para imprimir a arvore */
void imprime_arvore(struct arvore *t);

/* funcao para destruir a arvore */
void destroi_arvore(struct arvore *t) ;

#endif /* AVL_H */
