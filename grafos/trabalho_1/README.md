

## 🧩 Descrição Geral

Este projeto implementa um conjunto de módulos em linguagem **C** para **manipulação e análise de grafos direcionados**, com foco principal na **detecção de ciclos** utilizando a técnica de **busca em profundidade (DFS)** na versão **on-the-fly**  ou seja, o ciclo é detectado **durante** a execução da busca, sem necessidade de percorrer o grafo duas vezes.

A implementação é **modular**, permitindo que cada parte do sistema (leitura, estrutura de dados, análise e detecção) seja mantida e evoluída separadamente.

---

## 🧱 Estrutura do Projeto

| Arquivo                                 | Função                                                                                                                                                              |
| --------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **grafo.c / grafo.h**                   | Biblioteca principal de grafos baseada em **listas de adjacência** (versão utilizada no Trabalho 1). Responsável pela criação e manipulação de grafos direcionados. |
| **lista.c / lista.h**                   | Implementa listas ligadas para representar as adjacências de cada vértice.                                                                                          |
| **tabela.c / tabela.h**                 | Implementa uma **tabela hash** que faz o mapeamento entre nomes (strings) e índices numéricos, permitindo o uso eficiente dos vetores de análise.                   |
| **leitor_rede.c / leitor_rede.h**       | Lê arquivos de entrada que descrevem a rede (grafo), converte os nomes de vértices para IDs numéricos usando a tabela hash e constrói o grafo.                      |
| **analise.c**                           | Arquivo principal do programa. Controla o fluxo geral da execução: leitura do grafo, criação da estrutura, análise e chamada da função de detecção de ciclos.       |
| **detectar_ciclo.c / detectar_ciclo.h** | Implementa o algoritmo **on-the-fly** de **detecção de ciclos** em grafos direcionados.                                                                             |
| **makefile**                            | Define o processo de compilação e limpeza automática dos arquivos.                                                                                                  |

---

## 🔁 Relação entre os Módulos

1. **`leitor_rede.c`** lê o arquivo de entrada e usa a **tabela hash** (`tabela.c`) para mapear nomes de vértices (strings) em **índices inteiros únicos**.
2. Esses índices são usados pelo **`grafo.c`** para construir o grafo usando **listas de adjacência** (implementadas em `lista.c`).
3. O **`analise.c`** coordena a execução geral, chamando as funções de construção e análise do grafo.
4. O **`detectar_ciclo.c`** executa o algoritmo DFS on-the-fly, que usa vetores auxiliares (`pre[]`, `post[]`) para marcar o estado de cada vértice durante a busca.
5. Esses vetores dependem dos índices numéricos fornecidos pela tabela hash, pois o acesso em C é feito via `pre[v]`, `post[v]`, etc.

---

## 🧮 Uso da Tabela Hash

A **tabela hash** é utilizada para o mapeamento entre nomes e índices.

### 🔧 Razões para o uso da tabela hash

1. **Compatibilidade com vetores numéricos:**
   Os algoritmos de análise (como DFS e detecção de ciclo) dependem de vetores (`pre[]`, `post[]`) que só podem ser indexados por números inteiros.
   Sem o mapeamento via tabela hash, seria impossível acessar corretamente as posições desses vetores usando nomes de vértices do tipo string.

2. **Evita redundância e economiza memória:**
   Um mesmo nome (ex: `"C00149"`) pode aparecer dezenas de vezes.
   A tabela hash armazena a string apenas **uma vez**, associando-a a um número inteiro.
   Assim, economiza memória e evita duplicação de dados.

3. **Eficiência nas buscas:**
   Durante a leitura e construção do grafo, ocorrem muitas buscas por vértices.
   Segundo os conceitos da disciplina **Algoritmos e Estruturas de Dados 3**, quando há muitas buscas, **tabelas hash** oferecem o melhor desempenho médio (**O(1)** por operação).
   Outras alternativas, como listas lineares, seriam menos eficientes nesse contexto.

---

## 🧱 Estruturas de Dados Utilizadas

O projeto combina **estruturas dinâmicas e estáticas**.

| Estrutura                      | Tipo                      | Arquivo                      | Função e Justificativa                                                                                                  |
| ------------------------------ | ------------------------- | ---------------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| **Lista encadeada**            | Dinâmica                  | `lista.c`                    | Armazena as adjacências de cada vértice. Escolhida por permitir inserções rápidas e uso eficiente de memória.           |
| **Tabela hash**                | Dinâmica                  | `tabela.c`                   | Mapeia nomes (strings) para índices inteiros. Garante acesso e inserção rápidos (O(1) médio).                           |
| **Grafo (struct Graph)**       | Estrutura composta        | `grafo.c`                    | Contém o número de vértices e um vetor de listas de adjacência.                                         |
| **Strings**                    | Dinâmicas (via ponteiros) | `leitor_rede.c` / `tabela.c` | Representam os nomes dos vértices originais lidos do arquivo.                                                           |

