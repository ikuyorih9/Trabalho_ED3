#include "insertInto.h"
#include "mensagensErro.h"
#include "arquivos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * separaCamposLinha(char * linha, int numCampo);
int retornaRRNRegistroDisponivel(RegCab * registroCabecalho, FILE * arquivo);
int retornaValorInteiro(char * linha, int numCampo);
char * retornaValorString(char *linha, int numCampo);

void insertInto(const char * nomeArquivo){
    //3 "Campina Grande" "Brazil" "BR" 4 "G" 10
    //22 "Teresina" NULO "BR" NULO NULO NULO

    char * diretorioArquivo = retornaDiretorio(DIR_ENTRADA, nomeArquivo);
    FILE * arquivo = fopen(diretorioArquivo, "rb+");
    if(arquivo == NULL){
        imprimeErroArquivo();
        return;
    }

    RegCab registroCabecalho = retornaRegistroCabecalho(arquivo);

    registroCabecalho.status = '0';
    mudarCampoString(0, &(registroCabecalho.status), 1, arquivo);   //PÕE STATUS COMO '0' NA ESCRITA.

    int n;
    scanf("%d", &n);
    fflush(stdin);
    for(int i = 0; i < n; i++){
        int rrnDisponivel = retornaRRNRegistroDisponivel(&registroCabecalho, arquivo);
        int offset =  TAM_PAG + rrnDisponivel*TAM_REG_DADOS;
        
        char linha[128];
        fgets(linha, 128, stdin);

        RegDados registroDados;

        registroDados.removido = '0';
        registroDados.encadeamento = -1;
        registroDados.idConecta = retornaValorInteiro(linha, 0);
        registroDados.siglaPais = retornaValorString(linha, 3);
        registroDados.idPoPsConectado = retornaValorInteiro(linha, 4);
        registroDados.unidadeMedida = retornaValorString(linha,5);
        registroDados.velocidade = retornaValorInteiro(linha, 6);
        registroDados.nomePoPs = retornaValorString(linha, 1);
        registroDados.nomePais = retornaValorString(linha, 2);
        

        insereRegistroDados(offset, registroDados,arquivo);

        int proxRRN = registroCabecalho.proxRRN;
        //SE O rrnDisponivel < proxRRN ENTÃO FOI INSERIDO EM UM REMOVIDO.
        if(rrnDisponivel < proxRRN)
            registroCabecalho.nRegRem--;
        //SENÃO, FOI INSERIDO ONDE APONTAVA O proxRRN.
        else{
            registroCabecalho.proxRRN++;
            //ATUALIZA nPagDisco SE NECESSÁRIO.
            int nPagDisco =  retornaNumPaginasDisco(registroCabecalho.proxRRN);
            nPagDisco++;
            if(nPagDisco != registroCabecalho.nPagDisco)
                registroCabecalho.nPagDisco = nPagDisco;
        }
    }

    registroCabecalho.status = '1';
    mudarCampoString(0, &(registroCabecalho.status), 1, arquivo); //PÕE STATUS COMO '0' NA ESCRITA.
    mudarCampoInteiro(1, registroCabecalho.topo, arquivo);      //ATUALIZA topo NO REGISTRO DE CABEÇALHO. 
    mudarCampoInteiro(5, registroCabecalho.proxRRN, arquivo);   //ATUALIZA proxRRN NO REGISTRO DE CABEÇALHO.
    mudarCampoInteiro(9, registroCabecalho.nRegRem, arquivo);   //ATUALIZA nRegRem NO REGISTRO DE CABEÇALHO.
    mudarCampoInteiro(13, registroCabecalho.nPagDisco, arquivo); //ATUALIZA nPagDisco NO REGISTRO DE CABEÇALHO.

    fclose(arquivo);
    binarioNaTela(diretorioArquivo);
    free(diretorioArquivo);
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

int retornaRRNRegistroDisponivel(RegCab * registroCabecalho, FILE * arquivo){
    int topo = registroCabecalho->topo;
    int rrnDisponivel; //rrn onde vou colocar o regitro.

    if(topo != -1){
        rrnDisponivel = topo;
        //ATUALIZA TOPO
        int offsetEncadeamento = TAM_PAG + rrnDisponivel*TAM_REG_DADOS + 1; //PROCURA CAMPO 'encadeamento' NO REGISTRO DE RRN 'topo'
        int novoTopo = retornaCampoFixoInteiro(offsetEncadeamento, arquivo); //RETORNA CAMPO DO OFFSET ANTERIOR
        registroCabecalho->topo = novoTopo; //MUDA 'topo' DO REGISTRO DE CABEÇALHO PARA 'novoTopo.
    }
    else
        rrnDisponivel = registroCabecalho->proxRRN;

    return rrnDisponivel;
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