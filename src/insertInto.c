#include "insertInto.h"
#include "mensagensErro.h"
#include "arquivos.h"
#include "myString.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int retornaRRNRegistroDisponivel(FILE * arquivo){
    int topo = retornaCampoRegistroInteiro(1, arquivo);
    int rrnDisponivel; //rrn onde vou colocar o regitro.

    if(topo != -1){
        rrnDisponivel = topo;
        //ATUALIZA TOPO
        int offsetEncadeamento = TAM_PAG + rrnDisponivel*TAM_REG_DADOS + 1; //PROCURA CAMPO 'encadeamento' NO REGISTRO DE RRN 'topo'
        int novoTopo = retornaCampoRegistroInteiro(offsetEncadeamento, arquivo); //RETORNA CAMPO DO OFFSET ANTERIOR
        mudarCampoInteiro(1, novoTopo, arquivo); //MUDA 'topo' DO REGISTRO DE CABEÇALHO PARA 'novoTopo.
    }
    else{
        rrnDisponivel = retornaCampoRegistroInteiro(5, arquivo);
    }
    return rrnDisponivel;
}

void insertInto(const char * nomeArquivo){
    //3 "Campina Grande" "Brazil" "BR" 4 "G" 10
    FILE * arquivo = fopen(nomeArquivo, "rb+");
    if(arquivo == NULL){
        imprimeErroArquivo();
        return;
    }

    int n;
    scanf("%d", &n);
    for(int i = 0; i < n; i++){
        int rrnDisponivel = retornaRRNRegistroDisponivel(arquivo);
        int offset =  TAM_PAG + rrnDisponivel*TAM_REG_DADOS;
        
        char linha[128];
        fgets(linha, 128, stdin);
        

        //insereRegistroDados(offset,'0',-1,idConecta,sigla,idPoPsConectado,&unidadeMedida,velocidade,nomePoPs,nomePais,arquivo);

        int proxRRN = retornaCampoRegistroInteiro(5, arquivo);
        if(rrnDisponivel < proxRRN)
            decrementarCampoInteiro(9, arquivo);
        else
            incrementaCampoInteiro(5, arquivo);

    }

    fclose(arquivo);
    binarioNaTela(nomeArquivo);
}