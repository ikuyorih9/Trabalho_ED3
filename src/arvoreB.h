#ifndef ARVORE_B
#define ARVORE_B

#define TAM_PAG_ARV 65
#define TAM_CAB_ARV 17
#define ORDEM_ARV 5
#define MAX_CHAVES 4

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int filhoEsq;
  int chave;
  int rrnReferencia;
  int filhoDir;
} NucleoNo;

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
    NucleoNo nucleo[4];
}ArvoreDados;

void imprimeChavesOrdenado(int rrnNo, FILE * arquivo);
void imprimeNosSequencia(FILE * arquivo);
int retornaFilhoRRN(int chave, ArvoreDados no);

int buscaRRNRegistroArvore(int chave, int rrn, FILE * arquivo);

void alocaCabecalhoArvore(ArvoreCab * cabecalho, FILE * arquivo);
ArvoreCab criaCabecalho();
ArvoreCab retornaCabecalhoArvore(FILE * arquivo);

/*********************************
    MANIPULAÇÃO DE NÓS DA ÁRVORE.
**********************************/
void imprimeNo(ArvoreDados no);
void insereNoArquivo(ArvoreDados no, FILE * arquivo);
ArvoreDados retornaNoArquivo(int rrn, FILE * arquivo);

void insereOrdenadoNo(NucleoNo nucleoNo, ArvoreDados * no);
NucleoNo * insereNo(int idConecta, int rrnRef, int rrn, int alturaNo, ArvoreCab * cabecalho, FILE * arquivo);
void insereNoArvore(int idConecta, int rrnRef, ArvoreCab * cabecalho, FILE * arquivo);
NucleoNo * insereNucleoNo(NucleoNo * nucleo, ArvoreDados * no, ArvoreCab * cabecalho, FILE * arquivo);


#endif