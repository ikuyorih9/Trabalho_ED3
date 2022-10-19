#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#define TAM_PAG 960
#define TAM_REG_DADOS 64
#define TAM_REG_CAB 21
#define TAM_REG_DADOS_FIX 20

#include <stdio.h>

typedef struct{
    char status;
    int topo;
    int proxRRN;
    int nRegRem;
    int nPagDisco;
    int qtdCompacta;
}RegCab;


void binarioNaTela(const char *nomeArquivoBinario);
int retornaNumPaginasDisco(int numRegistros, FILE * arquivo);
char * retornaCampoLinha(char * linha, int numCampo);
int retornaNumRegistrosDados(FILE * arquivo);
char * retornaCampoRegistroString(int posCursor, int numBytes, FILE * arquivo);
int retornaCampoRegistroInteiro(int posCursor, FILE * arquivo);
RegCab retornaRegistroCabecalho(FILE * arquivo);
void imprimirRegistroDados(int offset, FILE * arquivo);
void decrementarCampoInteiro(int offset, FILE * arquivo);
int empilharRemovido(int rrnRemovido, RegCab * registroCabecalho);
void incrementaCampoInteiro(int offset,FILE * arquivo);
void mudarCampoString(int offset, char * campo, int tamCampo, FILE * arquivo);
void mudarCampoInteiro(int offset, int campo, FILE * arquivo);
void alocarRegistroCabecalho(char status,
                             int topo,
                             int proxRRN,
                             int nRegRem,
                             int nPagDisc,
                             int nCompacta,
                             FILE * out);

void insereRegistroDados(int posCursor,
                              char removido,
                              int encadeamento,
                              int idConecta, 
                              char *siglaPais, 
                              int idPoPsConectado, 
                              char * unidade, 
                              int velocidade, 
                              char * nomePoPs, 
                              char * nomePais,
                              FILE * arquivo);

void removeRegistroDados(int rrnRegistro, RegCab * registroCabecalho, FILE * arquivo);

#endif