#ifndef _LIBracionais_t_H
#define _LIBracionais_t_H

/* 
 * Tipos Abstratos de Dados - TAD's
 * Arquivo de header para TAD racional.
 * Feito em 17/08/2023 para a disciplina prog1.
 * Revisado em 26/03/2024.
*/

/***************** NAO ALTERE ESTE ARQUIVO ****************************/

struct racional {
    unsigned short int valido; /* um racional eh invalido se den == 0 */
    int num;                   /* numerador do racional               */
    int den;                   /* denominador do racional             */
};

/* Observacoes iniciais sobre como a struct racional deve ser
 * utilizada para representar o conceito abstrato de um numero
 * racional, conforme definido na matematica:
 *
 * Os campos "num" e "den" conterao respectivamente o numerador
 * e o denominador de um racional. O campo "valido" contera um
 * valor que pode ser 0 ou 1, dependendo da informacao contida
 * no denominador. Se este for 0 temos caracterizado um racional
 * invalido, por isso o campo "valido" devera ser inicializado
 * com o valor 0. Caso contrario devera ser inicializado com o
 * valor 1.
 *
 * Nossos racionais serao sempre simplificados, isto eh, conterao
 * os menores inteiros que representam o mesmo numero racional.
 * Por exemplo, o racional 10/8 pode ser simplificado para 5/4.
 *
 * Tambem consideraremos que, caso o racional seja negativo, o
 * sinal estara sempre no numerador e nunca no denominador.
 * Por exemplo, o racional 5/-4 devera ser representado por -5/4.
 *
 * Usaremos duas funcoes que permitem que um racional seja
 * inicializado pelos usuarios deste modulo. As funcoes serao
 * "cria_r" e "sorteia_r", que sao definidas abaixo.
 *
 * Dentro do conceito de Tipos Abstratos de Dados, o usuario
 * deste modulo nunca pode violar o tipo acessando diretamente
 * seus campos "num", "den" ou "valido". Por isso este modulo
 * contera tres funcoes para que o usuario tenha acesso a estes
 * campos. As funcoes definidas abaixo sao respectivamente
 * "numerador_r", "denominador_r" e "valido_r".
 *
 * Um Tipo Abstrato de Dados tambem define as operacoes que
 * podem ser feitas sobre a abstracao do numero racional.
 * No nosso caso definiremos somente quatro operacoes, que sao:
 * somar (soma_r), subtrair (subtrai_r), multiplicar (multiplica_r)
 * e, finalmente, dividir (divede_r).
 * Isto permitira que o usuario possa realizar as quatro operacoes
 * sem ter acesso aos campos internos da struct.
 * As 4 operacoes sao especificadas abaixo.
 *
 * Para permitir que o usuario compare dois numeros racionais,
 * este modulo tambem preve uma funcao para isso, para permitir
 * operacoes abstratas equivalentes aos operadores
 * "<", "==" e ">". Os demais operadores ("<=", "!=" e ">=")
 * poderao ser implementados com o uso dos operadores logicos
 * "&&", "||" e "!".
 *
 * Finalmente, este modulo tambem preve uma funcao que imprime
 * o racional na forma convencional que o ser humano prefere.
 *
 * A seguir, as especificacoes de cada funcao deste modulo.
*/

/* Cria um numero racional com base nas informacoes dos parametros.
 * Isto eh, os campos "num" e "den" da struct racional devem receber
 * respectivamente os valores dos parametros numerador e denominador.
 * O campo "valido" da struct racional deve ser devidamente inicializado,
 * isto eh, recebe 0 se o denominador for zero e recebe 1 caso contrario.
 * O racional deve ser retornado na forma simplificada, conforme
 * especificado acima.
*/
struct racional cria_r (int numerador, int denominador);

/* Retorna um numero racional gerado aleatoriamente.
 * O numerador e o denominador devem ser inteiros aleatorios cujos
 * valores variam de -n a n, onde o valor de n esta no parametro.
 * O campo "valido" da struct racional deve ser devidamente inicializado,
 * isto eh, recebe 0 se o denominador for zero e recebe 1 caso contrario.
 * O racional deve ser retornado na forma simplificada, conforme
 * especificado acima.
*/
struct racional sorteia_r (int n);

/* Retorna o numerador do racional r */
int numerador_r (struct racional r);

/* Retorna o denominador do racional r */
int denominador_r (struct racional r);

/* Retorna 1 se o racional r eh valido ou 0 caso contrario.
 * Esta funcao se limita a testar o campo "valido" da struct,
 * o qual deve ter sido inicializado previamente nas funcoes
 * que produzem numeros racionais (cria_r, sorteia_r)
 * bem como nas 4 operacoes aritmeticas definidas.
 * Um racional eh invalido se o denominador for nulo.  */
int valido_r (struct racional r);

/* Retorna a soma (simplificada) dos racionais r1 e r2 no parametro *r3 e 
 * Obs.: nao esquecer de inicializar o campo "valido".
 * Quem chama esta funcao deve garantir que os racionais r1 e r2 sao validos.
 * Eh matematicamente garantido que a soma de dois racionais validos seja
 * valido.  */
void soma_r (struct racional r1, struct racional r2, struct racional *r3);

/* Retorna a subtracao dos racionais r1 e r2 no parametro *r3 e
 * Obs.: nao esquecer de inicializar o campo "valido".
 * Eh matematicamente garantido que a subtracao de dois racionais validos seja
 * valido.  */
void subtrai_r (struct racional r1, struct racional r2, struct racional *r3);

/* Retorna a multiplicacao dos racionais r1 e r2 no parametro *r3 e
 * Obs.: nao esquecer de inicializar o campo "valido".
 * Eh matematicamente garantido que a multiplicacao de dois racionais validos seja
 * valido.  */

void multiplica_r (struct racional r1, struct racional r2, struct racional *r3);

/* Retorna a divisao dos racionais r1 e r2 no parametro *r3 e
 * retorna 1 se *r3 for valido ou 0 caso contrario.
 * --NAO eh-- matematicamente garantido que a divisao de dois racionais validos 
 * seja valido. Se o numerador do racional r2 for 0, entao o racional 
 * resultante eh invalido. */
int divide_r (struct racional r1, struct racional r2, struct racional *r3);

/* Retorna -1 se r1 < r2; 0 se r1 == r2; 1 se r1 > r2 */
int compara_r (struct racional r1, struct racional r2);

/* Imprime um racional r
   A impressao deve respeitar o seguinte:
   - o racional deve estar na forma simplificada;
   - o formato de saida deve ser "r.num/r.den", a menos dos casos abaixo;
   - nao use espacos em branco apos o numero e nao mude de linha;
   - se o numerador for igual a 0, deve ser impresso somente zero;
   - se o denominador for igual a 1, deve ser impresso somente o numerador;
   - se o numerador e denominador forem o mesmo valor, deve imprimir somente 1;
   - se o racional for negativo, o sinal deve ser impresso antes do numero;
   - se ambos numerador e denominador forem negativos, o racional eh positivo;
   - se o racional for invalido, deve imprimir a mensagem "INVALIDO"           */
void imprime_r (struct racional r);
 
#endif
