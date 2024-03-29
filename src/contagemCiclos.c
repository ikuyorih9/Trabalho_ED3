#include <stdio.h>
#include <stdlib.h>
#include "grafos.h"
#include "arquivos.h"
#include "mensagensErro.h"
#include "contagemCiclos.h"

void contagemCiclos(const char * nomeArquivo){
    //Abre o arquivo binário para leitura binária.
    FILE * arquivoBin = fopen(nomeArquivo, "rb");
    if(arquivoBin == NULL){
        imprimeErroArquivo_2();
        return;
    }

    //Recupera o registro de cabeçalho e verifica sua consistência.
    RegCab registroCabecalho = retornaRegistroCabecalho(arquivoBin);
    if(registroCabecalho.status == '0'){
        imprimeErroArquivo_2();
        return;
    }

    Grafo * grafo = criaGrafo();
    for(int i = 0; i < registroCabecalho.proxRRN; i++){
        RegDados registroDados = retornaRegistroDados(i, arquivoBin);
        insereGrafo(registroDados, grafo);
    }

    //Percorre todo o grafo.
    int numCiclos = 0;
    ListaAdj * noAtual = *grafo;
    while(noAtual != NULL){
        if(noAtual->cor == AZUL){
            numCiclos += procuraCiclos(noAtual, grafo);
        }
        noAtual = noAtual->proxNo;
    }

    printf("Quantidade de ciclos: %d\n", numCiclos);

    fclose(arquivoBin);
    
}