#include <stdio.h>

//ERRO AO ABRIR O ARQUIVO PARA LEITURA/ESCRITA.
void imprimeErroArquivo(){
    printf("Falha no processamento do arquivo.\n");
}

//ERRO AO IMPRIMIR REGISTROS INEXISTENTES.
void registroNaoAlocado(){
    printf("Registro inexistente.\n");
}