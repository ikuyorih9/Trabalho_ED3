#include "arvoreB.h"
#include "arquivos.h"
#include "mensagensErro.h"
#include <stdio.h>
#include <stdlib.h>

/*********************
    MÉTODOS A PARTE.
**********************/

//Calcula o offset do nó da árvore a partir do seu RRN.
int calculaOffset(int rrn, FILE * arquivo){
    return (rrn + 1) * TAM_PAG_ARV;
}

//Cria um núcleo da árvore B. O núcleo é composto apenas pelas informações principais do nó (filho esquerdo, chave, RRN do registro, filho direito).
NucleoNo criaNucleo(int idConecta, int rrnRegistro){
    NucleoNo nucleoNo;
    nucleoNo.filhoEsq = -1;
    nucleoNo.chave = idConecta;
    nucleoNo.rrnReferencia = rrnRegistro;
    nucleoNo.filhoDir = -1;

    return nucleoNo;
}


/***************************************
    MANIPULAÇÃO DO CABEÇALHO DA ÁRVORE;
****************************************/

//Põe o cabeçalho da árvore em disco.
void alocaCabecalhoArvore(ArvoreCab * cabecalho, FILE * arquivo){
    //Posiciona o leitor no início do arquivo.
    fseek(arquivo, 0 , SEEK_SET);

    //Bateria de inscrições de cada campo da struct no disco.
    fwrite(&(cabecalho->status), sizeof(char), 1, arquivo);
    fwrite(&(cabecalho->raiz), sizeof(int), 1, arquivo);
    fwrite(&(cabecalho->nChavesTotal), sizeof(int), 1, arquivo);
    fwrite(&(cabecalho->alturaArvore), sizeof(int), 1, arquivo);
    fwrite(&(cabecalho->proxNoRRN), sizeof(int), 1, arquivo);

    //Insere o lixo até preencher uma página de disco de 65 Bytes.
    insereLixo(TAM_CAB_ARV, TAM_PAG_ARV, arquivo);
}

//Cria e retorna um cabeçalho vazio em RAM.
ArvoreCab criaCabecalho(){
    ArvoreCab cabecalho;
    cabecalho.status = '0';
    cabecalho.raiz = -1;
    cabecalho.nChavesTotal = 0;
    cabecalho.alturaArvore = 0;
    cabecalho.proxNoRRN = 0;
    return cabecalho;
}

