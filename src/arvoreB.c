#include "arvoreB.h"
#include "arquivos.h"
#include <stdio.h>
#include <stdlib.h>

void alocaArvoreCabecalho(ArvoreCab cabecalho, FILE * arquivo){
    moveCursor(0,arquivo);

    fwrite(&cabecalho.status, sizeof(char), 1, arquivo);
    fwrite(&cabecalho.raiz, sizeof(int), 1, arquivo);
    fwrite(&cabecalho.nChavesTotal, sizeof(int), 1, arquivo);
    fwrite(&cabecalho.alturaArvore, sizeof(int), 1, arquivo);
    fwrite(&cabecalho.proxNoRRN, sizeof(int), 1, arquivo);
    insereLixo(TAM_CAB_ARV, TAM_PAG_ARV, arquivo);
}

ArvoreCab retornaCabecalhoArvore(FILE * arquivo){
    ArvoreCab cabecalho;
    moveCursor(0, arquivo);
    fread(&cabecalho.status, sizeof(char), 1, arquivo);
    fread(&cabecalho.raiz, sizeof(int), 1, arquivo);
    fread(&cabecalho.nChavesTotal, sizeof(int), 1, arquivo);
    fread(&cabecalho.alturaArvore, sizeof(int), 1, arquivo);
    fread(&cabecalho.proxNoRRN, sizeof(int), 1, arquivo);
    return cabecalho;
}

ArvoreCab criaCabecalhoArvore(){
    ArvoreCab cabecalho;
    cabecalho.status = '0';
    cabecalho.raiz = -1;
    cabecalho.nChavesTotal = 0;
    cabecalho.alturaArvore = 0;
    cabecalho.proxNoRRN = 0;
    return cabecalho;
}

ArvoreDados criaArvoreDados(){
    ArvoreDados dados;
    dados.folha = '1';
    dados.nChavesNo = 0;
    dados.alturaNo = 1;
    dados.noRRN = 0;
    for(int i = 0; i < ORDEM_ARV; i++){
        ponteiros[i] = -1;
        chaves[i] = -1;
        referencias[i] = -1;
        if(i == ORDEM_ARV-1)
            ponteiros[i+1] = -1;
    }
    return dados;
}

int buscaArvore(ArvoreCab cabecalho, FILE * arquivo){
    if(cabecalho.raiz == -1){
        return cabecalho.proxNoRRN;
    }
}