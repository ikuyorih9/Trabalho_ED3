#include "arquivos.h"
#include "mensagensErro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

//RETORNA NUMERO DE REGISTROS DE DADOS.
int retornaNumRegistrosDados(FILE * arquivo){
    int offset = 960;
    int regLidos = 0;
    int byteLido;
    char c;
    do{
        fseek(arquivo, offset, SEEK_SET);
        byteLido = fread(&c, sizeof(char), 1, arquivo);
        if(byteLido)
            regLidos++;
        offset += 64;
    }
    while(byteLido);

    return regLidos;
}

//RETORNA O NÚMERO DE PÁGINAS DE DISCO.
int retornaNumPaginasDisco(int numRegistros, FILE * arquivo){
    int numPag;
    if((numRegistros*TAM_REG_DADOS) % TAM_PAG == 0)
        numPag = (numRegistros*TAM_REG_DADOS)/TAM_PAG;
    else
        numPag = ((numRegistros*TAM_REG_DADOS)/TAM_PAG)+1;
    return numPag;
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

//RETORNA O REGISTRO DE CABEÇALHO NUMA STRUCT RegCab.
RegCab retornaRegistroCabecalho(FILE * arquivo){
    RegCab registroCabecalho;
    fseek(arquivo, 0, SEEK_SET);
    fread(&(registroCabecalho.status), sizeof(char), 1, arquivo);
    fread(&(registroCabecalho.topo), sizeof(int), 1, arquivo);
    fread(&(registroCabecalho.proxRRN), sizeof(int), 1, arquivo);
    fread(&(registroCabecalho.nRegRem), sizeof(int), 1, arquivo);
    fread(&(registroCabecalho.nPagDisco), sizeof(int), 1, arquivo);
    fread(&(registroCabecalho.qtdCompacta), sizeof(int), 1, arquivo);

    return registroCabecalho;
}

void imprimirRegistroDados(int offset, FILE * arquivo){
    char * removido = retornaCampoRegistroString(offset, 1, arquivo);
    int encadeamento = retornaCampoRegistroInteiro(offset+1, arquivo);
    int idConecta = retornaCampoRegistroInteiro(offset+5, arquivo);
    char * sigla = retornaCampoRegistroString(offset+9, 2, arquivo);
    int idPoPsConectado = retornaCampoRegistroInteiro(offset+11, arquivo);
    char * unidade = retornaCampoRegistroString(offset+15, 1, arquivo);
    int velocidade = retornaCampoRegistroInteiro(offset+16, arquivo);

    printf("REMOVIDO: %c\n", removido[0]);
    printf("ENCADEAMENTO: %d\n", encadeamento);
    printf("IDCONECTA: %d\n", idConecta);
    printf("SIGLA: %s\n", sigla);
    printf("IDPOPSCONECTADO: %d\n", idPoPsConectado);
    printf("UNIDADE: %c\n", unidade[0]);
    printf("VELOCIDADE: %d\n", velocidade);

    free(removido);
    free(sigla);
    free(unidade);
    
}

/******************************
    MODIFICAÇÃO DE REGISTROS.
*******************************/

void VerificaPaginaDisco(RegCab * registroCabecalho ,FILE * arquivo){
    int offset = registroCabecalho->proxRRN*TAM_REG_DADOS + TAM_PAG;
    if((registroCabecalho->proxRRN*TAM_REG_DADOS) % TAM_PAG == 0)
        registroCabecalho->nPagDisco = registroCabecalho->proxRRN*TAM_REG_DADOS/TAM_PAG;
    else{
        registroCabecalho->nPagDisco = (registroCabecalho->proxRRN*TAM_REG_DADOS/TAM_PAG)+1;
        fseek(arquivo, offset, SEEK_SET);
        insereLixo(0, 960, arquivo);
    }

    
}

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
int empilharRemovido(int rrnRemovido, RegCab * registroCabecalho){
    //RESGATA O TOPO NO REGISTRO DE CABEÇALHO.
    int topoAntigo = registroCabecalho->topo;

    //ATUALIZA O TOPO COM O RRN 'rrnRemovido' DO NOVO REMOVIDO.
    registroCabecalho->topo = rrnRemovido;

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
void insereLixo(int nBytesLidos, int tamRegistro, FILE * out){
    char lixo = '$';
    for(int a = nBytesLidos; a < tamRegistro; a++)
        fwrite(&lixo, sizeof(char), 1, out);
}

//  MUDA STRING DO CAMPO NO OFFSET 'posCursor'.
void mudarCampoString(int offset, char * campo, int tamCampo, FILE * arquivo){
    fseek(arquivo, offset, SEEK_SET);
    fwrite(campo, sizeof(char), tamCampo, arquivo);

}

//  MUDA INTEIRO DO CAMPO NO OFFSET 'posCursor'.
void mudarCampoInteiro(int offset, int campo, FILE * arquivo){
    fseek(arquivo, offset, SEEK_SET);
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
    insereLixo(TAM_REG_CAB, TAM_PAG, out);          
}

//INSERE UM NOVO REGISTRO DE DADOS.
void insereRegistroDados(int posCursor,
                              char removido,
                              int encadeamento,
                              int idConecta, 
                              char *siglaPais, 
                              int idPoPsConectado, 
                              char * unidade, 
                              int velocidade, 
                              char * nomePoPs, 
                              char * nomePais,
                              FILE * arquivo){
                            
    //VAI AO OFFSET 'posCursor' DO RRN A SE INSERIR.
    fseek(arquivo, posCursor, SEEK_SET);

    //ESCREVE OS CAMPOS FIXOS NO ARQUIVO BINARIO.
    fwrite(&removido, sizeof(char), 1, arquivo);
    fwrite(&encadeamento, sizeof(int), 1, arquivo);
    fwrite(&idConecta, sizeof(int), 1, arquivo);
    
    if(siglaPais != NULL)
        fwrite(siglaPais, sizeof(char), 2, arquivo);
    else
        insereLixo(0, 2, arquivo);

    fwrite(&idPoPsConectado, sizeof(int), 1, arquivo);

    if(unidade != NULL)
        fwrite(unidade, sizeof(char), 1, arquivo);
    else
        insereLixo(0, 1, arquivo);
    
    fwrite(&velocidade, sizeof(int),1,arquivo);

    //ESCREVE O CAMPO VARIÁVEL 'nomePoPs' NO ARQUIVO BINARIO.
    int tamNomePoPs = 0;
    if(nomePoPs != NULL){
        tamNomePoPs = strlen(nomePoPs);
        fwrite(nomePoPs, sizeof(char),tamNomePoPs, arquivo);
    }
    fwrite(&delimitador, sizeof(char), 1, arquivo);

    //ESCREVE O CAMPO VARIÁVEL 'nomePais' NO ARQUIVO BINARIO.
    int tamNomePais = 0;
    if(nomePais != NULL){
        tamNomePais = strlen(nomePais);
        fwrite(nomePais, sizeof(char),tamNomePais, arquivo);
    }
    fwrite(&delimitador, sizeof(char), 1, arquivo);

    //PREENCHE ESPAÇOS VAZIOS COM LIXO.
    insereLixo(TAM_REG_DADOS_FIX + tamNomePoPs + tamNomePais + 2, TAM_REG_DADOS, arquivo);
}

//REMOVE LOGICAMENTE UM REGISTRO DE DADOS DE RRN 'rrnRegistro'.
void removeRegistroDados(int rrnRegistro, RegCab * registroCabecalho, FILE * arquivo){

    int offset = TAM_PAG + rrnRegistro*TAM_REG_DADOS; //calcula offset do registro.
    char * removido = retornaCampoRegistroString(offset, 1, arquivo); //obtem o campo 'removido' do registro de dados.
    
    //VERIFICA SE REGISTRO JÁ ESTÁ REMOVIDO.
    if(removido[0] == '1'){
        registroNaoAlocado(); //mensagem de erro.
        free(removido);
        return;
    }
    else
        free(removido);

    //EMPILHA O TOPO E OBTEM O VALOR DO ENCADEAMENTO A SE COLOCAR NO REGISTRO REMOVIDO.
    int encadeamento = empilharRemovido(rrnRegistro, registroCabecalho);
    fseek(arquivo, offset, SEEK_SET);
    fwrite(&simboloRemovido, sizeof(char), 1, arquivo); //escreve a remoção como '1'.
    fwrite(&encadeamento, sizeof(int), 1, arquivo); //escreve o encadeamento com 'encadeamento'.

    //SOBREPÕE O RESTO DO REGISTRO COM LIXO.
    insereLixo(5, TAM_REG_DADOS, arquivo);

    //INCREMENTA O NÚMERO DE REGISTROS REMOVIDOS NO CABEÇALHO.
    registroCabecalho->nRegRem++;

}
