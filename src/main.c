/*
    Hugo Hiroyuki Nakamura (12732037) - 100%
        Create Table [1]
        Remoção Lógica [4]
        Insert Into [5]
        Compactação [6]
    Mateus Santos Messias (12548000) - 100%
        Impressão de todos os registros [3]
        Select dos registros [4]
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivos.h"
#include "arvoreB.h"
#include "createTable.h"
#include "insertInto.h"
#include "remocaoLogica.h"
#include "compactacao.h"
#include "limparBuffer.h"
#include "recuperaDados.h"
#include "impressaoRegistros.h"
#include "createIndex.h"

int main(){
    //FILE* in = fopen("../in/topologiaRede.csv","r");
    //FILE* out = fopen("../out/topologiaRede.bin","wb");

    int comando;
    char nomeEntrada[TAM_DIR];
    char nomeSaida[TAM_DIR];
    scanf("%d", &comando);

    switch(comando){
        case 1:
            scanf("%s", nomeEntrada);
            scanf("%s", nomeSaida);
            limparBuffer();
            createTable(nomeEntrada, nomeSaida);
            break;
        case 2:
            scanf("%s", nomeEntrada);
            limparBuffer();
            recuperaDados(nomeEntrada);
            break;
        case 3:
            scanf("%s", nomeEntrada);
            impressaoRegistros(nomeEntrada);
            break;
        case 4:
            scanf("%s", nomeEntrada);
            remocaoLogica(nomeEntrada);
            break;
        case 5:
            scanf("%s", nomeEntrada);
            insertInto(nomeEntrada);
            break;
        case 6:
            scanf("%s", nomeEntrada);
            compactacao(nomeEntrada);
            break;
        case 7:
            scanf("%s", nomeEntrada);
            scanf("%s", nomeSaida);
            limparBuffer();
            createIndex(nomeEntrada, nomeSaida);
            break;
    }

    return 0;
}