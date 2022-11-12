#include "createTable.h"
#include "mensagensErro.h"
#include "arquivos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * retornaCampoLinha(char * linha, int numCampo);
int recebeEntradaInteiro(char * linha, int numCampo);

void createTable(const char * nomeEntrada, const char * nomeSaida){
    //CRIA OS DIRETÓRIOS DOS ARQUIVOS A PARTIR DE SEUS NOMES.
    char * diretorioEntrada = retornaDiretorio(DIR_ENTRADA, nomeEntrada);
    char * diretorioSaida = retornaDiretorio(DIR_SAIDA, nomeSaida);

    FILE * in = fopen(diretorioEntrada, "r");
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

    //ALOCA REGISTROS EM DISCO.
    alocarRegistroCabecalho(registroCabecalho, out);

    //MOVE CURSOR DO ARQUIVO AO INICIO DA SEGUNDA PÁGINA DE DISCO.
    moveCursor(TAM_PAG, out);

    //RECEBE PRIMEIRA LINHA DO ARQUIVO CSV (APENAS CABEÇALHO).
    char linha[128];
    fgets(linha,128,in);    

    //LAÇO QUE RECEBE LINHAS NÃO NULAS DO ARQUIVO CSV.
    while(fgets(linha,128,in) != NULL){

        //RECEBE OS VALORES DO ARQUIVO CSV.
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

        //INSERE REGISTRO DE DADOS NO DISCO.
        int offset = TAM_PAG + registroCabecalho.proxRRN * TAM_REG_DADOS;
        insereRegistroDados(offset, registroDados, out);
        
        //O INDICADOR DO PRÓXIMO REGISTRO VAZIO AUMENTA.
        registroCabecalho.proxRRN++;

        //LIBERA VARIÁVEIS ALOCADAS DINAMICAMENTE.
        liberaRegistroDados(registroDados);
        
    }

    //ATUALIZA REGISTRO DE CABEÇALHO EM RAM.
    registroCabecalho.status = '1';
    registroCabecalho.nPagDisco = retornaNumPaginasDisco(registroCabecalho.proxRRN);
    registroCabecalho.nPagDisco++;

    //ATUALIZA REGISTRO DE CABEÇALHO EM DISCO.
    alocarRegistroCabecalho(registroCabecalho, out);

    fclose(in);
    fclose(out);

    binarioNaTela(diretorioSaida);

    //LIBERA OS DIRETÓRIOS ALOCADOS DINAMICAMENTE.
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
                if(j == 0 || (j == 1 && numCampo == 6)){
                    free(campo);
                    return NULL;
                }
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