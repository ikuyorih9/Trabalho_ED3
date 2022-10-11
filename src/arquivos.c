#include "arquivos.h"
#include <stdio.h>
#include <stdlib.h>

const char delimitador = '|';

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

int retornaNumPaginasDisco(int numRegistros, FILE * arquivo){
    int numPag;
    if((numRegistros*TAM_REG_DADOS) % TAM_PAG == 0)
        numPag = (numRegistros*TAM_REG_DADOS)/TAM_PAG;
    else
        numPag = ((numRegistros*TAM_REG_DADOS)/TAM_PAG)+1;
    return numPag;
}

int retornaTamanhoCampo(char * campo){
    int tamanho = 0;
    for(int i = 0; campo[i] != '\0' && campo[i] != '\n'; i++){
        tamanho++;
    }
    return tamanho;
}

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

int retornaCampoRegistroInteiro(int posCursor, FILE * arquivo){
    int campo;
    fseek(arquivo, posCursor, SEEK_SET);
    fread(&campo, sizeof(int), 1, arquivo);
    return campo;
}

void insereLixoRegistro(FILE * out, int nBytesLidos, int tamRegistro){
    char lixo = '$';
    for(int a = nBytesLidos; a < tamRegistro; a++)
        fwrite(&lixo, sizeof(char), 1, out);
}

void mudarCampoString(int posCursor, char * campo, int tamCampo, FILE * arquivo){
    fseek(arquivo, posCursor, SEEK_SET);
    fwrite(campo, sizeof(char), tamCampo, arquivo);

}

void mudarCampoInteiro(int posCursor, int campo, FILE * arquivo){
    fseek(arquivo, posCursor, SEEK_SET);
    fwrite(&campo, sizeof(int), 1, arquivo);
}

void alocarRegistroCabecalho(char status,
                             int topo,
                             int proxRRN,
                             int nRegRem,
                             int nPagDisc,
                             int nCompacta,
                             FILE * out){
    
    fwrite(&status, sizeof(char), 1, out);
    fwrite(&topo, sizeof(int), 1, out);
    fwrite(&proxRRN, sizeof(int), 1, out);
    fwrite(&nRegRem, sizeof(int),1,out);
    fwrite(&nPagDisc, sizeof(int),1,out);
    fwrite(&nCompacta, sizeof(int),1,out);
    insereLixoRegistro(out, TAM_REG_CAB, TAM_PAG);          
}

int alocarRegistroDados(int removido,
                        int encadeamento,
                        int idConecta, 
                        char *siglaPais, 
                        float idPoPsConectado, 
                        char * unidade, 
                        float velocidade, 
                        char * nomePoPs, 
                        char * nomePais,
                        FILE * out){
    
    int a;
    fwrite(&removido, sizeof(int), 1, out);
    fwrite(&encadeamento, sizeof(int), 1, out);
    fwrite(&idConecta, sizeof(int), 1, out);
    fwrite(siglaPais, sizeof(char), 2, out);
    fwrite(&idPoPsConectado, sizeof(float), 1, out);
    fwrite(unidade, sizeof(char), 1, out);
    fwrite(&velocidade, sizeof(float),1,out);

    int tamNomePoPs = retornaTamanhoCampo(nomePoPs);
    fwrite(nomePoPs, sizeof(char),tamNomePoPs, out);
    fwrite(&delimitador, sizeof(char), 1, out);

    int tamNomePais = retornaTamanhoCampo(nomePais);
    fwrite(nomePais, sizeof(char),tamNomePais, out);
    fwrite(&delimitador, sizeof(char), 1, out);

    insereLixoRegistro(out, TAM_REG_DADOS_FIX + tamNomePoPs + tamNomePais + 2, TAM_REG_DADOS);

}

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

    fseek(arquivo, posCursor, SEEK_SET);

    fwrite(&removido, sizeof(char), 1, arquivo);
    fwrite(&encadeamento, sizeof(int), 1, arquivo);
    fwrite(&idConecta, sizeof(int), 1, arquivo);
    fwrite(siglaPais, sizeof(char), 2, arquivo);
    fwrite(&idPoPsConectado, sizeof(float), 1, arquivo);
    fwrite(unidade, sizeof(char), 1, arquivo);
    fwrite(&velocidade, sizeof(float),1,arquivo);

    int tamNomePoPs = retornaTamanhoCampo(nomePoPs);
    fwrite(nomePoPs, sizeof(char),tamNomePoPs, arquivo);
    fwrite(&delimitador, sizeof(char), 1, arquivo);

    int tamNomePais = retornaTamanhoCampo(nomePais);
    fwrite(nomePais, sizeof(char),tamNomePais, arquivo);
    fwrite(&delimitador, sizeof(char), 1, arquivo);

    insereLixoRegistro(arquivo, TAM_REG_DADOS_FIX + tamNomePoPs + tamNomePais + 2, TAM_REG_DADOS);
}