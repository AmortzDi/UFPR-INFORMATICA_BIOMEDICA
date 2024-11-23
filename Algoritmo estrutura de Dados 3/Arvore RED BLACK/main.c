#include <stdio.h>

#include "arvore_rb.h"

int main() {
    // Inicializa a árvore
    struct arvore t;
    
     inicializar_arvore(&t);

    char tipo;
    int chave;

    // Lê a entrada e insere ou remove a chave da árvore rubro-negra
    while (scanf(" %c %d", &tipo, &chave) == 2) {
        if (tipo == 'i') {
            insere_elemento(&t, chave);  // Insere um elemento
        }
        else if (tipo == 'r') {
            // Chama a função buscar_no para encontrar o nó
            struct no *no_remover = buscar_no(&t, chave);
        
            if (no_remover != NULL) {
                rb_delete(&t, no_remover);  // Remove um elemento
                printf("Valor %d removido com sucesso!\n", chave);
            } else {
                printf("Valor %d não encontrado na árvore.\n", chave);
            }
        } else {
            printf("Comando inválido\n");
            return -1;
        }
    }

    // Imprime a árvore final após as inserções e remoções
    imprimir_arvore(&t);
    
    destroi_arvore(&t);
    
    return 0;
}