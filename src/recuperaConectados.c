#include "insercaoArvore.h"
#include "arvoreB.h"
#include "arquivos.h"
#include "limparBuffer.h"
#include "mensagensErro.h"
#include "insertInto.h"
#include <stdio.h>
#include <stdlib.h>

void recuperaConectados(const char * nomeArquivo1, const char * nomeArquivo2, const char * nomeCampo1, const char * nomeCampo2, const char * nomeArquivoIndice2){
    //Abre os três arquivos para leitura binária.
    FILE * arquivoRegistro1 = fopen(nomeArquivo1, "rb");
    FILE * arquivoRegistro2 = fopen(nomeArquivo2, "rb");
    FILE * arquivoIndice2 = fopen(nomeArquivoIndice2, "rb");

    //Obtém o registro de cabeçalho do primeiro arquivo de registros.
    RegCab registroCabecalho1 = retornaRegistroCabecalho(arquivoRegistro1);
    //Verifica a consistência desse arquivo.
    if(registroCabecalho1.status == '0'){
        imprimeErroArquivo();
        return;
    }

    //Obtém o registro de cabeçalho do segundo arquivo de registros.
    RegCab registroCabecalho2 = retornaRegistroCabecalho(arquivoRegistro2);
    //Verifica a consistência desse arquivo.
    if(registroCabecalho2.status == '0'){
        imprimeErroArquivo();
        return;
    }

    //Obtém o cabeçalho da árvore do arquivo de índice.
    ArvoreCab cabecalho = retornaCabecalhoArvore(arquivoIndice2);
    //Verifica a consistência desse arquivo.
    if(cabecalho.status == '0'){
        imprimeErroArquivo();
        return;
    }

    int registrosEncontrados = 0;   //Variável de controle que verifica se ao menos um registro foi encontrado.
    
    //Percorre todos os registros do primeiro arquivo de registros.
    for(int i = 0; i < registroCabecalho1.proxRRN; i++){
        RegDados dados1 = retornaRegistroDados(i, arquivoRegistro1); //Obtem o registro de dados de RRN i do primeiro arquivo de registros.
        int idPoPsConectado = dados1.idPoPsConectado;   //Guarda o idPoPsConectado desse registro, para facilitar o entendimento.
        
        //Verifica se o idPoPsConectado existe nesse registro.
        if(idPoPsConectado == -1)
            continue;

        int numPags = 0;

        //Retorna o RRN do registro de dados que corresponde a busca de 'idPoPsConectado' na árvore.
        int rrnRegistro2 = buscaRRNRegistroArvore(idPoPsConectado, cabecalho.raiz, &numPags, arquivoIndice2);

        //Se não encontrou uma correspondência na busca, então o laço segue a próxima iteração.
        if(rrnRegistro2 == -1)
            continue;
        
        //Abre o registro de dados do RRN encontrado no segundo arquivo de registros.
        RegDados dados2 = retornaRegistroDados(rrnRegistro2, arquivoRegistro2);
        
        //Imprime o registro atual do arquivo 1.
        printf("Identificador do ponto: %d\n", dados1.idConecta);
        if(dados1.nomePoPs[0] != '\0')
            printf("Nome do ponto: %s\n", dados1.nomePoPs);
        if(dados1.nomePais[0] != '\0')
            printf("Pais de localizacao: %s\n", dados1.nomePais);
        if(dados1.siglaPais[0] != '$')
            printf("Sigla do pais: %s\n", dados1.siglaPais);

        //Imprime o registro encontrado na busca, no arquivo de registros 2.
        printf("Identificador do ponto conectado: %d\n", dados2.idConecta);
        if(dados2.nomePoPs[0] != '\0')
            printf("Nome do ponto conectado: %s\n", dados2.nomePoPs);
        if(dados2.nomePais[0] != '\0')
            printf("Nome do pais conectado: %s\n", dados2.nomePais);
        if(dados2.siglaPais[0] != '$')
            printf("Sigla do pais: %s\n", dados2.siglaPais);
        if(dados1.velocidade != -1)
            printf("Velocidade de transmissao: %d %cbps\n\n", dados1.velocidade, dados1.unidadeMedida[0]);

        //Um registro foi encontrado.
        registrosEncontrados = 1;
    }

    //Se nenhum registro foi encontrado, então imprime que não há correspondências.
    if(!registrosEncontrados)
        registroNaoAlocado();

}
