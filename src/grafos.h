#ifndef GRAFOS_H
#define GRAFOS_H

#include "arquivos.h"
#include "listaEncadeada.h"

typedef struct noListaAdj ListaAdj;
typedef struct noListaLinear ListaLinear;

struct noListaAdj{
    int idConecta;
    char nomePoPs[64];
    char nomePais[64];
    char siglaPais[3];

    int estaDefinido;
    int visitado;

    ListaLinear * raizListaLinear;
    ListaAdj * proxNo;
    ListaAdj * anteNo;

    ListaEncadeada * AcessadoPor;
};

struct noListaLinear{
    int idPoPsConectado;
    int velocidade;
    
    ListaLinear * anteNo;
    ListaLinear * proxNo;
};

typedef ListaAdj* Grafo;

void imprimeGrafo(Grafo * grafo);

Grafo * criaGrafo();
ListaLinear * criaNoListaLinear(int idPoPsConectado, int velocidade);
ListaAdj * criaNoListaAdj(int idConecta, char * nomePoPs, char * nomePais, char * siglaPais);
void imprimeListaAdj(ListaAdj * listaAdj);
void insereNoListaAdj(ListaAdj * noListaAdj, Grafo * grafo);
void insereGrafo(RegDados registroDados, Grafo * grafo);
ListaAdj * buscaNoListaAdj(int idConecta, Grafo * grafo);


void insereNoListaLinear(ListaLinear * no, ListaAdj * listaAdj);
void imprimeListaLinear(ListaLinear * no);
#endif