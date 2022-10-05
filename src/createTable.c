#include "createTable.h"
#include "mensagensErro.h"
#include <stdio.h>

void createTable(FILE * in, FILE* out){
    if(in == NULL || out == NULL){
        imprimeErroArquivo();
        return;
    }


}