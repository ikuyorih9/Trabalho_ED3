#include "arvoreB.h"
#include "arquivos.h"
#include "mensagensErro.h"
#include <stdio.h>
#include <stdlib.h>

void createIndex(const char * nomeEntrada, const char * nomeSaida){
    char * diretorioEntrada = retornaDiretorio(DIR_ENTRADA, nomeEntrada);
    char * diretorioSaida = retornaDiretorio(DIR_SAIDA, nomeSaida);

    printf("%s\n%s\n", diretorioEntrada, diretorioSaida);

    FILE * arquivoEntrada = fopen(diretorioEntrada, "rb");
    FILE * arquivoSaida = fopen(diretorioSaida, "wb");

    if(arquivoEntrada == NULL || arquivoSaida == NULL){
        imprimeErroArquivo();
        return;
    }

    RegCab registroCabecalho = retornaRegistroCabecalho(arquivoEntrada);
    if(registroCabecalho.status == '0'){
        imprimeErroArquivo();
        return;
    }

    ArvoreCab cabecalho = criaCabecalhoArvore();
    alocaArvoreCabecalho(cabecalho, arquivoSaida);


    ArvoreDados dadosArvore = criaArvoreDados();
    //LÊ TODOS OS REGISTROS DO ARQUIVO DE ENTRADA.
    for(int i = 0; i < registroCabecalho.proxRRN; i++){
        int rrnDisponivel = buscaArvore(cabecalho, arquivo);
        ArvoreDados dados = criaArvoreDados();
        dados.noRRN = rrnDisponivel;
        for(int i = 0; i < ORDEM_ARV; i++){
            
        }
    }


    fclose(arquivoEntrada);
    fclose(arquivoSaida);

    free(diretorioEntrada);
    free(diretorioSaida);
}