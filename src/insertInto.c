#include "insertInto.h"
#include "mensagensErro.h"
#include <stdio.h>
#include <stdlib.h>

//ABORDAGEM DINAMINCA
void insertInto(const char * nomeArquivo){

    FILE * arquivo = fopen(nomeArquivo, "wb");
    if(arquivo == NULL){
        imprimeErroArquivo();
        return;
    }


}