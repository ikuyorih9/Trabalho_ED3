#include "arvoreB.h"
#include "arquivos.h"
#include "mensagensErro.h"
#include <stdio.h>
#include <stdlib.h>

/*********************
    MÉTODOS A PARTE.
**********************/

int calculaOffset(int rrn, FILE * arquivo){
    return (rrn + 1) * TAM_PAG_ARV;
}

NucleoNo criaNucleo(int idConecta, int rrnDado){
    NucleoNo nucleoNo;
    nucleoNo.filhoEsq = -1;
    nucleoNo.chave = idConecta;
    nucleoNo.rrnReferencia = rrnDado;
    nucleoNo.filhoDir = -1;

    return nucleoNo;
}

int retornaFilhoRRN(int chave, ArvoreDados no){
    int rrn = -1;
    for(int i = 0; i < no.nChavesNo; i++){
        NucleoNo nucleoNo = no.nucleo[i];
        if(rrn == -1)
            rrn = nucleoNo.filhoEsq;
        if(chave > nucleoNo.chave)
            rrn = nucleoNo.filhoDir;
        else if(chave == nucleoNo.chave)
            return -1;
    }
    return rrn;
}



/*
    MANIPULAÇÃO DO CABEÇALHO DA ÁRVORE;
*/

void alocaCabecalhoArvore(ArvoreCab * cabecalho, FILE * arquivo){
    fseek(arquivo, 0 , SEEK_SET);
    fwrite(&(cabecalho->status), sizeof(char), 1, arquivo);
    fwrite(&(cabecalho->raiz), sizeof(int), 1, arquivo);
    fwrite(&(cabecalho->nChavesTotal), sizeof(int), 1, arquivo);
    fwrite(&(cabecalho->alturaArvore), sizeof(int), 1, arquivo);
    fwrite(&(cabecalho->proxNoRRN), sizeof(int), 1, arquivo);
    insereLixo(TAM_CAB_ARV, TAM_PAG_ARV, arquivo);
}

ArvoreCab criaCabecalho(){
    ArvoreCab cabecalho;
    cabecalho.status = '0';
    cabecalho.raiz = -1;
    cabecalho.nChavesTotal = 0;
    cabecalho.alturaArvore = 0;
    cabecalho.proxNoRRN = 0;
    return cabecalho;
}

ArvoreCab retornaCabecalhoArvore(FILE * arquivo){
    ArvoreCab cabecalho;

    fseek(arquivo, 0, SEEK_SET);
    fread(&(cabecalho.status), sizeof(char), 1, arquivo);
    fread(&(cabecalho.raiz), sizeof(int), 1, arquivo);
    fread(&(cabecalho.nChavesTotal), sizeof(int), 1, arquivo);
    fread(&(cabecalho.alturaArvore), sizeof(int), 1, arquivo);
    fread(&(cabecalho.proxNoRRN), sizeof(int), 1, arquivo);

    return cabecalho;
}

/*********************************
    MANIPULAÇÃO DE NÓS DA ÁRVORE.
**********************************/

int buscaRRNRegistroArvore(int chave, int rrn, FILE * arquivo){
    if(rrn == -1){
        return -1;
    }
    
    ArvoreDados no = retornaNoArquivo(rrn, arquivo);
    for(int i = 0; i < no.nChavesNo; i++){
        //PROCURA  SE A CHAVE ESTÁ NO NÓ ATUAL.
        if(no.nucleo[i].chave == chave)
            //RETORNA O RRN DO REGISTRO REFERENTE A ESSA CHAVE.
            return no.nucleo[i].rrnReferencia;
    }
    //RECEBE O RRN DO NÓ FILHO.
    int rrnFilhoArvore = retornaFilhoRRN(chave, no);
    //BUSCA O REGISTRO NO RRN DO NÓ FILHO.
    return buscaRRNRegistroArvore(chave, rrnFilhoArvore, arquivo);
}

ArvoreDados criaNo(ArvoreCab * cabecalho, FILE * arquivo){
    ArvoreDados no;
    no.folha = '1';
    no.nChavesNo = 0;
    no.alturaNo = 1;
    no.noRRN = cabecalho->proxNoRRN;
    for(int i = 0; i < MAX_CHAVES; i++){
        no.nucleo[i] = criaNucleo(-1, -1);
    }
    
    (cabecalho->proxNoRRN)++;

    return no;
}

