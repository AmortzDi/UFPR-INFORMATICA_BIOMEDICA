# Análise de Desempenho do YOLOv4 para Detecção de Células

Este projeto consiste em um conjunto de scripts para analisar o desempenho do modelo YOLOv4 na detecção de células, extraindo métricas de classificação e gerando visualizações gráficas.

## 📋 Visão Geral

- **Script Principal**: `analise_yolov.sh`  
  Automatiza todo o fluxo de análise, desde a localização dos arquivos de dados até a geração do relatório final.  
- **Gerador de Gráficos**: `gerador_grafico.py`  
  Cria gráficos de Precisão-Revocação (Precision-Recall) a partir dos dados processados.  

---
## ⚠️ Pré-requisitos Importantes

1.  **Bash**: Ambiente Linux  
    **Python 3.7+**: Para execução do script de gráficos.  

2. **Localização dos Arquivos**:  
   - Os arquivos de dados **precisam estar no mesmo diretorio** que o script `analise_yolov.sh`.  
   - Formato esperado:  (ex: `512_yolov4_100_dados_celulas.txt`).  

3. **Dependências**:  
   - O script cria automaticamente um ambiente virtual Python (`processamento`) e instala:  
     - `numpy`, `matplotlib`, `scikit-learn`.  

---

## 🚀 Exemplo de Execução
```
# 1. Garanta que os arquivos de dados estão no diretório:
ls *.txt
# Exemplo: 512_yolov4_100_dados_celulas.txt  1024_yolov4_100_dados_celulas.txt

2. **Execute o script principal**:
   ```bash
   chmod +x analise_yolov.sh
   ./analise_yolov.sh

# 3. Saídas esperadas:
ls relatorio_final.txt resultados_analise.txt grafico_*.png

## 🛠️ Funcionalidades

1. **Processamento Automatizado**:
   - Localiza arquivos de dados no formato `[TAMANHO]_yolov4_[EPOCAS]_dados_celulas.txt`.
   - Extrai métricas como Precisão, Recall e F1-score para diferentes thresholds de confiança.

2. **Análise de Desempenho**:
   - Identifica o melhor F1-score para cada tamanho de imagem.
   - Calcula a Average Precision (AP) e gera curvas Precision-Recall.

3. **Relatório Final**:
   - Consolida os resultados em um arquivo (`relatorio_final.txt`).
   - Inclui recomendações com base nas métricas analisadas.

4. **Ambiente Virtual**:
   - Cria um ambiente Python isolado (`processamento`) e instala as dependências necessárias.

---

## 📊 Métricas Analisadas

- **Precisão (Precision)**: Proporção de detecções corretas entre todas as predições positivas.  
- **Recall (Revocação)**: Proporção de detecções corretas em relação aos ground truths.  
- **F1-score**: Média harmônica entre Precisão e Recall.  
- **Average Precision (AP)**: Área sob a curva Precision-Recall.  

---


## 📂 Arquivos Gerados

O script produz os seguintes arquivos de saída:

### 📄 **1. `relatorio_final.txt`**
- **Conteúdo**:  
  - Resumo consolidado de todas as análises realizadas.  
  - Lista dos arquivos processados.  
  - Métricas por tamanho de imagem (Threshold, F1-score, Precisão, Recall).  
  - Recomendação final com a configuração ideal (melhor F1-score).  
  - Interpretação dos resultados e guia prático para ajustes.  

### 📄 **2. `resultados_analise.txt`**
- **Conteúdo**:  
  - Dados brutos das métricas para cada tamanho de imagem.  
  - Estrutura organizada por seções (ex: `=== TAMANHO: 512px ===`).  
  - Detalhes como:  
    - Nome do arquivo processado.  
    - Melhor threshold encontrado.  
    - Valores de F1-score, Precisão e Recall correspondentes.  

### 📊 **3. Gráficos (`grafico_[TAMANHO].png`)**
- **Descrição**:  
  - São gerados **3 arquivos PNG** (um para cada tamanho de imagem analisado).  
  - Nomes no formato `grafico_512.png`, `grafico_1024.png`, etc.  
  - Mostram a **curva Precision-Recall** com destaque para:  
    - Average Precision (AP).  
    - Ponto de melhor F1-score.  


   Como Interpretar o Gráfico:

   Se a curva do modelo estiver acima da linha de referência, significa que ele está superando um modelo aleatório ou um baseline simples.

   Uma curva mais próxima do canto superior direito indica um melhor compromisso entre precisão e revocação.

   Se a curva for muito próxima da linha de referência, pode indicar um modelo fraco ou a necessidade de ajustes no treinamento.

---
