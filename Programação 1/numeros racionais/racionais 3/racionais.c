#include <stdio.h>
#include <stdlib.h>
#include "racionais.h"

/* retorna um numero aleatorio entre min e max, inclusive. */
int aleat (long int min, long int max){
    
	return min + rand () % (max - min + 1);
}

/* Maximo Divisor Comum entre a e b.         */
long int mdc (long int a, long int b){
   
	if (b == 0)
		return a;
   
	return mdc(b, a % b);
}

/* Minimo Multiplo Comum entre a e b */
long int mmc (long int a, long int b){
   
	return (a * b) / mdc (a,b);
}

/* Recebe um numero racional e o retorna simplificado no parametro *r.*/
void simplifica_r (struct racional *r){
    
	int divisor;
    
	divisor = mdc(r->num, r->den);
    
	r->num /= divisor;
	r->den /= divisor;
    

}

struct racional *cria_r (long int numerador, long int denominador){
	
	struct racional *r;

	if(!(r = malloc(sizeof(struct racional))))
		return NULL;
    
	r->num = numerador;
	r->den = denominador;
    
	return r;
}

/* Cria um ponteiro para um numero racional e o inicializa com valores
 * aleatorios. */
struct racional *sorteia_r (long int max){

	struct racional *r;
	
	if (!(r = malloc(sizeof(struct racional)))) {
		return NULL;
	}
    
	r->num = aleat(-max, max);
	r->den = aleat (-max, max);
    
	simplifica_r(r);

	return r;
     
}

/* Libera a memoria alocada para o racional *r,
 * e aterra o ponteiro *r */
void destroi_r (struct racional **r){
	
	free(*r);
	*r = NULL;
}

/* Retorna o numerador do racional *r */
long int numerador_r (struct racional *r){
	
	return (r->num);
}

/* Retorna o denominador do racional *r */
long int denominador_r (struct racional *r){
	
	return (r->den);
}

/* Retorna 1 se o racional *r eh valido ou 0 caso contrario. */
int valido_r (struct racional *r){
	
	if (r->den)
		return 0;
	
	return 1;
}	

/* Retorna a soma (simplificada) dos racionais *r1 e *r2. */
struct racional *soma_r (struct racional *r1, struct racional *r2){
	
	struct racional *result_soma;
	
	if (!(result_soma = malloc(sizeof(struct racional)))) {
		return NULL;
	}
	
	result_soma->num = (r1->num * r2->den) + (r2->num * r1->den);
	result_soma->den = r1->den * r2->den;
	
	simplifica_r (result_soma);

	return result_soma;
}

/* Retorna a subtracao (simplificada) dos racionais *r1 e *r2. */
struct racional *subtrai_r (struct racional *r1, struct racional *r2){

	struct racional *result_sub;

	if(!(result_sub = malloc(sizeof(struct racional))))
		return NULL;

	result_sub->num = (r1->num * r2->den) - (r2->num * r1->den);
	result_sub->den = r1->den * r2->den;

	simplifica_r (result_sub);

	return result_sub;
}

/* Retorna a multiplicacao (simplificada) dos racionais *r1 e *r2.*/
struct racional *multiplica_r (struct racional *r1, struct racional *r2){

	struct racional *result_mul;

	if(!(result_mul = malloc(sizeof(struct racional))))
		return NULL;

	result_mul->num = r1->num * r2->num;
	result_mul->den = r2->den * r2->den;

	simplifica_r(result_mul);

	return (result_mul);
}

/* Retorna a divisao (simplificada) dos racionais *r1 e *r2. */
struct racional *divide_r (struct racional *r1, struct racional *r2){

	struct racional *result_div;

	if(!(r2->num ))
		return NULL;

	if(!(result_div = malloc(sizeof(struct racional))))
		return NULL;

	result_div->num  = r1->num * r2->den;
	result_div->den = r1->den * r2->num;

	if (!(result_div->den)){
		free(result_div);
		return NULL;
	}

	simplifica_r(result_div);

	return (result_div);
}

int compara_r (struct racional *r1, struct racional *r2){

    int den = mmc(r1->den, r2->den);
    int num1 = r1->num * (den / r1->den);
    int num2 = r2->num * (den / r2->den);

    if (num1 < num2)
        return -1;
    else if (num1 > num2)
        return 1;
    else
        return 0;
}


/* Imprime um racional *r*/
void imprime_r (struct racional *r){

	if(valido_r(r)){
		printf(" INVALIDO");
		return;
	}

	//Verifica se o numerador eh igual a 0
    if (!r->num)
        printf(" 0");
    //Verifica se o numerador e o denominador sao iguais
    else if (r->num == r->den)
        printf(" 1");
    //Verifica se o denominador for igual a 1 vai imprimir apenas o numerador
    else if (r->den == 1)
        printf(" %ld", r->num);
    //Verifica se o racional  negativo (numerador ou denominador)
    else if (r->num > 0 && r->den < 0)
    //Imprime com sinal negativo
        printf(" -%ld/%ld", r->num, (r->den * -1));
    //Verifica se ambos numerador e denominador forem negativos, o racional eh positivo;
    else if (r->num < 0 && r->den < 0)
    // Imprime valores normais
        printf(" %ld/%ld", (r->num * -1), (r->den * -1));
    else
    //Imprime o racional normalmente
        printf(" %ld/%ld", r->num, r->den);

}

