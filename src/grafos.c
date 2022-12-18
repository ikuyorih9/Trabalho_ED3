#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafos.h"
#include "arquivos.h"
#include "listaEncadeada.h"


/*********************
    Métodos do Grafo.
**********************/

//Reserva espaço para um lista de adjacências.
Grafo * criaGrafo(){
    Grafo * grafo = malloc(sizeof(Grafo));
    if(grafo != NULL){
        *grafo = NULL;
    }
    return grafo;
}

//Imprime o grafo.
void imprimeGrafo(Grafo * grafo){
    //Se não há grafo alocado, então retorna.
    if(grafo == NULL)
        return;
    
    //Cria um nó de ListaAdj auxiliar, começando da raiz do grafo.
    ListaAdj * noAtualAdj = *grafo;

    //Para cada nó de ListaAdj no grafo.
    while(noAtualAdj != NULL){
        //Cria um nó de ListaLinear e recebe a raiz do nó de ListaAdj atual.
        ListaLinear * noAtualLin = noAtualAdj->raizListaLinear;
        
        //Para cada nó de ListaLinear
        while(noAtualLin != NULL){
            //Imprime o idConecta do atual nó de ListaAdj
            printf("%d ", noAtualAdj->idConecta);
            //Imprime o nomePoPs do atual nó de ListaAdj
            printf("%s ", noAtualAdj->nomePoPs);
            //Imprime o nomePais do atual nó de ListaAdj
            printf("%s ", noAtualAdj->nomePais);
            //Imprime a siglaPais do atual nó de ListaAdj
            printf("%s ", noAtualAdj->siglaPais);
            //Imprime o idPoPsConectado do atual nó de ListaLinear
            printf("%d ", noAtualLin->idPoPsConectado);
            //Imprime a velocidade do atual nó de ListaLinear
            printf("%dMbps\n", noAtualLin->velocidade);

            //Aponta para o próximo nó de ListaLinear
            noAtualLin = noAtualLin->proxNo;
        }

        //Aponta para o próximo nó de ListaAdj.
        noAtualAdj = noAtualAdj->proxNo;
    }
}

