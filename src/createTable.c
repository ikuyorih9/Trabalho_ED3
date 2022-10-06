#include "createTable.h"
#include "mensagensErro.h"
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

int retornaTamanhoCampo(char * campo){
    int tamanho = 0;
    for(int i = 0; campo[i] != '\0' && campo[i] != '\n'; i++){
        tamanho++;
    }
    return tamanho;
}

void insereLixoRegistro(FILE * out, int quantidadeBytes){
    char lixo = '$';
    for(int a = quantidadeBytes; a < 64; a++)
        fwrite(&lixo, sizeof(char), 1, out);
}

void createTable(FILE * in, FILE* out){
    if(in == NULL || out == NULL){
        imprimeErroArquivo();
        return;
    }
    char delimitador = '|';

    char linha[128];
    fgets(linha,128,in);    //RECEBE LINHA DE NOME DAS COLUNAS.

    //LAÇO QUE RECEBE UMA LINHAS NÃO NULAS.
    while(fgets(linha,128,in) != NULL){
        //INTEIRO COM A QUANTIDADE DE BYTES ESCRITOS.  
        int quantidadeBytes = 0;

        //ESCREVE CAMPO 'removido' NO ARQUIVO DE SAÍDA.
        int removido = 0;
        fwrite(&removido, sizeof(int), 1, out);
        quantidadeBytes += 4;

        //ESCREVE CAMPO 'encadeamento' NO ARQUIVO DE SAÍDA.
        int encadeamento = -1;
        fwrite(&encadeamento, sizeof(int), 1, out);
        quantidadeBytes += 4;

        //ESCREVE CAMPO 'idConecta' NO ARQUIVO DE SAÍDA.
        int idConecta;
        idConecta = atoi(retornaCampo(linha, 0));
        fwrite(&idConecta, sizeof(int), 1, out);
        quantidadeBytes += 4;

        //ESCREVE CAMPO 'siglaPais' NO ARQUIVO DE SAÍDA.
        char * siglaPais = retornaCampo(linha, 3);
        fwrite(siglaPais, sizeof(char), 2, out);
        quantidadeBytes += 2;

        //ESCREVE CAMPO 'idPoPsConectado' NO ARQUIVO DE SAÍDA.
        float idPoPsConectado = atof(retornaCampo(linha,4));
        fwrite(&idPoPsConectado, sizeof(float), 1, out);
        quantidadeBytes += 4;

        //ESCREVE CAMPO 'unidadeMedida' NO ARQUIVO DE SAÍDA.
        char * unidadeMedida = retornaCampo(linha, 5);
        fwrite(unidadeMedida, sizeof(char), 1, out);
        quantidadeBytes += 1;

        //ESCREVE CAMPO 'velocidade' NO ARQUIVO DE SAÍDA.
        float velocidade = atof(retornaCampo(linha, 6));
        fwrite(&velocidade, sizeof(float),1,out);
        quantidadeBytes += 4;

        //ESCREVE CAMPO 'nomePoPs' NO ARQUIVO DE SAÍDA.
        char * nomePoPs = retornaCampo(linha, 1);
        fwrite(nomePoPs, sizeof(char),retornaTamanhoCampo(nomePoPs), out);
        fwrite(&delimitador, sizeof(char), 1, out);
        quantidadeBytes += (retornaTamanhoCampo(nomePoPs)+1) ;

        //ESCREVE CAMPO 'nomePais' NO ARQUIVO DE SAÍDA.
        char * nomePais = retornaCampo(linha, 2);
        fwrite(nomePais, sizeof(char),retornaTamanhoCampo(nomePais), out);
        fwrite(&delimitador, sizeof(char), 1, out);
        quantidadeBytes += (retornaTamanhoCampo(nomePais)+1) ;

        //PREENCHE O RESTANTE DO REGISTRO COM '$'.
        insereLixoRegistro(out, quantidadeBytes);

    }

    

}