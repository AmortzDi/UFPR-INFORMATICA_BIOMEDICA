#include <stdio.h>
#include <stdlib.h>
#include "myavl.h"


int main() {
    struct arvore raiz; // Inicializa a raiz da árvore

    int valor;
    char comando;
    cria_arvore(&raiz);

    // Loop para receber comandos
    while (scanf(" %c", &comando) == 1 && scanf("%d", &valor) == 1) {
        if (comando == 'i') { // Insere um valor
            insere_nodo(&raiz, valor);
        } else if (comando == 'r') { // Remove um valor
            remove_no(&raiz, valor);
        }
    }

    // Imprime a árvore em ordem

    imprime_arvore(&raiz);

    // Desaloca a árvore
    destroi_arvore(&raiz); // Função para desalocar a memória

    return 0;
}