void imprimeNosSequencia(FILE * arquivo){
    fseek(arquivo, 0, SEEK_END);
    int qtd = (int)ftell(arquivo);
    qtd = qtd/65;
    qtd--;
    for(int i = 0; i < qtd; i++){
        ArvoreDados no = retornaNoArquivo(i, arquivo);
        imprimeNo(no);
        printf("\n");
        system("pause");
    }
}

void imprimeChavesOrdenado(int rrnNo, FILE * arquivo){
    if(rrnNo == -1)
        return;
    ArvoreDados no = retornaNoArquivo(rrnNo, arquivo);
    //imprimeNo(no);
    for (int i = 0; i < no.nChavesNo; i++) {
        if (i == 0){
            imprimeChavesOrdenado(no.nucleo[i].filhoEsq, arquivo);
        }
        printf("(%d,%d) ", no.alturaNo, no.nucleo[i].chave);
        imprimeChavesOrdenado(no.nucleo[i].filhoDir, arquivo);
    }
}

void imprimeNo(ArvoreDados no){
    printf("FOLHA : %c\n", no.folha);
    printf("nChavesNo: %d\n", no.nChavesNo);
    printf("alturaNo:%d\n", no.alturaNo);
    printf("noRRN: %d\n", no.noRRN);

    for(int i = 0; i < MAX_CHAVES; i++){
        NucleoNo nucleoNo = no.nucleo[i];
        printf("No %d:\n", i);
        printf("\tRRN esquerdo: %d\n", nucleoNo.filhoEsq);
        printf("\tChave: %d\n", nucleoNo.chave);
        printf("\tRRN referencia: %d\n", nucleoNo.rrnReferencia);
        printf("\tRRN direito: %d\n", nucleoNo.filhoDir);
    }
    printf("------------------\n");
}

void insereNoArquivo(ArvoreDados no, FILE * arquivo){
    int offset = calculaOffset(no.noRRN, arquivo);
    //printf("inserindo No no rrn %d\n", no.noRRN);
    //printf("inserindo no offset(rrn) %x(%d)\n", offset, no.noRRN);
    moveCursor(offset, arquivo);

    int sucesso = 1;

    sucesso *= fwrite(&(no.folha), sizeof(char), 1, arquivo);
    sucesso *= fwrite(&(no.nChavesNo), sizeof(int), 1, arquivo);
    sucesso *= fwrite(&(no.alturaNo), sizeof(int), 1, arquivo);
    sucesso *= fwrite(&(no.noRRN), sizeof(int), 1, arquivo);

    for(int i = 0; i < MAX_CHAVES; i++){
        NucleoNo * nucleoNo = &no.nucleo[i];
        if(i == 0)
            sucesso *= fwrite(&(nucleoNo->filhoEsq), sizeof(int), 1, arquivo);
        sucesso *= fwrite(&(nucleoNo->chave), sizeof(int), 1, arquivo);
        sucesso *= fwrite(&(nucleoNo->rrnReferencia), sizeof(int), 1, arquivo);
        sucesso *= fwrite(&(nucleoNo->filhoDir), sizeof(int), 1, arquivo);
    }
    if(!sucesso)
        imprimeErroEscrita(offset);
}

ArvoreDados retornaNoArquivo(int rrn, FILE * arquivo){
    int offset = calculaOffset(rrn, arquivo);
    moveCursor(offset, arquivo);

    ArvoreDados no;

    fread(&(no.folha), sizeof(char), 1, arquivo);
    fread(&(no.nChavesNo), sizeof(int), 1, arquivo);
    fread(&(no.alturaNo), sizeof(int), 1, arquivo);
    fread(&(no.noRRN), sizeof(int), 1, arquivo);

    int filhoDireita = -1;
    for(int i = 0; i < MAX_CHAVES; i++){
        NucleoNo * nucleoNo = &no.nucleo[i];
        if(i == 0)
            fread(&(nucleoNo->filhoEsq), sizeof(int), 1, arquivo);
        else
            nucleoNo->filhoEsq = filhoDireita;
        fread(&(nucleoNo->chave), sizeof(int), 1, arquivo);
        fread(&(nucleoNo->rrnReferencia), sizeof(int), 1, arquivo);
        fread(&(nucleoNo->filhoDir), sizeof(int), 1, arquivo);
        filhoDireita = nucleoNo->filhoDir;
    }

    return no;
}


