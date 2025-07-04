

# 🩻 Classificador de Imagens DICOM – Pneumotórax vs. Normal

Este projeto realiza o processamento e classificação de imagens DICOM (radiografias de tórax), diferenciando entre casos **normais** e com **pneumotórax**, usando métricas como correlação, Bhattacharyya, interseção e qui-quadrado.

## 📁 Estrutura do Projeto

```
├── processamento.ipynb       # Notebook principal com o pipeline de processamento e classificação
├── imagens/                  # Pasta com imagens DICOM (Normal e Pneumotórax)
├── resultados.txt               # Onde os resultados são salvos
├── README.md                 # Este arquivo
```

## ⚙️ Pré-requisitos

* Python 3.8+
* Jupyter Notebook
* Pip ou conda para gerenciar pacotes

### 📦 Bibliotecas necessárias

Instale com:

```bash
pip install opencv-python pydicom matplotlib numpy scikit-learn
```

Ou, se preferir usar `conda`:

```bash
conda install -c conda-forge opencv pydicom matplotlib numpy scikit-learn
```

## ▶️ Como Executar

1. **Clone ou baixe este repositório.**

2. **Abra o Jupyter Notebook:**

```bash
jupyter notebook
```

3. **Execute o notebook `processamento.ipynb`.**

   * O notebook carrega imagens DICOM de duas classes.
   * Realiza o pré-processamento das imagens.
   * Compara imagens usando diferentes métricas de similaridade.
   * Exibe os resultados de classificação.

4. **(Opcional)** Certifique-se de que as pastas `imagens/` ou outras utilizadas no notebook estejam corretamente organizadas, como:

```
imagens/
├── normal/
│   ├── imagem1.dcm
│   └── ...
└── pneumotorax/
    ├── imagem1.dcm
    └── ...
```

## 📊 Resultados Esperados

* Visualização de imagens de entrada.
* Cálculo de distâncias entre imagens.
* Classificação baseada em similaridade.
* Comparação entre as métricas utilizadas.

## 📌 Observações

* Certifique-se de que as imagens estejam no formato `.dcm`.
* Verifique os caminhos no notebook caso as pastas estejam em outro local.

---
