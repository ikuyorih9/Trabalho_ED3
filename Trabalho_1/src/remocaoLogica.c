#include "arquivos.h"
#include "mensagensErro.h"
#include "limparBuffer.h"
#include "insertInto.h"
#include <stdio.h>
#include <stdlib.h>


void remocaoLogica(const char * nomeArquivo){
    //CRIA DIRETÓRIO A PARTIR DO NOME DO ARQUIVO.
    char * diretorioArquivo = retornaDiretorio(DIR_ENTRADA, nomeArquivo);
    FILE * arquivo = fopen(diretorioArquivo, "rb+");
    if(arquivo == NULL){
        imprimeErroArquivo();
        return;
    }

    //LÊ REGISTRO DE CABEÇALHO DO DISCOE VERIFICA CONSISTÊNCIA.
    RegCab registroCabecalho = retornaRegistroCabecalho(arquivo);
    if(registroCabecalho.status == '0'){
        imprimeErroArquivo();
        return;
    }
    //MUDA CONSISTÊNCIA POIS HAVERÁ MUDANÇAS NO ARQUIVO.
    registroCabecalho.status = '0';
    mudarCampoString(0, &(registroCabecalho.status), 1, arquivo);

    //RECEBE QUANTIDADE DE REMOÇÕES.
    int n;
    scanf("%d", &n);
    limparBuffer();

    for(int i = 1; i <= n; i++){
        char linha[128];
        fgets(linha, 128, stdin);
        //SEPARA A LINHA DE ENTRADA EM DOIS.
        char * nomeCampo = separaCamposLinha(linha, 0);
        char * valorCampo = separaCamposLinha(linha, 1);
        
        //BUSCA E APAGA O REGISTRO.
        buscaApagaRegistros(nomeCampo, valorCampo, &registroCabecalho, arquivo);
    }
    //ATUALIZA REGISTRO DE CABEÇALHO EM DISCO.
    registroCabecalho.status = '1';
    alocarRegistroCabecalho(registroCabecalho, arquivo);

    fclose(arquivo);

    binarioNaTela(diretorioArquivo);
    free(diretorioArquivo);
}