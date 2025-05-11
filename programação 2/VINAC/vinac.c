#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<errno.h>
#include <sys/stat.h> 
#include <time.h>
#include <utime.h> 

#include "vinac.h"
#include "lz.h"
#include "lista.h"

/**
 * Exibe o menu de ajuda com as opções disponíveis do programa
 */
void mostrar_ajuda() {
    printf("------------------------------------------------------\n");
    printf("Uso: vinac <opcao> <archive> [membro1 membro2 ...]\n");
    printf("Opcões:\n");
    printf("-ip : insere membros sem compressao\n");
    printf("-ic : insere membros com compressao\n");
    printf("-m membro : move membro para apos o target\n");
    printf("-x : extrai membros\n");
    printf("-r : remove membros\n");
    printf("-c : lista conteudo do archive\n");
    printf("-------------------------------------------------------\n");
}

/**
 * Converte uma string de opção para o tipo enumerado correspondente
 * @param opcao String com a opção (ex: "-ip", "-x")
 * @return Valor do enumerado tipo_opcao ou opc_invalido se não reconhecer
 */
static tipo_opcao converter_tipo(const char *opcao){
    if(strcmp(opcao,"-ip") == 0)
        return opc_ip;
    if(strcmp(opcao,"-ic") == 0)
        return opc_ic;
    if(strcmp(opcao,"-m") == 0)
        return opc_m;
    if(strcmp(opcao,"-x") == 0)
        return opc_x;
    if(strcmp(opcao,"-r") == 0)
        return opc_r;
    if(strcmp(opcao,"-c") == 0)
        return opc_c;
    return opc_invalido;
}

/**
 * Coleta e valida os dados de entrada do programa
 * @param argc Contador de argumentos
 * @param argv Vetor de argumentos
 * @return Estrutura dados_entrada preenchida
 */
dados_entrada coletar_dados(int argc, char *argv[]) {
    dados_entrada dados;
    
     // Validação da opção principal
    dados.opcao = converter_tipo(argv[1]);
    if (dados.opcao == opc_invalido) {
        fprintf(stderr, "Erro: Opcao invalida '%s'\n", argv[1]);
        mostrar_ajuda();
        exit(EXIT_FAILURE);
    }
    
    // Preenchimento da estrutura com os demais parâmetros
    dados.nome_arquivo = argv[2];
    dados.membros = &argv[3];
    dados.num_membros = argc - 3;
    
    return dados;
}

/**
 * Função auxiliar para converter o tipo de opção de volta para string
 * (Útil para logging/debug)
 * @param opcao Tipo de operação
 * @return String representando a opção
 */
static const char* aux_imprimir(tipo_opcao opcao){
    switch (opcao){
    case opc_ip: 
        return "-ip";
    case opc_ic:
        return "-ic";
    case opc_m:
        return "-m";
    case opc_x:
        return "-x";
    case opc_r:
        return "-r";
    case opc_c:
        return "-c";
    default:
        return "Invalido";
    }
}

/**
 * Abre um arquivo com tratamento de erros
 * @param nome_arquivo Nome do arquivo a ser aberto
 * @param modo Modo de abertura (ex: "r", "w", "a")
 * @return Ponteiro para o arquivo ou NULL em caso de erro
 */
FILE* abrir_arquivo_aux(const char *nome_arquivo, const char *modo) {
    // Validação dos parâmetros de entrada
    if(nome_arquivo == NULL || modo == NULL) {
        fprintf(stderr, "ERRO: nome do arquivo ou modo invalido.\n");
        return NULL;
    }

    // Tentativa de abertura do arquivo
    FILE *arquivo = fopen(nome_arquivo, modo);
    if(!arquivo) {
        fprintf(stderr, "ERRO: Ao abrir o arquivo '%s' com modo '%s': ", nome_arquivo, modo);
        perror("");  // Mostra a mensagem de erro do sistema
        return NULL;
    }

    printf("Arquivo '%s' aberto com sucesso no modo '%s'.\n", nome_arquivo, modo);
    return arquivo;
}

/**
 * Fecha um arquivo com tratamento de erros
 * @param arqv Ponteiro para o arquivo a ser fechado
 */
void fechar_arquivo(FILE* arqv) {
    if (arqv == NULL) {
        printf("Aviso: Tentativa de fechar arquivo nulo.\n");
        return;
    }

    if (fclose(arqv)) {
        perror("Erro: Ao fechar o arquivo");
    } else {
        printf("Arquivo fechado com sucesso.\n");
    }
}

/**
 * Copia um nome removendo espaços e garantindo terminação nula
 * @param d Destino (deve ter tamanho suficiente)
 * @param o Origem
 * @param tam Tamanho máximo do destino (incluindo \0)
 */
static void copiar_nome(char *d, const char *o, unsigned int tam) {
    int j = 0;
    for(int i = 0; o[i] != '\0' && j < tam - 1; i++) {
        if(o[i] != ' ') {
            d[j++] = o[i];
        }
    }
    
    d[j] = '\0';
    printf("Nome processado (sem espaços): \"%s\"\n", d);
}

/**
 * Localiza e retorna o número de entradas na lista de diretórios
 * @param arquivo Ponteiro para o arquivo já aberto
 * @return Número de entradas ou -1 em caso de erro
 */
