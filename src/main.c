#include <stdio.h>
#include "createTable.h"

int main(){
    FILE* in = fopen("../in/topologiaRede.csv","r");
    FILE* out = fopen("../out/topologiaRede.bin","wb");
    createTable(in,out);

    fclose(in);
    fclose(out);
}