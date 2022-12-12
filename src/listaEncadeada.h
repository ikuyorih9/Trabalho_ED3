#ifndef LISTA_ENCADEADA_H
#define LISTA_ENCADEADA_H

typedef struct no {
  int dado;
  struct no *proxNo;
  struct no *anteNo;
} NoLista;

typedef NoLista *ListaEncadeada;

ListaEncadeada *criaLista();
NoLista *criaNoLista(int valor);
void inserirListaEncadeada(int valor, ListaEncadeada *lista);
void imprimeListaEncadeada(ListaEncadeada *lista);
void liberaListaEncadeada(ListaEncadeada * lista);
#endif