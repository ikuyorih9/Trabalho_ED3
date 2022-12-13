#include <stdio.h>
#include <stdlib.h>
#include "listaEncadeada.h"

ListaEncadeada *criaListaEncadeada() {
  ListaEncadeada *lista = malloc(sizeof(ListaEncadeada));
  if (lista == NULL) {
    fprintf(stderr, "Erro ao alocar memoria para a lista encadeada.\n");
    return NULL;
  }
  lista->listaEncadeada = NULL;
  lista->tamLista = 0;

  return lista;
}

NoLista *criaNoListaEncadeada(int valor) {
  NoLista *no = malloc(sizeof(NoLista));
  no->dado = valor;
  no->proxNo = NULL;
  no->anteNo = NULL;
  return no;
}

int estaNaListaEncadeada(int valor, ListaEncadeada * lista){
  if (lista == NULL) {
    fprintf(stderr, "Nao ha memoria alocada para a lista encadeada.\n");
    return -1;
  }

  NoLista * noAtual = lista->listaEncadeada;
  while(noAtual != NULL){
    if(noAtual->dado == valor){
      return 1;
    }
    noAtual = noAtual -> proxNo;
  }
  return 0;
}

int buscaListaEncadeada(int indice, ListaEncadeada * lista){
  if (lista == NULL) {
    fprintf(stderr, "Nao ha memoria alocada para a lista encadeada.\n");
    return -1;
  }

  NoLista * noAtual = lista->listaEncadeada;
  while(noAtual != NULL){
    if(noAtual->indice == indice){
      return noAtual->dado;
    }
    noAtual = noAtual -> proxNo;
  }
  return -1;

}

void inserirOrdenadoListaEncadeada(int valor, ListaEncadeada *lista) {
  if (lista == NULL) {
    fprintf(stderr, "Nao ha memoria alcoada para a lista encadeada.\n");
    return;
  }

  if (lista->listaEncadeada == NULL) {
    NoLista *no = criaNoListaEncadeada(valor);
    lista->listaEncadeada = no;
    lista->tamLista++;
    lista->listaEncadeada->indice = lista->tamLista;
    return;
  }

  int fimLista = 0;
  NoLista *noAtual = lista->listaEncadeada;

  while (!fimLista) {
    if (valor < noAtual->dado) {
      NoLista *no = criaNoListaEncadeada(valor);
      no->proxNo = noAtual;
      no->anteNo = noAtual->anteNo;
      if (no->anteNo == NULL)
        lista->listaEncadeada = no;
      if (noAtual->anteNo != NULL)
        noAtual->anteNo->proxNo = no;
      noAtual->anteNo = no;

      lista->tamLista++;
      no->indice = lista->tamLista;
      return;
    }
    if (noAtual->proxNo != NULL)
      noAtual = noAtual->proxNo;
    else
      fimLista = 1;
  }
  NoLista *no = criaNoListaEncadeada(valor);
  noAtual->proxNo = no;
  no->anteNo = noAtual;
  lista->tamLista++;
  no->indice = lista->tamLista;
  return;
}

void mudaValorListaEncadeada(int indice, int valor, ListaEncadeada *lista){
  if (lista == NULL) {
    fprintf(stderr, "Nao ha memoria alocada para a lista encadeada.\n");
    return;
  }
  NoLista * noAtual = lista->listaEncadeada;
  while(noAtual != NULL){
    if(noAtual->indice == indice){
      noAtual->dado = valor;
      return;
    }
    noAtual = noAtual->proxNo;
  }
  printf("Nao ha indice com esse valor.\n");
}

void inserirFimListaEncadeada(int valor, ListaEncadeada *lista) {
  if (lista == NULL) {
    fprintf(stderr, "Nao ha memoria alcoada para a lista encadeada.\n");
    return;
  }

  if (lista->listaEncadeada == NULL) {
    NoLista *no = criaNoListaEncadeada(valor);
    lista->listaEncadeada = no;
    lista->tamLista++;
    lista->listaEncadeada->indice = lista->tamLista;
    return;
  }

  NoLista *noAtual = lista->listaEncadeada;
  while (noAtual->proxNo != NULL) {
    noAtual = noAtual->proxNo;
  }

  NoLista *no = criaNoListaEncadeada(valor);
  noAtual->proxNo = no;
  no->anteNo = noAtual;
  lista->tamLista++;
  no->indice = lista->tamLista;

  return;
}

void imprimeListaEncadeada(ListaEncadeada *lista) {
  if (lista == NULL) {
    fprintf(stderr, "Nao ha memoria alcoada para a lista encadeada.\n");
    return;
  }
  if (lista->listaEncadeada == NULL) {
    fprintf(stderr, "Nao ha lista para imprimir.\n");
    return;
  }
  printf("Sua lista tem tamanho: %d\n", lista->tamLista);
  NoLista *noAtual = lista->listaEncadeada;
  while (noAtual != NULL) {
    printf("%d: %d\n", noAtual->indice, noAtual->dado);
    noAtual = noAtual->proxNo;
  }
  printf("\n");
}

void liberaNoListaEncadeada(NoLista *no) {
  if (no == NULL)
    return;
  liberaNoListaEncadeada(no->proxNo);
  free(no);
  return;
}

void liberaListaEncadeada(ListaEncadeada *lista) {
  if (lista == NULL) {
    fprintf(stderr, "Nao ha lista para liberar!\n");
    return;
  }
  liberaNoListaEncadeada(lista->listaEncadeada);
  free(lista);
}