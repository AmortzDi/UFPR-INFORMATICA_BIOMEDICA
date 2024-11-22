#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

#include "ordenacao.h"

void getNome(char nome[]) {
    // substitua por seu nome
    strncpy(nome, "Mozart Gustavo da Cruz Dias", MAX_CHAR_NOME);
    nome[MAX_CHAR_NOME - 1] =
        '\0';  // adicionada terminação manual para caso de overflow
}
// a função a seguir deve retornar o seu número de GRR
uint32_t getGRR() { return 454; }

void troca(int vetor[], int indice_1, int indice_2) {
    int temp = vetor[indice_1];
    vetor[indice_1] = vetor[indice_2];
    vetor[indice_2] = temp;
}

void intercala (int vetor[], int tam, uint64_t* numComparacoes){

    int p, i;

    p = buscaBinariaRec(vetor, tam - 1, vetor[tam],numComparacoes);
    i = tam;
    while (i > p + 1){
        troca(vetor,i, i-1);
        i--;
    }

}
ssize_t buscaSequencial(int vetor[], size_t tam, int valor, uint64_t* numComparacoes) {

    for (size_t i = 0; i < tam; ++i) {
        (*numComparacoes)++;

        // Se o elemento atual for igual ao valor procurado, retorna o índice
        if (vetor[i] == valor) {
            return i;
        }
    }
    
    // Se o valor não for encontrado, retorna -1
    return -1;
}

ssize_t buscaSequencialRec(int vetor[], size_t tam, int valor, uint64_t* numComparacoes) {

    (*numComparacoes)++;

    // Caso base: se o tamanho do vetor for 0, o elemento não está presente
    if (tam == 0) 
        return -1;
    

    // Se o elemento atual for igual ao valor procurado, retorna o índice
    if (vetor[tam - 1] == valor) 
        return tam - 1;
    

    return buscaSequencialRec(vetor, tam - 1, valor, numComparacoes);
}
    

ssize_t buscaBinaria(int vetor[], size_t tam, int valor, uint64_t* numComparacoes){ 

    ssize_t inicio = 0;
    ssize_t fim = tam - 1;
    
    while (inicio <= fim) {
        ssize_t meio = (inicio + fim) / 2;
        
        (*numComparacoes)++; 
        
        if (vetor[meio] == valor) 
            // Valor encontrado, retorna o índice
            return meio; 
         else if (vetor[meio] < valor) 
            // Busca na metade direita
            inicio = meio + 1; 
        else 
            // Busca na metade esquerda
            fim = meio - 1; 
    }
    // Valor não encontrado, retorna -1
    return -1;
}

int buscaBinariaRec_aux(int vetor[], int inicio, int fim, int valor, uint64_t* numComparacoes) {

    int meio;

    if (inicio <= fim) {
        meio = (inicio + fim) / 2;

        (*numComparacoes)++; // Incrementa a contagem de comparações

        if (vetor[meio] == valor) 
            return meio; // Elemento encontrado

        if (vetor[meio] > valor) 
            return buscaBinariaRec_aux(vetor, inicio, meio - 1, valor, numComparacoes); // Busca na metade esquerda
        
        return buscaBinariaRec_aux(vetor, meio + 1, fim, valor, numComparacoes); // Busca na metade direita
        
    }

    return -1; // Elemento não encontrado
}
ssize_t buscaBinariaRec(int vetor[], size_t tam, int valor, uint64_t* numComparacoes) {
    
    int indice = buscaBinariaRec_aux(vetor, 0, tam - 1, valor, numComparacoes);

    if(indice == -1)
        return -1;
    else
        return indice;    
}

uint64_t insertionSort(int vetor[], size_t tam) {

    uint64_t numComparacoes = 0;
    int i,chave;

    for (int j = 1; j < tam; j++){
        chave = vetor[j];
        i = j-1;
        while (i >= 0 && vetor[i] > chave){
            vetor [i + 1] = vetor[i];
            numComparacoes++;
            i--;
        }
        vetor[i + 1] = chave;
    }

    return numComparacoes;
}

void insertionSort_Rec(int vetor[], size_t tam, uint64_t* numComparacoes) {
    
    int a = 0;

    if (a >= tam)
        return;

    insertionSort_Rec(vetor,tam-1, numComparacoes);
    intercala(vetor, tam, numComparacoes);

    return;
}
uint64_t insertionSortRec(int vetor[], size_t tam) {
    uint64_t numComparacoes = 0;
    insertionSort_Rec(vetor, tam, &numComparacoes);
    return numComparacoes;
}

uint64_t selectionSort(int vetor[], size_t tam) {

    uint64_t numComparacoes = 0;
    int ind_min;

    for(int j = 0; j < tam; j++){
        ind_min = j;

        for(int i = j + 1; i < tam; i++){
            
            numComparacoes ++;
            if (vetor[i] < vetor[ind_min])
                ind_min = i;
        }

        if (ind_min != j)
            troca(vetor, j,ind_min);
    }

    return numComparacoes;
}
int minimo(int vetor[], int a, int b, uint64_t* numComparacoes) {

    int min;

    if (a == b)
        return a;

    min = minimo(vetor, a, b - 1, numComparacoes);

    // Incrementa a contagem de comparações.
    (*numComparacoes)++;

    if (vetor[b] < vetor[min])
        min = b;

    return min;
}
void selectionSortrec_aux(int vetor[], int a, int b, uint64_t* numComparacoes) {

    if (a >= b)
        return;

    troca(vetor, a, minimo(vetor, a, b, numComparacoes));
    selectionSortrec_aux(vetor, a + 1, b, numComparacoes);
}
uint64_t selectionSortRec(int vetor[], size_t tam) {
    uint64_t numComparacoes = 0;
    selectionSortrec_aux(vetor, 0, tam - 1, &numComparacoes);
    return numComparacoes;
}
void merge(int vetor[], int a, int meio, int b) {
    int i = a;
    int j = meio + 1;
    int posi;
    // Calcula o tamanho do vetor temporário
    int tamanho = (b - a) + 1;

    // Aloca memória dinamicamente para o vetor temporário 'u'
    int *u = (int*)malloc(tamanho * sizeof(int));
    if (u == NULL) {
        printf("Erro de alocação de memória.\n");
        return;
    }

    if (a > b) {
        // Retorna o número de comparações realizadas
        free(u);
        return;
    }

    for (int k = 0; k < tamanho; k++) {
        if (j > b || (i <= meio && vetor[i] <= vetor[j])) {
            posi = i;
            i ++;
         }else {
            posi = j;
            j ++;
         }
        
        u[k] = vetor[posi];

    }

    // Copia os elementos de 'u' de volta para o vetor original 'vetor'
    for (int x = 0, y = a; x < tamanho; x++, y++) {
        vetor[y] = u[x];
    }

    // Libera a memória alocada para o vetor temporário 'u'
    free(u);
}


void mergeSort(int vetor[], int a, int b, uint64_t* numComparacoes){
    int meio;

    if (a >= b)
        return;
    
    meio = ((a + b) / 2);

    mergeSort(vetor, a, meio, numComparacoes);
    mergeSort(vetor, meio + 1, b, numComparacoes);
    merge(vetor,a,meio,b);
    (*numComparacoes)++;

}

uint64_t mergeSortRec(int vetor[], size_t tam) {
    uint64_t numComparacoes = 0;
    mergeSort(vetor, 0, tam - 1, &numComparacoes);
    return numComparacoes;
}