Essas estruturas se integram de forma coesa:

* A tabela hash converte nomes em IDs inteiros;
* O grafo usa esses IDs para armazenar as conexões;
* As listas representam eficientemente as adjacências;
* Os vetores permitem execução rápida da DFS e detecção de ciclos.

---

## 🔍 Algoritmo de Detecção de Ciclos

Implementado em `detectar_ciclo.c`, o algoritmo usa **DFS on-the-fly**.


* `V`: número de vértices
* `A`: número de arcos (arestas direcionadas)

* o **algoritmo original do Sedgewick (com vetores globais)**, e
* a **versão implementada no projeto (com `pre` e `pos` dentro da struct `t_vertice`)**,

---

Implementado em `detectar_ciclo.c`, o algoritmo usa **busca em profundidade (DFS)** para detectar ciclos **on-the-fly** ou seja, durante a própria execução da busca, sem precisar examinar o grafo duas vezes.

### Lógica básica

* Cada vértice recebe dois marcadores:

  * `pre`: momento em que o vértice é descoberto;
  * `pos`: momento em que termina a exploração de suas adjacências.
* Ao explorar uma aresta `v → w`:

  * Se `w` ainda não foi visitado, a DFS continua.
  * Se `w` foi descoberto, mas ainda não finalizado (`pos == -1`), há um **arco de retorno** o grafo contém um **ciclo**.

---

### 🔄 Diferença entre o algoritmo original e o implementado

| Aspecto                              | Versão Sedgewick (original)          | Versão do Projeto (implementada)                               |
| ------------------------------------ | ------------------------------------ | -------------------------------------------------------------- |
| **Armazenamento de `pre` e `post`**  | Vetores globais: `pre[v]`, `post[v]` | Campos dentro da struct `t_vertice`: `v->pre`, `v->pos`        |
| **Identificação dos vértices**       | Índices inteiros (0…V–1)             | Estruturas de vértice (`t_vertice`) com ID, tipo, e fronteiras |
| **Acesso aos dados**                 | Via índice numérico                  | Via ponteiro para vértice                                      |
| **Dependência de variáveis globais** | Alta                                 | Nenhuma cada vértice guarda seu próprio estado               |
| **Estilo de implementação**          | Estruturado e procedural             | Modular e orientado a dados (encapsula atributos do vértice)   |

---


A decisão de **incluir `pre` e `pos` dentro da estrutura `t_vertice`** foi  por:

* **Organização e clareza:** Cada vértice carrega todas as informações relevantes (tipo, fronteiras, tempos de descoberta e finalização, pai na DFS).
* **Evitar variáveis globais:** O código fica  modular e seguro, reduzindo dependências externas.
* **Facilidade de extensão:** Permite expandir o algoritmo (ex: armazenar profundidade, cor ou outros atributos) sem alterar a lógica principal.
* **Integração natural com o grafo por listas:** Como os vértices já são acessados via ponteiros, manter os tempos `pre` e `pos` dentro deles é mais direto .

---

## ⚙️ Compilação e Execução

A compilação é controlada pelo **makefile**.

### Compilar:

```bash
make
```

### Executar:

```bash
./analise entrada.met
```

### Limpar:

```bash
make clean
```

---

## 🧰 Funções Principais

| Função                    | Descrição                                             |
| ------------------------- | ----------------------------------------------------- |
| `GRIA_GRAFO(V)`           | Inicializa um grafo com `V` vértices.                 |
| `Adiciona_aresta (G, v, w)`| Adiciona uma aresta direcionada `v → w`.             |
| `GRAPHcycle(G)`           | Verifica se o grafo possui ciclos.                    |
| `dfsRhcy(G, v)`           | Função recursiva auxiliar de detecção de ciclos.      |
| `tabelaInsere()`          | Insere um novo par (string, índice) na tabela hash.   |
| `tabelaBusca()`           | Retorna o índice associado a uma string.              |
| `leitor_rede()`           | Lê o arquivo e constrói o grafo usando a tabela hash. |

---

## ⚠️ Limitações Conhecidas

* Vetores de tamanho fixo (ex: até 1000 vértices).
* Detecta apenas a existência de ciclo, sem exibir o ciclo.
* O formato da entrada deve estar correto (um arco por linha).

---

## 🧠 Base Teórica

> **Sedgewick, R. & Wayne, K.**
> *Algoritmos: Teoria e Prática*
> Capítulo: *Ciclos e Dags*
> Seção: *Implementação on-the-fly do algoritmo de detecção de ciclos*
> Link: *https://www.ime.usp.br/~pf/algoritmos_para_grafos/aulas/cycles-and-dags.html#sec:on-the-fly*

---

## 🧩 Código Base Utilizado

A biblioteca de grafos deste projeto foi baseada no **Trabalho 1**, usando a **representação por listas de adjacência**.
Essa base foi expandida e adaptada para incluir a análise de ciclos e integração modular.

---


