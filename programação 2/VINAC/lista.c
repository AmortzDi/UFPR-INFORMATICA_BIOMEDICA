#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<string.h>

#include "lista.h"
#include "vinac.h"



struct lista *lista_cria() {
    struct lista *lista = (struct lista*)malloc(sizeof(struct lista));
    if (lista == NULL) {
        return NULL;
    }
    lista->ini = NULL;
    lista->fim = NULL; 
    lista->ptr = NULL;
    lista->tamanho = 0;
    return lista;
}


void lista_destroi(struct lista **lista) {
    if (!lista || !*lista) return;

    struct nodo *atual = (*lista)->ini;
    while (atual) {
        struct nodo *prox = atual->prox;
        
        // Libera o conteudo do no (diretorio)
        if (atual->info) {
            free(atual->info);
            atual->info = NULL; 
        }
        
        // Libera o no
        free(atual);
        atual = prox;
    }

    free(*lista);
    *lista = NULL;
}

 int verifica_duplicado(struct lista *lista, diretorio *novo) {
    if (!lista || !novo || lista_vazia(lista))
        return 0;

    struct nodo *atual = lista->ini;

    while (atual) {
        struct nodo *prox = atual->prox;

        if (strcmp(atual->info->nome_arquivo, novo->nome_arquivo) == 0 &&
            atual->info->d_uid == novo->d_uid) {

            printf("Arquivo duplicado encontrado: %s\n", atual->info->nome_arquivo);
            printf("Arquivo %s ja existe na lista com os mesmos dados:\n", novo->nome_arquivo);
            printf("UID: %u, Tamanho: %lu bytes\n", atual->info->d_uid, atual->info->d_tam_ori);
            printf("Data de Modificacao: %s", ctime(&atual->info->d_data_modi));

            if (atual->info->d_data_modi < novo->d_data_modi) {
                // Se o que js estava na lista eh mais velho, remove ele
                lista_remove(lista, atual->info);
                // Continua para checar se tem mais duplicados
            } else {
                // Se o novo é mais velho, nao insere o novo
                return 1;
            }
        }

        atual = prox;
    }

    return 0;
}


int lista_vazia(struct lista *lista) {
    return lista->tamanho == 0;
}

int lista_tamanho(struct lista *lista) {
    return lista->tamanho;
}


int lista_insere(struct lista *lista, int chave, diretorio *info) {
    if (!lista) return 0;

    if (verifica_duplicado(lista, info))
        return 0; // duplicado mais novo, entao nao insere

    struct nodo *novo = malloc(sizeof(struct nodo));
    if (!novo)
        return 0;

    novo->chave = chave;
    novo->info = info;
    novo->prox = NULL;
    novo->ant = NULL;

    if (lista_vazia(lista)) {
        lista->ini = lista->fim = novo;
        lista->tamanho++;
        return 1;
    }

    if (chave < lista->ini->chave) {
        novo->prox = lista->ini;
        lista->ini->ant = novo;
        lista->ini = novo;
        lista->tamanho++;
        return 1;
    }

    if (chave > lista->fim->chave) {
        novo->ant = lista->fim;
        lista->fim->prox = novo;
        lista->fim = novo;
        lista->tamanho++;
        return 1;
    }

    struct nodo *atual = lista->ini;
    while (atual && atual->chave < chave) {
        atual = atual->prox;
    }

    novo->prox = atual;
    novo->ant = atual->ant;
    if (atual->ant) atual->ant->prox = novo;
    atual->ant = novo;

    lista->tamanho++;
    return 1;
}


