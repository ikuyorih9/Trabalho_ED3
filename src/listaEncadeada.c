#include <stdio.h>
#include <stdlib.h>
#include "listaEncadeada.h"

ListaEncadeada *criaLista() {
  ListaEncadeada *lista = malloc(sizeof(ListaEncadeada));
  if (lista == NULL) {
    fprintf(stderr, "Erro ao alocar memoria para a lista encadeada.\n");
    return NULL;
  }
  *lista = NULL;
  return lista;
}

NoLista *criaNoLista(int valor) {
  NoLista *no = malloc(sizeof(NoLista));
  no->dado = valor;
  no->proxNo = NULL;
  return no;
}

void inserirListaEncadeada(int valor, ListaEncadeada *lista) {
  if (lista == NULL) {
    fprintf(stderr, "Nao ha memoria alcoada para a lista encadeada.\n");
    return;
  }

  if (*lista == NULL) {
    NoLista *no = criaNoLista(valor);
    *lista = no;
    return;
  }
  int fimLista = 0;
  NoLista *noAtual = *lista;
  while (!fimLista) {
    if (valor < noAtual->dado) {
      NoLista *no = criaNoLista(valor);
      no->proxNo = noAtual;
      no->anteNo = noAtual->anteNo;
      if (no->anteNo == NULL)
        *lista = no;
      if (noAtual->anteNo != NULL)
        noAtual->anteNo->proxNo = no;
      noAtual->anteNo = no;
      return;
    }
    if (noAtual->proxNo != NULL)
      noAtual = noAtual->proxNo;
    else
      fimLista = 1;
  }
  NoLista *no = criaNoLista(valor);
  noAtual->proxNo = no;
  no->anteNo = noAtual;
  return;
}

void imprimeListaEncadeada(ListaEncadeada *lista) {
  if (lista == NULL) {
    fprintf(stderr, "Nao ha memoria alcoada para a lista encadeada.\n");
    return;
  }
  if (*lista == NULL) {
    fprintf(stderr, "Nao ha lista para imprimir.\n");
    return;
  }

  NoLista *noAtual = *lista;
  while (noAtual != NULL) {
    printf("%d ", noAtual->dado);
    noAtual = noAtual->proxNo;
  }
  printf("\n");
}

void liberaNoListaEncadeada(NoLista * no){
    if(no == NULL)
        return;
    liberaNoListaEncadeada(no->proxNo);
    free(no);
    return;
}

void liberaListaEncadeada(ListaEncadeada * lista){
    if(lista == NULL){
        fprintf(stderr, "Nao ha lista para liberar!\n");
        return;
    }
    liberaNoListaEncadeada(*lista);
    free(lista);
}