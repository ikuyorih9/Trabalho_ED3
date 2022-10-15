#include "arquivos.h"
#include "mensagensErro.h"
#include <stdio.h>
#include <stdlib.h>

const char delimitador = '|';
const char simboloRemovido = '1';

void binarioNaTela(const char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}


/***********************************
    RETORNO DE REGISTROS E AFINS.
 ***********************************/

//RETORNA O NÚMERO DE PÁGINAS DE DISCO.
int retornaNumPaginasDisco(int numRegistros, FILE * arquivo){
    int numPag;
    if((numRegistros*TAM_REG_DADOS) % TAM_PAG == 0)
        numPag = (numRegistros*TAM_REG_DADOS)/TAM_PAG;
    else
        numPag = ((numRegistros*TAM_REG_DADOS)/TAM_PAG)+1;
    return numPag;
}

//RETORNA O TAMANHO DO CAMPO 'campo'.
int retornaTamanhoCampo(char * campo){
    int tamanho = 0;
    for(int i = 0; campo[i] != '\0' && campo[i] != '\n'; i++){
        tamanho++;
    }
    return tamanho;
}

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

//RETORNA VALOR DE UM CAMPO INTEIRO DA POSIÇÃO 'posCursor'.
char * retornaCampoRegistroString(int posCursor, int numBytes,FILE * arquivo){
    char * campo = malloc(sizeof(char)*(numBytes+1));

    fseek(arquivo, posCursor, SEEK_SET);
    int i;
    for(i = 0; i < numBytes; i++){
        fread(&(campo[i]), sizeof(char), 1, arquivo);
    }

    campo[i] = '\0';
    
    return campo;
}

//RETORNA VALOR DE UM CAMPO INTEIRO DA POSIÇÃO 'posCursor'.
int retornaCampoRegistroInteiro(int posCursor, FILE * arquivo){
    int campo;
    fseek(arquivo, posCursor, SEEK_SET);
    fread(&campo, sizeof(int), 1, arquivo);
    return campo;
}


/******************************
    MODIFICAÇÃO DE REGISTROS.
*******************************/

//DECREMENTA CAMPO INTEIRO DO OFFSET 'offset'.
void decrementarCampoInteiro(int offset, FILE * arquivo){
    int quantidade;

    //OBTEM VALOR DO CAMPO NO ARQUIVO.
    fseek(arquivo, offset, SEEK_SET);
    fread(&quantidade, sizeof(int), 1, arquivo);

    //DECREMENTA CAMPO.
    quantidade--;

    //ESCREVE CAMPO NO ARQUIVO.
    fseek(arquivo, offset, SEEK_SET);
    fwrite(&quantidade, sizeof(int), 1, arquivo);
}

//  RETORNA ENCADEAMENTO DO REGISTRO A REMOVER E EMPILHA TOPO.
int empilharRemovido(int rrnRemovido, FILE * arquivo){
    int topoAntigo;
    //RESGATA O TOPO NO REGISTRO DE CABEÇALHO.
    fseek(arquivo, 1, SEEK_SET);
    fread(&topoAntigo, sizeof(int), 1, arquivo);
    
    //ATUALIZA O TOPO COM O RRN 'rrnRemovido' DO NOVO REMOVIDO.
    fseek(arquivo, 1, SEEK_SET);
    fwrite(&rrnRemovido, sizeof(int), 1, arquivo);

    //INCRIMENTA O NÚMERO DE REGISTROS REMOVIDOS NO CABEÇALHO.
    incrementaCampoInteiro(9, arquivo);

    return topoAntigo;
}

//  INCREMENTA CAMPO INTEIRO DO OFFSET 'offset'.
void incrementaCampoInteiro(int offset,FILE * arquivo){
    int quantidade;
    
    //OBTEM VALOR DO CAMPO NO ARQUIVO.
    fseek(arquivo, offset, SEEK_SET);
    fread(&quantidade, sizeof(int), 1, arquivo);

    //INCREMENTA CAMPO.
    quantidade++;

    //ESCREVE CAMPO NO ARQUIVO.
    fseek(arquivo, offset, SEEK_SET);
    fwrite(&quantidade, sizeof(int), 1, arquivo);
}

//  PREENCHE LIXO NO REGISTRO DE 'nBytesLidos' até 'tamRegistro'.
void insereLixoRegistro(int nBytesLidos, int tamRegistro, FILE * out){
    char lixo = '$';
    for(int a = nBytesLidos; a < tamRegistro; a++)
        fwrite(&lixo, sizeof(char), 1, out);
}

//  MUDA STRING DO CAMPO NO OFFSET 'posCursor'.
void mudarCampoString(int posCursor, char * campo, int tamCampo, FILE * arquivo){
    fseek(arquivo, posCursor, SEEK_SET);
    fwrite(campo, sizeof(char), tamCampo, arquivo);

}

