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
    //Abre os dois arquivos para leitura e escrita binária.
    FILE * arquivoRegistro = fopen(nomeArquivoRegistro, "rb+");
    FILE * arquivoArvore = fopen(nomeArquivoArvore, "rb+");
    
    //Se algum dos arquivos não foi aberto corretamente, imprime uma mensagem de erro.
    if(arquivoRegistro == NULL || arquivoArvore == NULL){
        imprimeErroArquivo();
        return;
    }
    
    //Obtém o registro de cabeçalho do arquivo de registros.
    RegCab registroCabecalho = retornaRegistroCabecalho(arquivoRegistro);

    //Obtém o cabeçalho da árvore do arquivo de índices.
    ArvoreCab cabecalho = retornaCabecalhoArvore(arquivoArvore);

    //Se algum dos arquivos apresentar inconsistência, imprime mensagem de erro.
    if(registroCabecalho.status == '0' || cabecalho.status == '0'){
        imprimeErroArquivo();
        return;
    }

    //Haverá mudanças no arquivo de registros e no de cabeçalho.
    registroCabecalho.status = '0';
    cabecalho.status = '0';

    //Atualiza o cabeçalho dos registros em disco.
    fseek(arquivoRegistro, 0, SEEK_SET);
    fwrite(&registroCabecalho.status, sizeof(char), 1, arquivoRegistro);

    //Atualiza o cabeçalho dá arvore em disco.
    fseek(arquivoArvore, 0, SEEK_SET);
    fwrite(&cabecalho.status, sizeof(char), 1, arquivoArvore);

    //Obtém a quantidade de registros a se inserir.
    int n;
    scanf("%d", &n);
    limparBuffer(); //Limpa o buffer que resulta do '\n' do scanf.

    //Laço que se repete 'n' vezes, onde 'n' é o número de inserções.
    for(int i = 0; i < n; i++){
        
        //Lê a linha de entrada da funcionalidade.
        char linha[128];
        fgets(linha, 128, stdin);

        //Separa a linha em cada atributo da struct de registro de dados.
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
        
        //Procura o próximo RRN disponível no arquivo de registros.
        int rrnDisponivelRegistro = retornaRRNRegistroDisponivel(&registroCabecalho, arquivoRegistro);
        
        //Calcula o offset do rrn.
        int offset =  TAM_PAG + rrnDisponivelRegistro*TAM_REG_DADOS;


        //Insere o registro de dados no arquivo de registros.
        insereRegistroDados(offset, registroDados, arquivoRegistro);

        //Insere um nó ná arvore no arquivo de índices.
        insereNoArvore(registroDados.idConecta, rrnDisponivelRegistro, &cabecalho, arquivoArvore);

        int proxRRN = registroCabecalho.proxRRN;
        //SE O rrnDisponivel < proxRRN ENTÃO FOI INSERIDO EM UM REMOVIDO.
        // if(rrnDisponivel < proxRRN)
        //     registroCabecalho.nRegRem--;
        // //SENÃO, FOI INSERIDO ONDE APONTAVA O proxRRN.
        // else{
        //     registroCabecalho.proxRRN++;
        //     //ATUALIZA nPagDisco SE NECESSÁRIO.
        //     int nPagDisco =  retornaNumPaginasDisco(registroCabecalho.proxRRN);
        //     nPagDisco++;
        //     if(nPagDisco != registroCabecalho.nPagDisco)
        //         registroCabecalho.nPagDisco = nPagDisco;
        // }

        //LIBERA VARIÁVEIS ALOCADAS DINAMICAMENTE NO REGISTRO DE DADOS.
        liberaRegistroDados(registroDados);
    }

    //MUDA CONSISTENCIA E ALOCA REGISTRO DE CABEÇALHO.
    registroCabecalho.status = '1';
    alocarRegistroCabecalho(registroCabecalho, arquivoRegistro);
    alocaCabecalhoArvore(&cabecalho, arquivoArvore);

    //Fecha os arquivos abertos.
    fclose(arquivoRegistro);
    fclose(arquivoArvore);

    //Imprime o binário dos arquivos.
    binarioNaTela(nomeArquivoRegistro);
    binarioNaTela(nomeArquivoArvore);
}