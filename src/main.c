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
#include "recuperaArvore.h"
#include "insercaoArvore.h"
#include "recuperaConectados.h"

int main(){
    //FILE* in = fopen("../in/topologiaRede.csv","r");
    //FILE* out = fopen("../out/topologiaRede.bin","wb");

    int comando;
    char nomeArquivo1[TAM_DIR];
    char nomeArquivo2[TAM_DIR];
    char nomeCampo1[16];
    char nomeCampo2[16]; 
    char nomeIndice[TAM_DIR];
    scanf("%d", &comando);

    switch(comando){
        case 1:
            scanf("%s", nomeArquivo1);
            scanf("%s", nomeArquivo2);
            limparBuffer();
            createTable (nomeArquivo1, nomeArquivo2);
            break;
        case 2:
            scanf("%s", nomeArquivo1);
            limparBuffer();
            recuperaDados (nomeArquivo1);
            break;
        case 3:
            scanf("%s", nomeArquivo1);
            impressaoRegistros (nomeArquivo1);
            break;
        case 4:
            scanf("%s", nomeArquivo1);
            remocaoLogica (nomeArquivo1);
            break;
        case 5:
            scanf("%s", nomeArquivo1);
            insertInto (nomeArquivo1);
            break;
        case 6:
            scanf("%s", nomeArquivo1);
            compactacao (nomeArquivo1);
            break;
        case 7:
            scanf("%s", nomeArquivo1);
            scanf("%s", nomeArquivo2);
            limparBuffer();
            
            createIndex (nomeArquivo1, nomeArquivo2);
            break;
        case 8:
            scanf("%s", nomeArquivo1);
            scanf("%s", nomeArquivo2);
            recuperaArvore (nomeArquivo1, nomeArquivo2);
            break;
        case 9:
            scanf("%s", nomeArquivo1);
            scanf("%s", nomeArquivo2);
        insercaoArvore(nomeArquivo1, nomeArquivo2);
            break;
        case 10:
            scanf("%s", nomeArquivo1);
            scanf("%s", nomeArquivo2);
            scanf("%s", nomeCampo1);
            scanf("%s", nomeCampo2);
            scanf("%s", nomeIndice);
            recuperaConectados (nomeArquivo1, nomeArquivo2, nomeCampo1, nomeCampo2, nomeIndice);
            break;
    }

    return 0;
}