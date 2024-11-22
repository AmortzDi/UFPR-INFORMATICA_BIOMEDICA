#include <stdio.h>
#include <stdlib.h>
#include "racionais.h"

/* retorna um numero aleatorio entre min e max, inclusive. */
int aleat (int min, int max){

    return min + rand () % (max - min + 1);
}

/* Maximo Divisor Comum entre a e b      */
int mdc (int a, int b){
    
    if (b == 0)
        return a;
    
    return mdc(b, a % b);
}

/* Minimo Multiplo Comum entre a e b */
int mmc (int a, int b){
    
    return (a * b) / mdc (a, b);
}

/* Recebe um numero racional e o retorna simplificado.*/
void simplifica_r (struct racional *r){
    
    int divisor;

    divisor = mdc(r->num, r->den);

    
    r->num /= divisor;
    r->den /= divisor;


}
struct racional cria_r (int numerador, int denominador){

        struct racional r;

        r.num = numerador;
        r.den = denominador;

        return r;
}

/* Retorna um numero racional gerado aleatoriamente*/
struct racional sorteia_r (int n){

    struct racional r;

    r.num = aleat(0, n);
    r.den = aleat(0, n);
    r.valido = valido_r(r) != 0;

    simplifica_r(&r);

    return r;
}

/* Retorna o numerador do racional r */
int numerador_r (struct racional r){

    return (r.num);
}

/* Retorna o denominador do racional r */
int denominador_r (struct racional r){

    return (r.den);
}

/* Retorna 1 se o racional r eh valido ou 0 caso contrario.  */
int valido_r (struct racional r){

    if (r.den)
        return 0;

    return 1;
}

/* Retorna a soma (simplificada) dos racionais r1 e r2 no parametro *r3  */
void soma_r (struct racional r1, struct racional r2, struct racional *r3){

        r3->num = (r1.num * r2.den) + (r2.num * r1.den);
        r3->den = r1.den * r2.den;

        r3->valido = valido_r(*r3) != 0;

        simplifica_r (r3);
}

/* Retorna a subtracao dos racionais r1 e r2 no parametro *r3 */
void subtrai_r (struct racional r1, struct racional r2, struct racional *r3){

    r3->num = (r1.num * r2.den) - (r2.num * r1.den);
    r3->den = r1.den * r2.den;

    r3->valido = valido_r(*r3) != 0;

    simplifica_r (r3);
}

/* Retorna a multiplicacao dos racionais r1 e r2 no parametro *r3*/

void multiplica_r (struct racional r1, struct racional r2, struct racional *r3){

    r3->num = r1.num * r2.num;
    r3->den = r2.den * r1.den;

    r3->valido = valido_r(*r3) != 0;

    simplifica_r(r3);

}
/* Retorna a divisao dos racionais r1 e r2 no parametro *r3*/
int divide_r (struct racional r1, struct racional r2, struct racional *r3){

    if (r2.den == 0){
        r2.valido = 0;
        return 0;
    }else{
        r3->num = r1.num * r2.den;
        r3->den = r1.den * r2.num;

        r3->valido = valido_r(*r3) != 0;
    }
    simplifica_r(r3);

    return 1;
}

/* Retorna -1 se r1 < r2; 0 se r1 == r2; 1 se r1 > r2 */
int compara_r (struct racional r1, struct racional r2){

    int den;
    struct racional racional_1, racional_2;

    den = mmc(r1.den,r2.den);

    racional_1.num = r1.num * (den / r1.den);
    racional_2.num = r2.num * (den / r2.den);
    racional_1.den = den;
    racional_2.den = den;

    if (racional_1.num < racional_2.num)
        return -1;

    if (racional_1.num > racional_2.num)
        return 1;

    /*Se chegou até aqui, os racionais são iguais*/
    return 0; 
}

/* Imprime um racional r*/
void imprime_r (struct racional r){

    //Verificar se  valido o racional, caso a condicao do if nao seja imprime invalido
    if (r.valido){
        printf("INVALIDO");
        return;
    }
    //Verifica se o numerador eh igual a 0
    if (!r.num)
        printf(" 0");
    //Verifica se o numerador e o denominador sao iguais
    else if (r.num == r.den)
        printf(" 1");
    //Verifica se o denominador for igual a 1 vai imprimir apenas o numerador
    else if (r.den == 1)
        printf(" %d", r.num);
    //Verifica se o racional  negativo (numerador ou denominador)
    else if (r.num > 0 && r.den < 0)
    //Imprime com sinal negativo
        printf(" -%d/%d", r.num, (r.den * -1));
    //Verifica se ambos numerador e denominador forem negativos, o racional eh positivo;
    else if (r.num < 0 && r.den < 0)
    // Imprime valores normais
        printf(" %d/%d", (r.num * -1), (r.den * -1));
    else
    //Imprime o racional normalmente
        printf(" %d/%d ", r.num, r.den);

}