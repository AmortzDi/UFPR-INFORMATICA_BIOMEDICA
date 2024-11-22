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
    char nome[MAX_CHAR_NOME];
    int idxBuscaS,idxBuscaB,valor;
    uint64_t numComp,numComp_rec;
    int *vetor,*vetor_copy;
    int tamVetor;
    clock_t start, end;  // variáveis do tipo clock_t
    double total;

    srand(0);
    
    printf("Digite o tamanho do vetor:\n");
    scanf("%d", &tamVetor);

    if(!(vetor = malloc(tamVetor * sizeof(int)))){
        printf("Falha fatal. Impossível alocar memoria.");
        return 1;
    }
    
    if(!(vetor_copy = malloc(tamVetor * sizeof(int)))){
        printf("Falha fatal. Impossível alocar memoria.");
        return 1;
    }

    cria_vetor(vetor, tamVetor);
    
    //copia o vetor
    for (int i = 0; i < tamVetor; i++)
        vetor_copy[i] = vetor[i];

    
    getNome(nome);
    printf("Trabalho de %s\n", nome);
    printf("GRR %u\n", getGRR());
    
    printf("\nVetor nao ordenado\n");
    for (int i = 0; i < tamVetor; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
 /*-----------------------------------------------------MARGE SORT-------------------------------------------------------*/
    copia_vetor(vetor,vetor_copy,tamVetor);

    printf("\nMerge Sort");
    start = clock();  // start recebe o "ciclo" corrente
    numComp = mergeSortRec(vetor, tamVetor);
    end = clock();  // end recebe o "ciclo" corrente
    // o tempo total é a diferença dividia pelos ciclos por segundo
    total = ((double)end - start) / CLOCKS_PER_SEC;
    printf("\nTempo total : %f\n", total);

    numComp_rec = mergeSortRec(vetor, tamVetor);
    
    printf("Numero de comparacoes do Merge Sort: %lu\n", numComp_rec);

/*-----------------------------------------------------INSERTION SORT------------------------------------------------------*/
    printf("\nInsertion Sort");
    start = clock();  // start recebe o "ciclo" corrente
    numComp = insertionSortRec(vetor, tamVetor);
    end = clock();  // end recebe o "ciclo" corrente
    // o tempo total é a diferença dividia pelos ciclos por segundo
    total = ((double)end - start) / CLOCKS_PER_SEC;
    printf("\nTempo total : %f\n", total);

    
    numComp_rec = insertionSortRec(vetor, tamVetor);
    
    copia_vetor(vetor,vetor_copy,tamVetor);

    numComp = insertionSort(vetor, tamVetor);

    printf("Numero de comparacoes do Insertion Sort recursivo: %lu\n", numComp_rec);
    printf("Numero de comparacoes do Insertion Sort interativo: %lu\n", numComp);

/*-------------------------------------------------SELECTION SORT-----------------------------------------------*/
    copia_vetor(vetor,vetor_copy,tamVetor);
   
    printf("\nSelection Sort");
    start = clock();  // start recebe o "ciclo" corrente
    numComp = selectionSortRec(vetor, tamVetor);
    end = clock();  // end recebe o "ciclo" corrente
    // o tempo total é a diferença dividia pelos ciclos por segundo
    total = ((double)end - start) / CLOCKS_PER_SEC;
    printf("\nTempo total : %f\n", total);

    
    numComp_rec = selectionSortRec(vetor, tamVetor);
    
    copia_vetor(vetor,vetor_copy,tamVetor);

    numComp = selectionSort(vetor, tamVetor);

    
    printf("Numero de comparacoes do Insertion Sort recursivo: %lu\n", numComp_rec);
    printf("Numero de comparacoes do Insertion Sort interativo: %lu\n", numComp);
    
    printf("\nVetor ordenado\n");
    for (int i = 0; i < tamVetor; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\nQual elemento?\n");
    scanf("%d", &valor);


    idxBuscaS = buscaSequencialRec(vetor, tamVetor, valor, &numComp);
    idxBuscaB = buscaBinariaRec(vetor, tamVetor, valor, &numComp);

    if(idxBuscaB == -1 || idxBuscaS == -1){
        printf("O elemento %d nao esta no vetor\n" ,valor); 
        printf("Numero de comparacoes em busca sequencial recurisvo: %lu\n", numComp);
        printf("Numero de comparacoes em busca binaria recursiva: %lu\n", numComp_rec); 
    }else{
    printf("Indice do Busca Sequencial: %d  Numero de Comparacoes recursivo: %lu", idxBuscaS, numComp);
    printf("\n");
    printf("Indice do Busca Binarario: %d Numero de Comparacoes recurisvo: %lu", idxBuscaB, numComp_rec);
    printf("\n");
    }

    free(vetor_copy);
    free(vetor);

    return 0;
}
