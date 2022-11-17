#include "recuperaArvore.h"
#include "arquivos.h"
#include "arvoreB.h"
#include "mensagensErro.h"
#include "limparBuffer.h"
#include <stdio.h>
#include <stdlib.h>

char * separaCamposLinha(char * linha, int numCampo);
int retornaValorInteiro(char * linha, int numCampo);

void recuperaArvore(const char * nomeArquivoRegistro, const char * nomeArquivoArvore){
    //CRIA DIRETÓRIO DO ARQUIVO A PARTIR DE SEU NOME.
    char * diretorioRegistro = retornaDiretorio(DIR_ENTRADA, nomeArquivoRegistro);
    char * diretorioArvore = retornaDiretorio(DIR_ENTRADA, nomeArquivoArvore);

    FILE * arquivoRegistro = fopen(diretorioRegistro, "rb");
    FILE * arquivoArvore = fopen(diretorioArvore, "rb");
    
    if(arquivoRegistro == NULL || arquivoArvore == NULL){
        imprimeErroArquivo();
        return;
    }
    
    RegCab registroCabecalho = retornaRegistroCabecalho(arquivoRegistro);
    if(registroCabecalho.status == '0'){
        imprimeErroArquivo();
        return;
    }

    ArvoreCab cabecalho = retornaCabecalhoArvore(arquivoArvore);

    //RECEBE QUANTIDADE DE ITERAÇÕES.
    int n;
    scanf("%d", &n);
    limparBuffer();

    for(int i = 1; i <= n; i++){
        //LÊ LINHA DE ENTRADA.
        char linha[128];
        fgets(linha, 128, stdin);

        //SEPARA NOME DO CAMPO E O VALOR DO CAMPO EM VARIÁVEIS.
        char * nomeCampo = separaCamposLinha(linha, 0);
        char * valorCampo = separaCamposLinha(linha, 1);
        int idCampo = retornaCampoID(nomeCampo);
        //BUSCA NA ÁRVORE B.
        if(idCampo == 0){
            int chave = atoi(valorCampo);
            int rrnRegistro = buscaRRNRegistroArvore(chave, cabecalho.raiz,arquivoArvore);
            if(rrnRegistro != -1){
                RegDados registroDados = retornaRegistroDados(rrnRegistro, arquivoRegistro);
                printf("Busca %d\n", i);
                imprimeRegistro(registroDados, arquivoRegistro);
            }
            else{
                registroNaoAlocado();
                printf("\n");
            }
        }
        //BUSCA NO REGISTRO.
        else{
            printf("Busca %d\n", i);
            buscaImprimeRegistros(nomeCampo, valorCampo, &registroCabecalho, arquivoRegistro);
        }

        //IMPRIME O NÚMERO DE PÁGINAS DE DISCO.
        printf("Numero de paginas de disco: %d\n\n", registroCabecalho.nPagDisco);

    }

    //LIBERA MEMÓRIA DA VARIÁVEL DE DIRETÓRIO.
    free(diretorioRegistro);
    free(diretorioArvore);
}