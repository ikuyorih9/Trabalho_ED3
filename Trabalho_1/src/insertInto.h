#ifndef INSERT_INTO_H
#define INSERT_INTO_H

#include <stdio.h>
#include "arquivos.h"

void insertInto(const char * nomeArquivo);
char * separaCamposLinha(char * linha, int numCampo);
int retornaRRNRegistroDisponivel(RegCab * registroCabecalho, FILE * arquivo);
int retornaValorInteiro(char * linha, int numCampo);
char * retornaValorString(char *linha, int numCampo);

#endif