int lista_remove(struct lista *lista, diretorio *chave) {
    // Verificacao robusta de parametros
    if (!lista || lista_vazia(lista) || !chave) {
        return 0;
    }

    struct nodo *atual = lista->ini;
    
    // Busca pelo no a ser removido
    while (atual) {
        // Verificacao segura de ponteiros
        if (atual->info && 
            strcmp(atual->info->nome_arquivo, chave->nome_arquivo) == 0 &&
            atual->info->d_uid == chave->d_uid &&
            atual->info->d_tam_ori == chave->d_tam_ori &&
            atual->info->d_data_modi == chave->d_data_modi) {
            break;
        }
        atual = atual->prox;
    }

    // Se nao encontrou o no
    if (!atual) {
        return 0;
    }

    // Ajuste dos ponteiros da lista
    if (atual->ant) {
        atual->ant->prox = atual->prox;
    } else {
        lista->ini = atual->prox;
    }

    if (atual->prox) {
        atual->prox->ant = atual->ant;
    } else {
        lista->fim = atual->ant;
    }

    // Ajuste do iterador se necessario
    if (lista->ptr == atual) {
        lista->ptr = atual->prox;
    }

    // Liberacao segura da memoria
    if (atual->info) {
        free(atual->info);
    }
    free(atual);
    lista->tamanho--;

    return 1;
}

void lista_inicia_iterador(struct lista *lista) {
    if (!lista || lista_vazia(lista))
        return;
    lista->ptr = lista->ini;
}

int lista_incrementa_iterador(struct lista *lista, diretorio **info) {
    if (!lista || !lista->ptr)
        return 0;
    
    *info = lista->ptr->info;
    lista->ptr = lista->ptr->prox;
    
    return 1;
}

void lista_imprime(struct lista *lista) {
    if (!lista || lista_vazia(lista)) {
        printf("\n  --- LISTA VAZIA ---\n");
        return;
    }

    printf("\n  LISTA (%d elementos):\n", lista->tamanho);
    printf("  +-------+--------------------------+----------------+----------------+\n");
    printf("  | CHAVE |        ARQUIVO          | TAM. ORIGINAL  | TAM. COMPRIM.  |\n");
    printf("  +-------+--------------------------+----------------+----------------+\n");

    struct nodo *atual = lista->ini;
    while (atual) {
        printf("  | %-5d | %-24s | %-14ld | %-14ld |\n", 
               atual->chave, 
               atual->info->nome_arquivo,
               atual->info->d_tam_ori, 
               atual->info->d_tam_com);
        atual = atual->prox;
    }

    printf("  +-------+--------------------------+----------------+----------------+\n");
}


// Função auxiliar para encontrar um nodo pelo nome
static struct nodo* busca_nome(struct lista *lista, const char *nome) {
    if (!lista || !nome) return NULL;
    
    struct nodo *n = lista->ini;
    while (n) {
        if (n->info && strcmp(n->info->nome_arquivo, nome) == 0) {
            return n;
        }
        n = n->prox;
    }
    return NULL;
}

// Remove um nodo da lista
static void remover_nodo(struct lista *lista, struct nodo *nodo) {
    if (!lista || !nodo) return;
    
    if (nodo->ant) {
        nodo->ant->prox = nodo->prox;
    } else {
        lista->ini = nodo->prox;
    }
    
    if (nodo->prox) {
        nodo->prox->ant = nodo->ant;
    } else {
        lista->fim = nodo->ant;
    }
    
    lista->tamanho--;
}

// Insere um nodo após outro nodo específico
static void inserir_nodo_apos(struct lista *lista, struct nodo *alvo, struct nodo *novo) {
    if (!lista || !alvo || !novo) return;
    
    novo->prox = alvo->prox;
    novo->ant = alvo;
    
    if (alvo->prox) {
        alvo->prox->ant = novo;
    } else {
        lista->fim = novo;
    }
    
    alvo->prox = novo;
    lista->tamanho++;
}

// Reordena a lista movendo um membro para após a referência
int lista_reordenar(struct lista *lista, 
                   const char *nome_a_mover, 
                   const char *nome_referencia) {
    // Verificações iniciais
    if (!lista || !nome_a_mover || !nome_referencia) return 1;

    // Encontra os nodos correspondentes
    struct nodo *nodo_mover = busca_nome(lista, nome_a_mover);
    struct nodo *nodo_ref = busca_nome(lista, nome_referencia);

    if (!nodo_mover || !nodo_ref) return 1;
    
    // Verifica se já está na posição correta
    if (nodo_ref->prox == nodo_mover) return 0;

    // Remove o nodo a ser movido da posição atual
    remover_nodo(lista, nodo_mover);
    
    // Reinsere após o nodo de referência
    inserir_nodo_apos(lista, nodo_ref, nodo_mover);
    
    return 0;
}