//Insere um registro 'registroDados' num grafo 'grafo'.
void insereGrafo(RegDados registroDados, Grafo * grafo){
    //Se não há grafo alocado, então retorna.
    if(grafo == NULL)
        return;
    
    //Se a unidade de medida estiver em Giga, converte para Mega.
    if(registroDados.unidadeMedida[0] == 'G'){
        registroDados.velocidade *= 1024;
        registroDados.unidadeMedida[0] = 'M';
    }

    //Cria uma variável que representa um nó da ListaLinear.
    ListaLinear * noListaLinear = NULL;

    //Cria um nó de ListaAdj a partir dos dados do registro.
    ListaAdj * noListaAdj = criaNoListaAdj(registroDados.idConecta, registroDados.nomePoPs, registroDados.nomePais, registroDados.siglaPais);

    //Se houver velocidade no registro, então cria um nó de ListaLinear.
    if(registroDados.velocidade != -1)
        //Atribui ao noListaLinear um nó criado a partir dos dados do registro.
        noListaLinear = criaNoListaLinear(registroDados.idPoPsConectado, registroDados.velocidade);
    
    //Se não há uma lista de adjacências no grafo.
    if(*grafo == NULL){
        //A raiz de grafo se torna o nó de ListaAdj criado a partir do registro.
        *grafo = noListaAdj;

        //Insere o nó de ListaLinear na raiz do grafo.
        insereNoListaLinear(noListaLinear, *grafo);

        //Se houver lista linear, então se cria um novo vértice a partir do idPoPsConectado. Entretanto, esse vértice é indefinido por não haver informações além do idConecta.
        if(noListaLinear != NULL){
            //Cria um nó de ListaLinear indefinido, alocando memória.
            ListaLinear * noLinearIndefinido = malloc(sizeof(ListaLinear));

            //Faz com que o idPoPsConectado da ListaLinear indefinida seja o idConecta do nó de ListaAdj atual.
            noLinearIndefinido->idPoPsConectado = noListaAdj->idConecta;
            //A velocidade da aresta que liga ambos os vértices são iguais.
            noLinearIndefinido->velocidade = noListaLinear->velocidade;

            //Define um nó sem anterior e posterior.
            noLinearIndefinido->anteNo = NULL;
            noLinearIndefinido->proxNo = NULL;


            //Cria um nó indefinido de ListaAdj, alocando memória.
            ListaAdj * noIndefinido = malloc(sizeof(ListaAdj));

            //Faz com que o idConecta do nó indefinido de ListaAdj seja o idPoPsConectado da ListaLinear recebida pelo registro.
            noIndefinido->idConecta = noListaLinear->idPoPsConectado;

            //Esse novo vértice não está definido.
            noIndefinido->estaDefinido = 0;

            //Sua cor começa como AZUL.
            noIndefinido->cor = AZUL;

            //A raiz do nóIndefinido de ListaAdj é o noLinearIndefinido de ListaLinear.
            noIndefinido->raizListaLinear = noLinearIndefinido;

            //Indica que não há anteriores ou posteriores a esse nó.
            noIndefinido->anteNo = NULL;
            noIndefinido->proxNo = NULL;

            //Insere esse nó na lista de adjacências do grafo.
            insereNoListaAdj(noIndefinido, grafo);
        }
        return;
    }

    //Flag de parada do laço de repetição.
    int fimLista = 0;

    //Nó de ListaAdj que recebe todos os nós da lista de adjacências.
    ListaAdj * noAtual = *grafo;
    
    //Percorre enquanto não for o fim da lista de adjacências.
    while(!fimLista){
        //Recebe o idConecta do nó da lista de adjacências que se deseja inserir.
        int idConecta = noListaAdj->idConecta;

        //Se esse idConecta for menor que o idConecta do nó atual, então deve-se ordená-lo.
        if(idConecta < noAtual->idConecta){
            //O próximo nó do nó que se deseja inserir é o nó atualmente lido ma lista.
            noListaAdj->proxNo = noAtual;
            //Seu anterior é nó anterior ao nó atualmente lido.
            noListaAdj->anteNo = noAtual->anteNo;
            //Se não houver nó anterior, então o nó inserido é a raiz da lista de adjacências.
            if(noListaAdj->anteNo == NULL)
                //A raiz do grafo é o nó inserido.
                *grafo = noListaAdj;
            //Se houver nó anterior do nó atualmente lido.
            if(noAtual->anteNo != NULL)
                //O próximo do nó do nó anterior ao nó atual é o nó inserido.
                noAtual->anteNo->proxNo = noListaAdj;
            //O nó anterior do nó atual é o nó inserido.
            noAtual->anteNo = noListaAdj;
            
            //Insere no nó de ListaLinear criada a partir do registro na lista linear do nó da lista de adjacências.
            insereNoListaLinear(noListaLinear, noListaAdj);

            //Cria um nó de ListaAdj e um nó de ListaLinear indefinidos com as informações da lista linear. Da mesma forma que ocorreu anteriormente.
            if(noListaLinear != NULL){
                ListaLinear * noLinearIndefinido = malloc(sizeof(ListaLinear));

                noLinearIndefinido->idPoPsConectado = noListaAdj->idConecta;
                noLinearIndefinido->velocidade = noListaLinear->velocidade;
                noLinearIndefinido->anteNo = NULL;
                noLinearIndefinido->proxNo = NULL;

                ListaAdj * noIndefinido = malloc(sizeof(ListaAdj));
                noIndefinido->idConecta = noListaLinear->idPoPsConectado;
                noIndefinido->estaDefinido = 0;
                noIndefinido->cor = AZUL;
                noIndefinido->raizListaLinear = noLinearIndefinido;
                noIndefinido->anteNo = NULL;
                noIndefinido->proxNo = NULL;

                insereNoListaAdj(noIndefinido, grafo);
            }

            //Como o nó foi inserido na posição correta, retorna.
            return;
        }
        //Caso ocorra o encontro do vértice a se inserir e o vértice atual.
        else if(idConecta == noAtual->idConecta){
            //Verifica se o no atual está indefinido.
            if(!noAtual->estaDefinido){
                //Se estiver, copia as informações do nó a se inserir pro nó atual, sem criar um novo vértice.
                strcpy(noAtual->nomePoPs, noListaAdj->nomePoPs);
                strcpy(noAtual->nomePais, noListaAdj->nomePais);
                strcpy(noAtual->siglaPais, noListaAdj->siglaPais);
                //Configura o nó como definido, para que não entre de novo aqui.
                noAtual->estaDefinido = 1;
            }
            //Insere o nó da lista linear obtido na lista linear do nó encontrado na busca.
            insereNoListaLinear(noListaLinear, noAtual);

            //Libera o noListaAdj criado anteriormente, pois o nó já existia.
            free(noListaAdj);

            //Cria um nó de ListaAdj e um nó de ListaLinear indefinidos com as informações da lista linear. Da mesma forma que ocorreu anteriormente.
            if(noListaLinear != NULL){
                ListaLinear * noLinearIndefinido = malloc(sizeof(ListaLinear));

                noLinearIndefinido->idPoPsConectado = noAtual->idConecta;
                noLinearIndefinido->velocidade = noListaLinear->velocidade;
                noLinearIndefinido->anteNo = NULL;
                noLinearIndefinido->proxNo = NULL;

                ListaAdj * noIndefinido = malloc(sizeof(ListaAdj));
                noIndefinido->idConecta = noListaLinear->idPoPsConectado;
                noIndefinido->estaDefinido = 0;
                noIndefinido->cor = AZUL;
                noIndefinido->raizListaLinear = noLinearIndefinido;
                noIndefinido->anteNo = NULL;
                noIndefinido->proxNo = NULL;

                insereNoListaAdj(noIndefinido, grafo);
            }
            //Como o nó foi inserido na posição correta, retorna.
            return;
        }
        //Avança pro próximo nó se o próximo nó não for nulo.
        if(noAtual->proxNo != NULL)
            noAtual = noAtual->proxNo;
        //Caso o próximo nó seja nulo, altera a flag para o fim da lista.
        else
            fimLista = 1;
    }

    //Se toda alista foi percorrida e não foi possível encontrar uma posição certa para os nós que desejamos inserir, então ele deve ser inserido no fim da lista.
    //Nesse ponto do código, o noAtual está referenciado como o último nó da lista de adjacências do grafo.

    //O próximo nó do nó atual é o nó a se inserir.
    noAtual->proxNo = noListaAdj;
    //O nó anterior do nó a se inserir é o nó atual.
    noListaAdj->anteNo = noAtual;
    //Como o nó inserido é o último, não há próximo nó.
    noListaAdj->proxNo = NULL;
    //Insere a lista linear criada pelo registro na lista linear do nó.
    insereNoListaLinear(noListaLinear, noListaAdj);

    //Cria um nó de ListaAdj e um nó de ListaLinear indefinidos com as informações da lista linear. Da mesma forma que ocorreu anteriormente.
    ListaLinear * noLinearIndefinido = malloc(sizeof(ListaLinear));
    noLinearIndefinido->idPoPsConectado = noListaAdj->idConecta;
    noLinearIndefinido->velocidade = noListaLinear->velocidade;
    noLinearIndefinido->anteNo = NULL;
    noLinearIndefinido->proxNo = NULL;

    ListaAdj * noIndefinido = malloc(sizeof(ListaAdj));
    noIndefinido->idConecta = noListaLinear->idPoPsConectado;
    noIndefinido->estaDefinido = 0;
    noIndefinido->cor = AZUL;
    noIndefinido->raizListaLinear = noLinearIndefinido;
    noIndefinido->anteNo = NULL;
    noIndefinido->proxNo = NULL;

    insereNoListaAdj(noIndefinido, grafo);

    return;
}

