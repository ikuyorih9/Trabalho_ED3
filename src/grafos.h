#ifndef GRAFOS_H
#define GRAFOS_H

typedef struct noListaAdj ListaAdj;
typedef struct noListaLinear ListaLinear;

struct noListaAdj{
    int idConecta;
    char nomePoPs[64];
    char nomePais[64];
    char siglaPais[3];

    ListaLinear * raizListaLinear;
    ListaAdj * proxNo;
    ListaAdj * anteNo;
};

struct noListaLinear{
    int idPoPsConectado;
    int velocidade;
    
    ListaLinear * anteNo;
    ListaLinear * proxNo;
};

ListaAdj * criaListaAdj();
ListaLinear * criaNoListaLinear(int idPoPsConectado, int velocidade);
void adicionaNoLista(ListaLinear * no, ListaAdj * listaAdj);
void imprimeListaLinear(ListaLinear * no);
#endif