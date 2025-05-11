# Aluno: **Mozart Gustavo da Cruz Dias** 
# GRR: **20213396**

# 📦 Vinac Archive Manager

## 🔍 Visão Geral
**Vinac** é um gerenciador de arquivos compactados via linha de comando, o e eficiente, que oferece funcionalidades de compactação, extração e gerenciamento de arquivos em um único utilitário. Seus principais recursos incluem:

- 📁 Múltiplos modos de compactação (compactado e não compactado)
- 🔄 Reordenação de arquivos dentro do arquivo compactado
- 🧩 Extração seletiva
- 📜 Listagem do conteúdo do arquivo
- 🧠 Tratamento de arquivos duplicados com controle de versões

## ✨ Principais Funcionalidades

### 1️⃣ Modos de Compactação
- 🟤 **Inserção Não Compactada (`-ip`)**: Armazena os arquivos sem compactação.
- 🟢 **Inserção Compactada (`-ic`)**: Utiliza o algoritmo de compactação LZ para economia de espaço.

### 2️⃣ Gerenciamento de Arquivos
- 🔀 **Reordenação de Arquivos (`-m`)**: Move arquivos dentro do arquivo compactado.
  ```bash
  vinac -m arquivo.vnc arquivo_a_mover nova_posição
  ```

- 🗑️ **Remoção de Arquivos (`-r`)**: Remove arquivos do arquivo compactado.
  ```bash
  vinac -r arquivo.vnc arquivo1 arquivo2
  ```

### 3️⃣ Inspeção de Conteúdo
- 📄 **Listagem (`-c`)**: Exibe o conteúdo detalhado do arquivo, incluindo:
  - 📇 Metadados dos arquivos (UID, datas e horários)
  - 📏 Tamanhos original e compactado
  - 🧮 Ordem dos arquivos no arquivo compactado

### 4️⃣ Extração
- 📤 **Extração Seletiva (`-x`)**: Extrai arquivos específicos ou todos os arquivos.
  ```bash
  vinac -x arquivo.vnc arquivo1 arquivo2
  ```

## ⚙️ Destaques Técnicos

### 🏗️ Arquitetura 
- 🔁 Implementação com **lista duplamente encadeada**
- 🧵 **Operações seguras de memória**, com tratamento abrangente de erros
- 🗂️ Uso de **arquivos temporários** para garantir operações atômicas

### 🧠 Funcionalidades Avançadas
- 🧬 **Detecção de duplicatas** por data de modificação
- 🕒 **Preservação de metadados**
- 📌 **Gerenciamento de offsets** para arquivos grandes

## 🧪 Exemplos de Uso

```bash
# Criar novo arquivo com compactação
vinac -ic arquivo.vnc arquivo1.txt arquivo2.jpg

# Listar conteúdo do arquivo
vinac -c arquivo.vnc

# Mover arquivo dentro do arquivo compactado
vinac -m arquivo.vnc arquivo1.txt arquivo3.pdf

# Extrair todos os arquivos
vinac -x arquivo.vnc
```

## ❗ Tratamento de Erros
Mensagens de erro claras são exibidas para:
- Problemas de acesso a arquivos
- Operações inválidas
- Falhas de alocação de memória
- Arquivos corrompidos

## 🧱 Instruções de Compilação
Use `make` para compilar:

```bash
make
```

Use `make clean` para remover os arquivos objetos:

```bash
make clean
```
Compile com:
```bash
gcc -o vinac vina.c vinac.c lista.c lz.c
```

## 🚀 Considerações de Desempenho
- I/O com buffer para operações rápidas
- Cópia em blocos para arquivos grandes
- Uso de memória otimizado

## ⛔ Limitações
- Número máximo de arquivos definido por `LIMITE_MEMBROS`
- Formato proprietário (não compatível com ferramentas padrão)

## 🛠️ Melhorias Futuras
- Criptografia de arquivos
- Compactação multi-thread
- Verificação de integridade (checksum)

## 🗂️ Estrutura dos Arquivos

```
vinac/
├── vinac.c           # Comando principal e parser de argumentos
├── vina.c            # Lógica de manipulação dos arquivos
├── lista.c           # Implementação da lista duplamente encadeada
├── lz.c              # Algoritmo de compressão LZ
├── vinac.h, etc.     # Cabeçalhos e definições
└── arquivo.vc       # Arquivo compactado resultante
```

---

## 📁 Instruções de Execução

Para que o programa funcione corretamente, **todos os arquivos que serão inseridos ou extraídos devem estar no mesmo diretório que o executável `vinac`**.


