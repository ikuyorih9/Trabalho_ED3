#include "insertInto.h"
#include "mensagensErro.h"
#include "arquivos.h"
#include <stdio.h>
#include <stdlib.h>


//ISSO PODE NAO ESTAR FUNCIONANDO!
int procuraRegistroRemovido(int topo, FILE * arquivo){
    if(topo == -1){
        int rrnRemovido = (ftell(arquivo)-TAM_PAG)/TAM_REG_DADOS;
        return rrnRemovido;
    }

    int offset = TAM_PAG + (TAM_REG_DADOS*topo) + 1;
    int rrnTopo = retornaCampoRegistroInteiro(offset, arquivo);
    return procuraRegistroRemovido(rrnTopo, arquivo);
}

void insertInto(const char * nomeArquivo){

    FILE * arquivo = fopen(nomeArquivo, "rb");
    if(arquivo == NULL){
        imprimeErroArquivo();
        return;
    }

    int topo = retornaCampoRegistroInteiro(1, arquivo);
    int rrn; //rrn onde vou colocar o regitro.
    if(topo != -1)
        rrn = procuraRegistroRemovido(topo, arquivo);
    else
        rrn = retornaCampoRegistroInteiro(5, arquivo);

    int offset = TAM_PAG + rrn*TAM_REG_DADOS;

    int n;
    scanf("%d", &n);
    for(int i = 0; i < n; i++){
        int idConecta;
    }

    //insereRegistroDados();

    free(arquivo);
}