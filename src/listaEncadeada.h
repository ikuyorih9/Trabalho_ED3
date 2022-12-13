#ifndef LISTA_ENCADEADA_H
#define LISTA_ENCADEADA_H

typedef struct no {
  int dado;
  int indice;

  struct no *proxNo;
  struct no *anteNo;
} NoLista;

typedef struct {
  NoLista *listaEncadeada;
  int tamLista;
} ListaEncadeada;


ListaEncadeada *criaListaEncadeada();
NoLista *criaNoListaEncadeada(int valor);
void mudaValorListaEncadeada(int indice, int valor, ListaEncadeada *lista);
void inserirListaEncadeada(int valor, ListaEncadeada *lista);
void inserirFimListaEncadeada(int valor, ListaEncadeada *lista) ;
void imprimeListaEncadeada(ListaEncadeada *lista);
void liberaListaEncadeada(ListaEncadeada * lista);
#endif