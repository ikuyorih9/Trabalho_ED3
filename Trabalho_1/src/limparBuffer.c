#include <stdio.h>

void limparBuffer(){
    int buffer;
    do{
        buffer = fgetc(stdin);
    }
    while(buffer != EOF && buffer != '\n'); 
}