# Implementação de LISTA LIGADA 👨🏽‍💻

Este repositório contém a implementação de um arquivo **lista.c** que deve ser construído com base no novo arquivo de cabeçalho **`lista.h`** fornecido. O objetivo é criar as funções necessárias para manipulação de listas, utilizando a estrutura e as diretrizes definidas em `lista.h`.

---

## **Objetivo do Trabalho**

O objetivo principal deste trabalho é implementar as operações de manipulação de listas encadeadas (ou outras estruturas de dados, conforme especificado em `lista.h`). A implementação precisa seguir rigorosamente as funções e os protótipos especificados no arquivo de cabeçalho **`lista.h`**.

---

## **Estrutura do Repositório**

O repositório é composto por dois arquivos principais que são essenciais para a implementação do projeto:

1. **`lista.h`** - Arquivo de cabeçalho que define as estruturas de dados e os protótipos das funções que devem ser implementadas em **`lista.c`**.
   
2. **`lista.c`** - Arquivo que contém a implementação das funções listadas em `lista.h`, que são responsáveis por realizar operações em listas, como inserção, remoção, busca, entre outras.

---

## **Funções a Serem Implementadas**

Com base no arquivo `lista.h`, o arquivo `lista.c` deve implementar as seguintes funções (exemplo de funções que podem ser definidas em `lista.h`):

1. lista_cria()
   
Descrição:
Cria e retorna uma nova lista vazia.

Retorno:
Retorna um ponteiro para a lista criada ou NULL em caso de erro de alocação.

2. lista_destroi()
   
Descrição:
Desaloca toda a memória usada pela lista e faz o ponteiro da lista receber NULL.

3. lista_insere_inicio()
   
Descrição:
Insere um novo elemento no início da lista.

Retorno:
Retorna 1 em caso de sucesso e 0 em caso de falha.

4. lista_insere_fim()
   
Descrição:
Insere um novo elemento no final da lista.

Retorno:
Retorna 1 em caso de sucesso e 0 em caso de falha.

5. lista_insere_ordenado()
   
Descrição:
Insere um novo elemento mantendo a lista ordenada.

Retorno:
Retorna 1 em caso de sucesso e 0 em caso de falha.

6. lista_remove_inicio()
   
Descrição:
Remove o elemento no início da lista e o armazena no parâmetro chave.

Retorno:
Retorna 1 em caso de sucesso e 0 se a lista estiver vazia.

7. lista_remove_fim()
   
Descrição:
Remove o elemento no final da lista e o armazena no parâmetro chave.

Retorno:
Retorna 1 em caso de sucesso e 0 se a lista estiver vazia.

8. lista_remove_ordenado()
    
Descrição:
Remove um elemento da lista mantendo a ordem e garantindo que o elemento esteja presente.

Retorno:
Retorna 1 em caso de sucesso e 0 se a lista estiver vazia.

9. lista_vazia()
    
Descrição:

Retorna 1 se a lista estiver vazia, 0 caso contrário.

10. lista_tamanho()
Descrição:

Retorna o número de elementos na lista.

11. lista_pertence()
Descrição:

Retorna 1 se o elemento chave estiver presente na lista, 0 caso contrário.

-----


## **Estrutura de Arquivos**

- **`lista.h`**: Arquivo de cabeçalho contendo as declarações das funções e as definições das estruturas de dados.
- **`lista.c`**: Arquivo que implementa as funções para manipulação da lista conforme definido em `lista.h`.
- **`main.c`** (se aplicável): Arquivo de testes onde as funções de **`lista.c`** são chamadas para verificar a funcionalidade da implementação.

---


Este repositório foi desenvolvido como parte de um exercício acadêmico para estudar e aplicar os conceitos de manipulação de listas e estruturas de dados.
