#include "insertInto.h"
#include "mensagensErro.h"
#include "arquivos.h"
#include "limparBuffer.h"
#include "insertInto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insertInto(const char * nomeArquivo){
    //CRIA DIRETORIO DO ARQUIVO A PARTIR DE SEU NOME.
    char * diretorioArquivo = retornaDiretorio(DIR_ENTRADA, nomeArquivo);
    FILE * arquivo = fopen(diretorioArquivo, "rb+");
    if(arquivo == NULL){
        imprimeErroArquivo();
        return;
    }

    //RECEBE REGISTRO DE CABEÇALHO E VERIFICA CONSISTÊNCIA.
    RegCab registroCabecalho = retornaRegistroCabecalho(arquivo);
    if(registroCabecalho.status == '0'){
        imprimeErroArquivo();
        return;
    }

    registroCabecalho.status = '0';
    mudarCampoString(0, &(registroCabecalho.status), 1, arquivo);   //PÕE STATUS COMO '0' NA ESCRITA.

    //RECEBE QUANTIDADE DE ITERAÇÕES.
    int n;
    scanf("%d", &n);
    limparBuffer();

    for(int i = 0; i < n; i++){
        //PROCURA O PRÓXIMO RRN DISPONÍVEL (REMOVIDO OU NÃO).
        int rrnDisponivel = retornaRRNRegistroDisponivel(&registroCabecalho, arquivo);
        int offset =  TAM_PAG + rrnDisponivel*TAM_REG_DADOS;
        
        //LÊ LINHA DE ENTRADA.
        char linha[128];
        fgets(linha, 128, stdin);

        //SEPARA DADOS DA LINHA DE ENTRADA E PÕE NO REGISTRO EM RAM.
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
        
        //INSERE REGISTRO EM DISCO.
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

        //LIBERA VARIÁVEIS ALOCADAS DINAMICAMENTE NO REGISTRO DE DADOS.
        liberaRegistroDados(registroDados);
    }

    //MUDA CONSISTENCIA E ALOCA REGISTRO DE CABEÇALHO.
    registroCabecalho.status = '1';
    alocarRegistroCabecalho(registroCabecalho, arquivo);

    fclose(arquivo);
    binarioNaTela(diretorioArquivo);
    free(diretorioArquivo);
}

//DADA A LINHA, RETORNA UM VALOR INTEIRO CORRESPONDENTE AO CAMPO.
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

//RETORNA O PRÓXIMO RRN DISPONÍVEL.
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

//DADA A LINHA, RETORNA UMA STRING CORRESPONDENTE AO CAMPO.
char * retornaValorString(char *linha, int numCampo){
    char * campo = separaCamposLinha(linha, numCampo);
    int diferentes = strcmp(campo, "NULO");
    if(diferentes)
        return campo;
    else
        return NULL;
}

//SEPARA A LINHA EM SEGMENTOS, E RETORNA O SEGMENTO CONFORME O CAMPO ESCOLHIDO.
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