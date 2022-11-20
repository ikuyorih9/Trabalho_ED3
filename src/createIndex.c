#include "arvoreB.h"
#include "arquivos.h"
#include "mensagensErro.h"
#include <stdio.h>
#include <stdlib.h>

void createIndex(const char * nomeArquivoRegistro, const char * nomeArquivoIndice){
    //Cria o diretorio do arquivo de registro a partir de seu nome.
    char * diretorioRegistro = retornaDiretorio(DIR_ENTRADA, nomeArquivoRegistro);

    //Cria o diretorio do arquivo de indice a partir do seu nome.
    char * diretorioIndice = retornaDiretorio(DIR_SAIDA, nomeArquivoIndice);

    //Abre o arquivo de registro para leitura binária.
    FILE * arquivoRegistro = fopen(diretorioRegistro, "rb");

    //Abre o arquivo de índice para escrita e leitura binária.
    FILE * arquivoArvore = fopen(diretorioIndice, "wb+");
    
    //Verifica a abertura dos dois arquivos.
    if(arquivoRegistro == NULL || arquivoArvore == NULL){
        //Se não foi possível abrir os arquivos, imprime mensagem de erro.
        imprimeErroArquivo();
        return;
    }
    
    //Põe o registro de cabeçalho do disco para a RAM.
    RegCab registroCabecalho = retornaRegistroCabecalho(arquivoRegistro);

    //Verifica consistência do arquivo de registros.
    if(registroCabecalho.status == '0'){
        //Se o arquivo está inconsistente, imprime erro de arquivos.
        imprimeErroArquivo();
        return;
    }

    //Cria o cabeçalho de uma árvore B em RAM e a inicializa.
    ArvoreCab cabecalho = criaCabecalho();

    //Põe o cabeçalho da árvore em disco.
    alocaCabecalhoArvore(&cabecalho, arquivoArvore);

    //O laço percorre a quantidade de registros que tem em disco.
    for(int i = 0; i < registroCabecalho.proxRRN; i++){
        //Lê um registro de dados do arquivo.
        RegDados registroDados = retornaRegistroDados(i, arquivoRegistro);
        
        //Se o arquivo de dados é marcado como removido.
        if(registroDados.removido == '1'){
            //Ignora tudo e vai para o próximo loop.
            continue;
        }

        //Obtem a chave (idConecta) e o rrnRef (RRN do registro de dados).
        int chave = registroDados.idConecta;
        int rrnRef = i;

        //Insere a chave e o rrnRef na árvore B. 
        insereNoArvore(chave, rrnRef, &cabecalho, arquivoArvore);
    }
    
    //Atualiza o cabeçalho da árvore.
    cabecalho.status = '1';
    alocaCabecalhoArvore(&cabecalho, arquivoArvore);

    //Fecha os arquivos das árvores.
    fclose(arquivoRegistro);
    fclose(arquivoArvore);

    //Mostra o binário dos arquivos.
    binarioNaTela(diretorioIndice);

    //Libera memória alocada para o nome dos arquivos.
    free(diretorioRegistro);
    free(diretorioIndice);
}