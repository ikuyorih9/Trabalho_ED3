#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivos.h"
#include "createTable.h"
#include "insertInto.h"
#include "remocaoLogica.h"

int main(){
    //FILE* in = fopen("../in/topologiaRede.csv","r");
    //FILE* out = fopen("../out/topologiaRede.bin","wb");

    char nomeEntrada[32] = "in/";
    char nomeSaida[32] = "out/";

    int comando;
    char entrada[30];
    char saida[30];
    scanf("%d", &comando);

    switch(comando){
        case 1:
            scanf("%s", entrada);
            scanf("%s", saida);
            fflush(stdin);
            strcat(nomeEntrada, entrada);
            strcat(nomeSaida, saida);
            createTable(nomeEntrada, nomeSaida);
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            scanf("%s", entrada);
            strcat(nomeEntrada, entrada);
            insertInto(nomeEntrada);
            break;
        case 6:
            break;
    }

    return 0;
}