static int localizar_lista_diretorios(FILE *arquivo) {
    if (!arquivo) {
        fprintf(stderr, "ERRO: Arquivo inválido (NULL)\n");
        return -1;
    }

    // Lê o offset da lista que está no final do arquivo
    if (fseek(arquivo, -sizeof(long), SEEK_END) != 0) {
        perror("ERRO: Ao posicionar para ler offset da lista");
        return -1;
    }

    long offset_lista = 0;
    if (fread(&offset_lista, sizeof(long), 1, arquivo) != 1) {
        perror("ERRO: Ao ler offset da lista");
        return -1;
    }

    // Posiciona no início da lista
    if (fseek(arquivo, offset_lista, SEEK_SET) != 0) {
        perror("ERRO: Ao posicionar no início da lista");
        return -1;
    }

    // Calcula tamanho da lista
    long fim_arquivo;
    if (fseek(arquivo, 0, SEEK_END) != 0) {
        perror("ERRO: Ao posicionar no fim do arquivo");
        return -1;
    }
    fim_arquivo = ftell(arquivo);

    // Calcula número de entradas (cada entrada tem tamanho sizeof(diretorio))
    long tam_lista = fim_arquivo - offset_lista - sizeof(long);
    int num_entradas = tam_lista / sizeof(diretorio);

    // Reposiciona no início da lista para leitura posterior
    if (fseek(arquivo, offset_lista, SEEK_SET) != 0) {
        perror("ERRO: Ao reposicionar no início da lista");
        return -1;
    }

    return num_entradas;
}

/**
 * Lê e imprime a lista de diretórios do arquivo
 * @param arquivo Ponteiro para o arquivo já aberto
 */
static void ler_e_imprimir_lista_diretorios(FILE *arquivo) {
    int num_entradas = localizar_lista_diretorios(arquivo);
    if (num_entradas <= 0) {
        fprintf(stderr, "ERRO: Lista de diretórios inválida ou vazia\n");
        return;
    }

    printf("\n=== Lista de Diretórios (%d itens) ===\n", num_entradas);
    for (int i = 0; i < num_entradas; i++) {
        diretorio d;
        if (fread(&d, sizeof(diretorio), 1, arquivo) != 1) {
            perror("ERRO: Ao ler entrada de diretório");
            return;
        }

        // Formatação melhorada da saída
        printf("Arquivo: %s\n", d.nome_arquivo);
        printf("  - UID: %u\n", d.d_uid);
        printf("  - Tamanho (original/comprimido): %lu/%lu bytes\n", 
               d.d_tam_ori, d.d_tam_com);
        printf("  - Modificação: %s", ctime(&d.d_data_modi));
        printf("  - Ordem: %d\n", d.ordem);
        printf("  - Offset: %ld\n", d.offset);
        printf("---------------------------\n");
    }
}

/**
 * Coleta informações de um membro (arquivo) e preenche estrutura diretorio
 * @param membro Nome do arquivo a ser analisado
 * @param dire Estrutura de diretório a ser preenchida
 * @param ordem Número de ordem do arquivo
 * @param offset Posição no arquivo container
 * @return 0 em sucesso, 1 em erro
 */
static int coletar_info(const char *membro, diretorio *dire, int ordem, long offset) {
    struct stat stats;

    if (stat(membro, &stats) != 0) {
        perror("ERRO: Ao obter estatísticas do arquivo");
        return 1;
    }

    // Remove espaços do nome do arquivo
    copiar_nome(dire->nome_arquivo, membro, sizeof(dire->nome_arquivo));

    // Preenche os metadados
    dire->d_uid = stats.st_uid;
    dire->d_tam_ori = stats.st_size;
    dire->d_tam_com = 0;  // Será atualizado durante a compressão
    dire->d_data_modi = stats.st_mtime;
    dire->ordem = ordem;
    dire->offset = offset;

    return 0;
}

/**
 * Extrai dados de um arquivo para um buffer
 * @param buffer Buffer de destino
 * @param arqv_d Arquivo de origem
 * @param tam Quantidade de bytes a ler
 * @return 0 em sucesso, 1 em erro
 */
static int extrair_dados(void *buffer, FILE *arqv_d, unsigned long tam) {
    size_t lido = fread(buffer, 1, tam, arqv_d);
    if (lido != tam) {
        fprintf(stderr, "ERRO: Leitura incompleta (%zu de %lu bytes)\n", lido, tam);
        perror("Detalhe");
        return 1;
    }
    return 0;
}

/**
 * Escreve dados no arquivo e atualiza offset no diretorio
 * @param dire Estrutura com metadados (será atualizada)
 * @param buffer Dados a serem escritos
 * @param arqv_d Arquivo de destino
 * @param tam Quantidade de bytes a escrever
 * @return 0 em sucesso, 1 em erro
 */
static int escrever_dados(diretorio *dire, void *buffer, FILE *arqv_d, unsigned long tam){
    if(fseek(arqv_d, 0, SEEK_END) != 0){
        perror("ERRO: fseek no destino");
        return 1;
    }

    dire->offset = ftell(arqv_d);

    if(fwrite(buffer, 1, tam, arqv_d) != tam){
        perror("ERRO: Fwrite no destino");
        return 1;
    }

    return 0;
}


/**
 * Adiciona uma entrada de diretório na lista
 * @param lista Lista de diretórios
 * @param origem Dados a serem adicionados
 * @return 0 em sucesso, 1 em erro
 */
