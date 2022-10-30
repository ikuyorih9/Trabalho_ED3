#include "arquivos.h"
#include "limparBuffer.h"
#include "mensagensErro.h"
#include "impressaoRegistros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * separaCamposLinha(char * linha, int numCampo);
int retornaValorInteiro(char * linha, int numCampo);

void impressaoRegistros(const char * nomeArquivo){
    //CRIA DIRETÓRIO DO ARQUIVO A PARTIR DO SEU NOME.
    char * diretorioArquivo = retornaDiretorio(DIR_ENTRADA, nomeArquivo);

    //ABRE ARQUIVO PARA LEITURA E O VERIFICA.
    FILE * arquivo = fopen(diretorioArquivo, "rb");
    if(arquivo == NULL){
        imprimeErroArquivo();
        return;
    }

    //RECUPERA REGISTRO DE CABEÇALHO DE DISCO PARA RAM E VERIFICA SUA CONSISTÊNCIA.
    RegCab registroCabecalho = retornaRegistroCabecalho(arquivo);
    if(registroCabecalho.status == '0'){
        imprimeErroArquivo();
        return;
    }

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

        printf("Busca %d\n", i);
        buscaImprimeRegistros(nomeCampo, valorCampo, &registroCabecalho, arquivo);

        //IMPRIME O NÚMERO DE PÁGINAS DE DISCO.
        printf("Numero de paginas de disco: %d\n\n", registroCabecalho.nPagDisco);

    }
    
    free(diretorioArquivo);
}

