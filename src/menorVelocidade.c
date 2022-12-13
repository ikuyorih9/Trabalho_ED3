#include <stdio.h>
#include <stdlib.h>
#include "menorVelocidade.h"
#include "grafos.h"
#include "arquivos.h"
#include "mensagensErro.h"

void menorVelocidade(const char * nomeArquivo){
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
    
    int n;
    scanf("%d", &n);
    for(int i = 0; i < n; i++){
        int origem;
        scanf("%d", &origem);
        int destino;
        scanf("%d", &destino);
        int parada;
        scanf("%d", &parada);
        int parada1 = retornaMenorVelocidade(origem, parada, grafo);
        int parada2 = retornaMenorVelocidade(parada, destino, grafo);
        int menorVelocidade = parada1 + parada2;
        if(parada1 == -1 || parada2 == -1)
            printf("Comprimento do caminho entre %d e %d parando em %d: -1\n", origem, destino, parada);
        else
            printf("Comprimento do caminho entre %d e %d parando em %d: %dMbps\n", origem, destino, parada, menorVelocidade);
    }

}