static int adicionar_diretorio_na_lista(struct lista *lista, const diretorio *origem) {
    if (lista == NULL || origem == NULL) {
        fprintf(stderr, "ERRO: lista ou origem nulos em adicionar_diretorio_na_lista\n");
        return 1;
    }

    // Aloca nova entrada
    diretorio *ptr_dire = calloc(1, sizeof(diretorio));
    if (ptr_dire == NULL) {
        perror("ERRO: calloc do diretorio");
        return 1;
    }

    *ptr_dire = *origem; // copia os dados

    if (!lista_insere(lista, ptr_dire->ordem, ptr_dire)) {
        fprintf(stderr, "ERRO: Falha ao inserir na lista (duplicado ou invalido)\n");
        free(ptr_dire);
        return 1;
    }

    return 0;
}

/**
 * Processa a opção de inserção sem compressão (IP)
 * @param dados Estrutura com dados de entrada
 * @param ordem Número de ordem do arquivo
 * @param arq_destino Arquivo de destino já aberto
 * @param lista Lista de diretórios
 * @return 0 em sucesso, 1 em erro
 */
static int opcao_ip(const dados_entrada *dados, int ordem, FILE *arq_destino, struct lista *lista) {
    diretorio dire = {0};  // Inicializa com zeros
    void *buffer = NULL;

    // 1. Coleta informações do arquivo
    if(coletar_info(dados->membros[ordem], &dire, ordem, 0L)) {
        return 1;
    }

    // 2. Abre arquivo de origem
    FILE *arq_origem = abrir_arquivo_aux(dados->membros[ordem], "rb");
    if(arq_origem == NULL) {
        return 1;
    }

    // 3. Aloca buffer para os dados
    if(!(buffer = calloc(1, dire.d_tam_ori))) {
        perror("ERRO: Falha ao alocar buffer");
        fechar_arquivo(arq_origem);
        return 1;
    }

    // 4. Processamento principal (extração e escrita)
    int status = 0;
    do {
        if(extrair_dados(buffer, arq_origem, dire.d_tam_ori)) {
            status = 1;
            break;
        }

        if(escrever_dados(&dire, buffer, arq_destino, dire.d_tam_ori)) {
            status = 1;
            break;
        }

        if(adicionar_diretorio_na_lista(lista, &dire)) {
            status = 1;
            break;
        }
    } while(0);

    // 5. Limpeza final
    free(buffer);
    fechar_arquivo(arq_origem);
    return status;
}
/**
 * Processa a opção de inserção com compressão (IC)
 * @param dados Estrutura com dados de entrada
 * @param ordem Número de ordem do arquivo
 * @param arq_destino Arquivo de destino já aberto
 * @param lista Lista de diretórios
 * @return 0 em sucesso, 1 em erro
 */
static int opcao_ic(const dados_entrada *dados, int ordem, FILE *arq_destino, struct lista *lista) {
    diretorio dire = {0};
    void *buffer_original = NULL;
    void *buffer_comprimido = NULL;
    int status = 0;

    // 1. Coleta informações do arquivo
    if(coletar_info(dados->membros[ordem], &dire, ordem, 0L)) {
        perror("ERRO: ao coletar informações do arquivo");
        return 1;
    }

    // 2. Abre arquivo de origem
    FILE *arq_origem = abrir_arquivo_aux(dados->membros[ordem], "rb");
    if(arq_origem == NULL) {
        return 1;
    }

    // 3. Aloca buffers
    buffer_original = calloc(1, dire.d_tam_ori);
    if(!buffer_original) {
        perror("ERRO: Falha ao alocar buffer original");
        fechar_arquivo(arq_origem);
        return 1;
    }

    // Cálculo do tamanho máximo do buffer comprimido (original + 0.4% + margem)
    unsigned int tam_max = dire.d_tam_ori + (dire.d_tam_ori / 256) + 16;
    buffer_comprimido = calloc(1, tam_max);
    if(!buffer_comprimido) {
        perror("ERRO: Falha ao alocar buffer comprimido");
        free(buffer_original);
        fechar_arquivo(arq_origem);
        return 1;
    }

    // 4. Processamento principal
    do {
        // Extrai dados originais
        if(extrair_dados(buffer_original, arq_origem, dire.d_tam_ori)) {
            status = 1;
            break;
        }

        // Comprime dados
        int tam_comprimido = LZ_Compress((unsigned char *)buffer_original,
                                        (unsigned char *)buffer_comprimido,
                                        dire.d_tam_ori);
        if(tam_comprimido <= 0) {
            fprintf(stderr, "ERRO: Falha na compressão (tamanho inválido)\n");
            status = 1;
            break;
        }

        // Atualiza metadados
        dire.d_tam_com = tam_comprimido;

        // Escreve dados comprimidos
        if(escrever_dados(&dire, buffer_comprimido, arq_destino, tam_comprimido)) {
            status = 1;
            break;
        }

        // Adiciona à lista
        if(adicionar_diretorio_na_lista(lista, &dire)) {
            status = 1;
            break;
        }
    } while(0);

    // 5. Limpeza final
    free(buffer_original);
    free(buffer_comprimido);
    fechar_arquivo(arq_origem);
    return status;
}

/**
 * Salva a lista de diretórios no arquivo e libera recursos
 * @param arquivo Arquivo de destino já aberto
 * @param lista Lista de diretórios a ser salva
 */
