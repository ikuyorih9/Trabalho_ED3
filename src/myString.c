#include <stdio.h>
#include <stdlib.h>

//RETORNA CAMPO DE OFFSET 'numCampo' DE UMA LINHA CSV.
char * retornaCampoLinha(char * linha, int numCampo){
    char * campo = malloc(32*sizeof(char));
    int contCampo = 0;
    int j = 0;

    for(int i = 0; linha[i] != '\0' && linha[i] != '\n'; i++){
        if(linha[i] != ','){
            campo[j] = linha[i];
            j++;
        }
        else{
            if(contCampo == numCampo){
                if(j == 0)
                    return NULL;
                else
                    campo[j] = '\0';
                break;
            }
            else
                j = 0;

            contCampo++;
        }    
    }
    return campo;
}

int recebeEntradaInteiro(char * linha, int numCampo){
    char * campo = retornaCampo(linha, numCampo);
    if(campo == NULL)
        return -1;
    int valorInteiro = atoi(campo);
    free(campo);
    return valorInteiro;
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

char * separaCampoLinha(char * linha, int numCampo){
    char * campo = malloc(sizeof(char*)*128);
    int j = 0;
    int lendoCampo = 0;
    int qtdCampos = 0;
    for(int i = 0; linha[i] != '\0'; i++){
        if(linha[i] == '\"'){
            lendoCampo = !lendoCampo;
            if(qtdCampos == numCampo && !lendoCampo){
                return campo;
            }
            else
                j=0;
        }
        else{
            if(lendoCampo){
                campo[j] = linha[i];
                j++;
            }
        }
    }

    return NULL;
}