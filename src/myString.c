#include <stdio.h>
#include <stdlib.h>

int comparaStrings(char * str1, char * str2){
    for(int i = 0; str1[i] != '\0' || str2[i] != '\0'; i++){
        if(str1[i] != str2[i])
            return 0;
    }
    return 1;
}

void retiraEnterLinha(char * linha){
    for(int i = 0; linha[i] != '\0'; i++){
        if(linha[i] == '\n')
            linha[i] = '\0';
    }
}

//RETORNA O TAMANHO DA LINHA 'linha'.
int retornaTamanhoLinha(char * linha){
    int tamanho = 0;
    for(int i = 0; linha[i] != '\0' && linha[i] != '\n'; i++){
        tamanho++;
    }
    return tamanho;
}