static void salvar_listaD(FILE *arquivo, struct lista *lista) {
    // Validação de parâmetros
    if(arquivo == NULL || lista == NULL) {
        fprintf(stderr, "ERRO: Parâmetros inválidos (NULL)\n");
        return;
    }

    // 1. Posiciona no final do arquivo para obter offset
    if(fseek(arquivo, 0, SEEK_END) != 0) {
        perror("ERRO: Ao posicionar no final do arquivo");
        return;
    }

    long offset_lista = ftell(arquivo);

    // 2. Escreve todos os diretórios da lista
    lista_inicia_iterador(lista);
    diretorio *dire;
    while(lista_incrementa_iterador(lista, &dire)) {
        if(fwrite(dire, sizeof(diretorio), 1, arquivo) != 1) {
            perror("ERRO: Ao escrever entrada de diretório");
            break;
        }
    }

    // 3. Escreve o offset da lista no final
    if(fwrite(&offset_lista, sizeof(long), 1, arquivo) != 1) {
        perror("ERRO: Ao escrever offset da lista");
    }

    // 4. Debug e limpeza
    printf("\n=== Lista de Diretórios Salva ===\n");
    lista_imprime(lista);
    lista_destroi(&lista);
}

/**
 * Reconstrói a lista de diretórios a partir do arquivo
 * @param arquivo Ponteiro para o arquivo já aberto
 * @param lista Lista a ser preenchida
 * @return Número de entradas ou -1 em caso de erro
 */
 int reconstruir_lista(FILE *arquivo, struct lista *lista) {
    // Localiza e verifica a lista de diretórios
    int num_entradas = localizar_lista_diretorios(arquivo);
    if (num_entradas <= 0) {
        fprintf(stderr, "ERRO: Lista de diretórios inválida ou vazia\n");
        return -1;
    }

    // Lê cada entrada da lista
    for (int i = 0; i < num_entradas; i++) {
        diretorio d;
        if (fread(&d, sizeof(diretorio), 1, arquivo) != 1) {
            perror("ERRO: Falha ao ler entrada de diretório");
            return -1;
        }

        // Adiciona na lista mantendo a ordem original
        if (adicionar_diretorio_na_lista(lista, &d) != 0) {
            fprintf(stderr, "ERRO: Falha ao adicionar diretório na lista\n");
            return -1;
        }
    }

    return num_entradas;
}

/**
 * Regrava o arquivo com os membros na nova ordem
 * @param arquivo_original Arquivo fonte
 * @param arquivo_temp Arquivo temporário de destino
 * @param lista Lista com a nova ordem
 * @return 0 em sucesso, 1 em erro
 */
static int regravar_arquivo(FILE *arquivo_original, FILE *arquivo_temp, struct lista *lista) {
    // Validação rigorosa dos parâmetros
    if (!arquivo_original || !arquivo_temp || !lista || !lista->ini) {
        fprintf(stderr, "ERRO: Parâmetros inválidos para regravação\n");
        return 1;
    }

    struct nodo *n = lista->ini;
    int status = 0;

    while (n && !status) {
        if (!n->info) {
            fprintf(stderr, "ERRO: Nodo com informação inválida\n");
            return 1;
        }

        // Determina tamanho (comprimido ou original)
        unsigned long tam = n->info->d_tam_com > 0 ? n->info->d_tam_com : n->info->d_tam_ori;
        void *buffer = malloc(tam);
        if (!buffer) {
            perror("ERRO: Falha ao alocar buffer para dados");
            return 1;
        }

        // Operações de leitura/escrita com tratamento de erro
        do {
            // Lê do original
            if (fseek(arquivo_original, n->info->offset, SEEK_SET) != 0) {
                perror("ERRO: Posicionamento no arquivo original");
                status = 1;
                break;
            }

            if (fread(buffer, 1, tam, arquivo_original) != tam) {
                perror("ERRO: Leitura incompleta do arquivo original");
                status = 1;
                break;
            }

            // Atualiza offset para nova posição
            n->info->offset = ftell(arquivo_temp);

            // Escreve no temporário
            if (fwrite(buffer, 1, tam, arquivo_temp) != tam) {
                perror("ERRO: Escrita incompleta no arquivo temporário");
                status = 1;
                break;
            }
        } while(0);

        free(buffer);
        n = n->prox;
    }

    return status;
}
/**
 * Processa a opção de mover membros (M)
 * @param arquivo_original Arquivo original já aberto
 * @param nome_original Nome do arquivo original
 * @param nome_temp Nome do arquivo temporário
 * @param nome_a_mover Nome do membro a ser movido
 * @param nome_referencia Nome do membro de referência
 * @return 1 em sucesso, 0 em erro
 */
