#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#define TAM_PAG 960
#define TAM_REG_DADOS 64
#define TAM_REG_CAB 21
#define TAM_REG_DADOS_FIX 20

#define DIR_ENTRADA "in/"
#define DIR_SAIDA "out/"

#include <stdio.h>

typedef struct{
    char status;
    int topo;
    int proxRRN;
    int nRegRem;
    int nPagDisco;
    int qtdCompacta;
}RegCab;

typedef struct{
    char removido;
    int encadeamento;
    int idConecta;
    char * siglaPais;
    int idPoPsConectado;
    char * unidadeMedida;
    int velocidade;
    char * nomePoPs;
    char * nomePais;
}RegDados;

void binarioNaTela(const char *nomeArquivoBinario);
char * retornaDiretorio(const char caminho[32], const char * nomeArquivo);

/***********************************
    LEITURA DE REGISTROS E AFINS.
 ***********************************/

int retornaCampoFixoInteiro(int posCursor, FILE * arquivo);
char * retornaCampoFixoString(int posCursor, int numBytes,FILE * arquivo);
char * retornaCampoVariavel(int offset, FILE * arquivo);
int retornaNumPaginasDisco(int numRegistros);
RegCab retornaRegistroCabecalho(FILE * arquivo);

/*************************************
    MODIFICAÇÃO DE REGISTROS EM DISCO.
**************************************/

int empilharRemovido(int rrnRemovido, RegCab * registroCabecalho);
void insereLixo(int nBytesLidos, int tamRegistro, FILE * out);
void mudarCampoInteiro(int offset, int campo, FILE * arquivo);
void mudarCampoString(int offset, char * campo, int tamCampo, FILE * arquivo);

/********************************************
    INSERÇÃO E REMOÇÃO DE REGISTROS EM DISCO.
*********************************************/

void alocarRegistroCabecalho(RegCab registroCabecalho, FILE * out);
void insereRegistroDados(int offset, RegDados registroDados, FILE * arquivo);
void removeRegistroDados(int rrnRegistro, RegCab * registroCabecalho, FILE * arquivo);

#endif