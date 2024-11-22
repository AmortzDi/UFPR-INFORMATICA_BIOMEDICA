#include <stdio.h>
#include <stdlib.h>
#include "racionais.h"

/*Funcao vai ordenar o vetor de structs racionais usando 
  a logica do Insertion Sort*/
void orndenar_vet(struct racional *v[], int tamanho){
    
    int i;

    for (int j = 1; j < tamanho; j++) 
        // Verifica se o denominador é zero
        if (v[j]->den == 0) 
            return; 
        

    for (int j = 1; j < tamanho; j++) {

        struct racional *chave;

        if(!(chave = malloc(sizeof(struct racional))))
            return;

        chave->num = v[j]->num;
        chave->den = v[j]->den;

        i = j-1;

        while (i >= 0 && compara_r(v[i],chave) > 0 ){

            v[i + 1]->num = v[i]->num;
            v[i + 1]->den = v[i]->den;

            i--;
        }

        v[i+1]->num = chave->num;
        v[i+1]->den = chave->den;

        destroi_r(&chave);
    }
    
}
/*Função para eliminar racionais inválidos de um vetor*/
void eliminar_elemento(struct racional *v[], int *tamanho) {
    int j = 0; // Índice para o novo vetor (após eliminação)
    
    // Percorre o vetor original
    for (int i = 0; i < *tamanho; i++) {
        // Se o racional for válido, mantém no vetor
        if (valido_r(v[i]) == 0) {
            // Aloca memória para v[j]
            if(!(v[j] = malloc(sizeof(struct racional))))
                return;
            // Copia os valores do racional válido para a nova posição
            v[j]->num = v[i]->num;
            v[j]->den = v[i]->den;
            j++;
        } else {
            // Se o racional não for válido, libera a memória alocada
            destroi_r(&v[i]);
        }
    }
    
    // Atualiza o tamanho do vetor
    *tamanho = j;
}


/* Retorna a soma (simplificada) dos racionais *r1 e *r2. */
struct racional *soma_total (struct racional *v[], int tamanho){
	struct racional *result_soma_total;
	
	if (!(result_soma_total = malloc(sizeof(struct racional)))) {
		return NULL;
	}
	for(int i = 0; i < tamanho; i++){
	result_soma_total->num = (v[i]->num * v[i]->den) + (v[i]->num * v[i]->den);
	result_soma_total->den = v[i]->den * v[i]->den;
	
    }
	return result_soma_total;
}

int main (){
    
    /* equivalente a struct racional *v[] */
    struct racional **v;  
    struct racional *soma_total_vet; 
    int n, para;
    long int denominador, numerador;

    para = 1;
    do
    {
        scanf("%d", &n);
        if (n > 0 && n < 100)
            para = 0;
    } while (para);
    
    if(!(v = malloc(n * sizeof(struct racional))))
        return -1;

    for(int i = 0; i < n; i++){

       struct racional *r;

        if(!(r = malloc(sizeof(struct racional))))
            return -1;
        
        scanf("%ld %ld", &numerador, &denominador);

        r = cria_r(numerador,denominador);

        v[i] = r;
       
    }

    for(int i = 0; i < n; i++)
        imprime_r(v[i]);
    printf("\n");

    eliminar_elemento(v,&n);

    for(int i = 0; i < n; i++)
        imprime_r(v[i]);
    printf("\n");

    orndenar_vet(v,n);

    for(int i = 0; i < n; i++)
        imprime_r(v[i]);
    printf("\n");

    soma_total_vet = soma_total(v,n);

    printf("a soma de todos os elementos eh:");
    imprime_r(soma_total_vet);
    printf("\n");

    for (int i = 0; i < n; i++)
        destroi_r(&v[i]);

    destroi_r(&soma_total_vet);

    return 0;
}