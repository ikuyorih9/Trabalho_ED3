#include <stdio.h>
#include <stdlib.h>
#include "gerarGrafo.h"
#include "mensagensErro.h"
#include "arquivos.h"
#include "grafos.h"

void gerarGrafo(const char * nomeArquivo){
    //Abre o arquivo binário para leitura binária.
    FILE * arquivoBin = fopen(nomeArquivo, "rb");

    //Se não foi possível abrir o arquivo binário, imprime uma mensagem de erro e retorna.
    if(arquivoBin == NULL){
        imprimeErroArquivo_2();
        return;
    }

    //Recupera o registro de cabeçalho e verifica sua consistência.
    RegCab registroCabecalho = retornaRegistroCabecalho(arquivoBin);

    //Se o arquivo de registro é inconsistente, imprime uma mensagem de erro e retorna.
    if(registroCabecalho.status == '0'){
        imprimeErroArquivo_2();
        return;
    }

    //Aloca memória para um grafo.
    Grafo * grafo = criaGrafo();

    //Para cada registro do arquivo binário.
    for(int i = 0; i < registroCabecalho.proxRRN; i++){
        //Abre um regitro de dados em disco e o salva numa variável em RAM.
        RegDados registroDados = retornaRegistroDados(i, arquivoBin);

        //Insere esse registro no grafo.
        insereGrafo(registroDados, grafo);

        //Libera as variáveis alocadas dinamicamente do registro de dados.
        liberaRegistroDados(registroDados);

    }

    //Imprime o grafo.
    imprimeGrafo(grafo);

    //Libera memoria alocada para o grafo.
    liberaGrafo(grafo);

    //Fecha o arquivo binário.
    fclose(arquivoBin);
}