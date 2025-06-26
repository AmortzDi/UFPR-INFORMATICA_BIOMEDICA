# Extração de Características Radiômicas com PyRadiomics

Este projeto implementa um pipeline para extrair características radiômicas de imagens médicas. Ele utiliza a biblioteca `pyradiomics` para a extração e o algoritmo de Otsu para a segmentação automática das regiões de interesse (ROIs), caso as máscaras não sejam fornecidas.

## Visão Geral do Projeto

O pipeline é composto por três módulos principais:

1.  **`data_manager.py`**: Responsável por mapear as imagens originais e suas máscaras correspondentes. Se as máscaras não existirem, ele as gera automaticamente usando o método de limiarização de Otsu.
2.  **`radiomics_extractor.py`**: Encapsula a configuração e a execução do extrator de características do PyRadiomics. Ele é configurado para extrair diversas características, excluindo as de forma.
3.  **`pipeline_runner.py`**: Orquestra todo o processo, chamando o `DataManager` para preparar os dados e o `RadiomicsExtractor` para extrair as características de todas as imagens. Os resultados são salvos em um arquivo CSV.

Após a extração das características, o notebook demonstra um exemplo de aplicação para classificação usando K-Nearest Neighbors (KNN) com validação cruzada por *folds* (pastas de dados), e apresenta as métricas consolidadas e matrizes de confusão por *fold*.

## Estrutura do Repositório

.
├── src/
│   ├── data_manager.py
│   ├── radiomics_extractor.py
│   └── pipeline_runner.py
├── main.py
└── trabalho_final.ipynb (Este notebook)


## Configuração do Ambiente

O projeto requer as seguintes bibliotecas Python:

* `SimpleITK`
* `pandas`
* `gdown`
* `pyradiomics`
* `tqdm` (para barras de progresso)
* `scikit-learn` (para o exemplo de classificação)
* `seaborn`
* `matplotlib`

Você pode instalar todas as dependências usando pip:

```bash
pip install -q SimpleITK pandas gdown pyradiomics scikit-learn seaborn matplotlib tqdm
Como Usar
1. Download e Descompactação dos Dados
O notebook inicia baixando um arquivo ZIP do Google Drive que contém as imagens médicas organizadas em folds.

Python

import gdown
import os
import zipfile

# ... (código do notebook para download e descompactação) ...
As imagens descompactadas estarão no diretório /content/dados_extraidos/. Máscaras Otsu geradas serão salvas em /content/masks_otsu_generated/.

2. Execução do Pipeline de Extração
O script main.py é o ponto de entrada para a execução do pipeline de extração de características.

Python

!python main.py
O script realizará as seguintes etapas:

Mapeamento de Imagens e Máscaras: O DataManager percorrerá os diretórios (fold1, fold2, etc.) em BASE_DATA_DIR (/content/dados_extraidos/), identificará as imagens (.dcm, .nii, .nii.gz, .mha) e, se necessário, gerará máscaras Otsu para elas no diretório MASKS_OTSU_DIR (/content/masks_otsu_generated/).
Extração de Características Radiômicas: Para cada par imagem-máscara, o RadiomicsExtractor utilizará a configuração definida (radiomics_params) para calcular um conjunto de características radiômicas. Atualmente, as características de forma (shape e shape2D) estão desabilitadas na configuração.
Salvamento dos Resultados: Todas as características extraídas, juntamente com o patient_id, class_label, fold e original_filename, serão consolidadas em um DataFrame do pandas e salvas em um arquivo CSV (/content/radiomic_database.csv).
3. Exemplo de Classificação (KNN)
O notebook inclui uma seção que demonstra como usar as características extraídas para um problema de classificação multiclasse usando KNN, com validação cruzada por folds. Ele calcula métricas como precisão, recall, F1-score e especificidade para cada classe e apresenta as médias consolidadas, além de visualizar as matrizes de confusão para cada fold.

Python

import pandas as pd
from sklearn.preprocessing import StandardScaler
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import classification_report, confusion_matrix
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

# ... (código do notebook para classificação e visualização) ...
Parâmetros do PyRadiomics
A configuração atual do pyradiomics no main.py é a seguinte:

Python

radiomics_params = {
    'setting': {
        'force2D': True,
        'force2Ddimension': 0
    },
    'featureClass': {
        'shape': None,        # Desabilitado
        'shape2D': None,      # Desabilitado

    }
}
Você pode modificar esses parâmetros em main.py para incluir ou excluir diferentes classes de características, aplicar filtros de imagem, ou ajustar outras configurações do pyradiomics conforme necessário. Consulte a documentação do PyRadiomics para mais detalhes sobre as opções de configuração.

Classes de Características Radiômicas
As classes de características que podem ser extraídas pelo pyradiomics incluem:

First Order: Descrevem a distribuição de intensidade dos voxels da ROI (e.g., média, desvio padrão, skewness).
Shape (Forma): Descrevem as características geométricas da ROI (desabilitadas neste projeto).
GLCM (Gray Level Co-occurrence Matrix): Descrevem a textura da imagem, examinando a relação espacial entre pares de voxels.
GLSZM (Gray Level Size Zone Matrix): Descrevem a textura através da distribuição de zonas de voxels com a mesma intensidade.
GLDM (Gray Level Dependence Matrix): Descrevem a dependência de intensidade de um voxel em relação aos seus vizinhos.
GLRLM (Gray Level Run Length Matrix): Descrevem a textura através da distribuição de "corridas" de voxels com a mesma intensidade.
NGTDM (Neighborhood Gray Tone Difference Matrix): Descrevem a textura com base nas diferenças de intensidade entre um voxel e seus vizinhos
