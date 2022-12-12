#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafos.h"
#include "arquivos.h"
#include "listaEncadeada.h"


void imprimeGrafo(Grafo * grafo){
    if(grafo == NULL)
        return;
    ListaAdj * noAtualAdj = *grafo;
    while(noAtualAdj != NULL){
        ListaLinear * noAtualLin = noAtualAdj->raizListaLinear;
        // if(noAtualLin == NULL){
        //     noAtualAdj = noAtualAdj->proxNo;
        //     continue;
        // }
        
        while(noAtualLin != NULL){
            printf("%d ", noAtualAdj->idConecta);
            printf("%s ", noAtualAdj->nomePoPs);
            printf("%s ", noAtualAdj->nomePais);
            printf("%s ", noAtualAdj->siglaPais);
            printf("%d ", noAtualLin->idPoPsConectado);
            printf("%dMbps\n", noAtualLin->velocidade);
            noAtualLin = noAtualLin->proxNo;
        }
        noAtualAdj = noAtualAdj->proxNo;
    }
}


/************************************
    Métodos da Lista de Adjacências.
*************************************/

//Reserva espaço para um lista de adjacências.
Grafo * criaGrafo(){
    Grafo * grafo = malloc(sizeof(Grafo));
    if(grafo != NULL){
        *grafo = NULL;
    }
    return grafo;
}

ListaAdj * criaNoListaAdj(int idConecta, char * nomePoPs, char * nomePais, char * siglaPais){
    ListaAdj * listaAdj = malloc(sizeof(ListaAdj));
    listaAdj->idConecta = idConecta;
    strcpy(listaAdj->nomePoPs, nomePoPs);
    strcpy(listaAdj->nomePais, nomePais);
    strcpy(listaAdj->siglaPais, siglaPais);
    listaAdj->estaDefinido = 1;
    listaAdj->visitado = 0;
    listaAdj->raizListaLinear = NULL;
    listaAdj->AcessadoPor = NULL;
    listaAdj->proxNo = NULL;
    listaAdj->anteNo = NULL;
    return listaAdj;
}