void insereOrdenadoNo(NucleoNo nucleoNo, ArvoreDados * no){
    int i;
    //PERCORRE TODOS OS NÓS DA ÁRVORE.
    for (i = 0; i < no->nChavesNo; i++) {
        //SE O VALOR DA CHAVE DO NÓ ATUAL FOR MAIOR QUE A CHAVE 'maior'.
        if (no->nucleo[i].chave > nucleoNo.chave) {
            //TROCA O NÚCLEO DA ARVORE PELO NÚCLEO nucleoNo.
            NucleoNo aux;
            aux = no->nucleo[i];
            no->nucleo[i] = nucleoNo;
            nucleoNo = aux;
        }
    }
    no->nucleo[i] = nucleoNo;
    (no->nChavesNo)++;
}

NucleoNo * insereDivideNo(NucleoNo nucleo, ArvoreDados *no, ArvoreCab * cabecalho, FILE * arquivo){
    //printf("Dividindo no...\n");
    int idConecta = nucleo.chave;
    NucleoNo * nucleoPromovido = NULL;
    ArvoreDados novoNo = criaNo(cabecalho, arquivo);
    int i, j;
    int indicePromocao = (ORDEM_ARV/2);
    int salvaFilho;
    for(i = 0, j = i-(indicePromocao + 1); i < MAX_CHAVES; i++, j++){
        //COPIA PARA O NÓ ATUAL.
        if(i <= indicePromocao){
            //TROCA nucleo COM O NUCLEO DO NÓ ORDENADAMENTE.
            if(no->nucleo[i].chave > nucleo.chave){
                NucleoNo aux = no->nucleo[i];
                no->nucleo[i] = nucleo;
                nucleo = aux;
                nucleo.filhoEsq = no->nucleo[i].filhoDir;
            }
            //SE ESTIVER NA POSIÇÃO DE PROMOÇÃO, SALVA O NÚCLEO.
            if(i == indicePromocao){
                nucleoPromovido = malloc(sizeof(NucleoNo));
                *nucleoPromovido = no->nucleo[i];
                nucleoPromovido->filhoEsq = no->noRRN;
                nucleoPromovido->filhoDir = novoNo.noRRN;
                //isso só funciona se o promovido vier de um promovido.
                //na inserção de um nó promovido
                //é preciso que o seu nucleo esquerdo e seu nucleo direito
                //sejam atualizados pro valor do nucleo recebido.
                no->nucleo[i-1].filhoDir = no->nucleo[i].filhoEsq;
                salvaFilho = no->nucleo[i].filhoDir;
                no->nucleo[i] = criaNucleo(-1,-1); 
            }
        }
        //COPIA PARA O NOVO NÓ CRIADO.
        else{
            if(no->nucleo[i].chave > nucleo.chave){
                //COPIA O NUCLEO DE nucleo PARA O novoNo.
                novoNo.nucleo[j] = nucleo;
                nucleo = no->nucleo[i];
            }
            else{
                //COPIA O NUCLEO DE no PARA O novoNo
                novoNo.nucleo[j] = no->nucleo[i];
            }
            if(j == 0)
                novoNo.nucleo[0].filhoEsq = salvaFilho;
            //APAGA NUCLEO DO NÓ ATUAL.
            no->nucleo[i] = criaNucleo(-1, -1);
        }
    }
    novoNo.nucleo[j] = nucleo; //adiciona o ultimo termo.
    novoNo.folha = no->folha;
    novoNo.alturaNo = no->alturaNo;

    no->nChavesNo = indicePromocao;
    novoNo.nChavesNo = MAX_CHAVES - no->nChavesNo;

    insereNoArquivo(*no, arquivo);
    

    // printf("No esquerdo:\n");
    // imprimeNo(*no);
    insereNoArquivo(novoNo, arquivo);
    // printf("No direito: \n");
    // imprimeNo(novoNo);

    // printf("Nucleo promovido: (%d, %d, %d, %d)\n", nucleoPromovido->filhoEsq, nucleoPromovido->chave, nucleoPromovido->rrnReferencia, nucleoPromovido->filhoDir);

    return nucleoPromovido;
}

