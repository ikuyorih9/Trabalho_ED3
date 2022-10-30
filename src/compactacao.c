#include "arquivos.h"
#include "mensagensErro.h"
#include "compactacao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void compactacao(const char * nomeArquivo){
    char * diretorioArquivo = retornaDiretorio(DIR_ENTRADA, nomeArquivo);
    char * diretorioArquivoCompactado = retornaDiretorio(DIR_SAIDA, nomeArquivo);

    FILE * arquivo = fopen(diretorioArquivo, "rb");
    FILE * arquivoCompactado = fopen(diretorioArquivoCompactado, "wb");

    if(arquivo == NULL || arquivoCompactado == NULL){
        imprimeErroArquivo();
        return;
    }
    
    RegCab registroCabecalho = retornaRegistroCabecalho(arquivo);
    if(registroCabecalho.status == '0'){
        imprimeErroArquivo();
        return;
    }

    RegCab registroCabCompactado;
    registroCabCompactado.status = '0';
    registroCabCompactado.topo = -1;
    registroCabCompactado.proxRRN = 0;
    registroCabCompactado.nRegRem = 0;
    registroCabCompactado.nPagDisco = 0;
    registroCabCompactado.qtdCompacta = registroCabecalho.qtdCompacta + 1;

    alocarRegistroCabecalho(registroCabCompactado, arquivoCompactado);

    int qtdRegistros = registroCabecalho.proxRRN;

    for(int i = 0; i < qtdRegistros; i++){
        int offset = TAM_PAG + i*TAM_REG_DADOS;
        char * removido = retornaCampoFixoString(offset, 1, arquivo);
        if(*removido == '1'){
            free(removido);
            continue;
        }
        
        RegDados registroDados;
        registroDados.removido = '0';
        registroDados.encadeamento = -1;
        registroDados.idConecta = retornaCampoFixoInteiro(offset + 5, arquivo);
        registroDados.siglaPais = retornaCampoFixoString(offset + 9, 2, arquivo);
        registroDados.idPoPsConectado = retornaCampoFixoInteiro(offset + 11, arquivo);
        registroDados.unidadeMedida = retornaCampoFixoString(offset + 15, 1, arquivo);
        registroDados.velocidade = retornaCampoFixoInteiro(offset + 16, arquivo);
        
        registroDados.nomePoPs = retornaCampoVariavel(offset + 20, arquivo);
        registroDados.nomePais = retornaCampoVariavel(offset + 20 + strlen(registroDados.nomePoPs) + 1, arquivo);
        
        int offsetCompactado = TAM_PAG + registroCabCompactado.proxRRN*TAM_REG_DADOS;
        insereRegistroDados(offsetCompactado, registroDados, arquivoCompactado);

        registroCabCompactado.proxRRN++;

        free(removido);
        free(registroDados.siglaPais);
        free(registroDados.unidadeMedida);
        free(registroDados.nomePoPs);
        free(registroDados.nomePais);
    }

    registroCabCompactado.status = '1';
    registroCabCompactado.nPagDisco = retornaNumPaginasDisco(registroCabCompactado.proxRRN) + 1;
    mudarCampoString(0, &(registroCabCompactado.status), 1, arquivoCompactado);     //ATUALIZA CONSISTENCIA status NO ARQUIVO BINARIO.
    mudarCampoInteiro(5, registroCabCompactado.proxRRN, arquivoCompactado);      //ATUALIZA proxRRN NO ARQUIVO BINARIO.
    mudarCampoInteiro(13, registroCabCompactado.nPagDisco, arquivoCompactado);    //ATUALIZA nPagDisco NO ARQUIVO BINARIO.


    fclose(arquivo);
    fclose(arquivoCompactado);

    binarioNaTela(diretorioArquivoCompactado);

    free(diretorioArquivo);
    free(diretorioArquivoCompactado);
}