int opcao_m(FILE *arquivo_original, const char *nome_original, const char *nome_temp,
            const char *nome_a_mover, const char *nome_referencia) {
    // Validação completa dos parâmetros
    if (!arquivo_original || !nome_original || !nome_temp || 
        !nome_a_mover || !nome_referencia) {
        fprintf(stderr, "ERRO: Parâmetros inválidos para operação M\n");
        return 0;
    }

    // 1. Preparação do arquivo temporário
    FILE *temp = fopen(nome_temp, "wb+");
    if (!temp) {
        perror("ERRO: Criação do arquivo temporário");
        return 0;
    }

    // 2. Reconstrução da lista de diretórios
    struct lista *lista = lista_cria();
    if (!lista) {
        fprintf(stderr, "ERRO: Criação da lista\n");
        fclose(temp);
        return 0;
    }

    if (reconstruir_lista(arquivo_original, lista) < 0) {
        fprintf(stderr, "ERRO: Reconstrução da lista de diretórios\n");
        lista_destroi(&lista);
        fclose(temp);
        return 0;
    }

    // 3. Reordenação dos membros
    if (1 == lista_reordenar(lista, nome_a_mover, nome_referencia)) {
        fprintf(stderr, "ERRO: Reordenação dos membros\n");
        lista_destroi(&lista);
        fclose(temp);
        return 0;
    }

    // 4. Regravação do arquivo na nova ordem
    if (regravar_arquivo(arquivo_original, temp, lista) != 0) {
        fprintf(stderr, "ERRO: Regravação do arquivo\n");
        lista_destroi(&lista);
        fclose(temp);
        return 0;
    }

    // 5. Atualização dos metadados
    printf("\n=== ATUALIZANDO METADADOS ===\n");
    salvar_listaD(temp, lista);  // Observação: lista é destruída aqui

    // 6. Finalização
    fclose(temp);

    // Substituição atômica do arquivo
    if (remove(nome_original) != 0) {
        perror("ERRO: Remoção do arquivo original");
        remove(nome_temp);  // Limpeza do temporário
        return 0;
    }

    if (rename(nome_temp, nome_original) != 0) {
        perror("ERRO: Renomeação do arquivo temporário");
        return 0;
    }

    printf("Operação concluída com sucesso: '%s' movido para após '%s'\n",
           nome_a_mover, nome_referencia);
    return 1;
}

/**
 * Remove um membro da lista pelo nome
 * @param lista Lista de diretórios
 * @param nome Nome do membro a ser removido
 * @return 1 se removido com sucesso, 0 caso contrário
 */
int remover_membro_por_nome(struct lista *lista, const char *nome) {
    // Validação dos parâmetros de entrada
    if (!lista || lista_vazia(lista) || !nome) {
        fprintf(stderr, "ERRO: Parâmetros inválidos para remoção\n");
        return 0;
    }

    struct nodo *atual = lista->ini;
    while (atual) {
        // Verifica se encontrou o membro pelo nome
        if (atual->info && strcmp(atual->info->nome_arquivo, nome) == 0) {
            printf("Removendo membro: %s\n", nome);
            return lista_remove(lista, atual->info);  // Remove usando função segura
        }
        atual = atual->prox;
    }

    fprintf(stderr, "AVISO: Membro '%s' não encontrado\n", nome);
    return 0;  // Não encontrado
}
/**
 * Copia membros do arquivo original para o temporário
 * @param arquivo_original Arquivo fonte
 * @param arquivo_temp Arquivo temporário de destino
 * @param lista Lista de diretórios atualizada
 * @param nome_temp Nome do arquivo temporário (para limpeza em caso de erro)
 * @return 1 em sucesso, 0 em erro
 */
int copiar_membros(FILE *arquivo_original, FILE *arquivo_temp, 
                  struct lista *lista, const char *nome_temp) {
    // Validação rigorosa dos parâmetros
    if (!arquivo_original || !arquivo_temp || !lista) {
        fprintf(stderr, "ERRO: Parâmetros inválidos para cópia\n");
        return 0;
    }

    struct nodo *atual = lista->ini;
    long offset_corrigido = 0;
    int primeiro = 1;
    int status = 1;

    while (atual && status) {
        // Verificação crítica da integridade dos dados
        if (!atual->info) {
            fprintf(stderr, "ERRO CRÍTICO: Nó sem informação válida\n");
            status = 0;
            break;
        }

        // Determina tamanho (comprimido ou original)
        size_t tam = atual->info->d_tam_com > 0 ? atual->info->d_tam_com : atual->info->d_tam_ori;

        // Atualiza offsets
        if (primeiro) {
            atual->info->offset = 0;  // Primeiro membro começa no offset 0
            primeiro = 0;
        } else {
            atual->info->offset = offset_corrigido;
        }

        // Aloca buffer para os dados
        void *buffer = malloc(tam);
        if (!buffer) {
            fprintf(stderr, "ERRO: Falha ao alocar %lu bytes para '%s'\n", 
                    tam, atual->info->nome_arquivo);
            status = 0;
            break;
        }

        // Operações de leitura/escrita com tratamento de erro
        do {
            // Posiciona e lê do original
            if (fseek(arquivo_original, atual->info->offset, SEEK_SET) != 0) {
                perror("ERRO: Posicionamento no arquivo original");
                status = 0;
                break;
            }

            if (fread(buffer, 1, tam, arquivo_original) != tam) {
                fprintf(stderr, "ERRO: Leitura incompleta de '%s'\n", 
                        atual->info->nome_arquivo);
                status = 0;
                break;
            }

            // Atualiza offset para nova posição no arquivo temporário
            atual->info->offset = ftell(arquivo_temp);

            // Escreve no temporário
            if (fwrite(buffer, 1, tam, arquivo_temp) != tam) {
                perror("ERRO: Escrita incompleta no temporário");
                status = 0;
                break;
            }

            // Atualiza offset para próximo membro
            offset_corrigido = atual->info->offset + tam;
        } while(0);

        free(buffer);
        atual = atual->prox;
    }

    // Limpeza em caso de erro
    if (!status) {
        fprintf(stderr, "ERRO: Falha durante a cópia dos membros\n");
        lista_destroi(&lista);
        fclose(arquivo_temp);
        remove(nome_temp);
        return 0;
    }

    return 1;
}
/**
 * Processa a opção de remoção de membros (R)
 * @param arquivo_original Arquivo original já aberto
 * @param nome_temp Nome do arquivo temporário
 * @param dados Estrutura com dados de entrada
 * @return 1 em sucesso, 0 em erro
 */
