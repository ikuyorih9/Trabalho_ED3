#ifndef ARVORE_B
#define ARVORE_B

#define TAM_PAG_ARV 65
#define TAM_CAB_ARV 17
#define TAM_DADOS_ARV 65
#define ORDEM_ARV 5

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char status;
    int raiz;
    int nChavesTotal;
    int alturaArvore;
    int proxNoRRN;
}ArvoreCab;

typedef struct{
    char folha;
    int nChavesNo;
    int alturaNo;
    int noRRN;
    int ponteiros[5];
    int chaves[4];
    int referencias[4];
}ArvoreDados;

void alocaArvoreCabecalho(ArvoreCab cabecalho, FILE * arquivo);
ArvoreCab retornaCabecalhoArvore(FILE * arquivo);
ArvoreCab criaCabecalhoArvore();

ArvoreDados criaArvoreDados(int rrn);
ArvoreDados retornaArvoreDados(int rrnNo, FILE * arquivo);

//int buscaArvore(ArvoreCab cabecalho, int valor, FILE * arquivo);
int buscaArvore(int rrnAtual, int valor, FILE * arquivo);
#endif