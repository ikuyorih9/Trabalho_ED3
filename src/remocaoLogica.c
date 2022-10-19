#include "arquivos.h"
#include "mensagensErro.h"
#include <stdio.h>


void remocaoLogica(const char * nomeArquivo){
    FILE * arquivo = fopen(nomeArquivo, "rb+");
    if(arquivo == NULL){
        imprimeErroArquivo();
        return;
    }

    RegCab registroCabecalho = retornaRegistroCabecalho(arquivo);
    registroCabecalho.status = '0';
    mudarCampoString(0, &(registroCabecalho.status), 1, arquivo);

    int n;
    scanf("%d", &n);
    for(int i = 0; i < n; i++){
        removeRegistroDados(i, &registroCabecalho, arquivo);
    }

    mudarCampoString(0, &(registroCabecalho.status), 1, arquivo);     //ATUALIZA CONSISTENCIA status NO ARQUIVO BINARIO.
    mudarCampoInteiro(1, registroCabecalho.topo, arquivo);      //ATUALIZA proxRRN NO ARQUIVO BINARIO.
}