#include "insertInto.h"
#include "mensagensErro.h"
#include "arquivos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * separaCamposLinha(char * linha, int numCampo);
int retornaValorInteiro(char * linha, int numCampo);
char * retornaValorString(char *linha, int numCampo);

//ISSO PODE NAO ESTAR FUNCIONANDO!
int procuraRegistroRemovido(int topo, FILE * arquivo){
    if(topo == -1){
        int rrnRemovido = (ftell(arquivo)-TAM_PAG)/TAM_REG_DADOS;
        return rrnRemovido;
    }

    int offset = TAM_PAG + (TAM_REG_DADOS*topo) + 1;
    int rrnTopo = retornaCampoRegistroInteiro(offset, arquivo);
    return procuraRegistroRemovido(rrnTopo, arquivo);
}

int retornaRRNRegistroDisponivel(FILE * arquivo){
    int topo = retornaCampoRegistroInteiro(1, arquivo);
    int rrnDisponivel; //rrn onde vou colocar o regitro.

    if(topo != -1){
        rrnDisponivel = topo;
        //ATUALIZA TOPO
        int offsetEncadeamento = TAM_PAG + rrnDisponivel*TAM_REG_DADOS + 1; //PROCURA CAMPO 'encadeamento' NO REGISTRO DE RRN 'topo'
        int novoTopo = retornaCampoRegistroInteiro(offsetEncadeamento, arquivo); //RETORNA CAMPO DO OFFSET ANTERIOR
        mudarCampoInteiro(1, novoTopo, arquivo); //MUDA 'topo' DO REGISTRO DE CABEÇALHO PARA 'novoTopo.
    }
    else{
        rrnDisponivel = retornaCampoRegistroInteiro(5, arquivo);
    }
    return rrnDisponivel;
}

void insertInto(const char * nomeArquivo){
    //3 "Campina Grande" "Brazil" "BR" 4 "G" 10
    //22 "Teresina" NULO "BR" NULO NULO NULO
    FILE * arquivo = fopen(nomeArquivo, "rb+");
    if(arquivo == NULL){
        imprimeErroArquivo();
        return;
    }

    int n;
    scanf("%d", &n);
    fflush(stdin);
    for(int i = 0; i < n; i++){
        int rrnDisponivel = retornaRRNRegistroDisponivel(arquivo);
        int offset =  TAM_PAG + rrnDisponivel*TAM_REG_DADOS;
        
        char linha[128];
        fgets(linha, 128, stdin);

        int idConecta = retornaValorInteiro(linha, 0);
        char * sigla = retornaValorString(linha, 3);
        int idPoPsConectado = retornaValorInteiro(linha, 4);
        char * unidade = retornaValorString(linha,5);
        int velocidade = retornaValorInteiro(linha, 6);
        char * nomePoPs = retornaValorString(linha, 1);
        char * nomePais = retornaValorString(linha, 2);
        

        insereRegistroDados(offset,'0',-1,idConecta,sigla,idPoPsConectado,unidade,velocidade,nomePoPs,nomePais,arquivo);

        int proxRRN = retornaCampoRegistroInteiro(5, arquivo);
        if(rrnDisponivel < proxRRN)
            decrementarCampoInteiro(9, arquivo);
        else
            incrementaCampoInteiro(5, arquivo);

    }

    fclose(arquivo);
    binarioNaTela(nomeArquivo);
}

int retornaValorInteiro(char * linha, int numCampo){
    char * campo = separaCamposLinha(linha, numCampo);
    int valorCampo;
    int diferentes = strcmp(campo, "NULO");
    if(diferentes)
        valorCampo = atoi(campo);
    else
        valorCampo = -1;
    free(campo);
    return valorCampo;
}

char * retornaValorString(char *linha, int numCampo){
    char * campo = separaCamposLinha(linha, numCampo);
    int diferentes = strcmp(campo, "NULO");
    if(diferentes)
        return campo;
    else
        return NULL;
}

char * separaCamposLinha(char * linha, int numCampo){
    char * campo = malloc(sizeof(char*) * 128);
    int j = 0;
    int dentroAspas = 0;
    int quantidadeCampos = 0;
    for(int i = 0; linha[i] != '\0' && linha[i] != '\n'; i++){
        //FIM DE CAMPO ENCONTRADO.
        if(linha[i] == ' ' && !dentroAspas){
            if(quantidadeCampos == numCampo)
                break;
            else
                j = 0;
            quantidadeCampos++;
        }
        //ASPAS ENCONTRADAS.
        else if (linha[i] == '\"'){
            dentroAspas = !dentroAspas;
        }
        //LENDO O CAMPO.
        else{
            campo[j] = linha[i];
            j++;
        }
    }
    
    campo[j] = '\0';
    return campo;
}