#!/bin/bash

# Script de análise de desempenho do YOLOv4 para detecção de células

# Variáveis globais
PLOT_SCRIPT="gerador_grafico.py"
DATA_FILE="resultados_analise.txt"
OUTPUT_REPORT="relatorio_final.txt"
VIRTUAL="processamento"

#######################################
# Localiza arquivos para processamento
# Globais:
#   Nenhuma
# Argumentos:
#   Nenhum
# Retorno:
#   Lista de arquivos encontrados
#######################################
localizar_arquivos() {
    local padrao='[0-9]+_yolov4_[0-9]+_dados_celulas\.txt'
    local files=($(ls | grep -E "$padrao" | sort -n))
    
    if [ ${#files[@]} -eq 0 ]; then
        echo "ERRO: Nenhum arquivo encontrado no formato [TAMANHO]_yolov4_[EPOCAS]_dados_celulas.txt" >&2
        return 1
    fi
    
    echo "${files[@]}"
}

#######################################
# Extrai o tamanho do nome do arquivo
# Globais:
#   Nenhuma
# Argumentos:
#   $1: nome do arquivo
# Retorno:
#   Tamanho extraído ou vazio se não encontrar
#######################################
extrair_tamanho() {
    local file="$1"
    echo "$file" | grep -o -E '^[0-9]+' | head -1
}

#######################################
# Processa as métricas do arquivo de entrada
# Globais:
#   Nenhuma
# Argumentos:
#   $1: arquivo de entrada
#   $2: arquivo de saída
#   $3: tamanho da imagem
# Retorno:
#   Salva métricas no arquivo de saída
#######################################
# Funcao para processar metricas de classificacao
# Recebe: arquivo de entrada, arquivo de saida e tamanho
processamento_metricas() {
    local input_file="$1"
    local output_file="$2"
    local size="$3"
    
    # Verifica se o arquivo de entrada existe
    if [ ! -f "$input_file" ]; then
        echo "Erro: Arquivo de entrada nao encontrado: $input_file" >&2
        return 1
    fi

    # Extrai linhas com limiares e calcula metricas
    grep "for conf_thresh = " "$input_file" | awk -v size="$size" '{
        # Extrai e limpa os valores
        thresh = substr($4, 1, length($4)-1);  # Remove ultimo caractere (virgula)
        tp = $7; gsub(/,/, "", tp);           # Remove virgulas de TP
        fp = $10; gsub(/,/, "", fp);          # Remove virgulas de FP
        fn = $13; gsub(/,/, "", fn);          # Remove virgulas de FN

        # Converte strings para numeros
        thresh += 0; tp += 0; fp += 0; fn += 0;

        # Calcula metricas com protecao contra divisao por zero
        precisao = (tp + fp > 0) ? tp / (tp + fp) : 0;  # Precision
        recall = (tp + fn > 0) ? tp / (tp + fn) : 0;    # Recall/Sensitivity
        f1 = (precisao + recall > 0) ? 2 * (precisao * recall) / (precisao + recall) : 0;  # F1-score

        # Saida formatada com 4 casas decimais
        printf "%.4f %.4f %.4f %.4f\n", thresh, precisao, recall, f1;
    }' > "$output_file"

    # Verifica se o arquivo de saida foi criado com conteudo
    if [ ! -s "$output_file" ]; then
        echo "Erro: Nenhum dado foi processado. Verifique o formato do arquivo de entrada." >&2
        return 1
    fi
}

# Funcao para encontrar o melhor F1-score
# Recebe: arquivo com metricas processadas
melhor_f1() {
    local input_file="$1"

    # Verifica se o arquivo existe e nao esta vazio
    if [ ! -s "$input_file" ]; then
        echo "Erro: Arquivo para calculo de F1 nao encontrado ou vazio: $input_file" >&2
        return 1
    fi

    # Encontra a linha com o maior F1-score
    awk 'BEGIN {
        max_f1 = 0
        melhor_th = 0
        melhor_p = 0
        melhor_r = 0
    }
    {
        if ($4 > max_f1) {
            max_f1 = $4
            melhor_th = $1
            melhor_p = $2
            melhor_r = $3
        }
    }
    END {
        printf "%.4f %.4f %.4f %.4f", melhor_th, max_f1, melhor_p, melhor_r
    }' "$input_file"
}

