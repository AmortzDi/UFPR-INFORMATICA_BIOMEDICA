#### 3. **Tabela Hash com Algoritmo Cuckoo Hashing** 👨🏽‍💻
- **Descrição:**  
  O Cuckoo Hashing é uma técnica que resolve colisões em tabelas hash por meio de realocações. Quando uma posição já está ocupada, o elemento existente é movido para outra posição de hash, o que pode desencadear uma série de realocações.

- **Características Principais:**  
  - Usa duas ou mais funções de hash para calcular diferentes possíveis posições para cada elemento.  
  - Garante inserções em \( O(1) \) no caso médio.  
  - Evita o uso de listas encadeadas para resolver colisões, utilizando o espaço de forma mais eficiente.  
