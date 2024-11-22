# Lógica dos Algoritmos 👨🏽‍💻

Este documento descreve os principais algoritmos de ordenação e técnicas de busca, destacando suas características, diferenças e métodos de implementação.

---

## **Algoritmos de Ordenação**

### **1. Merge Sort**
- **Funcionamento:**  
  O Merge Sort utiliza a técnica de **divisão e conquista**. Ele divide a lista em partes menores até que cada sublista contenha um único elemento e depois combina essas sublistas de forma ordenada.  

- **Formas de Implementação:**  
  - **Recursiva:** Divide a lista repetidamente e mescla as sublistas de maneira ordenada.

- **Características:**  
  - Complexidade: \(O(n \log n)\) no melhor e no pior caso.  
  - Estável: preserva a ordem relativa de elementos iguais.  
  - Bom desempenho para listas grandes.

---

### **2. Selection Sort**
- **Funcionamento:**  
  O Selection Sort percorre a lista, seleciona o menor elemento da parte não ordenada e move-o para a posição correta na parte ordenada.  

- **Formas de Implementação:**  
  - **Iterativa:** Usa laços para mover elementos repetidamente até que a lista esteja ordenada.

- **Características:**  
  - Complexidade: \(O(n^2)\) no melhor e no pior caso.  
  - Simples de implementar, mas ineficiente para listas grandes.  

---

### **3. Insertion Sort**
- **Funcionamento:**  
  O Insertion Sort insere cada novo elemento na posição correta dentro da sublista já ordenada.  

- **Formas de Implementação:**  
  - **Iterativa:** Insere cada elemento um por vez em sua posição correta.  

- **Características:**  
  - Complexidade: \(O(n^2)\) no pior caso e \(O(n)\) no melhor caso (quando a lista está quase ordenada).  
  - Estável: preserva a ordem relativa de elementos iguais.  
  - Eficiente para listas pequenas ou quase ordenadas.

---

## **Técnicas de Busca**

### **1. Busca Binária**
- **Funcionamento:**  
  A busca binária requer que a lista esteja previamente ordenada. O algoritmo divide a lista pela metade em cada iteração, comparando o elemento central com o alvo, reduzindo drasticamente o espaço de busca.  

- **Características:**  
  - Complexidade: \(O(\log n)\) no pior caso e \(O(1)\) no melhor caso.  
  - Muito eficiente para listas grandes.  

- **Exemplo de Uso:**  
  Similar a procurar uma palavra em um dicionário.

---

### **2. Busca Sequencial**
- **Funcionamento:**  
  A busca sequencial percorre cada elemento da lista, comparando-o com o alvo até encontrá-lo ou alcançar o final.  

- **Características:**  
  - Complexidade: \(O(n)\) no pior caso e \(O(1)\) no melhor caso.  
  - Funciona em listas desordenadas.  

- **Exemplo de Uso:**  
  Similar a procurar um item em uma prateleira examinando item por item.

---