# Funcao principal para manipulacao dos dados
# Recebe: arquivo original, tamanho, arquivo de saida, arquivo de entrada
manipulacao_dados() {
    local file="$1"
    local size="$2"
    local output_file="$3"
    local input_file="$4"
    
    echo "Processando: $file (Tamanho: ${size}px)" >&2

    # Processa as metricas
    if ! processamento_metricas "$file" "$output_file" "$size"; then
        echo "Erro durante o processamento das metricas!" >&2
        return 1
    fi
   
    # Verifica se o arquivo foi criado com conteudo
    if [ ! -s "$output_file" ]; then
        echo "Erro: Nenhum dado foi extraido para $output_file!" >&2
        return 1
    fi

    echo "Dados extraidos com sucesso para $output_file" >&2

    # Encontra o melhor F1-score
    local melhor
    if ! melhor=$(melhor_f1 "$output_file"); then
        return 1
    fi
    
    # Retorna resultados no formato: threshold f1-score precision recall
    echo "$melhor"
}
#######################################
# Gera gráficos a partir dos dados
# Globais:
#   Nenhuma
# Argumentos:
#   $1: tamanho da imagem
# Retorno:
#   Gera arquivos de imagem com os gráficos
#######################################
gerar_graficos() {
    local size="$1"
    local output_file="$2"
    
    if [ ! -f "$output_file" ]; then
        echo "Erro: Arquivo de dados '$output_file' não encontrado!" >&2
        return 1
    fi

    if [ ! -s "$output_file" ]; then
        echo "Erro: Arquivo de dados '$output_file' está vazio!" >&2
        return 1
    fi

    if [ ! -f "$PLOT_SCRIPT" ]; then
        echo "Erro: Script Python '$PLOT_SCRIPT' não encontrado!" >&2
        
        return 1
    fi

    if ! python3 "$PLOT_SCRIPT" "${size}" "$output_file" "grafico_${size}"; then
        echo "Erro ao gerar gráficos com o script Python!" >&2
        
        return 1
    fi

    

    echo "Gráfico gerado com sucesso:"
   
}

#######################################
# Salva resultados individuais
# Globais:
#   DATA_FILE: arquivo de saída
# Argumentos:
#   $1: tamanho da imagem
#   $2: arquivo processado
#   $3: melhores métricas
#   $4: valor AUC
# Retorno:
#   Adiciona resultados ao arquivo DATA_FILE
#######################################
salvar_resultados() {
    local size="$1"
    local file="$2"
    local best="$3"
   
    
    {
        echo "=== TAMANHO: ${size}px ==="
        echo "Arquivo: $file"
        echo "Melhor Threshold: $(echo "$best" | awk '{printf "%.3f", $1}')"
        echo "F1-score: $(echo "$best" | awk '{printf "%.3f", $2}')"
        echo "Precisao: $(echo "$best" | awk '{printf "%.3f", $3}')"
        echo "Recall: $(echo "$best" | awk '{printf "%.3f", $4}')"
        echo "========================="
        echo ""
    } >> "$DATA_FILE"
}