int opcao_r(FILE *arquivo_original, const char *nome_temp, const dados_entrada *dados) {
    // Validação rigorosa dos parâmetros
    if (!arquivo_original || !nome_temp || !*nome_temp || 
        !dados || !dados->membros || dados->num_membros <= 0) {
        fprintf(stderr, "ERRO: Parâmetros inválidos para remoção\n");
        return 0;
    }

    // 1. Preparação do arquivo temporário
    FILE *temp = fopen(nome_temp, "wb+");
    if (!temp) {
        perror("ERRO: Criação do arquivo temporário");
        return 0;
    }

    // 2. Reconstrução da lista de diretórios
    struct lista *lista = lista_cria();
    if (!lista || reconstruir_lista(arquivo_original, lista) < 0) {
        fprintf(stderr, "ERRO: Falha ao reconstruir lista\n");
        if (lista) lista_destroi(&lista);
        fclose(temp);
        return 0;
    }

    // 3. Processamento de remoção dos membros
    int membros_removidos = 0;
    for (int i = 0; i < dados->num_membros; i++) {
        const char *nome = dados->membros[i];
        if (!nome) {
            fprintf(stderr, "AVISO: Nome de membro nulo no índice %d\n", i);
            continue;
        }

        if (remover_membro_por_nome(lista, nome)) {
            membros_removidos++;
            printf("SUCESSO: Removido '%s'\n", nome);
        } else {
            fprintf(stderr, "AVISO: Membro não encontrado ou erro ao remover: '%s'\n", nome);
        }
    }

    // 4. Verificação se alguma remoção foi realizada
    if (membros_removidos == 0) {
        fprintf(stderr, "AVISO: Nenhum membro foi removido\n");
        lista_destroi(&lista);
        fclose(temp);
        remove(nome_temp);
        return 0;
    }

    // 5. Cópia dos membros restantes para o arquivo temporário
    if (!copiar_membros(arquivo_original, temp, lista, nome_temp)) {
        return 0;
    }

    // 6. Atualização dos metadados
    printf("\n=== ATUALIZANDO METADADOS ===\n");
    salvar_listaD(temp, lista);  // Observação: lista é destruída aqui

    // 7. Finalização
    fclose(temp);
    fclose(arquivo_original);

    // 8. Substituição atômica do arquivo
    if (remove(dados->nome_arquivo) != 0 || rename(nome_temp, dados->nome_arquivo) != 0) {
        perror("ERRO: Ao substituir arquivo original");
        remove(nome_temp);
        return 0;
    }

    printf("\n=== OPERAÇÃO CONCLUÍDA ===\n");
    printf("SUCESSO: %d/%d membros removidos\n", membros_removidos, dados->num_membros);
    return 1;
}

/**
 * Preserva a data de modificação original de um arquivo
 * @param filename Nome do arquivo
 * @param modtime Tempo de modificação original (time_t)
 */
static void preservar_data_modificacao(const char *filename, time_t modtime) {
    struct utimbuf novos_tempos = {
        .actime = time(NULL),   // Data de acesso (atual)
        .modtime = modtime      // Data de modificação (original)
    };
    
    if (utime(filename, &novos_tempos) != 0) {
        perror("AVISO: Não foi possível preservar data de modificação");
    }
}
/**
 * Extrai um único membro do archive, com suporte a descompressão
 * @param archive Arquivo fonte já aberto
 * @param info Estrutura com metadados do membro
 * @return 1 em sucesso, 0 em erro
 */
