#include "arvoreB.h"
#include "arquivos.h"
#include <stdio.h>
#include <stdlib.h>

int retornaOffsetArvore(int rrnArvore){
    return (rrnArvore + 1) * TAM_PAG_ARV;
}

void alocaArvoreCabecalho(ArvoreCab cabecalho, FILE * arquivo){
    moveCursor(0,arquivo);

    fwrite(&cabecalho.status, sizeof(char), 1, arquivo);
    fwrite(&cabecalho.raiz, sizeof(int), 1, arquivo);
    fwrite(&cabecalho.nChavesTotal, sizeof(int), 1, arquivo);
    fwrite(&cabecalho.alturaArvore, sizeof(int), 1, arquivo);
    fwrite(&cabecalho.proxNoRRN, sizeof(int), 1, arquivo);
    insereLixo(TAM_CAB_ARV, TAM_PAG_ARV, arquivo);
}

void alocaArvoreDados(int rrnNo, ArvoreDados dados, FILE * arquivo){
    int offset = retornaOffsetArvore(rrnNo);
    moveCursor(offset, arquivo);
    fwrite(&dados.folha, sizeof(char), 1, arquivo);
    fwrite(&dados.nChavesNo, sizeof(int), 1, arquivo);
    fwrite(&dados.alturaNo, sizeof(int), 1, arquivo);
    fwrite(&dados.noRRN, sizeof(int), 1, arquivo);
    for(int i = 0; i < ORDEM_ARV; i++){
        fwrite(&dados.ponteiros[i], sizeof(int), 1, arquivo);
        fwrite(&dados.chaves[i], sizeof(char), 1, arquivo);
        fwrite(&dados.referencias[i], sizeof(int), 1, arquivo);
    }
    fwrite(&dados.ponteiros[4], sizeof(int), 1, arquivo);
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

ArvoreDados criaArvoreDados(int rrn){
    ArvoreDados dados;
    dados.folha = '1';
    dados.nChavesNo = 0;
    dados.alturaNo = 1;
    dados.noRRN = rrn;
    for(int i = 0; i < ORDEM_ARV; i++){
        dados.ponteiros[i] = -1;
        dados.chaves[i] = -1;
        dados.referencias[i] = -1;
        if(i == ORDEM_ARV-1)
            dados.ponteiros[i+1] = -1;
    }
    return dados;
}

ArvoreDados retornaArvoreDados(int rrnNo, FILE * arquivo){
    int offset = retornaOffsetArvore(rrnNo);
    moveCursor(offset, arquivo);
    ArvoreDados dados;
    fread(&dados.folha, sizeof(char), 1, arquivo);
    fread(&dados.nChavesNo, sizeof(int), 1, arquivo);
    fread(&dados.alturaNo, sizeof(int), 1, arquivo);
    fread(&dados.noRRN, sizeof(int), 1, arquivo);
    for(int i = 0; i < ORDEM_ARV; i++){
        fread(&dados.ponteiros[i], sizeof(int), 1, arquivo);
        fread(&dados.chaves[i], sizeof(char), 1, arquivo);
        fread(&dados.referencias[i], sizeof(int), 1, arquivo);
    }
    fread(&dados.ponteiros[4], sizeof(int), 1, arquivo);
}

int buscaArvore(int rrnAtual, int valor, FILE * arquivo){
    if(rrnAtual == -1)
        return -1;

    ArvoreDados dados = retornaArvoreDados(rrnAtual, arquivo); 
    
    if(dados.folha == '1'){
        return rrnAtual;
    }

    if(valor < dados.chaves[0]){
        int rrn = dados.ponteiros[0];
        return buscaArvore(rrn, valor, arquivo);
    }
    else if(valor > dados.chaves[0] && valor < dados.chaves[1]){
        int rrn = dados.ponteiros[1];
        return buscaArvore(rrn, valor, arquivo);
    }
    else if(valor > dados.chaves[1] && valor < dados.chaves[2]){
        int rrn = dados.ponteiros[2];
        return buscaArvore(rrn, valor, arquivo);
    }
    else if(valor > dados.chaves[2] && valor < dados.chaves[3]){
        int rrn = dados.ponteiros[3];
        return buscaArvore(rrn, valor, arquivo);
    }
    else{
        int rrn = dados.ponteiros[4];
        return buscaArvore(rrn, valor, arquivo);
    } 
}

void insereNoArvore(int chave, ArvoreCab * cabecalho, FILE * arquivo){
    if(cabecalho->raiz == -1){
        ArvoreDados dados = criaArvoreDados(cabecalho->proxNoRRN);
        insereOrdenado(chave, &dados);

        cabecalho->raiz = cabecalho->proxNoRRN;
        (cabecalho->proxNoRRN)++;

        dados.nChavesNo++;
        dados.noRRN = cabecalho->raiz;
    
        alocaArvoreDados(cabecalho->raiz, dados, arquivo);
        return;
    }

    int split = insereNo(cabecalho->raiz, chave, cabecalho, arquivo);
    if(split == -1)
        //CRIAR NOVO NÓ PARA SER A RAIZ
    
}

int insereNo(int rrnNo, int chave, ArvoreCab * cabecalho, FILE * arquivo){
    ArvoreDados dados = retornaArvoreDados(rrnNo, arquivo);
    //SE O NÓ É FOLHA[
    
    int split;
    if(dados.folha == '1'){
       //SE O NÓ NÃO ESTÁ CHEIO.
       if(dados.nChavesNo < ORDEM_ARV){
            insereOrdenado(chave, &dados);
            alocaArvoreDados(dados.noRRN, dados, arquivo); //verificar o RRN
            return -1;
       }
       //SE O NÓ ESTÁ CHEIO.
       else{
            //faz o split
            //return split
       }
    }
    //SE O NÓ NÃO FOLHA.
    else{
        int indice = retornaIndiceFilho(chave, dados);
        int rrnFilho = dados.ponteiros[indice];
        //INSERE NO FILHO APROPRIADO.
        split = insereNo(rrnFilho, chave, cabecalho, arquivo);
        //ponto da volta de uma inserção.
    }

    //AJEITAR OS SPLITS.
    if(split != -1){
        //tem espaço no no
        if(dados.nChavesNo < ORDEM_ARV){
            insereOrdenado(split, &dados);
        }
        //nao tem espaço no no
        else{
            //faz o split
            return split;
        }
    }
    return -1;
}

int retornaIndiceFilho(int chave, ArvoreDados dados){
    if(chave < dados.chaves[0]){
        return 0;
    }
    else if(chave > dados.chaves[0] && chave < dados.chaves[1]){
        return 1;
    }
    else if(chave > dados.chaves[1] && chave < dados.chaves[2]){
        return 2;
    }
    else if(chave > dados.chaves[2] && chave < dados.chaves[3]){
        return 3;
    }
    else
        return 4;
}

int insereOrdenado(int valorChave, ArvoreDados * dados){
    if(dados->folha == '0')
        return 0;
    int maior = valorChave;
    int i;
    for(i = 0; i < dados->nChavesNo; i++){
        if(dados->chaves[i] > maior){
            int aux = dados->chaves[i];
            dados->chaves[i] = maior;
            maior = aux;
        }
    }
    dados->chaves[i] = maior;
    return 1;
}