#######################################
# Gera o relatório final
# Globais:
#   DATA_FILE: arquivo de entrada
#   OUTPUT_REPORT: arquivo de saída
# Argumentos:
#   Nenhum
# Retorno:
#   Cria o arquivo de relatório final
#######################################
gerar_relatorio() {
    
    # Processa o melhor resultado baseado no F1-score
    local best_config=$(awk '
        BEGIN {OFS=" "}
        /=== TAMANHO:/ {size=$3; getline; file=$2} 
        /Melhor Threshold:/ {th=$3} 
        /F1-score:/ {f1=$2}
        /Precisao:/ {p=$2}
        /Recall:/ {r=$2}
        {if (f1 != "") {print size, file, th, f1, p, r; f1=""}}' "$DATA_FILE" | 
        sort -k4 -nr | head -n1)

    {
        echo "RELATÓRIO FINAL - YOLOv4"
        echo "================================="
        echo "Data: $(date '+%d/%m/%Y %H:%M:%S')"
        echo "Nome: Mozart Dias"
        echo "GRR: 20213396"
        echo ""
        echo "ARQUIVOS ANALISADOS:"
        grep "Arquivo:" "$DATA_FILE" | sed 's/Arquivo:/ - /'
        echo ""
        echo "RESUMO DAS MÉTRICAS:"
        cat "$DATA_FILE"
        echo ""
        
        if [ -n "$best_config" ]; then
            echo "Recomendação Final:"
            echo "------------------"
            echo "O tamanho de imagem ideal é $(echo "$best_config" | awk '{print $1}') com threshold $(echo "$best_config" | awk '{print $3}') porque:"
            echo "- Apresentou o maior F1-score: $(echo "$best_config" | awk '{print $4}')"
            echo "- Boa combinação de Precision ($(echo "$best_config" | awk '{print $5}')) e Recall ($(echo "$best_config" | awk '{print $6}'))"
        else
            echo "AVISO: Não foi possível determinar uma configuração ótima"
        fi
        
        echo ""
        echo "INTERPRETAÇÃO DOS RESULTADOS:"
        echo "1. Desempenho Geral:"
        echo "   - O modelo apresenta um bom desempenho geral (AP ~91% nos dados analisados)"
        echo "   - O IoU médio aumenta com thresholds mais altos (47-73%)"
        echo "   - Detecções mais confiantes têm melhor sobreposição com os ground truths"
        echo ""
        echo "2. Trade-off Precision-Recall:"
        echo "   - Thresholds baixos (0.00-0.15): Alto Recall, Baixa Precision"
        echo "   - Thresholds altos (0.70-0.95): Alta Precision, Baixo Recall"
        echo ""
        echo "3. Guia Prático:"
        echo "   - Diagnóstico médico: Prefira thresholds baixos (0.15-0.30)"
        echo "   - Automação: Use thresholds médios (0.40-0.60)"
        echo "   - Máxima precisão: Thresholds altos (0.70+)"
        echo ""
        echo "GRÁFICOS GERADOS:"
        echo "- grafico_[TAMANHO]: Curvas Precision-Recall"
        } > "$OUTPUT_REPORT"
}
criar_ambiente_virtual() {
    local nome_ambiente="${1:-venv}"
    local python_cmd="${2:-python3}"
    
    # Verificar se Python está instalado
    if ! command -v "$python_cmd" &>/dev/null; then
        echo "Erro: Python ($python_cmd) não encontrado!" >&2
        return 1
    fi

    # Verificar versão do Python
    if ! "$python_cmd" -c "import sys; sys.exit(sys.version_info < (3, 7))"; then
        echo "Erro: Python 3.7 ou superior necessário!" >&2
        return 1
    fi

    # Criar ambiente virtual
    echo "Criando ambiente virtual '$nome_ambiente'..."
    "$python_cmd" -m venv "$nome_ambiente" || {
        echo "Erro ao criar ambiente virtual!" >&2
        return 1
    }

    # Ativar e instalar pacotes
    echo "Instalando dependências..."
    source "$nome_ambiente/bin/activate" && \
    pip install --upgrade pip && \
    pip install numpy matplotlib scikit-learn && \
    

    echo -e "\n Ambiente pronto!"
    echo "Para ativar: source $nome_ambiente/bin/activate"
    echo "Para desativar: deactivate"
    echo "Pacotes instalados:"
    pip list | grep -E "numpy|matplotlib|scikit-learn"
}
remover_ambiente_virtual() {
    # Verifica se há ambiente virtual ativo
    if [ -z "$VIRTUAL_ENV" ]; then
        echo "Nenhum ambiente virtual ativo encontrado."
        return 1
    fi

    local nome_ambiente=$(basename "$VIRTUAL_ENV")
    local caminho_ambiente="$VIRTUAL_ENV"

    echo "Iniciando remoção do ambiente: $nome_ambiente"
    echo "--------------------------------"

    # 1. Desinstalar todas as bibliotecas
    echo "Desinstalando bibliotecas..."
    pip freeze | xargs pip uninstall -y 2>/dev/null

    # 2. Desativar ambiente
    echo "Desativando ambiente..."
    deactivate

    # 3. Remover diretório do ambiente
    echo "Removendo diretório..."
    rm -rf "$caminho_ambiente"

    echo "Remoção concluída. Ambiente '$nome_ambiente' foi completamente removido."
}
#######################################
# Função principal
# Globais:
#   Nenhuma
# Argumentos:
#   Nenhum
# Retorno:
#   Executa todo o fluxo de análise
#######################################
main() {
    # Configuração inicial
    clear
     # Limpeza de arquivos anteriores
    > "$DATA_FILE"
    rm -f *.tmp *.png "$OUTPUT_REPORT" 2>/dev/null

    criar_ambiente_virtual "$VIRTUAL" || {
        echo "ERRO: Falha ao criar ambiente virtual" >&2
        exit 1
    }
    source "$VIRTUAL/bin/activate"
    echo "INÍCIO DO PROCESSAMENTO - $(date '+%d/%m/%Y %H:%M:%S')"
    echo "============================================"

    # Localização de arquivos
    echo -e "\n[ETAPA 1/4] Localizando arquivos para análise..."
    files=($(localizar_arquivos)) || { 
        echo "ERRO: Falha ao localizar arquivos para processamento" >&2
        exit 1
    }

    echo -e "\nArquivos encontrados (${#files[@]}):"
    printf " - %s\n" "${files[@]}"
    echo ""

    # Processamento dos arquivos
    echo "[ETAPA 2/4] Processando arquivos..."
    for file in "${files[@]}"; do
        size=$(extrair_tamanho "$file")
        
        if [ -z "$size" ]; then
            echo "AVISO: Não foi possível determinar o tamanho de $file - Pulando..." >&2
            continue
        fi

        local output_file="dados_${size}.tmp"
        local input_file="melhor_f1${size}.txt"  

        echo -e "\n----------------------------------------"
        echo "PROCESSANDO: $file"
        echo "TAMANHO: ${size}px"
        echo "----------------------------------------"
        
        # Processa dados e obtém métricas
        resultados=$(manipulacao_dados "$file" "$size" "$output_file" "$input_file")
        if [ $? -ne 0 ]; then
            echo "ERRO: Falha no processamento de $file - Pulando..." >&2
            continue
        fi
    
        best=$(echo "$resultados" | awk '{print $1, $2, $3, $4}')

        salvar_resultados "$size" "$file" "$best" || {
            echo "ERRO: Falha ao salvar resultados para $file" >&2
            continue
        }

        
        # Salva resultados e gera gráficos
        gerar_graficos "$size" "$output_file"
        echo -e "\nRESULTADOS PARA ${size}px:"
        echo " - Melhor Threshold: $(echo "$best" | awk '{print $1}')"
        echo " - F1-score: $(echo "$best" | awk '{print $2}')"
        echo " - Precision: $(echo "$best" | awk '{print $3}')"
        echo " - Recall: $(echo "$best" | awk '{print $4}')"
    done

    # Geração de relatório final
    echo -e "\n[ETAPA 3/4] Gerando relatório consolidado..."
    gerar_relatorio || {
        echo "ERRO: Falha ao gerar relatório final" >&2
        exit 1
    }
    echo -e "\n[ETAPA 4/4] Limpando arquivos temporários..."
    rm -f *.tmp 2>/dev/null

    # Mensagem final
    echo -e "\nPROCESSAMENTO CONCLUÍDO :) - $(date '+%d/%m/%Y %H:%M:%S')"
    echo "============================================"
    echo -e "\nRELATÓRIO FINAL DISPONÍVEL EM:"
    echo " - $OUTPUT_REPORT"
    echo -e "\nGRÁFICOS GERADOS :)"

    echo ""

    remover_ambiente_virtual
}

# Executa o script principal
main
