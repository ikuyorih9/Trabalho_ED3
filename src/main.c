#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivos.h"
#include "createTable.h"
#include "insertInto.h"
#include "remocaoLogica.h"
#include "compactacao.h"
#include "limparBuffer.h"

int main(){
    //FILE* in = fopen("../in/topologiaRede.csv","r");
    //FILE* out = fopen("../out/topologiaRede.bin","wb");

    int comando;
    char nomeEntrada[64];
    char nomeSaida[64];
    scanf("%d", &comando);

    switch(comando){
        case 1:
            scanf("%s", nomeEntrada);
            scanf("%s", nomeSaida);
            limparBuffer();
            createTable(nomeEntrada, nomeSaida);
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            scanf("%s", nomeEntrada);
            insertInto(nomeEntrada);
            break;
        case 6:
            scanf("%s", nomeEntrada);
            compactacao(nomeEntrada);
            break;
    }

    return 0;
}