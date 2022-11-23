#include "insercaoArvore.h"
#include "arvoreB.h"
#include "arquivos.h"
#include "limparBuffer.h"
#include "mensagensErro.h"
#include "insertInto.h"
#include <stdio.h>
#include <stdlib.h>

void recuperaConectados(const char * nomeArquivo1, const char * nomeArquivo2, const char * nomeCampo1, const char * nomeCampo2, const char * nomeArquivoIndice2){
    char * diretorioRegistro1 = retornaDiretorio(DIR_ENTRADA, nomeArquivo1);
    char * diretorioRegistro2 = retornaDiretorio(DIR_ENTRADA, nomeArquivo2);
    char * diretorioIndice = retornaDiretorio(DIR_ENTRADA, nomeArquivoIndice2);

    FILE * arquivoRegistro1 = fopen(diretorioRegistro1, "rb");
    FILE * arquivoRegistro2 = fopen(diretorioRegistro2, "rb");
    FILE * arquivoIndice2 = fopen(diretorioIndice, "rb");

    RegCab registroCabecalho1 = retornaRegistroCabecalho(arquivoRegistro1);
    if(registroCabecalho1.status == '0'){
        imprimeErroArquivo();
        free(diretorioRegistro1);
        free(diretorioRegistro2);
        free(diretorioIndice);
        return;
    }

    RegCab registroCabecalho2 = retornaRegistroCabecalho(arquivoRegistro2);
    if(registroCabecalho2.status == '0'){
        imprimeErroArquivo();
        free(diretorioRegistro1);
        free(diretorioRegistro2);
        free(diretorioIndice);
        return;
    }

    ArvoreCab cabecalho = retornaCabecalhoArvore(arquivoIndice2);
    if(cabecalho.status == '0'){
        imprimeErroArquivo();
        free(diretorioRegistro1);
        free(diretorioRegistro2);
        free(diretorioIndice);
        return;
    }
    int registrosEncontrados = 0;
    for(int i = 0; i < registroCabecalho1.proxRRN; i++){
        RegDados dados1 = retornaRegistroDados(i, arquivoRegistro1);
        int idPoPsConectado = dados1.idPoPsConectado;
        if(idPoPsConectado == -1)
            continue;
        int numPags = 0;
        int rrnRegistro2 = buscaRRNRegistroArvore(idPoPsConectado, cabecalho.raiz, &numPags, arquivoIndice2);
        if(rrnRegistro2 == -1)
            continue;
        RegDados dados2 = retornaRegistroDados(rrnRegistro2, arquivoRegistro2);
        
        printf("Identificador do ponto: %d\n", dados1.idConecta);
        if(dados1.nomePoPs[0] != '\0')
            printf("Nome do ponto: %s\n", dados1.nomePoPs);
        if(dados1.nomePais[0] != '\0')
            printf("Pais de localizacao: %s\n", dados1.nomePais);
        if(dados1.siglaPais[0] != '$')
            printf("Sigla do pais: %s\n", dados1.siglaPais);

        printf("Identificador do ponto conectado: %d\n", dados2.idConecta);
        if(dados2.nomePoPs[0] != '\0')
            printf("Nome do ponto conectado: %s\n", dados2.nomePoPs);
        if(dados2.nomePais[0] != '\0')
            printf("Nome do pais conectado: %s\n", dados2.nomePais);
        if(dados2.siglaPais[0] != '$')
            printf("Sigla do pais: %s\n", dados2.siglaPais);
        if(dados1.velocidade != -1)
            printf("Velocidade de transmissao: %d %cbps\n\n", dados1.velocidade, dados1.unidadeMedida[0]);

        registrosEncontrados = 1;
    }

    if(!registrosEncontrados)
        registroNaoAlocado();


    free(diretorioRegistro1);
    free(diretorioRegistro2);
    free(diretorioIndice);
}
