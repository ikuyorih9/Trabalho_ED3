#include <stdio.h>
#include <stdlib.h>
#include "gerarGrafo.h"
#include "mensagensErro.h"
#include "arquivos.h"
#include "grafos.h"

void gerarGrafo(const char * nomeArquivo){
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
        liberaRegistroDados(registroDados);
    }
    imprimeGrafo(grafo);

    //Libera memoria do grafo!
    
    fclose(arquivoBin);

}