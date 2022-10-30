#include "arquivos.h"
#include "mensagensErro.h"
#include "compactacao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void compactacao(const char * nomeArquivo){
    //CRIA OS DIRETÓRIOS DO ARQUIVO A SE COMPACTAR E DO COMPACTADO.
    char * diretorioArquivo = retornaDiretorio(DIR_ENTRADA, nomeArquivo);
    char * diretorioArquivoCompactado = retornaDiretorio(DIR_SAIDA, nomeArquivo);

    //ABRE OS ARQUIVOS E OS VERIFICA.
    FILE * arquivo = fopen(diretorioArquivo, "rb");
    FILE * arquivoCompactado = fopen(diretorioArquivoCompactado, "wb");

    if(arquivo == NULL || arquivoCompactado == NULL){
        imprimeErroArquivo();
        return;
    }

    //LÊ O REGISTRO DE CABEÇALHO DO DISCO.
    RegCab registroCabecalho = retornaRegistroCabecalho(arquivo);
    if(registroCabecalho.status == '0'){
        imprimeErroArquivo();
        return;
    }

    //ALOCA REGISTRO DE CABEÇALHO DO ARQUIVO COMPACTADO NO DISCO.
    RegCab registroCabCompactado;
    registroCabCompactado.status = '0';
    registroCabCompactado.topo = -1;
    registroCabCompactado.proxRRN = 0;
    registroCabCompactado.nRegRem = 0;
    registroCabCompactado.nPagDisco = 0;
    registroCabCompactado.qtdCompacta = registroCabecalho.qtdCompacta + 1;
    alocarRegistroCabecalho(registroCabCompactado, arquivoCompactado);

    int qtdRegistros = registroCabecalho.proxRRN;

    //PERCORRE TODOS OS REGISTROS DE DADOS DO ARQUIVO NÃO COMPACTADO.
    for(int i = 0; i < qtdRegistros; i++){
        //LÊ O REGISTRO DE DADOS DO ARQUIVO NÃO COMPACTADO NO DISCO.
        RegDados registroDados = retornaRegistroDados(i, arquivo);

        //VERIFICA SE ESTÁ REMOVIDO.
        if(registroDados.removido == '1')
            continue;
        
        //INSERE O REGISTRO NÃO REMOVIDO NO ARQUIVO COMPACTADO.
        int offsetCompactado = TAM_PAG + registroCabCompactado.proxRRN*TAM_REG_DADOS;
        insereRegistroDados(offsetCompactado, registroDados, arquivoCompactado);

        //INCREMENTA O PRÓXIMO RRN DISPONÍVEL.
        registroCabCompactado.proxRRN++;

        //LIBERA AS VARIÁVEIS USADAS NO REGISTRO DE DADOS.
        liberaRegistroDados(registroDados);
    }

    //ATUALIZA REGISTRO DE CABEÇALHO DO ARQUIVO COMPACTADO.
    registroCabCompactado.status = '1';
    registroCabCompactado.nPagDisco = retornaNumPaginasDisco(registroCabCompactado.proxRRN) + 1;
    alocarRegistroCabecalho(registroCabCompactado, arquivoCompactado);


    fclose(arquivo);
    fclose(arquivoCompactado);

    binarioNaTela(diretorioArquivoCompactado);

    free(diretorioArquivo);
    free(diretorioArquivoCompactado);
}