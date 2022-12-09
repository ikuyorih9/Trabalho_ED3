#include <stdio.h>
#include <stdlib.h>
#include "gerarGrafo.h"
#include "mensagensErro.h"
#include "arquivos.h"
#include "grafos.h"

void gerarGrafo(const char * nomeArquivo){
    //Abre o arquivo binário para leitura binária.
    FILE * arquivoBin = fopen(nomeArquivo, "rb");
    if(arquivoBin == NULL){
        imprimeErroArquivo();
    }

    //Recupera o registro de cabeçalho e verifica sua consistência.
    RegCab registroCabecalho = retornaRegistroCabecalho(arquivoBin);
    if(registroCabecalho.status == '0'){
        imprimeErroArquivo();
        return;
    }

    ListaAdj * listaAdj = criaListaAdj();
    for(int i = 0; i < 5; i++){
        int num;
        scanf("%d", &num);
        ListaLinear * no = criaNoListaLinear(num,0);  
        adicionaNoLista(no, listaAdj);  
    }
    imprimeListaLinear(listaAdj->raizListaLinear);
    

}