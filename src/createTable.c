#include "createTable.h"
#include "mensagensErro.h"
#include "arquivos.h"
#include <stdio.h>
#include <stdlib.h>

void createTable(const char * nomeEntrada, const char * nomeSaida){
    FILE * in = fopen(nomeEntrada, "r");
    FILE * out = fopen(nomeSaida, "wb");

    if(in == NULL || out == NULL){
        imprimeErroArquivo();
        return;
    }

    //CRIA REGISTRO DE CABEÇALHO.
    alocarRegistroCabecalho(0, -1, 0, 0, 0, 0, out);

    fseek(out, 960,SEEK_SET);

    //CRIA REGISTRO DE DADOS.
    char linha[128];
    fgets(linha,128,in);    //RECEBE LINHA DE NOME DAS COLUNAS.

    int numRegistros = 0;

    //LAÇO QUE RECEBE UMA LINHAS NÃO NULAS.
    while(fgets(linha,128,in) != NULL){  
        //printf("%d: %s", numRegistros,linha);
        char * campoRetornado;

        int idConecta;
        campoRetornado = retornaCampoLinha(linha, 0);
        idConecta = atoi(campoRetornado);
        char * siglaPais = retornaCampoLinha(linha, 3);
        campoRetornado = retornaCampoLinha(linha,4);
        float idPoPsConectado = atof(campoRetornado);
        char * unidadeMedida = retornaCampoLinha(linha, 5);
        campoRetornado = retornaCampoLinha(linha,6);
        float velocidade = atof(campoRetornado);
        char * nomePoPs = retornaCampoLinha(linha, 1);
        char * nomePais = retornaCampoLinha(linha, 2);

        int posCursor = (numRegistros * TAM_REG_DADOS) + TAM_PAG;

        insereRegistroDados(posCursor,'0',-1,idConecta,siglaPais,idPoPsConectado,unidadeMedida,velocidade,nomePoPs,nomePais,out);
        numRegistros++;

        free(campoRetornado);
        free(siglaPais);
        free(unidadeMedida);
        free(nomePoPs);
        free(nomePais);
    }
    char status = '1';
    int numPag = retornaNumPaginasDisco(numRegistros, out);
    mudarCampoString(0, &status, 1, out);
    mudarCampoInteiro(5, numRegistros, out);    //numRegistros = RRN proximo registro.
    mudarCampoInteiro(13, numPag, out);

    fclose(in);
    fclose(out);

    binarioNaTela(nomeSaida);
}