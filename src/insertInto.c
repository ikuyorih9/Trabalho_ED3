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

int retornaOffsetRegistroDisponivel(FILE * arquivo){
    int topo = retornaCampoRegistroInteiro(1, arquivo);
    int rrn; //rrn onde vou colocar o regitro.
    if(topo != -1){
        rrn = topo;
        //ATUALIZA TOPO
        int offsetEncadeamento = TAM_PAG + rrn*TAM_REG_DADOS + 1; //PROCURA CAMPO 'encadeamento' NO REGISTRO DE RRN 'topo'
        int novoTopo = retornaCampoRegistroInteiro(offsetEncadeamento, arquivo); //RETORNA CAMPO DO OFFSET ANTERIOR
        mudarCampoInteiro(1, novoTopo, arquivo); //MUDA 'topo' DO REGISTRO DE CABEÇALHO PARA 'novoTopo'
    }
    else
        rrn = retornaCampoRegistroInteiro(5, arquivo);

    int offsetDisponivel = TAM_PAG + rrn*TAM_REG_DADOS;
    return offsetDisponivel;
}

void insertInto(const char * nomeArquivo){

    FILE * arquivo = fopen(nomeArquivo, "rb");
    if(arquivo == NULL){
        imprimeErroArquivo();
        return;
    }

    int n;
    scanf("%d", &n);
    for(int i = 0; i < n; i++){

        int offset = retornaOffsetRegistroDisponivel(arquivo);

        int idConecta;
        scanf("%d", &idConecta);

        char sigla[3];
        for(int i = 0; i < 3; i++)
            scanf("%c", &sigla[i]);  //falta \0

        int idPoPsConectado;
        scanf("%d", &idPoPsConectado);

        char unidadeMedida[2];
        for(int i = 0; i < 2; i++)
            scanf("%c", &unidadeMedida[i]);
    }

    //insereRegistroDados();

    free(arquivo);
}