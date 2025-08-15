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
    // 1. Imprime os índices das colunas (cabeçalho)
    printf("     "); // Imprime um espaço inicial para alinhar com os índices das linhas
    for (int j = 0; j < n; j++) {
        printf("[%d]\t", j); // Imprime cada índice de coluna
    }
    printf("\n"); // Pula para a próxima linha

    // Imprime uma linha de separação para melhor visualização
    printf("----");
    for (int j = 0; j < n; j++) {
        printf("-----\t");
    }
    printf("\n");


    // 2. Itera sobre as linhas para imprimir o índice da linha e os dados
    for (int i = 0; i < n; i++) {
        // Imprime o índice da linha atual
        printf("[%d] |\t", i); 
        
        // Imprime os elementos da matriz para a linha atual
        for (int j = 0; j < n; j++) {
            printf("%d\t", matriz[i][j]); // \t para alinhar as colunas
        }
        printf("\n"); // Pula para a próxima linha ao final de cada linha da matriz
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


