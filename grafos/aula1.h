#ifndef _matrix__
#define _matrix__

void preencher_matriz(int n, int matriz[n][n]);
void imprimirMatriz(int n, int matriz[n][n]);
void copiarMatriz(int n, int matrizOriginal[n][n], int matrizCopia[n][n]);
void inicializarComZeros(int n, int matriz[n][n]);
void colocarUm(int n, int matriz[n][n], int u, int v);
void multiplicarMatrizes(int n, int matrizA[n][n], int matrizB[n][n], int matrizResultado[n][n]);
#endif
