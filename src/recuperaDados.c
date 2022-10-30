#include "recuperaDados.h"
#include "arquivos.h"
#include "limparBuffer.h"
#include "mensagensErro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void recuperaDados(const char * nomeArquivo){
    //CRIA DIRETÓRIO DO ARQUIVO A PARTIR DE SEU NOME.
    char * diretorioArquivo = retornaDiretorio(DIR_ENTRADA, nomeArquivo);

    //ABRE E VERIFICA ARQUIVO PARA LEITURA/ESCRITA.
    FILE * arquivo = fopen(diretorioArquivo, "rb");
    if(arquivo == NULL){
        imprimeErroArquivo();
        return;
    }

    //LÊ O REGISTRO DE CABEÇALHO DE DISCO E SALVA EM RAM.
    RegCab registroCabecalho = retornaRegistroCabecalho(arquivo);

    //VERIFICA CONSISTÊNCIA.
    if(registroCabecalho.status == '0'){
        imprimeErroArquivo();
        return;
    }

    //VERIFICA SE HÁ REGISTROS DE DADOS NO ARQUIVO.
    if (registroCabecalho.proxRRN != 0){
        //PERCORRE TODOS OS REGISTROS E OS IMPRIME.
        for (int i = 0; i < registroCabecalho.proxRRN; i++){
            RegDados registroDados = retornaRegistroDados(i, arquivo);
            imprimeRegistro(registroDados, arquivo);
            liberaRegistroDados(registroDados);
        }
    }
    else{
        registroNaoAlocado();
        printf("\n");
    }

    //IMPRIME O NÚMERO DE PÁGINAS DE DISCO.
    printf("Numero de paginas de disco: %d\n\n", registroCabecalho.nPagDisco);

    //LIBERA MEMÓRIA DA VARIÁVEL DE DIRETÓRIO.
    free(diretorioArquivo);
    
}