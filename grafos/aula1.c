#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

#include "aula1.h"
#include "lista.h"

void preencher_matriz(int n, int matriz[n][n]){
    for(int l = 0; l < n; l++)
        for(int c = 0; c < n; n++)
            matriz[l][c] = 0;
}

void imprimirMatriz(int n, int matriz[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d\t", matriz[i][j]); // \t para alinhar as colunas
        }
        printf("\n"); // Pula para a próxima linha
    }
}
void copiarMatriz(int n, int matrizOriginal[n][n], int matrizCopia[n][n]){
    for(int l = 0; l < n; l++)
        for(int c = 0; c < n; c++){
            matrizCopia[l][c] = matrizOriginal[l][c];

        }
}
void inicializarComZeros(int n, int matriz[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matriz[i][j] = 0;
        }
    }

}
int verifica(int n, int matriz[n][n], int u, int v) {
    return matriz[u][v];
}
void colocarUm(int n, int matriz[n][n], int u, int v){

   if (verifica(n, matriz, u, v) == 0) {
        // Adiciona a aresta nos dois sentidos (grafo não direcionado)
        matriz[u][v] = 1;
        matriz[v][u] = 1;
        printf("--> Aresta (%d, %d) foi adicionada com sucesso.\n", u, v);
    } else {
        printf("--> Aresta (%d, %d) já existe. Nenhuma alteração foi feita.\n", u, v);
    }
}
void multiplicarMatrizes(int n, int matrizA[n][n], int matrizB[n][n], int matrizResultado[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrizResultado[i][j] = 0; // Inicializa o elemento do resultado
            for (int k = 0; k < n; k++) {
                matrizResultado[i][j] += matrizA[i][k] * matrizB[k][j];
            }
        }
    }
}
int main() {
    int n,u,v; // Variável para a dimensão da matriz

    printf("--- Funções para Matrizes: Inicializar e Copiar ---\n\n");

    // 1. Obter o tamanho da matriz
    printf("Digite o tamanho N para a matriz quadrada (N x N): ");
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