## 🐛 Bugs Conhecidos

* Há um **vazamento de memória** conhecido ao executar o programa com as opções `-ip` ou `-ic`.
* Mesmo com o uso de ferramentas como o `valgrind`, **não foi possível localizar precisamente a origem** do vazamento. Isso pode indicar um problema relacionado à manipulação de arquivos ou à estrutura de dados em uso.

---

## 🧠 Estruturas de Dados e Decisões de Projeto

### 📚 Estruturas Utilizadas

Inicialmente foi considerada a utilização de **vetores** para armazenar os membros do arquivo `.vc`. No entanto, vetores possuem limitações:

* ❌ Tamanho fixo (difícil redimensionamento dinâmico)
* ❌ Ineficiência ao inserir/remover arquivos frequentemente

Para contornar essas limitações, optou-se por usar uma **lista duplamente encadeada**, que permite:

* ✅ Inserção e remoção eficientes
* ✅ Reordenação flexível dos arquivos
* ✅ Melhora no gerenciamento de memória e offsets

### 🧮 Algoritmos Empregados

* 🔁 **Leitura/escrita em blocos** para operações com grandes volumes de dados
* 🧠 **Compressão LZ** simples para otimização de espaço
* 📌 **Gerenciamento de offsets e metadados** via estrutura ligada em memória

Essa abordagem oferece **flexibilidade e escalabilidade**, especialmente útil quando o número de arquivos dentro do `.vc` cresce significativamente.

--- 
##  Fluxograma da lógica do programa VINAC
┌───────────────────────────────────────────────────────────────────────┐
│                  GERENCIADOR DE ARQUIVOS VINAC - FLUXOGRAMA           │
└───────────────────────────────────────────────────────────────────────┘
                                      │
                                      ▼
┌───────────────────────────────────────────────────────────────────────┐
│                            FLUXO PRINCIPAL                            │
└───────────────────────────────────────────────────────────────────────┘
                                      │
                                      ▼
                       ┌───────────────────────────────┐
                       │  Ler Argumentos da Linha de   │
                       │         Comando               │
                       └───────────────────────────────┘
                                      │
                                      ▼
                       ┌───────────────────────────────┐
                       │  Validar Parâmetros           │
                       │  - Verificar opção válida     │
                       │  - Checar arquivos membros    │
                       └───────────────────────────────┘
                                      │
                                      ▼
                       ┌───────────────────────────────┐
                       │  Abrir/Criar Arquivo Vinac    │
                       │  - Modo 'ab+' para operações  │
                       └───────────────────────────────┘
                                      │
                                      ▼
                       ┌───────────────────────────────┐
                       │  Executar Operação Selecionada│
                       └───────────────────────────────┘
                                      │
                                      ▼
┌───────────┐  ┌───────────┐  ┌───────────┐  ┌───────────┐  ┌───────────┐
│  -ip/-ic  │  │   -m      │  │   -x      │  │   -r      │  │   -c      │
│ Inserir   │  │ Mover     │  │ Extrair   │  │ Remover   │  │ Listar    │
└─────┬─────┘  └─────┬─────┘  └─────┬─────┘  └─────┬─────┘  └─────┬─────┘
      │              │              │              │              │
      ▼              ▼              ▼              ▼              ▼
┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐
│Criar    │    │Reordenar│    │Extrair  │    │Remover  │    │Ler      │
│Lista de │    │arquivos │    │arquivos │    │entradas │    │metadados│
│diretórios│   │do archive│   │seleciona-│   │do archive    │e exibir │
└─────────┘    └─────────┘    │dos      │    └─────────┘    └─────────┘
      │              │        └─────────┘
      ▼              ▼              │
┌─────────┐    ┌─────────┐         ▼
│Processar│    │Regravar │    ┌─────────┐
│cada     │    │arquivo  │    │Preservar│
│arquivo  │    │         │    │datas de │
│membro   │    └─────────┘    │modificação
└─────────┘         │         └─────────┘
      │             │              │
      ▼             ▼              ▼
┌───────────────────────────────────────────────────────────────────────┐
│                  Atualizar Metadados e Fechar Arquivo                 │
└───────────────────────────────────────────────────────────────────────┘
                                      │
                                      ▼
                       ┌───────────────────────────────┐
                       │  Salvar Lista de Diretórios   │
                       │  - Escrever offset final      │
                       │  - Atualizar metadados        │
                       └───────────────────────────────┘
                                      │
                                      ▼
                       ┌───────────────────────────────┐
                       │  Fechar Arquivo e Liberar     │
                       │         Memória               │
                       └───────────────────────────────┘