int extrair_membro_individual(FILE *archive, const diretorio *info) {
    // Validação dos parâmetros
    if (!archive || !info) {
        fprintf(stderr, "ERRO: Parâmetros inválidos\n");
        return 0;
    }

    // 1. Criar arquivo de saída
    FILE *output = fopen(info->nome_arquivo, "wb");
    if (!output) {
        perror("ERRO: Ao criar arquivo de saída");
        return 0;
    }

    int sucesso = 0;
    unsigned char *buffer_in = NULL;
    unsigned char *buffer_out = NULL;

    // 2. Verificar modo de extração (comprimido ou não)
    if (info->d_tam_com > 0) {
        // Arquivo comprimido - precisa descomprimir
        printf("Descomprimindo: %s (%lu -> %lu bytes)\n", 
               info->nome_arquivo, info->d_tam_com, info->d_tam_ori);

        // 3. Alocar buffers para descompressão
        buffer_in = malloc(info->d_tam_com);
        buffer_out = malloc(info->d_tam_ori);
        if (!buffer_in || !buffer_out) {
            fprintf(stderr, "ERRO: Falha ao alocar buffers\n");
            goto cleanup;
        }

        // 4. Ler dados comprimidos
        if (fseek(archive, info->offset, SEEK_SET) != 0 ||
            fread(buffer_in, 1, info->d_tam_com, archive) != info->d_tam_com) {
            fprintf(stderr, "ERRO: Falha ao ler dados comprimidos\n");
            goto cleanup;
        }

        // 5. Descomprimir
        LZ_Uncompress(buffer_in, buffer_out, info->d_tam_com);
        
        // 6. Escrever arquivo descomprimido
        if (fwrite(buffer_out, 1, info->d_tam_ori, output) != info->d_tam_ori) {
            fprintf(stderr, "ERRO: Falha ao escrever arquivo descomprimido\n");
            goto cleanup;
        }
    } else {
        // Arquivo não comprimido - cópia direta com buffer
        size_t buffer_size = info->d_tam_ori < 65536 ? info->d_tam_ori : 65536;
        unsigned char *buffer = malloc(buffer_size);
        if (!buffer) {
            fprintf(stderr, "ERRO: Falha ao alocar buffer\n");
            goto cleanup;
        }

        size_t restante = info->d_tam_ori;
        if (fseek(archive, info->offset, SEEK_SET) != 0) {
            free(buffer);
            goto cleanup;
        }

        // 7. Cópia em blocos para economizar memória
        while (restante > 0) {
            size_t ler = restante > buffer_size ? buffer_size : restante;
            size_t lidos = fread(buffer, 1, ler, archive);
            if (lidos != ler || fwrite(buffer, 1, lidos, output) != lidos) {
                free(buffer);
                goto cleanup;
            }
            restante -= lidos;
        }
        free(buffer);
    }

    sucesso = 1;

cleanup:
    // 8. Liberação de recursos
    free(buffer_in);
    free(buffer_out);
    fclose(output);

    // 9. Tratamento de erro
    if (!sucesso) {
        remove(info->nome_arquivo);
        return 0;
    }

    // 10. Preservação de metadados
    preservar_data_modificacao(info->nome_arquivo, info->d_data_modi);

    return sucesso;
}

/**
 * Processa a opção de extração de membros (X)
 * @param archive_ptr Ponteiro para o ponteiro do arquivo archive (será atualizado)
 * @param nome_archive Nome do arquivo archive
 * @param lista Lista de diretórios dos membros
 * @param membros_extrair Vetor de nomes dos membros a extrair (NULL para todos)
 * @param num_membros Número de membros a extrair (0 para todos)
 * @return 1 em sucesso, 0 em erro
 */
int opcao_x(FILE **archive_ptr, const char *nome_archive, struct lista *lista, 
            char **membros_extrair, int num_membros) {
    FILE *archive = *archive_ptr;
    
    // 1. Validações iniciais
    if (!archive || !nome_archive || !lista) {
        fprintf(stderr, "ERRO: Parâmetros inválidos\n");
        return 0;
    }

    printf("\n=== EXTRAÇÃO DO MEMBRO ===\n");

    // 2. Criar archive temporário
    char temp_file[] = "temp_vinac_extrai.bin";
    FILE *novo_archive = fopen(temp_file, "wb+");
    if (!novo_archive) {
        perror("Erro ao criar archive temporário");
        return 0;
    }

    // 3. Processar membros
    long offset_corrente = 0;
    int extraidos = 0;
    struct nodo *atual = lista->ini;
    struct nodo *proximo = NULL;

    while (atual) {
        proximo = atual->prox;

        // 4. Verificar se deve extrair/remover
        int deve_extrair = (num_membros == 0); // Extrai todos se nenhum especificado
        for (int i = 0; !deve_extrair && i < num_membros; i++) {
            if (strcmp(atual->info->nome_arquivo, membros_extrair[i]) == 0) {
                deve_extrair = 1;
                break;
            }
        }

        if (deve_extrair) {
            // 5. Extrair membro para arquivo individual
            printf("Extraindo e removendo: %s\n", atual->info->nome_arquivo);
            if (!extrair_membro_individual(archive, atual->info)) {
                fclose(novo_archive);
                remove(temp_file);
                return 0;
            }
            
            // 6. Remover da lista (não copiar para o novo archive)
            lista_remove(lista, atual->info);
            extraidos++;
        } else {
            // 7. Copiar membro para o novo archive
            size_t tamanho = atual->info->d_tam_com > 0 ? atual->info->d_tam_com : atual->info->d_tam_ori;
            
            if (fseek(archive, atual->info->offset, SEEK_SET) != 0) {
                fclose(novo_archive);
                remove(temp_file);
                fprintf(stderr, "Erro ao posicionar no archive\n");
                return 0;
            }

            // Atualizar offset no novo archive
            atual->info->offset = offset_corrente;
            offset_corrente += tamanho;
        }

        atual = proximo;
    }

    // 8. Salvar lista ATUALIZADA no novo archive
   salvar_listaD(novo_archive, lista);

    // 9. Fechar arquivos
    fclose(novo_archive);
    fclose(archive);

    // 10. Substituir archive original
    if (remove(nome_archive)){
        perror("Erro ao remover archive original");
        remove(temp_file);
        return 0;
    }

    if (rename(temp_file, nome_archive)) {
        perror("Erro ao renomear archive temporário");
        return 0;
    }

    // 11. Reabrir archive atualizado
    *archive_ptr = fopen(nome_archive, "rb+");
    if (!*archive_ptr) {
        perror("Erro ao reabrir archive atualizado");
        return 0;
    }

    printf("\n=== OPERAÇÃO CONCLUÍDA ===\n");
    printf("Membros extraídos: %d\n", extraidos);
    printf("Archive atualizado: %s\n", nome_archive);
    
    return 1;
}
/**
 * Processa a opção selecionada pelo usuário
 * @param dados Estrutura com os dados de entrada
 * @param arquivo Ponteiro para o arquivo já aberto
 * @return 0 em sucesso, 1 em erro
 */
