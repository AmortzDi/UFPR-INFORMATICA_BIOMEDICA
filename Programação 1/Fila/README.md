## Projeto: Implementação de Fila e Lista de Eventos Futuros (LEF) 👨🏽‍💻

Este repositório contém a implementação de dois tipos abstratos de dados (TADs) fundamentais para sistemas de simulação e manipulação de eventos, especificamente **Fila (FIFO)** e **Lista de Eventos Futuros (LEF)**. A fila é implementada utilizando uma lista ligada com nodo cabeça, seguindo a política **First In First Out (FIFO)**. A LEF também é baseada em uma lista ligada com nodo cabeça, mas os eventos são armazenados e ordenados conforme o tempo, sendo removidos sempre do início da lista.

---

## Estrutura do Repositório

O projeto é composto pelos seguintes arquivos:

1. **`fila.h` e `fila.c`**: Implementação do TAD **Fila** (FIFO), onde as operações de inserção e remoção respeitam a ordem de chegada dos elementos.
2. **`LEF.h` e `LEF.c`**: Implementação do TAD **Lista de Eventos Futuros (LEF)**, onde eventos são armazenados e ordenados pelo campo **tempo** de cada evento, respeitando a política FIFO para eventos com o mesmo tempo.
3. **`testa_fila.c` e `testa_lef.c`**: Arquivos de teste para validar as operações de ambos os TADs.

---

## Descrição dos TADs

### **1. Fila (FIFO)**

A **Fila** é uma estrutura de dados que segue a política **First In First Out (FIFO)**. A inserção é realizada no final da fila e a remoção no início. A fila é implementada com uma lista ligada com nodo cabeça.

#### Funções Principais:

- **`fila_cria()`**: Cria uma nova fila vazia.
- **`fila_destroi()`**: Destrói a fila e libera a memória.
- **`enqueue()`**: Insere um elemento no final da fila.
- **`dequeue()`**: Remove um elemento do início da fila.
- **`fila_vazia()`**: Verifica se a fila está vazia.
- **`fila_tamanho()`**: Retorna o número de elementos na fila.

### **2. Lista de Eventos Futuros (LEF)**

A **Lista de Eventos Futuros (LEF)** é uma lista ligada que armazena eventos ordenados pelo campo **tempo**. Quando inserido, o evento é colocado na LEF de maneira que a lista fique ordenada em ordem crescente de tempo. Quando removido, o evento é retirado do início da lista.

#### Funções Principais:

- **`cria_evento()`**: Cria um evento com os dados fornecidos (tempo, tipo, dado1, dado2).
- **`destroi_evento()`**: Destroi um evento.
- **`cria_lef()`**: Cria uma LEF vazia.
- **`destroi_lef()`**: Destroi a LEF e libera a memória.
- **`insere_lef()`**: Insere um evento na LEF respeitando a ordem de tempos crescentes.
- **`retira_lef()`**: Retira o primeiro evento da LEF.
- **`vazia_lef()`**: Verifica se a LEF está vazia.
- **`imprime_lef()`**: Imprime os eventos da LEF para depuração.

---

Este repositório contém a implementação de duas estruturas de dados importantes para sistemas de simulação de eventos: a **Fila** (FIFO) e a **Lista de Eventos Futuros (LEF)**. Ambas são implementações fundamentais para manipulação de dados de forma eficiente em diversas aplicações.
