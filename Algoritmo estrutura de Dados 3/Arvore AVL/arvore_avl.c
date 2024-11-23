#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"myavl.h"


/* Função para criar um novo nó */
static struct no* cria_no(int chave) {
    struct no *n = malloc(sizeof(struct no));
    if (n == NULL) {
        return NULL;
    }
    n->chave = chave;
    n->filho_e = NULL;
    n->filho_d = NULL;
    n->pai = NULL;
    n->altura = 0; 
    return n; 
}
void cria_arvore(struct arvore *t) {
    t->raiz = NULL; // Inicializa a raiz como NULL
}


static void destroi_arvore_aux(struct no *x) {
    if (x == NULL)
        return;
    
    destroi_arvore_aux(x->filho_e);  
    destroi_arvore_aux(x->filho_d);
    free(x);
}
void destroi_arvore(struct arvore *t) {
    if (t->raiz != NULL) {
        destroi_arvore_aux(t->raiz); // Chama a função auxiliar
        t->raiz = NULL; // Define a raiz como NULL após a destruição
    }
}
/* funcao para rotacionar para a esquerda */
static struct no* rotacao_esq(struct arvore *t, struct no *x) {
    if (x == NULL || x->filho_d == NULL) {
        return NULL; 
    }

    struct no *y = x->filho_d;

    x->filho_d = y->filho_e;

    if (y->filho_e != NULL) 
        y->filho_e->pai = x;

    y->pai = x->pai;

    if (x->pai == NULL)
        t->raiz = y; 
    else if (x == x->pai->filho_e)
        x->pai->filho_e = y;
    else 
        x->pai->filho_d = y;

    y->filho_e = x;
    x->pai = y;

    return y; // Retornando o novo nó que se tornou o pai
}

/* funcao para rotacionar para a direita */
static struct no* rotacao_dir(struct arvore *t, struct no *x) {
    if (x == NULL || x->filho_e == NULL) {
        return NULL; // Retorno adequado se a rotação não puder ser realizada
    }

    struct no *y = x->filho_e;
    
    x->filho_e = y->filho_d;

    if (y->filho_d != NULL) 
        y->filho_d->pai = x;
    
    y->pai = x->pai;

    if (x->pai == NULL) 
        t->raiz = y; 
    else if (x == x->pai->filho_d) 
        x->pai->filho_d = y;
    else 
        x->pai->filho_e = y;

    y->filho_d = x;
    x->pai = y;

    return y;
}

static struct no* rotacao_e_d(struct arvore *t, struct no *n){
    n->filho_e = rotacao_esq(t,n->filho_e);
    return rotacao_dir(t,n);
}

static struct no* rotacao_d_e(struct arvore *t, struct no *n){
    n->filho_d = rotacao_dir(t,n->filho_d);
    return rotacao_esq(t,n);
}
static int verifica_balanceamento(struct no *n) {
    if (n == NULL) {
        return 0; // Se o nó é nulo, o fator de balanceamento é 0
    }

    int altura_esq = (n->filho_e != NULL) ? n->filho_e->altura : -1; // Altura do filho esquerdo
    int altura_direita = (n->filho_d != NULL) ? n->filho_d->altura : -1; // Altura do filho direito

    return altura_esq - altura_direita; // Retorna a diferença de altura
}

/* Função para verificar o balanceamento e balancear a árvore */
static struct no* balanciamento(struct arvore *t, struct no *x){
    int fb = verifica_balanceamento(x);

    if(fb < -1 && verifica_balanceamento(x->filho_d) <= 0)
        x = rotacao_esq(t, x);

    else if (fb > 1 && verifica_balanceamento(x->filho_e) >= 0)
        x = rotacao_dir(t,x);

    else if( fb > 1 && verifica_balanceamento(x->filho_e) < 0)
        x = rotacao_e_d(t,x);

    else if( fb < -1 && verifica_balanceamento(x->filho_d) > 0)
        x = rotacao_d_e(t,x);

    return x;
}

/* Função para atualizar a altura de um nó */
static void atualiza_altura(struct no *n) {
    if (n == NULL) return; // Se o nó for nulo, não faz nada

    // Obtém as alturas dos filhos, tratando o caso de nós nulos
    int altura_esq = (n->filho_e ? n->filho_e->altura : 0);
    int altura_direita = (n->filho_d ? n->filho_d->altura : 0);

    // Atualiza a altura do nó atual
    n->altura = (altura_esq > altura_direita ? altura_esq : altura_direita) + 1;
}

/* Função de inserção recursiva */
static struct no* insere_nodo_rec(struct arvore *T, struct no *n, int z) {
    if (n == NULL) {
        struct no *r = cria_no(z);
        return r;
    }

