#ifndef TABELA_H
#define TABELA_H

#define TENT_MAX 10 // Número máximo de tentativas de inserção antes de falhar
#define TAM_MAX 11  // Tamanho de cada tabela hash

#define TAM_NOME 25 // Um tamanho razoável para os nomes

enum EstadoSlot {VAZIO,OCUPADO,REMOVIDO};
// Estrutura para cada slot da tabela hash
struct tabela_hash {
  char nome[TAM_NOME]; // A chave agora é uma string
  int id_vertice;      // O valor que queremos encontrar
  int estado;          // 0 = vazia, 1 = ocupada, 2 = removida
};

/* --- Funções de Hash para Strings --- */
// Função de hash para a tabela 1
int h1(const char *chave);
// Função de hash para a tabela 2
int h2(const char *chave);

/* --- Funções Principais da Tabela Hash --- */

/**
 *  Busca um nome nas tabelas T1 e T2.
 * @return O id_vertice associado ao nome se encontrado, caso contrário, -1.
 */
int busca(struct tabela_hash *T1, struct tabela_hash *T2, const char *nome);

/**
 *  Insere um par (nome, id_vertice) na tabela hash usando a lógica Cuckoo.
 */
void insere(struct tabela_hash *T1, struct tabela_hash *T2, const char *nome, int id_vertice);

/**
 *  Remove um nome das tabelas, marcando seu estado como 'removido'.
 */
void remover(struct tabela_hash *T1, struct tabela_hash *T2, const char *nome);

/**
 *  Imprime o conteúdo de ambas as tabelas de forma ordenada pelo nome.
 */
void imprime(struct tabela_hash *T1, struct tabela_hash *T2);

#endif // TABELA_H