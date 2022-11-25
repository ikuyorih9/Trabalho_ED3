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
    //Abre o arquivo de registro e o da árvore para leitura binária.
    FILE * arquivoRegistro = fopen(nomeArquivoRegistro, "rb");
    FILE * arquivoArvore = fopen(nomeArquivoArvore, "rb");
    
    //Verifica erro na abertura do arquivo.
    if(arquivoRegistro == NULL || arquivoArvore == NULL){
        imprimeErroArquivo();
        return;
    }

    //Recupera o registro de cabeçalho do arquivo de registros.
    RegCab registroCabecalho = retornaRegistroCabecalho(arquivoRegistro);
    //Verifica consistência dos registros. Se for '0', não está consistente.
    if(registroCabecalho.status == '0'){
        imprimeErroArquivo();
        return;
    }
    
    //Recupera o cabeçalho do arquivo da árvore.
    ArvoreCab cabecalho = retornaCabecalhoArvore(arquivoArvore);

    //Recebe a quantidade de iterações e limpa o buffer da leitura do scanf.
    int n;
    scanf("%d", &n);
    limparBuffer();

    //Percorre as 'n' iterações.
    for(int i = 1; i <= n; i++){
        //Lê a linha de entrada.
        char linha[128];
        fgets(linha, 128, stdin);

        //Separa o nome do campo e o valor do campo em duas variáveis.
        char * nomeCampo = separaCamposLinha(linha, 0);
        char * valorCampo = separaCamposLinha(linha, 1);

        //Obtem o id do campo. Os campos são identificados com números de 0 a 7, na ordem do registro.
        int idCampo = retornaCampoID(nomeCampo);
    
        //Variável que conta o número de páginas acessadas.
        int numPagsAcessadas = 0;

        //Verifica se o campo acessado é 'idConecta'.
        if(idCampo == 0){
            numPagsAcessadas = 1;  //Inclui o acesso ao cabeçalho da árvore.
            int chave = atoi(valorCampo);   //Converte o valor do campo para inteiro.
            int rrnRegistro = buscaRRNRegistroArvore(chave, cabecalho.raiz, &numPagsAcessadas, arquivoArvore); //Busca o RRN do registro salvo nó que possui a chave de busca.
            //Se a chave foi encontrada na árvore.
            if(rrnRegistro != -1){
                RegDados registroDados = retornaRegistroDados(rrnRegistro, arquivoRegistro); //Recupera o registro em disco no RRN encontrado anteriormente.
                numPagsAcessadas++; //Acessa a página de disco do registro.
                printf("Busca %d\n", i);
                imprimeRegistro(registroDados, arquivoRegistro); //Imprime o registro de dados.
                numPagsAcessadas++;
            }
            //Se a chave não foi encontrada na árvore.
            else{
                registroNaoAlocado(); //Imprime erro.
                printf("\n");
            }
        }
        //Caso o campo não seja o 'idConecta'.
        else{
            printf("Busca %d\n", i);
            buscaImprimeRegistros(nomeCampo, valorCampo, &registroCabecalho, arquivoRegistro); //Imprime todas as correspondências da busca.
            numPagsAcessadas = registroCabecalho.nPagDisco; //Obrigatoriamente, a busca deve acessar todos os registros para ter certeza que encontrou tudo.
        }

        //IMPRIME O NÚMERO DE PÁGINAS DE DISCO.
        printf("Numero de paginas de disco: %d\n\n", numPagsAcessadas);

    }

    //LIBERA MEMÓRIA DA VARIÁVEL DE DIRETÓRIO.
}