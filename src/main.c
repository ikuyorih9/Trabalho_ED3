#include <stdio.h>
#include "arquivos.h"
#include "createTable.h"
#include "insertInto.h"

int main(){
    //FILE* in = fopen("../in/topologiaRede.csv","r");
    //FILE* out = fopen("../out/topologiaRede.bin","wb");

    const char nomeEntrada[32] = "../in/topologiaRede.csv";
    const char nomeSaida[32] = "../out/topologiaRede.bin";

    createTable(nomeEntrada, nomeSaida);
    //insertInto(nomeSaida);
<<<<<<< HEAD
=======
    removeRegistroDados(0, nomeSaida);
    removeRegistroDados(1, nomeSaida);

>>>>>>> hugo
}