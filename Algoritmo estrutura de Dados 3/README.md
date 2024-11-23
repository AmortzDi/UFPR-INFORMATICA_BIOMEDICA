# Algoritmos de Estruturas de Dados - Árvores e Tabelas de Hash 👨🏽‍💻

## Descrição

Este repositório contém três implementações de algoritmos avançados para manipulação de dados utilizando estruturas como árvores binárias e tabelas de hash. Os algoritmos são focados na eficiência, tanto em termos de tempo quanto de memória, abordando cenários específicos de aplicações computacionais.

### Estruturas e Algoritmos Implementados

#### 1. **Árvore Vermelho-Preta (Red-Black Tree)**
- **Descrição:**  
  A árvore Red-Black é uma árvore binária de busca autobalanceada, garantindo uma altura máxima \( O(\log n) \).  
  É amplamente utilizada em sistemas onde o desempenho em operações de busca, inserção e remoção deve ser garantido mesmo no pior caso.

- **Características Principais:**  
  - Cada nó é vermelho ou preto.  
  - A raiz é sempre preta.  
  - Nenhum caminho de um nó para um filho nulo possui duas vezes mais nós vermelhos do que outro.  
  - Inserção e remoção mantêm as propriedades da árvore por meio de rotações e alterações de cores.

#### 2. **Árvore AVL**
- **Descrição:**  
  A árvore AVL é uma estrutura binária de busca autobalanceada que mantém o equilíbrio utilizando a diferença de alturas entre as subárvores esquerda e direita de qualquer nó.

- **Características Principais:**  
  - A diferença de alturas entre as subárvores de qualquer nó (fator de balanceamento) é no máximo 1.  
  - Realiza rotações simples ou duplas para corrigir o balanceamento após inserções ou remoções.  
  - Ideal para cenários onde a frequência de buscas é maior que a de inserções e remoções.

#### 3. **Tabela Hash com Algoritmo Cuckoo Hashing**
- **Descrição:**  
  O Cuckoo Hashing é uma técnica que resolve colisões em tabelas hash por meio de realocações. Quando uma posição já está ocupada, o elemento existente é movido para outra posição de hash, o que pode desencadear uma série de realocações.

- **Características Principais:**  
  - Usa duas ou mais funções de hash para calcular diferentes possíveis posições para cada elemento.  
  - Garante inserções em \( O(1) \) no caso médio.  
  - Evita o uso de listas encadeadas para resolver colisões, utilizando o espaço de forma mais eficiente.  

---

## Arquivos do Repositório

- **`rb_tree.c`**: Implementação da árvore Red-Black.  
- **`avl_tree.c`**: Implementação da árvore AVL.  
- **`cuckoo_hash.c`**: Implementação do algoritmo de Cuckoo Hashing.  
- **`main.c`**: Arquivo principal com exemplos de uso e testes para todas as estruturas.  


---

## Aplicações Práticas

- **Árvore Red-Black:**  
  Utilizada em bibliotecas padrão como `std::map` e `std::set` em C++ e Java.  
- **Árvore AVL:**  
  Indicada para sistemas que priorizam operações de busca como bancos de dados e sistemas de gerenciamento de cache.  
- **Cuckoo Hashing:**  
  Ideal para sistemas que necessitam de tabelas hash rápidas e com controle rígido de memória, como firewalls e sistemas embarcados.

---
