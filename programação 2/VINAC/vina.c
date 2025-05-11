#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vinac.h"
#include "lz.h"
#include"lista.h"


int main(int argc, char *agrv[]){

    if(argc < 3){
        mostrar_ajuda();
        return 3;
    }

    dados_entrada dados = coletar_dados(argc,agrv);

    if(verifica_membro(&dados))
      return 2;

    FILE *arquivo = abrir_arquivo(&dados);

    imprimir_dados(&dados);

    if(!processar_opcao(&dados,arquivo))
       return 1;
   
    fechar_arquivo(arquivo);

    return 0;
}