    if (z < n->chave) {
        n->filho_e = insere_nodo_rec(T, n->filho_e, z);
        n->filho_e->pai = n; // Atualiza o pai
    } else if (z > n->chave) {
        n->filho_d = insere_nodo_rec(T, n->filho_d, z);
        n->filho_d->pai = n; // Atualiza o pai
    } else {
        return n; // Não insere duplicados
    }

    // Atualiza a altura do nó atual
    atualiza_altura(n);

    // Realiza o balanceamento e retorna o nó balanceado
    return balanciamento(T, n);
}

/* Função para inserir um nó na árvore */
void insere_nodo(struct arvore *t, int z) {
    if (t->raiz == NULL) {
        // Se a árvore está vazia, criamos a raiz
        t->raiz = cria_no(z);
    } else {
        // Caso contrário, chamamos a função recursiva para inserir
        insere_nodo_rec(t, t->raiz, z);
    }
}

/* Função para imprimir a árvore em ordem */
static void imprime_arvore_aux(struct no *raiz, int nivel) {
    if (raiz) {
        imprime_arvore_aux(raiz->filho_e, raiz->altura); // Chama recursivamente o filho à esquerda

        // Imprime o valor do nó e o nível
        printf("%d, %d\n", raiz->chave, nivel);
       
        imprime_arvore_aux(raiz->filho_d, raiz->altura); // Chama recursivamente o filho à direita
    }
}

void imprime_arvore(struct arvore *t) {
    if (t->raiz != NULL) {
        imprime_arvore_aux(t->raiz, NULL); // Chama a função auxiliar, começando do nível 0
    }
}
static void atualizar_balanceamento(struct arvore *t, struct no *n) {
    // Percorre para cima até a raiz, atualizando o balanceamento
    while (n != NULL) {
        // Atualiza a altura do nó n
        atualiza_altura(n);

        // Verifica o fator de balanceamento do nó n
        int fator_balanceamento = verifica_balanceamento(n);

        // Verifica se o nó está desbalanceado
        if (fator_balanceamento < -1 || fator_balanceamento > 1) {
            n = balanciamento(t, n); // Rebalanceia a árvore
        }

        n = n->pai; // Sobe para o pai
    }
}
static void transplante(struct arvore *t, struct no *u, struct no *v) {
    if (u->pai == NULL) {
        t->raiz = v; // Se u é a raiz, v se torna a nova raiz
    } else if (u == u->pai->filho_e) {
        u->pai->filho_e = v; // Substitui o filho esquerdo
    } else {
        u->pai->filho_d = v; // Substitui o filho direito
    }
    if (v != NULL) {
        v->pai = u->pai; // Atualiza o pai de v
    }
}



static struct no* busca_recursiva(struct no *atual, int chave) {
    if (atual == NULL) {
        return NULL; // Nó não encontrado
    }

    if (chave < atual->chave) {
        return busca_recursiva(atual->filho_e, chave); // Move para a subárvore esquerda
    } else if (chave > atual->chave) {
        return busca_recursiva(atual->filho_d, chave); // Move para a subárvore direita
    } else {
        return atual; // Nó encontrado
    }
}
static struct no* busca(struct arvore *t, int chave) {
    return busca_recursiva(t->raiz, chave);
}

static struct no* menor_elemento(struct no *n) {
    while (n->filho_e != NULL) {
        n = n->filho_e; // Encontra o menor elemento
    }
    return n;
}

static struct no* eleminar(struct arvore *t, struct no *z) {
    struct no *y;

    if (z->filho_e == NULL) {
        transplante(t, z, z->filho_d); // Substitui z por seu filho direito
    } else if (z->filho_d == NULL) {
        transplante(t, z, z->filho_e); // Substitui z por seu filho esquerdo
    } else {
        y = menor_elemento(z->filho_d); // Encontra o menor elemento da subárvore direita
        if (y->pai != z) {
            transplante(t, y, y->filho_d); // Substitui y por seu filho direito
            y->filho_d = z->filho_d; // Conecta o filho direito de z a y
            y->filho_d->pai = y; // Atualiza o pai de y->filho_d
        }
        transplante(t, z, y); // Substitui z por y
        y->filho_e = z->filho_e; // Conecta o filho esquerdo de z a y
        y->filho_e->pai = y; // Atualiza o pai de y->filho_e
    }

    atualizar_balanceamento(t, z->pai); // Atualiza o balanceamento

    return z; // Retorna o nó removido
}

void remove_no(struct arvore *t, int chave) {
    // Busca o nó a ser removido
    struct no *nodoRemover = busca(t, chave);

    if (nodoRemover != NULL) {
        // Chama a função de remoção
        eleminar(t, nodoRemover);
    }
}

