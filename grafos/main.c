#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

#include "aula1.h"
#include "lista.h"

int main() {
    int n,u,v; // Variável para a dimensão da matriz

    printf("--- Funções para Matrizes: Inicializar e Copiar ---\n\n");

    // 1. Obter o tamanho da matriz
    printf("Digite o tamanho N para a matriz quadrada (N x N): \n");
    scanf("%d", &n);

    // Validação simples para evitar tamanhos negativos ou zero
    if (n <= 0) {
        printf("O tamanho da matriz deve ser um número positivo.\n");
        return 1; // Termina o programa com erro
    }

    // 2. Declarar as matrizes
    int matrizOriginal[n][n];
    int matrizCopia[n][n];
    int matrizResultado[n][n];

    // 3. Chamar a função para preencher a matriz original com zeros
    printf("\nInicializando a matriz original com zeros...\n");
    inicializarComZeros(n, matrizOriginal);

    // Imprimir a matriz original para verificar
    printf("--- Matriz Original (após inicialização) ---\n");
    imprimirMatriz(n, matrizOriginal);

    printf("--Digite dois vertices que tem uma areasta entre eles--");
    do{
    scanf("%d %d", &u,&v);
    colocarUm(n, matrizOriginal, u,v);
    }while ((u != 0 || v != 0));    
    
    // ---------------------------------------------------------------------

    // 4. Chamar a função para copiar a matriz original para a matrizCopia
    printf("\nCopiando a matriz original para uma nova matriz...\n");
    copiarMatriz(n, matrizOriginal, matrizCopia);

    printf("--- Matriz Original ---\n");
    imprimirMatriz(n, matrizOriginal);


    // 5. Imprimir a matriz cópia para verificar
    printf("--- Matriz Cópia ---\n");
    imprimirMatriz(n, matrizCopia);


    printf("--- Matriz multiplicada ---\n");
    multiplicarMatrizes(n,matrizOriginal,matrizCopia,matrizResultado);
    imprimirMatriz(n, matrizResultado);
    
  

    return 0; // Sucesso
}
