#ifndef VINAC_H_
#define VINAC_H_

#include <time.h>
#include <stdio.h>

#define LIMITE_MEMBROS 1000000

typedef enum{
    opc_ip,
    opc_ic,
    opc_m,
    opc_x,
    opc_r,
    opc_c,
    opc_invalido,
} tipo_opcao;

typedef struct{
  tipo_opcao opcao;
  const char *nome_arquivo;
  char **membros;
  int num_membros;
}dados_entrada;

typedef struct {
  char nome_arquivo[250];
  unsigned int d_uid;
  unsigned long d_tam_ori;
  unsigned long d_tam_com;
  time_t d_data_modi;
  int ordem;
  long int offset;
}diretorio;


void mostrar_ajuda();
dados_entrada coletar_dados(int argc, char *argv[]);
int processar_opcao(const dados_entrada *dados,FILE *arquivo) ;
void imprimir_dados(const dados_entrada *dados);
int verifica_membro(const dados_entrada *dados) ;
FILE *abrir_arquivo(dados_entrada *dados);
void fechar_arquivo(FILE* arqv);




#endif  // VINAC_H_