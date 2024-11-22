#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#include "ordenacao.h"

int aleat(int min, int max){

    return min + rand () % (max - min +1);
}
void cria_vetor(int vetor[], int tamVetor){

    for( int i = 0; i < tamVetor; i++){
        vetor[i] = aleat(0, 100);
    }
}
void copia_vetor(int vetor[], int vetor_copy[], int tamVetor){

        for(int i = 0; i < tamVetor; i++)
            vetor[i] = vetor_copy[i];
}
int main() {
    char nome[MAX_CHAR];
    uint64_t numComp;
    int *vetor,*vetor_copy;
    int tamVetor;
    clock_t start, end;  
    double total;

    srand(0);

    getNome(nome);
    printf("Trabalho de %s\n", nome);
    printf("GRR %u\n", getGRR());

    printf("Digite o tamanho do vetor:\n");
    scanf("%d", &tamVetor);
    
    if(!(vetor = (int*) malloc(tamVetor * sizeof(int)))){
        printf("Falha fatal. Impossível alocar memoria.");
        return 1;
    }
    
    if(!(vetor_copy = (int*) malloc(tamVetor * sizeof(int)))){
        printf("Falha fatal. Impossível alocar memoria.");
        return 1;
    }

    cria_vetor(vetor, tamVetor);
    
    for (int i = 0; i < tamVetor; i++)
        vetor_copy[i] = vetor[i];
    
    printf("\nVetor nao ordenado\n");
    for (int i = 0; i < tamVetor; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
/*-----------------------------------------------------MARGE SORT RECURSIVO-------------------------------------------------------*/
    copia_vetor(vetor,vetor_copy,tamVetor);

    printf("\nMerge Sort RECURSIVO");
    start = clock();  // start recebe o "ciclo" corrente
    numComp = mergeSort(vetor, tamVetor);
    end = clock();  // end recebe o "ciclo" corrente
    // o tempo total é a diferença dividia pelos ciclos por segundo
    total = ((double)end - start) / CLOCKS_PER_SEC;
    printf("\nTempo total : %f\n", total);
    
    printf("Numero de comparacoes do Merge Sort: %lu\n", numComp);
/*-----------------------------------------------------MARGE SORT INTERATIVO-------------------------------------------------------*/
    copia_vetor(vetor,vetor_copy,tamVetor);
    numComp = 0;
    printf("\nMerge Sort INTERATIVO");
    start = clock();  // start recebe o "ciclo" corrente
    numComp = mergeSortSR(vetor, tamVetor);
    end = clock();  // end recebe o "ciclo" corrente
    // o tempo total é a diferença dividia pelos ciclos por segundo
    total = ((double)end - start) / CLOCKS_PER_SEC;
    printf("\nTempo total : %f\n", total);
    
    printf("Numero de comparacoes do Merge Sort INTERATIVO: %lu\n", numComp);
/*-----------------------------------------------------QUICK SORT-------------------------------------------------------*/
    copia_vetor(vetor,vetor_copy,tamVetor);
    numComp = 0;

    printf("\nQuick Sort");
    start = clock();  // start recebe o "ciclo" corrente
    numComp = quickSort(vetor, tamVetor);
    end = clock();  // end recebe o "ciclo" corrente
    // o tempo total é a diferença dividia pelos ciclos por segundo
    total = ((double)end - start) / CLOCKS_PER_SEC;
    printf("\nTempo total : %f\n", total);
    
    printf("Numero de comparacoes do Quick Sort: %lu\n", numComp);
/*-----------------------------------------------------QUICK SORT INTERATIVO-------------------------------------------------------*/
    copia_vetor(vetor,vetor_copy,tamVetor);
    numComp = 0;

    printf("\nQuick Sort INTERATIVO");
    start = clock();  // start recebe o "ciclo" corrente
    numComp = quickSortSR(vetor, tamVetor);
    end = clock();  // end recebe o "ciclo" corrente
    // o tempo total é a diferença dividia pelos ciclos por segundo
    total = ((double)end - start) / CLOCKS_PER_SEC;
    printf("\nTempo total : %f\n", total);
    
    printf("Numero de comparacoes do Quick Sort INTERATIVO: %lu\n", numComp);
/*-----------------------------------------------------HEAP SORT-------------------------------------------------------*/
    copia_vetor(vetor,vetor_copy,tamVetor);
    numComp = 0;

    printf("\nHeap Sort");
    start = clock();  // start recebe o "ciclo" corrente
    numComp = heapSort(vetor, tamVetor);
    end = clock();  // end recebe o "ciclo" corrente
    // o tempo total é a diferença dividia pelos ciclos por segundo
    total = ((double)end - start) / CLOCKS_PER_SEC;
    printf("\nTempo total : %f\n", total);
    
    printf("Numero de comparacoes do Heap Sort: %lu\n", numComp);
/*-----------------------------------------------------HEAP SORT INTERATIVO-------------------------------------------------------*/
    copia_vetor(vetor,vetor_copy,tamVetor);
    numComp = 0;

    printf("\nHeap Sort INTERATIVO");
    start = clock();  // start recebe o "ciclo" corrente
    numComp = heapSortSR(vetor, tamVetor);
    end = clock();  // end recebe o "ciclo" corrente
    // o tempo total é a diferença dividia pelos ciclos por segundo
    total = ((double)end - start) / CLOCKS_PER_SEC;
    printf("\nTempo total : %f\n", total);
    
    printf("Numero de comparacoes do Heap Sort INTERATIVO: %lu\n", numComp);

    printf("\nVetor ordenado\n");
    for (int i = 0; i < tamVetor; i++) {
        printf("%d", vetor[i]);
        printf(" ");
    }

    printf("\n");

    free(vetor_copy);
    free(vetor);

    return 0;
}

