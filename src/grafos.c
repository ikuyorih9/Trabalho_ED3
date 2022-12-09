#include <stdio.h>
#include <stdlib.h>
#include "grafos.h"

//Reserva espaço para um lista de adjacências.
ListaAdj * criaListaAdj(){
    ListaAdj * listaAdj = malloc(sizeof(ListaAdj));
    listaAdj->raizListaLinear = NULL;
    listaAdj->proxNo = NULL;
    listaAdj->anteNo = NULL;
    return listaAdj;
}

ListaLinear * criaNoListaLinear(int idPoPsConectado, int velocidade){
    ListaLinear * listaLinear = malloc(sizeof(ListaLinear));
    listaLinear->idPoPsConectado = idPoPsConectado;
    listaLinear->velocidade = velocidade;
    listaLinear->anteNo = NULL;
    listaLinear->proxNo = NULL;
    
    return listaLinear;
}

void adicionaNoLista(ListaLinear * no, ListaAdj * listaAdj){
    //Se não há Lista Linear na Lista de Adjacências, insere o nó da LL na raíz.
    if(listaAdj->raizListaLinear == NULL){
        no->anteNo = NULL;
        no->proxNo = NULL;
        listaAdj->raizListaLinear = no;
        return;
    }
    //Obtém o Nó da Lista Linear (LL) atualmente lido.
    ListaLinear * noAtual = listaAdj->raizListaLinear;
    int fimLista = 0;
    while(!fimLista){
        //Obtém o idPoPsConectado do nó da atual da LL.
        int idPoPsConectado = noAtual->idPoPsConectado;
        
        //Se o nó que eu quero inserir deve estar antes do nó atual.
        if(no->idPoPsConectado < idPoPsConectado){
            //O anterior do 'no' recebe o anterior do nó atual.
            no->anteNo = noAtual->anteNo;
            //Se não houver anterior, então esse nó é a raiz.
            if(no->anteNo == NULL)
                listaAdj->raizListaLinear = no;
            //O proximo do 'no' recebe o nó Atual
            no->proxNo = noAtual;

            //Se houver nó anterior, então o próximo nó do nó anterior deve ser 'no'
            if(noAtual->anteNo != NULL)
                noAtual->anteNo->proxNo = no;
            
            //O anterior do 'noAtual' deve ser 'no'.
            noAtual->anteNo = no;
            return;
        }
        if(noAtual->proxNo != NULL)
            noAtual = noAtual->proxNo;
        else
            fimLista = 1;
    }
    noAtual->proxNo = no;
    no->anteNo = noAtual;
    no->proxNo = NULL;

    return;
}

void imprimeListaLinear(ListaLinear * no){
    if(no == NULL)
        return;
    ListaLinear * noAtual = no;
    while(noAtual != NULL){
        printf("%d ", noAtual->idPoPsConectado);
        noAtual = noAtual->proxNo;
    }
    printf("\n");
}