#include "createTable.h"
#include "mensagensErro.h"
#include "arquivos.h"
#include <stdio.h>
#include <stdlib.h>

char * retornaCampo(char * linha, int numCampo){
    char * campo = malloc(32*sizeof(char));
    int contCampo = 0;
    int j = 0;

    for(int i = 0; linha[i] != '\0' && linha[i] != '\n'; i++){
        if(linha[i] != ','){
            campo[j] = linha[i];
            j++;
        }
        else{
            if(contCampo == numCampo){
                campo[j] = '\0';
                break;
            }
            else
                j = 0;

            contCampo++;
        }    
    }

    return campo;
}

char * retornaCampoLinha(char * linha, int numCampo);
int recebeEntradaInteiro(char * linha, int numCampo);

void createTable(const char * nomeEntrada, const char * nomeSaida){
    FILE * in = fopen(nomeEntrada, "rb");
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

        int idConecta = recebeEntradaInteiro(linha, 0);
        char * siglaPais = retornaCampoLinha(linha, 3);
        int idPoPsConectado = recebeEntradaInteiro(linha, 4);
        char * unidadeMedida = retornaCampoLinha(linha, 5);
        int velocidade = recebeEntradaInteiro(linha, 6);

        char * nomePoPs = retornaCampoLinha(linha, 1);
        char * nomePais = retornaCampoLinha(linha, 2);

        int posCursor = (numRegistros * TAM_REG_DADOS) + TAM_PAG;

        insereRegistroDados(posCursor,'0',-1,idConecta,siglaPais,idPoPsConectado,unidadeMedida,velocidade,nomePoPs,nomePais,out);
        numRegistros++;

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

//RETORNA CAMPO DE OFFSET 'numCampo' DE UMA LINHA CSV.
char * retornaCampoLinha(char * linha, int numCampo){
    char * campo = malloc(32*sizeof(char));
    int contCampo = 0;
    int j = 0;

    for(int i = 0; linha[i] != '\0' && linha[i] != '\n'; i++){
        if(linha[i] != ','){
            campo[j] = linha[i];
            j++;
        }
        else{
            if(contCampo == numCampo){
                if(j == 0)
                    return NULL;
                else
                    campo[j] = '\0';
                break;
            }
            else
                j = 0;

            contCampo++;
        }    
    }
    return campo;
}

int recebeEntradaInteiro(char * linha, int numCampo){
    char * campo = retornaCampoLinha(linha, numCampo);
    if(campo == NULL)
        return -1;
    int valorInteiro = atoi(campo);
    free(campo);
    return valorInteiro;
}