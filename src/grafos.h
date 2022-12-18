#ifndef GRAFOS_H
#define GRAFOS_H

#include "arquivos.h"
#include "listaEncadeada.h"

#define AZUL 0
#define VERDE 1
#define VERMELHO 2

typedef struct noListaAdj ListaAdj;
typedef struct noListaLinear ListaLinear;

//Estrutura de um nó da lista de adjacências.
struct noListaAdj{
    //Valor do vértice.
    int idConecta;
    //Nome do ponto conectado do vértice.
    char nomePoPs[64];
    //Nome do país de localização do vértice.
    char nomePais[64];
    //Sigla do país de localização do vértice.
    char siglaPais[3];

    //Flag que indica se o vértice está bem definido, ou seja, com todas suas informações consistentes.
    int estaDefinido;

    //Indica a cor de acessos ao nó.
    //-Azul (0): Não foi acessado.
    //-Verde (1): Foi acessado uma vez.
    //-Vermelho (2): Acessou todos os vértices adjacentes. 
    int cor;

    //Aponta para a raiz da lista linear, contendo todos seus vértices conectados.
    ListaLinear * raizListaLinear;

    //Aponta para o próximo nó na lista de adjacências.
    ListaAdj * proxNo;

    //Aponta para o nó anterior na lista de adjacências.
    ListaAdj * anteNo;
};

struct noListaLinear{
    //Vértice conectado pela aresta.
    int idPoPsConectado;
    //Peso da aresta
    int velocidade;
    
    //Aponta para o nó anterior da lista.
    ListaLinear * anteNo;
    //Aponta para o próximo nó da lista.
    ListaLinear * proxNo;
};

//Define que um tipo Grafo é um ponteiro para uma ListaAdj.
typedef ListaAdj* Grafo;

/*********************
    Métodos do Grafo.
**********************/

Grafo * criaGrafo();
void imprimeGrafo(Grafo * grafo);
void insereGrafo(RegDados registroDados, Grafo * grafo);
void liberaGrafo(Grafo * grafo);

/************************************
    Métodos da Lista de Adjacências.
*************************************/

ListaAdj * buscaNoListaAdj(int idConecta, Grafo * grafo);
ListaAdj * criaNoListaAdj(int idConecta, char * nomePoPs, char * nomePais, char * siglaPais);
void imprimeListaAdj(ListaAdj * listaAdj);
void insereNoListaAdj(ListaAdj * noListaAdj, Grafo * grafo);
void liberaListaAdj(ListaAdj * listaAdj);
int procuraCiclos(ListaAdj * noInicio, Grafo * grafo);
int retornaMenorVelocidade(int origem, int destino, Grafo * grafo);
int procuraMaiorFluxo(int origem, int destino, Grafo * grafo);

/*******************************
    Métodos da Lista Linear.
********************************/

ListaLinear * criaNoListaLinear(int idPoPsConectado, int velocidade);
void insereNoListaLinear(ListaLinear * no, ListaAdj * listaAdj);
void imprimeListaLinear(ListaLinear * no);
void liberaListaLinear(ListaLinear * listaLinear);

#endif