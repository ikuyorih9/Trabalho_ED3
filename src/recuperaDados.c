#include "recuperaDados.h"
#include "arquivos.h"
#include "limparBuffer.h"
#include "mensagensErro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void recuperaDados(const char * nomeArquivo){
    char * diretorioArquivo = retornaDiretorio(DIR_ENTRADA, nomeArquivo);
    FILE * arquivo = fopen(diretorioArquivo, "rb+");
    if(arquivo == NULL){
        imprimeErroArquivo();
        return;
    }

    RegCab registroCabecalho = retornaRegistroCabecalho(arquivo);
    if(registroCabecalho.status == '0'){
        imprimeErroArquivo();
        return;
    }

    if (registroCabecalho.proxRRN != 0){
        for (int i = 0; i < registroCabecalho.proxRRN; i++)
        {
            int offset = TAM_PAG + i * TAM_REG_DADOS;
            imprimeRegistro(offset, arquivo);
        }
    }
    else{
        registroNaoAlocado();
        printf("\n");
    }
    printf("Numero de paginas de disco: %d\n", registroCabecalho.nPagDisco);
    
}