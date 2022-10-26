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
                break;
            }
            else
                j = 0;

            contCampo++;
        }    
    }

    campo[j] = '\0';
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

    //CRIA REGISTRO DE CABEÇALHO EM RAM.
    RegCab registroCabecalho;
    registroCabecalho.status = '0';
    registroCabecalho.topo = -1;
    registroCabecalho.proxRRN = 0;
    registroCabecalho.nRegRem = 0;
    registroCabecalho.nPagDisco = 0;
    registroCabecalho.qtdCompacta = 0;

    alocarRegistroCabecalho('0', -1, 0, 0, 0, 0, out);

    fseek(out, 960,SEEK_SET);

    //CRIA REGISTRO DE DADOS.
    char linha[128];
    fgets(linha,128,in);    //RECEBE LINHA DE NOME DAS COLUNAS.
    int i = 1;
    //LAÇO QUE RECEBE UMA LINHAS NÃO NULAS.
    while(fgets(linha,128,in) != NULL){  
        int idConecta = recebeEntradaInteiro(linha, 0);
        char * siglaPais = retornaCampoLinha(linha, 3);
        int idPoPsConectado = recebeEntradaInteiro(linha, 4);
        char * unidadeMedida = retornaCampoLinha(linha, 5);
        int velocidade = recebeEntradaInteiro(linha, 6);
        

        char * nomePoPs = retornaCampoLinha(linha, 1);
        char * nomePais = retornaCampoLinha(linha, 2);

        int posCursor = (registroCabecalho.proxRRN * TAM_REG_DADOS) + TAM_PAG;

        insereRegistroDados(posCursor,'0',-1,idConecta,siglaPais,idPoPsConectado,unidadeMedida,velocidade,nomePoPs,nomePais,out);
        registroCabecalho.proxRRN++;

        free(siglaPais);
        free(unidadeMedida);
        free(nomePoPs);
        free(nomePais);
        i++;
    }
    registroCabecalho.status = '1';
    registroCabecalho.nPagDisco = retornaNumPaginasDisco(registroCabecalho.proxRRN, out);
    registroCabecalho.nPagDisco++;
    mudarCampoString(0, &(registroCabecalho.status), 1, out);     //ATUALIZA CONSISTENCIA status NO ARQUIVO BINARIO.
    mudarCampoInteiro(5, registroCabecalho.proxRRN, out);      //ATUALIZA proxRRN NO ARQUIVO BINARIO.
    mudarCampoInteiro(13, registroCabecalho.nPagDisco, out);    //ATUALIZA nPagDisco NO ARQUIVO BINARIO.

    fclose(in);
    fclose(out);

    binarioNaTela(nomeSaida);
}

//RETORNA CAMPO DE OFFSET 'numCampo' DE UMA LINHA CSV.
char * retornaCampoLinha(char * linha, int numCampo){
    char * campo = malloc(32*sizeof(char));
    int contCampo = 0;
    int j = 0;

    for(int i = 0; linha[i] != '\0'; i++){
        if(linha[i] != ',' && linha[i] != '\n'){
            campo[j] = linha[i];
            j++;
        }
        else{
            if(contCampo == numCampo){
                if(j == 0)
                    return NULL;
                break;
            }
            else
                j = 0;

            contCampo++;
        }    
    }
    campo[j] = '\0';
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