#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ordenacao.h"

struct pilha{
    int *vetor_indices; // Ponteiro para armazenar os elementos da pilha
    int tamanho_pilha;
    int indice_do_topo;
};

// Função para criar uma nova pilha com tamanho especificado
struct pilha* cria_pilha(int tamanho) {
    struct pilha *pilha;

    if (!(pilha = (struct pilha*)malloc(sizeof(struct pilha)))) {
        printf("ERRO PILHA");
        return NULL;
    }

    if (!(pilha->vetor_indices = (int*) malloc(tamanho * sizeof(int)))) {
        printf("ERRO VETOR INDICES");
        free(pilha); 
        return NULL;
    }

    pilha->indice_do_topo = -1;
    pilha->tamanho_pilha = tamanho;

    return pilha;
}

// Função para empilhar um elemento
void empilha(struct pilha *pilha, int indice) {
    if (pilha->indice_do_topo == pilha->tamanho_pilha ) {
        printf("ERRO PILHA CHEIA \n");
        return;
    }

    pilha->vetor_indices[++(pilha->indice_do_topo)] = indice;
  
}

// Função para desempilhar um elemento
int desempilha(struct pilha *pilha) {
    if (pilha->indice_do_topo == -1) {
        printf("ERRO PILHA VAZIA\n");
        return -1;
    }

    return pilha->vetor_indices[pilha->indice_do_topo--];
}

// Função para destruir a pilha e liberar a memória alocada
void destroi_pilha(struct pilha *pilha) {
    free(pilha->vetor_indices);
    free(pilha);
}

// Função para verificar se a pilha está vazia
int pilha_vazia(struct pilha *pilha) {
    return pilha->indice_do_topo == -1;
}

void getNome(char nome[]) {
    // substitua por seu nome
    strncpy(nome, "Mozart Gustavo da Cruz Dias", MAX_CHAR);
    // adicionada terminação manual para caso de overflow
    nome[MAX_CHAR - 1] = '\0';
}

// a função a seguir deve retornar o seu número de GRR
uint32_t getGRR() { return 20213396; }

// Função para mesclar dois subvetores ordenados
void merge(int vetor[], int a, int meio, int b, uint64_t *numComparacoes) {
    int i = a;
    int j = meio + 1;
    int posi;
    int tamanho = (b - a) + 1;
    int *u;

    // Aloca um vetor temporário para armazenar os elementos mesclados
    if (!(u = (int*) malloc(tamanho * sizeof(int)))) {
        printf("Erro de alocação de memória\n");
        return;
    }

    for (int k = 0; k < tamanho; k++) {
        if (j > b || (i <= meio && vetor[i] <= vetor[j])) {
            posi = i;
            i++;
        } else {
            posi = j;
            j++;
        }

        u[k] = vetor[posi];
        (*numComparacoes)++;
    }

    // Copia os elementos ordenados de volta para o vetor original
    for (int x = 0, y = a; x < tamanho; x++, y++)
        vetor[y] = u[x];

    free(u); 
}
void mergeSortRec(int vetor[], int a, int b,uint64_t* numComparacoes ){
    int meio;

    if (a >= b)
        return;

    meio = ((a + b) / 2);

    mergeSortRec(vetor, a, meio, numComparacoes);
    mergeSortRec(vetor, meio + 1, b, numComparacoes);
    merge(vetor,a,meio,b,numComparacoes);
    (*numComparacoes)++;
}
//feito
uint64_t mergeSort(int vetor[], size_t tam) {
    uint64_t numComparacoes = 0;
    mergeSortRec(vetor, 0, tam - 1, &numComparacoes);
    return numComparacoes;
}
void troca(int vetor[], int i, int j) {
    int temp = vetor[i];
    vetor[i] = vetor[j];
    vetor[j] = temp;
}

int particiona(int vetor[], int a, int tam, uint64_t* numComparacoes) {
    int pivo = vetor[tam];
    int m = a;

    for (int i = a; i < tam; i++) {
        (*numComparacoes)++;
        if (vetor[i] <= pivo) {
            troca(vetor, m, i);
            m++;
        }
    }

    troca(vetor, m, tam);
    return m;
}

void quickSortRec(int vetor[], int a, int tam, uint64_t* numComparacoes) {
    if (a >= tam)
        return;

    int m = particiona(vetor, a, tam, numComparacoes);

    quickSortRec(vetor, a, m - 1, numComparacoes);
    quickSortRec(vetor, m + 1, tam, numComparacoes);
    (*numComparacoes)++;
}

//
uint64_t quickSort(int vetor[], size_t tam) {
    uint64_t numComparacoes = 0;
    if (tam > 0)
        quickSortRec(vetor, 0, tam - 1, &numComparacoes);
    return numComparacoes;
}
// Função max_heapify
void max_heapify(int vetor[], int i, int tamanho, uint64_t* numComparacoes) {
    int maior;
    int esquerdo = 2 * i ; 
    int direito = 2 * i + 1;  

    (*numComparacoes)++;
    if (esquerdo < tamanho && vetor[esquerdo] > vetor[i])
        maior = esquerdo;
    else
        maior = i;

    (*numComparacoes)++;
    if (direito < tamanho && vetor[direito] > vetor[maior])
        maior = direito;

    if (maior != i) {
        troca(vetor, maior, i);
        max_heapify(vetor, maior, tamanho, numComparacoes);
    }
}