//  MUDA INTEIRO DO CAMPO NO OFFSET 'posCursor'.
void mudarCampoInteiro(int posCursor, int campo, FILE * arquivo){
    fseek(arquivo, posCursor, SEEK_SET);
    fwrite(&campo, sizeof(int), 1, arquivo);
}


/**************************************
    INSERÇÃO E REMOÇÃO DE REGISTROS.
***************************************/

//CRIA REGISTRO DE CABEÇALHO.
void alocarRegistroCabecalho(char status,
                             int topo,
                             int proxRRN,
                             int nRegRem,
                             int nPagDisc,
                             int nCompacta,
                             FILE * out){

    //ESCREVE OS CAMPOS NO ARQUIVO BINARIO.
    fwrite(&status, sizeof(char), 1, out);
    fwrite(&topo, sizeof(int), 1, out);
    fwrite(&proxRRN, sizeof(int), 1, out);
    fwrite(&nRegRem, sizeof(int),1,out);
    fwrite(&nPagDisc, sizeof(int),1,out);
    fwrite(&nCompacta, sizeof(int),1,out);

    //PREENCHE ESPAÇOS VAZIOS COM LIXO.
    insereLixoRegistro(TAM_REG_CAB, TAM_PAG, out);          
}

//INSERE UM NOVO REGISTRO DE DADOS.
void insereRegistroDados(int posCursor,
                              char removido,
                              int encadeamento,
                              int idConecta, 
                              char *siglaPais, 
                              float idPoPsConectado, 
                              char * unidade, 
                              float velocidade, 
                              char * nomePoPs, 
                              char * nomePais,
                              FILE * arquivo){
                            
    //VAI AO OFFSET 'posCursor' DO RRN A SE INSERIR.
    fseek(arquivo, posCursor, SEEK_SET);

    //ESCREVE OS CAMPOS FIXOS NO ARQUIVO BINARIO.
    fwrite(&removido, sizeof(char), 1, arquivo);
    fwrite(&encadeamento, sizeof(int), 1, arquivo);
    fwrite(&idConecta, sizeof(int), 1, arquivo);
    fwrite(siglaPais, sizeof(char), 2, arquivo);
    fwrite(&idPoPsConectado, sizeof(float), 1, arquivo);
    fwrite(unidade, sizeof(char), 1, arquivo);
    fwrite(&velocidade, sizeof(float),1,arquivo);

    //ESCREVE O CAMPO VARIÁVEL 'nomePoPs' NO ARQUIVO BINARIO.
    int tamNomePoPs = retornaTamanhoCampo(nomePoPs);
    fwrite(nomePoPs, sizeof(char),tamNomePoPs, arquivo);
    fwrite(&delimitador, sizeof(char), 1, arquivo);

    //ESCREVE O CAMPO VARIÁVEL 'nomePais' NO ARQUIVO BINARIO.
    int tamNomePais = retornaTamanhoCampo(nomePais);
    fwrite(nomePais, sizeof(char),tamNomePais, arquivo);
    fwrite(&delimitador, sizeof(char), 1, arquivo);

    //PREENCHE ESPAÇOS VAZIOS COM LIXO.
    insereLixoRegistro(TAM_REG_DADOS_FIX + tamNomePoPs + tamNomePais + 2, TAM_REG_DADOS, arquivo);
}

//REMOVE LOGICAMENTE UM REGISTRO DE DADOS DE RRN 'rrnRegistro'.
void removeRegistroDados(int rrnRegistro, const char * nomeArquivoBinario){
    //ABRE E VERIFICA ARQUIVO.
    FILE * arquivo = fopen(nomeArquivoBinario, "rb+");
    if(arquivo == NULL){
        imprimeErroArquivo();
        return;
    }

    int offset = TAM_PAG + rrnRegistro*TAM_REG_DADOS; //procura registro.
    char * removido = retornaCampoRegistroString(offset, 1, arquivo); //obtem o campo 'removido'.
    
    //VERIFICA SE REGISTRO JÁ ESTÁ REMOVIDO.
    if(removido[0] == '1'){
        registroNaoAlocado(); //mensagem de erro.
        free(removido);
        return;
    }
    else
        free(removido);

    //EMPILHA O TOPO E OBTEM O VALOR DO ENCADEAMENTO A SE COLOCAR NO REGISTRO REMOVIDO.
    int encadeamento = empilharRemovido(rrnRegistro, arquivo);
    fseek(arquivo, offset, SEEK_SET);
    fwrite(&simboloRemovido, sizeof(char), 1, arquivo); //escreve a remoção como '1'.
    fwrite(&encadeamento, sizeof(int), 1, arquivo); //escreve o encadeamento com 'encadeamento'.

    //SOBREPÕE O RESTO DO REGISTRO COM LIXO.
    insereLixoRegistro(5, TAM_REG_DADOS, arquivo);

    //FECHA ARQUIVO.
    fclose(arquivo);
    
}
