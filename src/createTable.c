#include "createTable.h"
#include "mensagensErro.h"
#include "arquivos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char * diretorioEntrada = retornaDiretorio(DIR_ENTRADA, nomeEntrada);
    char * diretorioSaida = retornaDiretorio(DIR_SAIDA, nomeSaida);

    FILE * in = fopen(diretorioEntrada, "rb");
    FILE * out = fopen(diretorioSaida, "wb");

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

    alocarRegistroCabecalho(registroCabecalho, out);

    fseek(out, 960,SEEK_SET);

    //CRIA REGISTRO DE DADOS.
    char linha[128];
    fgets(linha,128,in);    //RECEBE LINHA DE NOME DAS COLUNAS.
    int i = 1;
    //LAÇO QUE RECEBE UMA LINHAS NÃO NULAS.
    while(fgets(linha,128,in) != NULL){
        RegDados registroDados;
        registroDados.removido = '0';
        registroDados.encadeamento = -1;  
        registroDados.idConecta = recebeEntradaInteiro(linha, 0);
        registroDados.siglaPais = retornaCampoLinha(linha, 3);
        registroDados.idPoPsConectado = recebeEntradaInteiro(linha, 4);
        registroDados.unidadeMedida = retornaCampoLinha(linha, 5);
        registroDados.velocidade = recebeEntradaInteiro(linha, 6);
        

        registroDados.nomePoPs = retornaCampoLinha(linha, 1);
        registroDados.nomePais = retornaCampoLinha(linha, 2);

        int offset = TAM_PAG + registroCabecalho.proxRRN * TAM_REG_DADOS;

        insereRegistroDados(offset, registroDados, out);
        registroCabecalho.proxRRN++;

        free(registroDados.siglaPais);
        free(registroDados.unidadeMedida);
        free(registroDados.nomePoPs);
        free(registroDados.nomePais);
        i++;
    }
    registroCabecalho.status = '1';
    registroCabecalho.nPagDisco = retornaNumPaginasDisco(registroCabecalho.proxRRN);
    registroCabecalho.nPagDisco++;
    mudarCampoString(0, &(registroCabecalho.status), 1, out);     //ATUALIZA CONSISTENCIA status NO ARQUIVO BINARIO.
    mudarCampoInteiro(5, registroCabecalho.proxRRN, out);      //ATUALIZA proxRRN NO ARQUIVO BINARIO.
    mudarCampoInteiro(13, registroCabecalho.nPagDisco, out);    //ATUALIZA nPagDisco NO ARQUIVO BINARIO.

    fclose(in);
    fclose(out);

    binarioNaTela(diretorioSaida);

    free(diretorioEntrada);
    free(diretorioSaida);
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