void insereGrafo(RegDados registroDados, Grafo * grafo){
    if(grafo == NULL)
        return;
    
    if(registroDados.unidadeMedida[0] == 'G'){
        registroDados.velocidade *= 1024;
        registroDados.unidadeMedida[0] = 'M';
    }
    ListaLinear * noListaLinear = NULL;
    ListaAdj * noListaAdj = criaNoListaAdj(registroDados.idConecta, registroDados.nomePoPs, registroDados.nomePais, registroDados.siglaPais);
    if(registroDados.velocidade != -1)
        noListaLinear = criaNoListaLinear(registroDados.idPoPsConectado, registroDados.velocidade);
    
    if(*grafo == NULL){
        *grafo = noListaAdj;
        insereNoListaLinear(noListaLinear, *grafo);
        if(noListaLinear != NULL){
            ListaLinear * noLinearIndefinido = malloc(sizeof(ListaLinear));

            noLinearIndefinido->idPoPsConectado = noListaAdj->idConecta;
            noLinearIndefinido->velocidade = noListaLinear->velocidade;
            noLinearIndefinido->anteNo = NULL;
            noLinearIndefinido->proxNo = NULL;

            ListaAdj * noIndefinido = malloc(sizeof(ListaAdj));
            noIndefinido->idConecta = noListaLinear->idPoPsConectado;
            noIndefinido->estaDefinido = 0;
            noIndefinido->raizListaLinear = noLinearIndefinido;
            noIndefinido->anteNo = NULL;
            noIndefinido->proxNo = NULL;

            insereNoListaAdj(noIndefinido, grafo);
        }
        return;
    }

    int fimLista = 0;
    ListaAdj * noAtual = *grafo;
    
    while(!fimLista){
        int idConecta = noListaAdj->idConecta;
        if(idConecta < noAtual->idConecta){
            noListaAdj->proxNo = noAtual;
            noListaAdj->anteNo = noAtual->anteNo;
            if(noListaAdj->anteNo == NULL)
                *grafo = noListaAdj;
            if(noAtual->anteNo != NULL)
                noAtual->anteNo->proxNo = noListaAdj;
            noAtual->anteNo = noListaAdj;
            
            insereNoListaLinear(noListaLinear, noListaAdj);
            if(noListaLinear != NULL){
                ListaLinear * noLinearIndefinido = malloc(sizeof(ListaLinear));

                noLinearIndefinido->idPoPsConectado = noListaAdj->idConecta;
                noLinearIndefinido->velocidade = noListaLinear->velocidade;
                noLinearIndefinido->anteNo = NULL;
                noLinearIndefinido->proxNo = NULL;

                ListaAdj * noIndefinido = malloc(sizeof(ListaAdj));
                noIndefinido->idConecta = noListaLinear->idPoPsConectado;
                noIndefinido->estaDefinido = 0;
                noIndefinido->raizListaLinear = noLinearIndefinido;
                noIndefinido->anteNo = NULL;
                noIndefinido->proxNo = NULL;

                insereNoListaAdj(noIndefinido, grafo);
            }
            return;
        }
        else if(idConecta == noAtual->idConecta){
            if(!noAtual->estaDefinido){
                strcpy(noAtual->nomePoPs, noListaAdj->nomePoPs);
                strcpy(noAtual->nomePais, noListaAdj->nomePais);
                strcpy(noAtual->siglaPais, noListaAdj->siglaPais);
                noAtual->estaDefinido = 1;
            }
            insereNoListaLinear(noListaLinear, noAtual);
            free(noListaAdj);

            if(noListaLinear != NULL){
                ListaLinear * noLinearIndefinido = malloc(sizeof(ListaLinear));

                noLinearIndefinido->idPoPsConectado = noAtual->idConecta;
                noLinearIndefinido->velocidade = noListaLinear->velocidade;
                noLinearIndefinido->anteNo = NULL;
                noLinearIndefinido->proxNo = NULL;

                ListaAdj * noIndefinido = malloc(sizeof(ListaAdj));
                noIndefinido->idConecta = noListaLinear->idPoPsConectado;
                noIndefinido->estaDefinido = 0;
                noIndefinido->raizListaLinear = noLinearIndefinido;
                noIndefinido->anteNo = NULL;
                noIndefinido->proxNo = NULL;

                insereNoListaAdj(noIndefinido, grafo);
            }

            return;
        }

        if(noAtual->proxNo != NULL)
            noAtual = noAtual->proxNo;
        else
            fimLista = 1;
    }

    noAtual->proxNo = noListaAdj;
    noListaAdj->anteNo = noAtual;
    noListaAdj->proxNo = NULL;
    insereNoListaLinear(noListaLinear, noListaAdj);

    ListaLinear * noLinearIndefinido = malloc(sizeof(ListaLinear));
    noLinearIndefinido->idPoPsConectado = noListaAdj->idConecta;
    noLinearIndefinido->velocidade = noListaLinear->velocidade;
    noLinearIndefinido->anteNo = NULL;
    noLinearIndefinido->proxNo = NULL;

    ListaAdj * noIndefinido = malloc(sizeof(ListaAdj));
    noIndefinido->idConecta = noListaLinear->idPoPsConectado;
    noIndefinido->estaDefinido = 0;
    noIndefinido->raizListaLinear = noLinearIndefinido;
    noIndefinido->anteNo = NULL;
    noIndefinido->proxNo = NULL;

    insereNoListaAdj(noIndefinido, grafo);

    return;
}

void insereNoListaAdj(ListaAdj * noListaAdj, Grafo * grafo){
    if(grafo == NULL || *grafo == NULL || noListaAdj == NULL)
        return;

    int fimLista = 0;
    ListaAdj * noAtual = *grafo;
    
    while(!fimLista){
        int idConecta = noListaAdj->idConecta;
        if(idConecta < noAtual->idConecta){
            noListaAdj->proxNo = noAtual;
            noListaAdj->anteNo = noAtual->anteNo;
            
            if(noListaAdj->anteNo == NULL)
                *grafo = noListaAdj;
            if(noAtual->anteNo != NULL)
                noAtual->anteNo->proxNo = noListaAdj;
            noAtual->anteNo = noListaAdj;
            return;
        }
        else if(idConecta == noAtual->idConecta){
            insereNoListaLinear(noListaAdj->raizListaLinear, noAtual);
            free(noListaAdj);
            return;
        }

        if(noAtual->proxNo != NULL)
            noAtual = noAtual->proxNo;
        else
            fimLista = 1;
    }

    noAtual->proxNo = noListaAdj;
    noListaAdj->anteNo = noAtual;
    noListaAdj->proxNo = NULL;
    return;
}   