// Função para construir a max-heap
void construir_maxheap(int vetor[], int tamanho, uint64_t* numComparacoes) {
    for (int i = tamanho / 2 - 1; i >= 0; i--) {
        max_heapify(vetor, i, tamanho, numComparacoes);
    }
}

// Função heapSort_aux para ordenar o vetor
void heapSort_aux(int vetor[], int tamanho, uint64_t* numComparacoes) {
    construir_maxheap(vetor, tamanho, numComparacoes);
    for (int i = tamanho - 1; i > 0; i--) {
        troca(vetor, 0, i);
        max_heapify(vetor, 0, i, numComparacoes);
    }
}

uint64_t heapSort(int vetor[], size_t tam) {
    uint64_t numComparacoes = 0;
    if (tam > 0)
        heapSort_aux(vetor, tam, &numComparacoes);
    return numComparacoes;
}
// Função Merge Sort iterativa usando pilha
void merge_sort(int vetor[], int tam, uint64_t *numComparacoes) {
    int esquerdo, direito;
    
    int tam_bloco = 1;

    while (tam_bloco < tam) {
        esquerdo = 0;
        
        while (esquerdo + tam_bloco < tam) {
            direito = esquerdo + 2 * tam_bloco;
            
            // Se o índice direito ultrapassar o limite do vetor, ajusta para o tamanho do vetor
            if (direito > tam)
                direito = tam;
            
            // O bloco esquerdo vai de `esquerdo` até `esquerdo + tam_bloco - 1`
            // O bloco direito vai de `esquerdo + tam_bloco` até `direito - 1`
            merge(vetor, esquerdo, esquerdo + tam_bloco - 1, direito - 1, numComparacoes);
            
            // Avança para o próximo bloco esquerdo
            esquerdo = esquerdo + 2 * tam_bloco;
        }
        
        tam_bloco *= 2;
    }
}


// Função de interface para chamar o Merge Sort iterativo
uint64_t mergeSortSR(int vetor[], size_t tam) {
    uint64_t numComparacoes = 0;
    if (tam > 0)
        merge_sort(vetor, tam, &numComparacoes);
    return numComparacoes;
}

// Função Quick Sort não recursiva
void quick_sortSR(int vetor[], int a, int b, uint64_t* numComparacoes) {
    struct pilha *pilha;
    int m;

    // Criando a pilha com um tamanho do vetor
    pilha = cria_pilha((b - a + 1) * 2);

    empilha(pilha, a);
    empilha(pilha, b);

    while (!pilha_vazia(pilha)) {
        b = desempilha(pilha);
        a = desempilha(pilha);

        if (a < b) {
            m = particiona(vetor, a, b, numComparacoes);
            empilha(pilha, a);
            empilha(pilha, m - 1);
            empilha(pilha, m + 1);
            empilha(pilha, b);
        }
    }
    destroi_pilha(pilha);
}

// Função para chamar o Quick Sort não recursivo
uint64_t quickSortSR(int vetor[], size_t tam) {
    uint64_t numComparacoes = 0;
    if (tam > 0)
        quick_sortSR(vetor, 0, tam - 1, &numComparacoes);
    return numComparacoes;
}

// Função max_heapify iterativa
void heap_sortSR(int vetor[], int i, int tamanho, uint64_t *numComparacoes) {
    int maior;
    int esquerdo, direito;
    int para = 1;

    do {
        esquerdo = 2 * i;
        direito = 2 * i + 1; 
        
        (*numComparacoes)++;
        if (esquerdo < tamanho && vetor[esquerdo] > vetor[i])
            maior = esquerdo;
        else
            maior = i;
        
        (*numComparacoes)++;
        if (direito < tamanho && vetor[direito] > vetor[maior])
            maior = direito;

        if (maior != i) {
            troca(vetor, maior, i);
            i = maior;
        } else 
            para = 0;
        
    } while (para);
}

// Função para construir a max-heap
void construir_maxheapSR(int vetor[], int tamanho, uint64_t *numComparacoes) {
    for (int i = tamanho / 2 - 1; i >= 0; i--) {
        heap_sortSR(vetor, i, tamanho, numComparacoes);
    }
}

// Função heap_SR para ordenar o vetor
void heap_SR(int vetor[], int tamanho, uint64_t *numComparacoes) {
    construir_maxheapSR(vetor, tamanho, numComparacoes);
    for (int i = tamanho - 1; i > 0; i--) {
        troca(vetor, 0, i);
        heap_sortSR(vetor, 0, i, numComparacoes);
    }
}

// Função principal para Heap Sort com contagem de comparações
uint64_t heapSortSR(int vetor[], size_t tam) {
    uint64_t numComparacoes = 0;
    if (tam > 0)
        heap_SR(vetor, tam, &numComparacoes);
    return numComparacoes;
}