//Libera a memória alocada para o grafo, junto as suas listas.
void liberaGrafo(Grafo * grafo){
    //Se não há grafo, então retorna.
    if(grafo == NULL)
        return;
    //Libera a lista de adjacencias no grafo.
    liberaListaAdj(*grafo);
    //Libera memória do grafo.
    free(grafo);
    return;
}

/************************************
    Métodos da Lista de Adjacências.
*************************************/

//Busca um nó na lista de adjacencias a partir de um valor idConecta.
ListaAdj * buscaNoListaAdj(int idConecta, Grafo * grafo){
    //Verifica a existência do grafo.
    if(grafo == NULL){
        fprintf(stderr, "Nao ha memoria alocada para esse grafo.\n");
        return NULL;
    }
    
    //Verifica a existência da lista de adjacencias.
    if(*grafo == NULL){
        fprintf(stderr, "Lista de adjacencias nao alocada!\n");
        return NULL;
    }

    //Percorre todas os nós da lista de adjacencias.
    ListaAdj * noAtual = *grafo;
    while(noAtual != NULL){
        int vertice = noAtual->idConecta;
        //Ao encontrar um vertice com o mesmo valor do idConecta procurado, retorna-se esse nó.
        if(vertice == idConecta)
            return noAtual;
        else
            noAtual = noAtual->proxNo;
    }
    //Se não for encontrado o valor, retorna-se NULL.
    return NULL;
}

