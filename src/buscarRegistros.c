#include "arquivos.h"
#include "limparBuffer.h"
#include "mensagensErro.h"
#include "buscarRegistros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * separaCamposLinha(char * linha, int numCampo);
int retornaValorInteiro(char * linha, int numCampo);

void buscarRegistros(const char * nomeArquivo){
    char * diretorioArquivo = retornaDiretorio(DIR_ENTRADA, nomeArquivo);
    FILE * arquivo = fopen(diretorioArquivo, "rb");
    if(arquivo == NULL){
        imprimeErroArquivo();
        return;
    }

    RegCab registroCabecalho = retornaRegistroCabecalho(arquivo);
    if(registroCabecalho.status == '0'){
        imprimeErroArquivo();
        return;
    }

    int n;
    scanf("%d", &n);
    limparBuffer();

    for(int i = 1; i <= n; i++){
        char linha[128];
        fgets(linha, 128, stdin);

        RegCab registroCabecalho = retornaRegistroCabecalho(arquivo);

        char * nomeCampo = separaCamposLinha(linha, 0);
        char * valorCampo = separaCamposLinha(linha, 1);
        int idCampo = retornaCampoID(nomeCampo);

        int * rrnRegistros = buscaRegistros(nomeCampo, valorCampo, &registroCabecalho, arquivo);

        printf("Busca %d\n", i);
        if(rrnRegistros[0] == -1){
            registroNaoAlocado();
            printf("\n");
        }
        else{
            for(int j = 0; rrnRegistros[j] != -1; j++){
                int offset = TAM_PAG + rrnRegistros[j]*TAM_REG_DADOS;
                imprimeRegistro(offset, arquivo);
                
            }
        }
        printf("Numero de paginas de disco: %d\n\n", registroCabecalho.nPagDisco);
        free(rrnRegistros);
    }
    
    free(diretorioArquivo);
}

