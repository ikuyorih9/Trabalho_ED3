#include "insercaoArvore.h"
#include "arvoreB.h"
#include "arquivos.h"
#include "limparBuffer.h"
#include "mensagensErro.h"
#include "insertInto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insercaoArvore(const char * nomeArquivoRegistro, const char * nomeArquivoArvore){
    //CRIA DIRETÓRIO DO ARQUIVO A PARTIR DE SEU NOME.
    char * diretorioRegistro = retornaDiretorio(DIR_ENTRADA, nomeArquivoRegistro);
    char * diretorioArvore = retornaDiretorio(DIR_ENTRADA, nomeArquivoArvore);

    FILE * arquivoRegistro = fopen(diretorioRegistro, "rb+");
    FILE * arquivoArvore = fopen(diretorioArvore, "rb+");
    
    if(arquivoRegistro == NULL || arquivoArvore == NULL){
        imprimeErroArquivo();
        return;
    }
    
    RegCab registroCabecalho = retornaRegistroCabecalho(arquivoRegistro);
    ArvoreCab cabecalho = retornaCabecalhoArvore(arquivoArvore);
    if(registroCabecalho.status == '0' || cabecalho.status == '0'){
        imprimeErroArquivo();
        return;
    }

    //RECEBE QUANTIDADE DE ITERAÇÕES.
    int n;
    scanf("%d", &n);
    limparBuffer();

    for(int i = 0; i < n; i++){
        //PROCURA O PRÓXIMO RRN DISPONÍVEL (REMOVIDO OU NÃO).
        int rrnDisponivel = retornaRRNRegistroDisponivel(&registroCabecalho, arquivoRegistro);
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
        insereRegistroDados(offset, registroDados, arquivoRegistro);
        //INSERE INDICE EM DISCO.
        insereNoArvore(registroDados.idConecta, rrnDisponivel, &cabecalho, arquivoArvore);

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
    alocarRegistroCabecalho(registroCabecalho, arquivoRegistro);
    alocaCabecalhoArvore(&cabecalho, arquivoArvore);

    fclose(arquivoRegistro);
    fclose(arquivoArvore);

    binarioNaTela(diretorioRegistro);
    binarioNaTela(diretorioArvore);

    free(diretorioRegistro);
    free(diretorioArvore);
}