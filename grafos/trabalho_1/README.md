# Trabalhos de Grafos

Este repositório contém **dois trabalhos práticos sobre Grafos**, desenvolvidos como parte dos estudos de Estruturas de Dados.

Cada trabalho apresenta uma forma diferente de **representação de grafos**:

* **Matriz de Adjacência**
* **Lista de Adjacência**

O objetivo é comparar as implementações, compreender suas diferenças e analisar vantagens e desvantagens de cada abordagem.

---

## 📁 Estrutura de Pastas

O repositório está organizado da seguinte forma:

```
.
├── grafo_matriz/
│   ├── grafo.h
│   ├── grafo.c
│   ├── testa_grafos.c
│   └── Makefile
│
├── grafo_lista/
│   ├── lista.h
│   ├── lista.c
│   ├── grafo.h
│   ├── grafo.c
│   ├── testa_grafos.c
│   └── Makefile
│
└── README.md
```

---

## 🧮 Trabalho 1 — Grafo por Matriz de Adjacência

Neste trabalho, o grafo é representado utilizando uma **matriz de adjacência**, onde:

* As linhas e colunas representam os vértices;
* Cada posição da matriz indica a existência (ou peso) de uma aresta entre dois vértices.

### Características

* Implementação simples;
* Acesso rápido para verificar se existe uma aresta entre dois vértices;
* Maior consumo de memória, principalmente para grafos esparsos.

### Arquivos principais

* `grafo.h` / `grafo.c`: definição da estrutura do grafo e suas operações;
* `testa_grafos.c`: arquivo de teste das funcionalidades;
* `Makefile`: compilação do projeto.

---

## 📜 Trabalho 2 — Grafo por Lista de Adjacência

Neste trabalho, o grafo é representado utilizando **listas de adjacência**, onde:

* Cada vértice possui uma lista contendo seus vizinhos;
* A estrutura de listas é implementada manualmente.

### Características

* Uso mais eficiente de memória para grafos esparsos;
* Percorrer vizinhos é mais eficiente;
* Verificação direta de arestas pode ser mais custosa.

### Arquivos principais

* `lista.h` / `lista.c`: implementação da lista encadeada;
* `grafo.h` / `grafo.c`: implementação do grafo usando listas;
* `testa_grafos.c`: testes das operações do grafo;
* `Makefile`: compilação do projeto.

---

## ⚙️ Compilação e Execução

Para compilar qualquer um dos trabalhos, entre na pasta correspondente e execute:

```bash
make
```

Após a compilação, execute o programa gerado:

```bash
./testa_grafos
```

---

## 🎯 Objetivos do Trabalho

* Compreender diferentes formas de representação de grafos;
* Implementar operações básicas em grafos;
* Comparar desempenho e consumo de memória;
* Praticar modularização e uso de arquivos `.h` e `.c`.


---

Se quiser, posso **adaptar o texto para um trabalho acadêmico**, **simplificar**, ou **alinhar exatamente com o enunciado do professor**.
