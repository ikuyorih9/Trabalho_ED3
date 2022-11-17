#include "arvoreB.h"
#include "arquivos.h"
#include "mensagensErro.h"
#include <stdio.h>
#include <stdlib.h>

void createIndex(const char * nomeEntrada, const char * nomeSaida){
    //7 binario1.bin indice1.bin
    char * diretorioEntrada = retornaDiretorio(DIR_ENTRADA, nomeEntrada);
    char * diretorioSaida = retornaDiretorio(DIR_SAIDA, nomeSaida);

    // char diretorioEntrada[64] = "in/indice4.bin";
    // char diretorioSaida[64] = "teste.bin";
    //printf("%s %s", diretorioEntrada, diretorioSaida);

    FILE * arquivoRegistro = fopen(diretorioEntrada, "rb");
    FILE * arquivoArvore = fopen(diretorioSaida, "wb+");
    
    if(arquivoRegistro == NULL || arquivoArvore == NULL){
        imprimeErroArquivo();
        return;
    }
    
    RegCab registroCabecalho = retornaRegistroCabecalho(arquivoRegistro);
    if(registroCabecalho.status == '0'){
        imprimeErroArquivo();
        return;
    }

    ArvoreCab cabecalho = criaCabecalho();
    alocaCabecalhoArvore(&cabecalho, arquivoArvore);

    for(int i = 0; i < registroCabecalho.proxRRN; i++){
        RegDados registroDados = retornaRegistroDados(i, arquivoRegistro);
        if(registroDados.removido == '1'){
            continue;
        }
        int chave = registroDados.idConecta;
        int rrnRef = i;
        //printf("\n\nInserindo [%d] de referencia %d.", chave, i);
        insereNoArvore(chave, i, &cabecalho, arquivoArvore);
        
        //imprimeChavesOrdenado(cabecalho.raiz, arquivoArvore);
        //printf("\n");
        //system("pause");
        // printf("alturaArvore: %d\n", cabecalho.alturaArvore);
        
    }

    //imprimeChavesOrdenado(cabecalho.raiz, arquivoArvore);
    // printf("\nAltura arvore: %d\n", cabecalho.alturaArvore);
    
    // ArvoreDados no = retornaNoArquivo(31, arquivoArvore);
    // imprimeNo(no);
    
    cabecalho.status = '1';
    alocaCabecalhoArvore(&cabecalho, arquivoArvore);

    fclose(arquivoRegistro);
    fclose(arquivoArvore);

    binarioNaTela(diretorioSaida);

    free(diretorioEntrada);
    free(diretorioSaida);
}