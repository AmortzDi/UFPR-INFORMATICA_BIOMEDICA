# Lógica dos Algoritmos 👨🏽‍💻

Este documento descreve os principais algoritmos de ordenação, destacando suas características, formas de implementação e casos de uso. Todos os algoritmos apresentados possuem versões iterativas e recursivas.

---

## **Algoritmos de Ordenação**

### **1. Merge Sort**
- **Funcionamento:**  
  O Merge Sort utiliza a técnica de **divisão e conquista**. Ele divide a lista em sublistas menores até que cada uma contenha apenas um elemento e, em seguida, combina essas sublistas de forma ordenada.

- **Formas de Implementação:**  
  - **Recursiva:** Divide a lista repetidamente e combina as sublistas ordenadas.
  - **Iterativa:** Usa laços para dividir e mesclar iterativamente as sublistas.

- **Características:**  
  - Complexidade: \(O(n \log n)\) no melhor e no pior caso.  
  - Estável: preserva a ordem relativa de elementos iguais.  
  - Ideal para listas grandes.

---

### **2. Quick Sort**
- **Funcionamento:**  
  O Quick Sort também utiliza a técnica de **divisão e conquista**. Ele seleciona um **pivô** e rearranja os elementos para que os menores fiquem antes do pivô e os maiores fiquem após. Este processo se repete para as sublistas.

- **Formas de Implementação:**  
  - **Recursiva:** Utiliza chamadas recursivas para ordenar sublistas à esquerda e à direita do pivô.
  - **Iterativa:** Utiliza uma pilha ou estrutura auxiliar para substituir as chamadas recursivas.

- **Características:**  
  - Complexidade:  
    - Melhor caso: \(O(n \log n)\).  
    - Pior caso: \(O(n^2)\) (quando o pivô é mal escolhido).  
  - Não é estável: pode alterar a ordem relativa de elementos iguais.  
  - É muito eficiente para listas médias e grandes.

---

### **3. Heap Sort**
- **Funcionamento:**  
  O Heap Sort transforma a lista em uma **estrutura de heap** (máximo ou mínimo) e, em seguida, extrai o maior (ou menor) elemento do heap repetidamente para criar a lista ordenada.

- **Formas de Implementação:**  
  - **Iterativa:** Constrói e ajusta o heap utilizando laços.
  - **Recursiva:** Ajusta o heap por meio de chamadas recursivas.

- **Características:**  
  - Complexidade: \(O(n \log n)\) no melhor e no pior caso.  
  - Não é estável.  
  - Útil quando o espaço adicional precisa ser minimizado, pois é um algoritmo **in-place** (não requer espaço extra significativo).