NucleoNo * insereNucleoNo(NucleoNo * nucleo, ArvoreDados * no, ArvoreCab * cabecalho, FILE * arquivo){
    NucleoNo * nucleoPromovido = NULL;
    //SE HÁ ESPAÇO NO NÓ.
    if(no->nChavesNo < MAX_CHAVES){
       // printf("Existe espaco! Altura do no: %d. Inserindo...\n", no->alturaNo);
        //insere o núcleo no Nó.
        insereOrdenadoNo(*nucleo, no);
        // //atualiza arquivo.
    }
    //SE NÃO HÁ ESPAÇO NO NÓ.
    else{
        //printf("sem espaco no RRN %d!\n", no->noRRN);
        //DIVIDE O NÓ E RETORNA A SUA PROMOÇÃO.
        nucleoPromovido = insereDivideNo(*nucleo, no, cabecalho, arquivo); //aqui volta o núcleo criado que ainda vai ser inserido.
        //printf("Nucleo promovido: (%d, %d, %d, %d)\n", nucleoPromovido->filhoEsq, nucleoPromovido->chave, nucleoPromovido->rrnReferencia, nucleoPromovido->filhoDir);
    }
    free(nucleo);
    return nucleoPromovido;
}

NucleoNo * insereNo(int idConecta, int rrnRef, int rrn, int alturaNo, ArvoreCab * cabecalho, FILE * arquivo){
    NucleoNo * nucleoPromocao = NULL;
    //SE O RRN É -1, CRIA-SE UM NOVO NÓ.
    if(rrn == -1){
        NucleoNo * raiz = malloc(sizeof(NucleoNo));
        *raiz = criaNucleo(idConecta, rrnRef);
        return raiz;
    }

    //POSICIONA LEITOR NA POSIÇÃO DO NÓ.
    int offset = calculaOffset(rrn, arquivo);
    moveCursor(offset, arquivo);

    //ABRE A ÁRVORE DO ARQUIVO.
    ArvoreDados no = retornaNoArquivo(rrn, arquivo);

    //SE É UM NÓ FOLHA.
    if(no.folha == '1'){
        NucleoNo  * novoNucleo = malloc(sizeof(NucleoNo));
        *novoNucleo = criaNucleo(idConecta, rrnRef);
        nucleoPromocao = insereNucleoNo(novoNucleo, &no, cabecalho, arquivo);
    }
    //SE NÃO É UM NÓ FOLHA.
    else{
        //TENTA INSERIR NÓ NO FILHO APROPRIADO.
        int rrnFilho = retornaFilhoRRN(idConecta, no);
        NucleoNo * nucleoPromovido = insereNo(idConecta, rrnRef, rrnFilho, alturaNo-1, cabecalho, arquivo);
        //printf("Voltando da recursao. Altura: %d\n", alturaNo);

        //SE HOUVER UM NÚCLEO PROMOVIDO DA RECURSÃO.
        if(nucleoPromovido != NULL){
            //TENTA INSERIR O NÚCLEO PROMOVIDO NO NÓ ATUAL.
            nucleoPromocao = insereNucleoNo(nucleoPromovido, &no, cabecalho, arquivo);
        }
    }
    no.alturaNo = alturaNo;
    insereNoArquivo(no, arquivo);
    return nucleoPromocao;
}

void insereNoArvore(int idConecta, int rrnRef, ArvoreCab * cabecalho, FILE * arquivo){
    NucleoNo * nucleoPromovido = insereNo(idConecta, rrnRef,cabecalho->raiz, cabecalho->alturaArvore, cabecalho, arquivo);
    //SE HOUVER PROMOÇÃO DE UM NÓ.
    if(nucleoPromovido != NULL){
        //CRIA UM NOVO NÓ.
        ArvoreDados no = criaNo(cabecalho, arquivo);
        //no.alturaNo = cabecalho->alturaArvore;
        //SE NÃO HOUVER UMA RAIZ, ENTÃO O NÓ CRIADO É FOLHA.
        if(cabecalho->raiz == -1)
            no.folha = '1';
        else
            no.folha = '0';

        //ATUALIZA CABEÇALHO.
        cabecalho->raiz = no.noRRN;
        cabecalho->alturaArvore++;
        no.alturaNo = cabecalho->alturaArvore;
        //printf("Nova raiz criada no RRN %d. Altura da arvore agora e %d\n", no.noRRN, cabecalho->alturaArvore);

        //INSERE O NUCLEO PROMOVIDO NO NOVO NÓ CRIADO.
        insereOrdenadoNo(*nucleoPromovido, &no);
        insereNoArquivo(no, arquivo);

        //LIBERA MEMORIA.
        free(nucleoPromovido);
        //printf("\n");
        //system("pause");
    }
    //printf("\n");
    cabecalho->nChavesTotal++;
}