//Retorna o cabeçalho da árvore do arquivo de índices.
ArvoreCab retornaCabecalhoArvore(FILE * arquivo){
    ArvoreCab cabecalho;
    //Posiciona o leitor no início do arquivo.
    fseek(arquivo, 0, SEEK_SET);

    //Bateria de leituras de cada campo do disco na struct.
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

//Busca o RRN de registro do nó que contenha a 'chave', começando do nó de RRN 'rrn'.
int buscaRRNRegistroArvore(int chave, int rrn, int * numPags, FILE * arquivo){
    //Se o RRN 'rrn' recebido for -1, não há mais busca e retorna erro.
    if(rrn == -1){
        return -1;
    }
    
    ArvoreDados no = retornaNoArquivo(rrn, arquivo);
    (*numPags)++; //Ocorre o acesso a outra página de disco.
    for(int i = 0; i < no.nChavesNo; i++){
        //Verifica se a chave está nó atual.
        if(no.nucleo[i].chave == chave)
            //Retorna RRN do registro que contenha essa chave.
            return no.nucleo[i].rrnReferencia;
    }
    //Recebe RRN do nó filho apropriado, segundo a chave.
    int rrnFilhoArvore = retornaFilhoRRN(chave, no);

    //Busca recursivamente.
    return buscaRRNRegistroArvore(chave, rrnFilhoArvore, numPags, arquivo);
}

//Cria e retorna um nó folha vazio em RAM.
ArvoreDados criaNo(ArvoreCab * cabecalho, FILE * arquivo){
    ArvoreDados no;
    no.folha = '1';
    no.nChavesNo = 0;
    no.alturaNo = 1;
    no.noRRN = cabecalho->proxNoRRN;

    for(int i = 0; i < MAX_CHAVES; i++)
        no.nucleo[i] = criaNucleo(-1, -1);
    
    (cabecalho->proxNoRRN)++;

    return no;
}

//Realiza a divisão do nó ao tentar inserir um núcleo 'nucleo' num nó 'no' cheio. Retorna o núcleo promovido da divisão.
NucleoNo * insereDivideNo(NucleoNo nucleo, ArvoreDados *no, ArvoreCab * cabecalho, FILE * arquivo){
    NucleoNo * nucleoPromovido = NULL;

    //Cria um novo nó para dividir o nó atual.
    ArvoreDados novoNo = criaNo(cabecalho, arquivo);
    int i, j;
    int indicePromocao = (ORDEM_ARV/2);
    int salvaFilho;

    //Percorre todos os núcleos do nó atual.
    //A variável j começa da posição i-(indicePromocao+1), para alinhar a leitura do nó original e do novo nó.
    for(i = 0, j = i-(indicePromocao + 1); i < MAX_CHAVES; i++, j++){

        //Se o núcleo atual está antes do (ou no) núcleo de promoção.
        if(i <= indicePromocao){
            //Se a chave do núcleo atual é maior que a chave do núcleo a se inserir, deve-se por o núcleo a se inserir no nó atual. 
            if(no->nucleo[i].chave > nucleo.chave){
                NucleoNo aux = no->nucleo[i];
                no->nucleo[i] = nucleo; //Põe o núcleo a se inserir no núcleo atualmente lido.
                nucleo = aux;           //Põe o antigo núcleo do nó atual no núcleo a se inserir.
                nucleo.filhoEsq = no->nucleo[i].filhoDir;   //O filho esquerdo do núcleo a se inserir se torna o filho direito do núcleo atual.
            }

            //Se o núcleo estiver no índice de promoção, então o núcleo atual deve ser promovido e retirado do nó atual.
            if(i == indicePromocao){
                nucleoPromovido = malloc(sizeof(NucleoNo)); //Aloca memória pro núcleo promovido.
                *nucleoPromovido = no->nucleo[i];           //O núcleo promovido é o núcleo atual.
                nucleoPromovido->filhoEsq = no->noRRN;      //O filho esquerdo do núcleo promovido é o rrn do nó atual.
                nucleoPromovido->filhoDir = novoNo.noRRN;   //O filho direito do núcleo promovido é o rrn do novo nó.

                no->nucleo[i-1].filhoDir = no->nucleo[i].filhoEsq;  //O filho direito do nucleo anterior é o filho esquerdo do nucleo atual.
                salvaFilho = no->nucleo[i].filhoDir;                //Salva-se o filho direito do nucleo atual.
                no->nucleo[i] = criaNucleo(-1,-1);                  //Apaga o núcleo atual (ele foi promovido).
            }
        }

        //Se o núcleo atual está depois do núcleo de promoção.
        else{
            //Se a chave do núcleo atual é maior que a chave do núcleo a se inserir, então o núcleo a se inserir deve ser inserido no novo nó.
            if(no->nucleo[i].chave > nucleo.chave){
                novoNo.nucleo[j] = nucleo;  //Copia o núcleo a se inserir pro núcleo do novo nó.
                nucleo = no->nucleo[i];     //o núcleo a se inserir agora é o núcleo do nó atual.
            }

            //Se a chave do núcleo atual é menor que a chave do núcleo a se inserir, então o núcleo do nó atual deve ser inserido ao novo nó.
            else
                novoNo.nucleo[j] = no->nucleo[i]; //Põe o núcleo atual no núcleo do novo nó.

            //Se é a posição do primeiro núcleo do novo nó, é necessário conectá-lo ao ultimo núcleo do nó atual.
            if(j == 0)
                novoNo.nucleo[0].filhoEsq = salvaFilho; //O filho direito salvo na promoção é o filho esquerdo do primeiro núcleo do novo nó.
            
            //Apaga o núcleo atual.
            no->nucleo[i] = criaNucleo(-1, -1);
        }
    }

    novoNo.nucleo[j] = nucleo; //Põe o núcleo a se inserir no novo nó.
    novoNo.folha = no->folha;  //O novo nó é folha se o nó atual é folha.
    novoNo.alturaNo = no->alturaNo; //A altura do novo nó é a mesma do nó atual.

    no->nChavesNo = indicePromocao; //O número de chaves no novo nó é o valor do índice de promoção.
    novoNo.nChavesNo = MAX_CHAVES - no->nChavesNo;  //Atualiza o número de chaves do nó.

    insereNoArquivo(*no, arquivo); //Atualiza o nó atual no arquivo de índice.
    insereNoArquivo(novoNo, arquivo); //Insere o novo nó no arquivo de índice.
    
    return nucleoPromovido; //Retorna o núcleo promovido.
}

//Insere um núcleo ordenadamente no nó.
void insereOrdenadoNo(NucleoNo nucleoNo, ArvoreDados * no){
    if(no->nChavesNo == MAX_CHAVES){
        printf("ERRO: Voce esta tentando inserir num nó cheio. Voce nao esta fazendo algo errado?");
        return;
    }
    int i;
    //Percorre todos os nós da árvore.
    for (i = 0; i < no->nChavesNo; i++) {
        //Se a chave do nó atual for maior que o nó do núcleo a inserir.
        if (no->nucleo[i].chave > nucleoNo.chave) {
            //Insere o núcleo externo na posição do núcleo do nó atual.
            NucleoNo aux;
            aux = no->nucleo[i];
            no->nucleo[i] = nucleoNo;
            //Salva o núcleo do nó atual no núcleo externo.
            nucleoNo = aux;
        }
    }
    //No fim, o núcleo externo salvo é posto no último núcleo do nó.
    no->nucleo[i] = nucleoNo;
    
    //Aumenta o número de chaves no nó.
    (no->nChavesNo)++;
}

//Insere um nó na arvore.
NucleoNo * insereNo(int idConecta, int rrnRef, int rrn, int alturaNo, ArvoreCab * cabecalho, FILE * arquivo){
    NucleoNo * nucleoPromocao = NULL;
    //Se o RRN é -1, então não existe uma árvore. Cria-se uma raiz.
    if(rrn == -1){
        NucleoNo * raiz = malloc(sizeof(NucleoNo)); //Aloca-se memória para a raíz.
        *raiz = criaNucleo(idConecta, rrnRef);      //A raiz é um núcleo folha com chave IdConecta.
        return raiz;                                //Retorna a raiz.
    }

    //Posiciona a ponta de leitura.
    int offset = calculaOffset(rrn, arquivo);
    moveCursor(offset, arquivo);

    //Obtem o nó de RRN 'rrn' da árvore.
    ArvoreDados no = retornaNoArquivo(rrn, arquivo);

    //Se for um nó folha, então deve-se tentar inserir um núcleo nele.
    if(no.folha == '1'){
        NucleoNo  * novoNucleo = malloc(sizeof(NucleoNo));  //Aloca memória pra um núcleo novo.
        *novoNucleo = criaNucleo(idConecta, rrnRef);        //Cria um núcleo com chave idConecta.
        nucleoPromocao = insereNucleoNo(novoNucleo, &no, cabecalho, arquivo);   //Insere o núcleo no nó e retorna o núcleo de promoção, caso ocorra.
    }

    //Se não for um nó folha, deve-se inserir no filho apropriado.
    else{
        int rrnFilho = retornaFilhoRRN(idConecta, no);  //retorna o RRN do filho onde a chave se encaixa.
        NucleoNo * nucleoPromovido = insereNo(idConecta, rrnRef, rrnFilho, alturaNo-1, cabecalho, arquivo); //Insere recursivamente e retorna um núcleo de promoção, se ocorrer.

        //Caso haja um núcleo de promoção vindo da inserção no filho, tenta inserir no nó atual.
        if(nucleoPromovido != NULL){
            nucleoPromocao = insereNucleoNo(nucleoPromovido, &no, cabecalho, arquivo); //Tenta inserir o núcleo de promoção no nó atual.
                                                                                        //Caso o nó esteja cheio, então retorna uma nova promoçaõ.
        }
    }

    no.alturaNo = alturaNo; //Atualiza a altura do nó.
    insereNoArquivo(no, arquivo);   //Atualiza o nó no arquivo.
    return nucleoPromocao;
}

//Insere o nó no arquivo de índice em disco.
void insereNoArquivo(ArvoreDados no, FILE * arquivo){
    //Calcula o offset do nó.
    int offset = calculaOffset(no.noRRN, arquivo);

    //Posiciona a ponta de leitura.
    moveCursor(offset, arquivo);

    int sucesso = 1;

    //Escreve as informações de controle do nó em disco.
    sucesso *= fwrite(&(no.folha), sizeof(char), 1, arquivo);
    sucesso *= fwrite(&(no.nChavesNo), sizeof(int), 1, arquivo);
    sucesso *= fwrite(&(no.alturaNo), sizeof(int), 1, arquivo);
    sucesso *= fwrite(&(no.noRRN), sizeof(int), 1, arquivo);

    //Percorre todas as chaves do nó.
    for(int i = 0; i < MAX_CHAVES; i++){
        NucleoNo *nucleoNo = &no.nucleo[i];
        //Se é a primeira chave, escreve o filho esquerdo no arquivo. Isso serve para evitar sobrescrita dos filhos.
        if(i == 0)
            sucesso *= fwrite(&(nucleoNo->filhoEsq), sizeof(int), 1, arquivo);
        
        //Escreve a chave, rrn do registro e o filho direito no arquivo.
        sucesso *= fwrite(&(nucleoNo->chave), sizeof(int), 1, arquivo);
        sucesso *= fwrite(&(nucleoNo->rrnReferencia), sizeof(int), 1, arquivo);
        sucesso *= fwrite(&(nucleoNo->filhoDir), sizeof(int), 1, arquivo);
    }
    //Se a escrita falhar, imprime erro de escrita.
    if(!sucesso)
        imprimeErroEscrita(offset);
}

//Insere um nó na árvore a partir de sua raiz. Ela lida com o problema de criar uma nova raiz caso necessário.
void insereNoArvore(int idConecta, int rrnRef, ArvoreCab * cabecalho, FILE * arquivo){
    //Tenta inserir o nó na árvore.
    NucleoNo * nucleoPromovido = insereNo(idConecta, rrnRef,cabecalho->raiz, cabecalho->alturaArvore, cabecalho, arquivo);
    //Se houver a promoção de um núcleo, então deve-se criar uma nova raiz.
    if(nucleoPromovido != NULL){
        ArvoreDados no = criaNo(cabecalho, arquivo); //Cria um novo nó para ser a raiz.

        //Se não houver uma raiz, então a nova raiz criada é um nó folha (porque é a primeira raiz).
        if(cabecalho->raiz == -1)
            no.folha = '1';
        else
            no.folha = '0';

        cabecalho->raiz = no.noRRN; //O RRN da raiz é o RRN da nova raiz criada.
        cabecalho->alturaArvore++;  //A altura da árvore aumenta em um nível.
        no.alturaNo = cabecalho->alturaArvore;  //A altura da raiz é a altura da árvore.

        insereOrdenadoNo(*nucleoPromovido, &no);    //Insere o núcleo promovido no novo nó criado.
        insereNoArquivo(no, arquivo);               //Atualiza arquivo de índice.

        free(nucleoPromovido); //Libera a memória do núcleo promovido.
    }
    cabecalho->nChavesTotal++; //O número de chaves, no total, aumenta em uma unidade a cada inserção.
}

//Insere um núcleo em um nó, verificando se é possível ou se deve realizar a divisão.
NucleoNo * insereNucleoNo(NucleoNo * nucleo, ArvoreDados * no, ArvoreCab * cabecalho, FILE * arquivo){
    NucleoNo * nucleoPromovido = NULL;
    //Se houver espaço no nó, então o insere ordenadamente.
    if(no->nChavesNo < MAX_CHAVES)
        insereOrdenadoNo(*nucleo, no); //Insere o núcleo ordenadamente no nó.

    //Se não houver espaço no nó, é preciso dividí-lo
    else
        nucleoPromovido = insereDivideNo(*nucleo, no, cabecalho, arquivo); //Divide e retorna o núcleo promovido.

    free(nucleo);
    return nucleoPromovido;
}

//Imprime todas as chaves da árvore ordenadamente.
void imprimeChavesOrdenado(int rrnNo, FILE * arquivo){
    //Se rrnNo = -1, é o fim da recursão.
    if(rrnNo == -1)
        return;

    //Recupera o nó no RRN 'rrnNo' do arquivo de índice.
    ArvoreDados no = retornaNoArquivo(rrnNo, arquivo);

    //Percorre todas as chaves do nó.
    for (int i = 0; i < no.nChavesNo; i++) {
        //Se é a primeira chave, chama a impressão pro filho esquerdo.
        if (i == 0){
            imprimeChavesOrdenado(no.nucleo[i].filhoEsq, arquivo);
        }
        //Imprime a chave.
        printf("%d ", no.nucleo[i].chave);

        //Chama a impressão pro filho direito.
        imprimeChavesOrdenado(no.nucleo[i].filhoDir, arquivo);
    }
}

//Imprime um nó completo.
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

//Retorna o RRN do filho em que uma chave deve ser posta, dado um nó da árvore.. 
int retornaFilhoRRN(int chave, ArvoreDados no){
    //Variável do RRN a ser retornado pela função.
    int rrn = -1;
    //Percorre todos os núcleos de um nó.
    for(int i = 0; i < no.nChavesNo; i++){
        //Recupera um núcleo do nó.
        NucleoNo nucleoNo = no.nucleo[i];
        //Se não há RRN escolhido.
        if(rrn == -1)
            rrn = nucleoNo.filhoEsq;    //O RRN recebe o filho esquerdo do nó.

        //Se a chave for maior que a chave do núcleo atual.
        if(chave > nucleoNo.chave)
            rrn = nucleoNo.filhoDir;    //O RRN recebe o filho direito do nó.

        //Se a chave for igual à chave do núcleo, então há repetição. Retorna erro.
        else if(chave == nucleoNo.chave)
            return -1;
    }

    return rrn;
}

//Retorna um nó, de RRN 'rrn', lido de um arquivo de índice.
ArvoreDados retornaNoArquivo(int rrn, FILE * arquivo){
    //Calcula o offset do nó.
    int offset = calculaOffset(rrn, arquivo);

    //Move a ponta de leitura para a posição do offset.
    //OBS: essa função verifica se é preciso fazer fseek.
    moveCursor(offset, arquivo);

    ArvoreDados no;

    //Lê as variáveis de controle do nó do arquivo.
    fread(&(no.folha), sizeof(char), 1, arquivo);
    fread(&(no.nChavesNo), sizeof(int), 1, arquivo);
    fread(&(no.alturaNo), sizeof(int), 1, arquivo);
    fread(&(no.noRRN), sizeof(int), 1, arquivo);

    int filhoDireita = -1;
    //Percorre todas as chaves do nó.
    for(int i = 0; i < MAX_CHAVES; i++){
        NucleoNo * nucleoNo = &no.nucleo[i];

        //Se é o primeiro núcleo a ser lido, lê do arquivo. Isso evita leituras desnecessárias.
        if(i == 0)
            fread(&(nucleoNo->filhoEsq), sizeof(int), 1, arquivo);

        //Se não é o primeiro núcleo a ser lido, então o seu filho esquerdo é o filho direito do núcleo anterior.
        else
            nucleoNo->filhoEsq = filhoDireita;
        //Lê a chave, rrn de registro e filho direito do nó em disco.
        fread(&(nucleoNo->chave), sizeof(int), 1, arquivo);
        fread(&(nucleoNo->rrnReferencia), sizeof(int), 1, arquivo);
        fread(&(nucleoNo->filhoDir), sizeof(int), 1, arquivo);

        //Salva seu filho direito para por no filho esquerdo do próximo núcleo.
        filhoDireita = nucleoNo->filhoDir;
    }

    return no;
}



