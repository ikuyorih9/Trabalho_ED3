#include "arquivos.h"
#include "mensagensErro.h"
#include "limparBuffer.h"
#include "insertInto.h"
#include <stdio.h>
#include <stdlib.h>


void remocaoLogica(const char * nomeArquivo){
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
    registroCabecalho.status = '0';
    mudarCampoString(0, &(registroCabecalho.status), 1, arquivo);

    int n;
    scanf("%d", &n);
    limparBuffer();

    for(int i = 1; i <= n; i++){
        char linha[128];
        fgets(linha, 128, stdin);
        char * nomeCampo = separaCamposLinha(linha, 0);
        char * valorCampo = separaCamposLinha(linha, 1);
        int idCampo = retornaCampoID(nomeCampo);
        
        buscaApagaRegistros(nomeCampo, valorCampo, &registroCabecalho, arquivo);
    }
    registroCabecalho.status = '1';
    mudarCampoString(0, &(registroCabecalho.status), 1, arquivo);     //ATUALIZA CONSISTENCIA status NO ARQUIVO BINARIO.
    mudarCampoInteiro(1, registroCabecalho.topo, arquivo);      //ATUALIZA topo NO ARQUIVO BINARIO.
    mudarCampoInteiro(9, registroCabecalho.nRegRem, arquivo);      //ATUALIZA topo NO ARQUIVO BINARIO.

    fclose(arquivo);

    binarioNaTela(diretorioArquivo);
    free(diretorioArquivo);
}