int processar_opcao(const dados_entrada *dados, FILE *arquivo) {
    // Validação básica dos parâmetros
    if (!dados || !arquivo) {
        fprintf(stderr, "ERRO: Parâmetros inválidos\n");
        return 1;
    }

    printf("\n=== PROCESSANDO OPERAÇÃO ===\n");
    imprimir_dados(dados);  // Exibe informações da operação

    // Verificação de membros necessários
    if (verifica_membro(dados)) {
        return 1;
    }

    int resultado = 0;
    
    switch (dados->opcao) {
        case opc_ip: 
            printf("Inserindo %d membros sem compressão\n", dados->num_membros);
            
            struct lista *lista = lista_cria();
            if (!lista) {
                fprintf(stderr, "ERRO: Falha ao criar lista\n");
                return 1;
            }

            for (int i = 0; i < dados->num_membros; i++) {
                if (opcao_ip(dados, i, arquivo, lista) != 0) {
                    fprintf(stderr, "ERRO: Falha ao inserir membro %d\n", i);
                    lista_destroi(&lista);
                    return 1;
                }
            }

            salvar_listaD(arquivo, lista);
            break;

        case opc_ic:
            printf("Inserindo %d membros com compressão\n", dados->num_membros);
            
            struct lista *listaC = lista_cria();
            if (!listaC) {
                fprintf(stderr, "ERRO: Falha ao criar lista\n");
                return 1;
            }

            for (int i = 0; i < dados->num_membros; i++) {
                if (opcao_ic(dados, i, arquivo, listaC) != 0) {
                    fprintf(stderr, "ERRO: Falha ao inserir membro %d\n", i);
                    lista_destroi(&listaC);
                    return 1;
                }
            }

            salvar_listaD(arquivo, listaC);
            break;

        case opc_m:
            printf("Movendo membro '%s' após '%s'\n", 
                   dados->membros[0], dados->membros[1]);
            
            if (dados->num_membros != 2) {
                fprintf(stderr, "ERRO: Operação -m requer exatamente 2 membros\n");
                return 1;
            }

            resultado = opcao_m(arquivo, dados->nome_arquivo,
                              "temp_vinac.bin", dados->membros[0],
                              dados->membros[1]);
            break;

        case opc_x:
            printf("Extraindo %s%d membros\n", 
                  dados->num_membros == 0 ? "todos " : "", dados->num_membros);
            
            struct lista *listaX = lista_cria();
            if (!listaX || reconstruir_lista(arquivo, listaX) < 0) {
                fprintf(stderr, "ERRO: Falha ao carregar lista de membros\n");
                if (listaX) lista_destroi(&listaX);
                return 1;
            }

            resultado = opcao_x(&arquivo, dados->nome_arquivo, listaX,
                               dados->membros, dados->num_membros);
            break;

        case opc_r:
            printf("Removendo %d membros\n", dados->num_membros);
            resultado = opcao_r(arquivo, "temp_vinac_remover.bin", dados);
            break;

        case opc_c:
            printf("Listando conteúdo do archive\n");
            ler_e_imprimir_lista_diretorios(arquivo);
            break;

        default:
            fprintf(stderr, "ERRO: Opção desconhecida\n");
            return 1;
    }

    if (resultado != 0) {
        fprintf(stderr, "ERRO: Falha ao processar operação\n");
        return 1;
    }

    printf("\n=== OPERAÇÃO CONCLUÍDA COM SUCESSO ===\n");
    return 0;
}

/**
 * Imprime os dados de entrada para verificação
 * @param dados Estrutura com os dados a serem impressos
 */
void imprimir_dados(const dados_entrada *dados) {
    if (!dados) {
        fprintf(stderr, "Erro: Dados de entrada invalidos\n");
        return;
    }

    printf("Opcão: %s\n", aux_imprimir(dados->opcao));
    printf("Arquivo: %s\n", dados->nome_arquivo);

    // So mostra membros se não for opcão -c ou -x
    if (dados->opcao != opc_c && dados->opcao != opc_x) {
        printf("Membros (%d):\n", dados->num_membros);
        for (int i = 0; i < dados->num_membros; i++) {
            printf("  %s\n", dados->membros[i]);
        }
    }
}
/**
 * Verifica se os membros fornecidos são válidos para a operação
 * @param dados Estrutura com os dados de entrada
 * @return 0 se válido, 1 se inválido
 */
int verifica_membro(const dados_entrada *dados) {
    if (dados == NULL) {
        fprintf(stderr, "Erro: Dados de entrada invalidos\n");
        return 1;
    }

    // Opcões que não requerem membros especificos
    if (dados->opcao == opc_x || dados->opcao == opc_c) {
        return 0;
    }

    // Demais opcões requerem pelo menos 1 membro
    if (dados->num_membros == 0) {
        fprintf(stderr, "Erro: Esta opcão requer pelo menos 1 membro\n");
        return 1;
    }

    if (dados->num_membros > LIMITE_MEMBROS) {
        fprintf(stderr, "Erro: Limite de %d membros excedido\n", LIMITE_MEMBROS);
        return 1;
    }

    return 0;
}

FILE *abrir_arquivo(dados_entrada *dados){
    return abrir_arquivo_aux(dados->nome_arquivo, "ab+");
}