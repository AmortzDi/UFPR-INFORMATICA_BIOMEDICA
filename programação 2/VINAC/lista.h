#ifndef _lista_t_H
#define _lista_t_H

#include "vinac.h"

// Estrutura de um nó da lista duplamente encadeada.
// Armazena uma chave inteira, um ponteiro para os dados do tipo 'diretorio',
// e ponteiros para o nó anterior e o próximo na lista.
struct nodo {
  int chave;
  diretorio *info;
  struct nodo *prox;
  struct nodo *ant;
};

// Estrutura da lista duplamente encadeada.
// Guarda ponteiros para o início, fim e para um iterador (ptr) usado na iteração,
// além do tamanho atual da lista.
struct lista {
  struct nodo *ini;
  struct nodo *fim;
  struct nodo *ptr;
  int tamanho;
};

/**
 * Cria uma nova lista vazia.
 * 
 * return Ponteiro para a nova lista criada, ou NULL em caso de falha.
 */
struct lista *lista_cria();

/**
 * Libera toda a memória ocupada pela lista, incluindo os dados dos nós.
 * 
 * lista Ponteiro para o ponteiro da lista a ser destruída.
 */
void lista_destroi(struct lista **lista);

/**
 * Verifica se a lista está vazia.
 * 
 * lista Ponteiro para a lista.
 * return 1 se estiver vazia, 0 caso contrário.
 */
int lista_vazia(struct lista *lista);

/**
 * Retorna o número de elementos presentes na lista.
 * 
 * lista Ponteiro para a lista.
 * return Quantidade de elementos.
 */
int lista_tamanho(struct lista *lista);

/**
 * Insere um novo elemento na lista, respeitando a ordenação pela chave.
 * Evita duplicatas com base em critérios específicos do conteúdo.
 * 
 * lista Ponteiro para a lista.
 * chave Valor inteiro associado ao elemento (usado para ordenação).
 * info Ponteiro para a estrutura 'diretorio' que será armazenada.
 * return 1 em caso de sucesso, 0 se falhar ou for duplicado.
 */
int lista_insere(struct lista *lista, int chave, diretorio *info);

/**
 * Remove da lista o elemento que contém as mesmas informações do diretório fornecido.
 * 
 * lista Ponteiro para a lista.
 * chave Ponteiro para os dados do diretório a ser removido.
 * return 1 se a remoção for bem-sucedida, 0 caso contrário.
 */
int lista_remove(struct lista *lista, diretorio *chave);

/**
 * Inicializa o iterador da lista, posicionando-o no início.
 * Deve ser chamada antes de usar `lista_incrementa_iterador`.
 * 
 * lista Ponteiro para a lista.
 */
void lista_inicia_iterador(struct lista *lista);

/**
 * Avança o iterador da lista e retorna o próximo item.
 * 
 * lista Ponteiro para a lista.
 * info Ponteiro para onde o ponteiro do dado atual será armazenado.
 * return 1 se houver próximo elemento, 0 se o fim for alcançado.
 */
int lista_incrementa_iterador(struct lista *lista, diretorio **info);

/**
 * Imprime o conteúdo da lista, mostrando a chave e o nome do arquivo de cada item.
 * 
 * lista Ponteiro para a lista.
 */
void lista_imprime(struct lista *lista);
int lista_reordenar(struct lista *lista, const char *nome_a_mover, const char *nome_referencia);
#endif