void imprimeListaAdj(ListaAdj * listaAdj){
    if(listaAdj == NULL){
        fprintf(stderr, "O no da lista de adjacencias nao existe!\n");
        return;
    }
    ListaAdj * noAtual = listaAdj;
    printf("%d, %s, %s\n", noAtual->idConecta, noAtual->nomePoPs, noAtual->nomePais);
}

ListaAdj * buscaNoListaAdj(int idConecta, Grafo * grafo){
    if(grafo == NULL){
        fprintf(stderr, "Nao ha memoria alocada para esse grafo.\n");
        return NULL;
    }
    
    if(*grafo == NULL){
        fprintf(stderr, "Lista de adjacencias nao alocada!\n");
        return NULL;
    }

    ListaAdj * noAtual = *grafo;
    while(noAtual != NULL){
        int vertice = noAtual->idConecta;
        if(vertice == idConecta)
            return noAtual;
        else
            noAtual = noAtual->proxNo;
    }
    return NULL;
}

int procuraCiclos(ListaAdj * noInicio, Grafo * grafo){
    if(grafo == NULL){
        fprintf(stderr, "Nao ha memoria alocada para esse grafo.\n");
        return;
    }

    if(*grafo == NULL){
        fprintf(stderr, "A lista de adjacencias esta vazia.\n");
        return;
    }
    
    //Prepara todos os vertices como não visitados para iniciar a busca.
    ListaAdj * verticeAtual = *grafo;
    while(verticeAtual != NULL){
        liberaListaEncadeada(verticeAtual->AcessadoPor);
        verticeAtual->AcessadoPor = NULL;
        verticeAtual->visitado = 0;
    }
    int numCiclos = 0;
    encontraCiclo(noInicio, noInicio->idConecta, &numCiclos, grafo);
    return numCiclos;
}

void encontraCiclo(ListaAdj * vertice, int verticeInicio, int * numCiclos, Grafo * grafo){
    //Vértice atual foi visitado.
    vertice->visitado = 1;
    //Recebe os vértices conectados ao vértice atual.
    ListaLinear * verticeConectado = vertice->raizListaLinear;

    while(verticeConectado != NULL){
        ListaAdj * proxVertice = buscaNoListaAdj(verticeConectado->idPoPsConectado, grafo);
        if(proxVertice == NULL)
            return;
        
        inserirListaEncadeada(vertice->idConecta, proxVertice->AcessadoPor);

        //Se o vertice foi visitado, verifica se é o início do ciclo.
        if(proxVertice->visitado){
            if(proxVertice->idConecta == verticeInicio)
                //Se encontrou o vértice de início, aumenta o número de ciclos.
                *numCiclos++;
            verticeConectado = verticeConectado->proxNo;
            continue;
        }
        
        encontraCiclo(proxVertice, verticeInicio, numCiclos, grafo);
        verticeConectado = verticeConectado->proxNo;
    }
    return;
}


/*******************************
    Métodos da Lista Linear.
********************************/
ListaLinear * criaNoListaLinear(int idPoPsConectado, int velocidade){
    ListaLinear * listaLinear = malloc(sizeof(ListaLinear));
    listaLinear->idPoPsConectado = idPoPsConectado;
    listaLinear->velocidade = velocidade;
    listaLinear->anteNo = NULL;
    listaLinear->proxNo = NULL;
    
    return listaLinear;
}

void insereNoListaLinear(ListaLinear * no, ListaAdj * listaAdj){
    if(no == NULL)
        return;
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
        //printf("Inserindo na lista linear...\n");
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