//Cria um nó de uma lista de adjacencias.
ListaAdj * criaNoListaAdj(int idConecta, char * nomePoPs, char * nomePais, char * siglaPais){
    ListaAdj * listaAdj = malloc(sizeof(ListaAdj));
    listaAdj->idConecta = idConecta;
    strcpy(listaAdj->nomePoPs, nomePoPs);
    strcpy(listaAdj->nomePais, nomePais);
    strcpy(listaAdj->siglaPais, siglaPais);
    listaAdj->estaDefinido = 1;
    listaAdj->cor = AZUL;
    listaAdj->raizListaLinear = NULL;
    listaAdj->proxNo = NULL;
    listaAdj->anteNo = NULL;
    return listaAdj;
}

//Função que encontra a quantidade de ciclos que um grafo não direcionado realiza.
void encontraCiclo(ListaAdj * vertice, int * numCiclos, Grafo * grafo){
    //Vértice atual foi visitado pela primeira vez, ele é VERDE.
    vertice->cor = VERDE;

    //Recebe os vértices conectados ao vértice atual.
    ListaLinear * verticeConectado = vertice->raizListaLinear;

    //Para cada elemento da lista linear.
    while(verticeConectado != NULL){
        //Encontra-se o vertice indicado pelo elemento da lista linear atualmente lido.
        ListaAdj * proxVertice = buscaNoListaAdj(verticeConectado->idPoPsConectado, grafo);

        //Se o vertice foi visitado, verifica se é o início do ciclo.
        if(proxVertice->cor != AZUL){
            //Se encontrou um vértice fechado, então um ciclo foi encontrado.
            if(proxVertice->cor == VERMELHO)
                (*numCiclos)++;
            
            //Prossegue o laço, ignorando o restante dele.
            verticeConectado = verticeConectado->proxNo;
            continue;
        }

        //Se o vertice não foi visitado, avança na recursão.
        encontraCiclo(proxVertice, numCiclos, grafo);
        verticeConectado = verticeConectado->proxNo;
    }
    //Se o vertice não tem mais nós na lista linear para visitar, então ele agora é VERMELHO.
    vertice->cor = VERMELHO;

    return;
}

