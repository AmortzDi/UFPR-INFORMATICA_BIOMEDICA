#include<stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "lef.h"

/* 
 * Cria um evento contendo os parâmetros informados.
 * Retorna um ponteiro para o evento ou NULL se falhar.
 */
struct evento_t *cria_evento (int tempo, int tipo, int dado1, int dado2){
    struct evento_t *e;

    if(!(e = malloc(sizeof(struct evento_t))))
        return NULL;

    e->tempo = tempo;
    e->tipo = tipo;
    e->dado1 = dado1;
    e->dado2 = dado2;

    return e;
}

/* 
 * Destroi um evento e retorna NULL.
 */ 
struct evento_t *destroi_evento (struct evento_t *e){
    
    if(e){
        free(e);
        return NULL;
    }

    return NULL;
}

/* 
 * Cria uma LEF vazia 
 * Retorna um ponteiro para a LEF ou NULL se falhar.
 */
struct lef_t *cria_lef (){
    struct lef_t *lef;

    if(!(lef = malloc(sizeof(struct lef_t))))
        return NULL;

    lef->primeiro = NULL;

    return lef;
}

/* 
 * Destroi a LEF, liberando a memória ocupada por ela, inclusive os eventos.
 * Retorna NULL.
 */ 
struct lef_t *destroi_lef (struct lef_t *l){
    if(l){
        struct nodo_lef_t *aux = l->primeiro;
        struct nodo_lef_t *atual;
        while (aux != NULL){
            atual = aux->prox;
            free(aux);
            aux = atual;
        }
        free(l);
    }
    return NULL;
}

/*
 * Insere o evento apontado na LEF na ordem de tempos crescentes.
 * Eventos com o mesmo tempo devem respeitar a politica FIFO.
 * Retorna 1 em caso de sucesso ou 0 caso contrario.
*/
int insere_lef (struct lef_t *l, struct evento_t *e){

    if(!l || !e)
        return 0;
    
    struct nodo_lef_t *novo;

    if(!(novo = malloc(sizeof(struct nodo_lef_t))))
        return 0;
    
    novo->evento = e;
    novo->prox = NULL;

    if(!l->primeiro)
        l->primeiro = novo;
    else{
        struct nodo_lef_t *aux = l->primeiro;
        struct nodo_lef_t *atual = NULL;

        while (aux && aux->evento->tempo <= e->tempo){
            atual = aux;
            aux = atual->prox;
        }

        if(!atual){
            novo->prox = l->primeiro;
            l->primeiro = novo;
        }else{
            atual->prox = novo;
            novo->prox = aux;
        }  
    }
    return 1;
}

/* 
 * Retira o primeiro evento da LEF.
 * Retorna ponteiro para o evento ou NULL se falhar.
 */
struct evento_t *retira_lef (struct lef_t *l){
    /* Retorna NULL se a lista estiver vazia ou for nula*/
    if (!l || !l->primeiro) 
        return NULL;
    

    struct nodo_lef_t *aux_no = l->primeiro;
    struct evento_t *ev_retirado = aux_no->evento;

    l->primeiro = aux_no->prox; 

    free(aux_no); 

    return ev_retirado; 
}
  
/* 
 * Informa se a LEF está vazia.
 * Retorna 1 se vazia e 0 senao ou se falhar.
 */
int vazia_lef(struct lef_t *l) {
    if (!l) 
        return -1;

    if (!l->primeiro) 
        return 1; 
     else 
        return 0;       
}
/* 
 * Imprime a LEF. Não faz parte do TAD, mas serve para depuração.
 * Formato de saída:
 * tempo tipo dado1 dado2
 * tempo tipo dado1 dado2
 * ...
 * total XX eventos
 */
void imprime_lef (struct lef_t *l){ 

    struct nodo_lef_t *aux = l->primeiro;
    int total_ev = 0;

    if (!l || !l->primeiro) {
        printf("LEF:\n");
        printf("  Total %d eventos\n", total_ev);        
        return;
    }else{
        printf("LEF:\n");
    while (aux) {
        struct evento_t *ev = aux->evento;
        printf("  ");
        printf("tempo %d tipo %d d1 %d d2 %d\n", ev->tempo, ev->tipo, ev->dado1, ev->dado2);
        total_ev++;
        aux = aux->prox;
    }
    printf("  ");
    printf("Total %d eventos\n", total_ev);
    }
}