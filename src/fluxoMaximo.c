#include <stdio.h>
#include "fluxoMaximo.h"
#include "grafos.h"
#include "arquivos.h"
#include "mensagensErro.h"

void fluxoMaximo(const char * nomeArquivo){
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
    
    retornaMenorVelocidade(0, 0, grafo);
    // int n;
    // scanf("%d", &n);
    // for(int i = 0; i < n; i++){
    //     int origem;
    //     scanf("%d", &origem);
    //     int destino;
    //     scanf("%d", &destino);

    //     retornaMenorVelocidade(origem, destino, grafo);

    // }

}