//Insere um nó 'noListaAdj' no grafo.
void insereNoListaAdj(ListaAdj * noListaAdj, Grafo * grafo){
    //Verifica a existência do grafo, da lista e do nó a se inserir.
    if(grafo == NULL || *grafo == NULL || noListaAdj == NULL)
        return;

    int fimLista = 0;
    ListaAdj * noAtual = *grafo;

    //Percorre toda a lista de adjacencias.
    while(!fimLista){
        int idConecta = noListaAdj->idConecta;

        //Ao encontrar uma posição do nó a se inserir, organiza-o na lista.
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

        //Se já houver um nó igual ao que se deseja inserir, então apenas põe sua lista linear.
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

    //Se o nó a se inserir for maior que todos, põe no fim da lista.
    noAtual->proxNo = noListaAdj;
    noListaAdj->anteNo = noAtual;
    noListaAdj->proxNo = NULL;
    return;
}   

//Imprime um nó da lista de adjacências.
void imprimeListaAdj(ListaAdj * listaAdj){
    if(listaAdj == NULL){
        fprintf(stderr, "O no da lista de adjacencias nao existe!\n");
        return;
    }
    ListaAdj * noAtual = listaAdj;
    printf("%d, %s, %s\n", noAtual->idConecta, noAtual->nomePoPs, noAtual->nomePais);
}

//Libera memória de um nó da lista de adjacências.
void liberaListaAdj(ListaAdj * listaAdj){
    if(listaAdj == NULL)
        return;
    //Chama recursivamente para liberar a memória do final pro inicio.
    liberaListaAdj(listaAdj->proxNo);
    //Libera a memmória da lista linear associada a lista de adjacências.
    liberaListaLinear(listaAdj->raizListaLinear);
    //Libera a lista de adjacências.
    free(listaAdj);
    return;
}

//Retorna o número de ciclos que um grafo não direcionado realiza.
int procuraCiclos(ListaAdj * noInicio, Grafo * grafo){
    if(grafo == NULL){
        fprintf(stderr, "Nao ha memoria alocada para esse grafo.\n");
        return -1;
    }

    if(*grafo == NULL){
        fprintf(stderr, "A lista de adjacencias esta vazia.\n");
        return -1;
    }
    
    int numCiclos = 0;
    encontraCiclo(noInicio, &numCiclos, grafo);
    return numCiclos;
}

//Configura uma lista com os menores caminhos a cada vértice de um grafo e outra lista com os maiores fluxos de cada vertice do grafo, ambas partindo de um vertice de início.
void retornaListaMenorCaminho(ListaAdj * verticeInicio, ListaEncadeada * lista, ListaEncadeada * listaMaiorFluxo, Grafo * grafo){
    if(verticeInicio == NULL){
        fprintf(stderr, "Vertice de partida nao esta alocado.\n");
        return;
    }
    //Cria lista de vértices com o menor caminho já definido.
    ListaEncadeada * verticesDeterminados = criaListaEncadeada();

    //Inicia o algoritmo com o menor caminho pro vértice de início.
    inserirFimListaEncadeada(verticeInicio->idConecta, verticesDeterminados);
    mudaValorListaEncadeada(verticeInicio->idConecta, 0, lista);

    //Se houver a lista do maior fluxo, inicia-se o maior fluxo do primeiro vértice como 0.
    if(listaMaiorFluxo != NULL)
        mudaValorListaEncadeada(verticeInicio->idConecta, 0, listaMaiorFluxo);

    ListaAdj * verticeAtual = verticeInicio;

    int fim = 0;
    while(!fim){
        //Verifica o custo para os vértices adjacentes.
        ListaLinear * verticeConectado = verticeAtual->raizListaLinear;
        while(verticeConectado != NULL){
            int peso = verticeConectado->velocidade;

            //Obtem o custo de ir ao nó adjacente.
            int i = verticeAtual->idConecta;
            int custo = buscaListaEncadeada(i, lista) + peso;

            //Obtem o menor custo atual para ir ao no adjacente.
            int j = verticeConectado->idPoPsConectado;
            int menorCustoAtual = buscaListaEncadeada(j, lista);

            //Não volta a vertices já definidos.
            if(estaNaListaEncadeada(j, lista)){
               verticeConectado = verticeConectado->proxNo;
               continue; 
            }

            //Verifica se a lista de maior fluxo está disponível.
            if(listaMaiorFluxo != NULL){
                //Fluxo de dados que leva para outro vertice.
                int novoFluxo = peso;
                //Menor fluxo já salvo.
                int fluxoSalvo = buscaListaEncadeada(i, listaMaiorFluxo);

                //Verifica se o mantém o menor fluxo já salvo ou se mantém o novo fluxo.
                if(fluxoSalvo == 0 || novoFluxo < fluxoSalvo){
                    mudaValorListaEncadeada(j, novoFluxo, listaMaiorFluxo);
                }
                else{
                    mudaValorListaEncadeada(j, fluxoSalvo, listaMaiorFluxo);
                }
            }

            //Verifica se o novo custo calculado é menor que o menor custo conhecido.
            if(menorCustoAtual == -1 || custo < menorCustoAtual){
                mudaValorListaEncadeada(j, custo, lista);
            }
            verticeConectado = verticeConectado->proxNo;
        }
        //Procura o vértice com menor custo.
        int indiceMenorValor = -1;
        int menorValor = -1;

        NoLista * noListaEncadeada = lista->listaEncadeada;
        int primeiraIteracao = 1;
        //Para cada vértice da lista.
        while(noListaEncadeada != NULL){
            //Vertice lido na iteração atual.
            int i = noListaEncadeada->indice;

            //Verifica se o indice atual é um vértice já definido com o menor caminho.
            if(estaNaListaEncadeada(i, verticesDeterminados)){
                noListaEncadeada = noListaEncadeada->proxNo;
                continue;
            }
            //Se o vertice ainda não foi inicializado (-1).
            if(noListaEncadeada->dado == -1){
                noListaEncadeada = noListaEncadeada->proxNo;
                continue;
            }
            //Observa o menor valor da lista até a atual iteração.
            if(primeiraIteracao || noListaEncadeada->dado < menorValor){
                menorValor = noListaEncadeada->dado;
                indiceMenorValor = i;
            }
            noListaEncadeada = noListaEncadeada->proxNo;
            primeiraIteracao = 0;
        } 

        //Se não for possivel encontrar um vertice que já não seja mínimo.
        if(indiceMenorValor == -1){
            fim = 1;
            continue;
        }
        //Insere o indice de menor valor na lista de vertices já determinados.
        inserirFimListaEncadeada(indiceMenorValor, verticesDeterminados);

        //Encontra o vértice do menor índice.
        verticeAtual = buscaNoListaAdj(indiceMenorValor, grafo);
    }
    liberaListaEncadeada(verticesDeterminados); 
} 

int retornaMenorVelocidade(int origem, int destino, Grafo * grafo){
    if(grafo == NULL){
        fprintf(stderr, "Nao ha memoria alocada para esse grafo.\n");
        return -1;
    }

    if(*grafo == NULL){
        fprintf(stderr, "A lista de adjacencias esta vazia.\n");
        return -1;
    }

    //Cria uma lista encadeada dos menores caminhos a cada vértice, inicializando eles como -1.
    ListaEncadeada * listaMenorCaminho = criaListaEncadeada();
    ListaAdj * verticeAtual = *grafo;
    while(verticeAtual != NULL){
        inserirFimListaEncadeada(-1, listaMenorCaminho);
        verticeAtual = verticeAtual->proxNo;
    }

    //Obtém o vértice de inicio, dado pelo idConecta 'origem'.
    ListaAdj * verticeInicio = buscaNoListaAdj(origem, grafo);

    //Procura o menor caminho através do algoritmo de dijkstra.
    retornaListaMenorCaminho(verticeInicio, listaMenorCaminho, NULL, grafo);

    //Busca o valor do menor caminho do destino na lista de menores caminhos.
    int menorCaminho = buscaListaEncadeada(destino, listaMenorCaminho);

    //Libera memória da lista encadeada de menor caminho.
    liberaListaEncadeada(listaMenorCaminho);
    
    return menorCaminho;
}

int procuraMaiorFluxo(int origem, int destino, Grafo * grafo){
    if(grafo == NULL){
        fprintf(stderr, "Nao ha memoria alocada para esse grafo.\n");
        return -1;
    }

    if(*grafo == NULL){
        fprintf(stderr, "A lista de adjacencias esta vazia.\n");
        return -1;
    }

    //Cria lista dos menores caminhos de um vértice inicial a todos os outros.
    ListaEncadeada * listaMenorCaminho = criaListaEncadeada();

    //Cria lista dos maiores fluxos de um vértice inicial a todos os outros.
    ListaEncadeada * listaMaiorFluxo = criaListaEncadeada();
    
    //Inicializa-os com -1.
    ListaAdj * verticeAtual = *grafo;
    while(verticeAtual != NULL){
        inserirFimListaEncadeada(-1, listaMenorCaminho);
        inserirFimListaEncadeada(-1, listaMaiorFluxo);
        verticeAtual = verticeAtual->proxNo;
    }

    //Obtém o vértice de início, dado pelo idConecta 'origem'.
    ListaAdj * verticeInicio = buscaNoListaAdj(origem, grafo);

    //Configura as listas através do algoritmo de dijkstra.
    retornaListaMenorCaminho(verticeInicio, listaMenorCaminho, listaMaiorFluxo, grafo);

    //Procura o maior fluxo do ponto de origem na lista de maiores fluxos.
    int maiorFluxo = buscaListaEncadeada(destino, listaMaiorFluxo);

    //Libera memória das duas listas encadeadas criadas.
    liberaListaEncadeada(listaMenorCaminho);
    liberaListaEncadeada(listaMaiorFluxo);
    
    return maiorFluxo;
}

/*******************************
    Métodos da Lista Linear.
********************************/

//Cria um nó de uma lista linear, inicializando com seu 'idPoPsConectado' e sua 'velocidade'.
ListaLinear * criaNoListaLinear(int idPoPsConectado, int velocidade){
    ListaLinear * listaLinear = malloc(sizeof(ListaLinear));
    listaLinear->idPoPsConectado = idPoPsConectado;
    listaLinear->velocidade = velocidade;
    listaLinear->anteNo = NULL;
    listaLinear->proxNo = NULL;
    
    return listaLinear;
}

//Insere um nó na lista linear.
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

//Imprime um nó de uma lista linear.
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

//Libera uma lista linear inteira.
void liberaListaLinear(ListaLinear * listaLinear){
    if(listaLinear == NULL)
        return;
    liberaListaLinear(listaLinear->proxNo);
    free(listaLinear);
    return;
}