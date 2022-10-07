#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#define TAM_PAG 960
#define TAM_REG_DADOS 64
#define TAM_REG_CAB 20
#define TAM_REG_DADOS_FIX 23

#include <stdio.h>

void binarioNaTela(const char *nomeArquivoBinario);
int retornaNumPaginasDisco(int numRegistros, FILE * arquivo);
void mudarCampoString(int posCursor, char * campo, int tamCampo, FILE * arquivo);
void mudarCampoInteiro(int posCursor, int campo, FILE * arquivo);
void alocarRegistroCabecalho(char status,
                             int topo,
                             int proxRRN,
                             int nRegRem,
                             int nPagDisc,
                             int nCompacta,
                             FILE * out);

int alocarRegistroDados(int removido,
                        int encadeamento,
                        int idConecta, 
                        char *siglaPais, 
                        float idPoPsConectado, 
                        char * unidade, 
                        float velocidade, 
                        char * nomePoPs, 
                        char * nomePais,
                        FILE * out);

void insereRegistroDados(int posCursor,
                              int removido,
                              int encadeamento,
                              int idConecta, 
                              char *siglaPais, 
                              float idPoPsConectado, 
                              char * unidade, 
                              float velocidade, 
                              char * nomePoPs, 
                              char * nomePais,
                              FILE * arquivo);

#endif