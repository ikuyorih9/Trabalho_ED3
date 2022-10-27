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

//RETORNA O DIRETORIO ONDE UM ARQUIVO SERÁ USADO.
char * retornaDiretorio(const char caminho[32], const char * nomeArquivo){
    char * diretorio = malloc(sizeof(char)*64);
    strcpy(diretorio, caminho);
    strcat(diretorio, nomeArquivo);
    return diretorio;
}

/***********************************
    LEITURA DE REGISTROS E AFINS.
 ***********************************/

//IMPRIME REGISTRO DA POSIÇÃO OFFSET
void imprimeRegistro(int offset, FILE * arquivo){
    RegDados registroDados;
    char * removido = retornaCampoFixoString(offset, 1, arquivo);
    registroDados.removido = *removido;
    if (registroDados.removido == '1') 
        return;
        
    registroDados.idConecta = retornaCampoFixoInteiro(offset + 5, arquivo);
    registroDados.siglaPais = retornaCampoFixoString(offset + 9, 2, arquivo);
    registroDados.idPoPsConectado = retornaCampoFixoInteiro(offset + 11, arquivo);
    registroDados.unidadeMedida = retornaCampoFixoString(offset + 15, 1, arquivo);
    registroDados.velocidade = retornaCampoFixoInteiro(offset + 16, arquivo);
    registroDados.nomePoPs = retornaCampoVariavel(offset + 20, arquivo);
    registroDados.nomePais = retornaCampoVariavel(offset + 20 + strlen(registroDados.nomePoPs) + 1, arquivo);
    
    if (registroDados.idConecta != -1)
        printf("Identificador do ponto: %d\n", registroDados.idConecta);

    if(registroDados.nomePoPs[0] != '\0')
        printf("Nome do ponto: %s\n", registroDados.nomePoPs);
    
    if(registroDados.nomePais[0] != '\0')
        printf("Pais de localizacao: %s\n", registroDados.nomePais);
    
    if(registroDados.siglaPais[0] != '$')
        printf("Sigla do pais: %s\n", registroDados.siglaPais);
    
    if(registroDados.idPoPsConectado != -1)
        printf("Identificador do ponto conectado: %d\n", registroDados.idPoPsConectado);
    
    if(registroDados.velocidade != -1){
        printf("Velocidade de transmissao: %d ", registroDados.velocidade);
        printf("%sbps\n", registroDados.unidadeMedida);
    }
    printf("\n");

    free(removido);
    free(registroDados.siglaPais);
    free(registroDados.unidadeMedida);
    free(registroDados.nomePoPs);
    free(registroDados.nomePais);

}

//RETORNA VALOR DE UM CAMPO INTEIRO DA POSIÇÃO 'posCursor'.
int retornaCampoFixoInteiro(int posCursor, FILE * arquivo){
    int campo;
    fseek(arquivo, posCursor, SEEK_SET);
    fread(&campo, sizeof(int), 1, arquivo);
    return campo;
}

//RETORNA VALOR DE UM CAMPO INTEIRO DA POSIÇÃO 'posCursor'.
char * retornaCampoFixoString(int posCursor, int numBytes,FILE * arquivo){
    char * campo = malloc(sizeof(char)*(numBytes+1));

    fseek(arquivo, posCursor, SEEK_SET);
    int i;
    for(i = 0; i < numBytes; i++){
        fread(&(campo[i]), sizeof(char), 1, arquivo);
    }

    campo[i] = '\0';
    
    return campo;
}

char * retornaCampoVariavel(int offset, FILE * arquivo){
    fseek(arquivo, offset, SEEK_SET);
    char * campo = malloc(sizeof(char)*64);
    char byteLido = 0;
    
    int i = 0;
    int fimCampo = 0;
    while(!fimCampo){
        char byte;
        fread(&byte, sizeof(char), 1, arquivo);
        if(byte != '|')
            campo[i] = byte;
        else{
            campo[i] = '\0';
            fimCampo = !fimCampo;
        }
        i++;
    }

    return campo;
}

//RETORNA O NÚMERO DE PÁGINAS DE DISCO.
int retornaNumPaginasDisco(int numRegistros){
    int numPag;
    if((numRegistros*TAM_REG_DADOS) % TAM_PAG == 0)
        numPag = (numRegistros*TAM_REG_DADOS)/TAM_PAG;
    else
        numPag = ((numRegistros*TAM_REG_DADOS)/TAM_PAG)+1;
    return numPag;
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


/*************************************
    MODIFICAÇÃO DE REGISTROS EM DISCO.
**************************************/

//  RETORNA ENCADEAMENTO DO REGISTRO A REMOVER E EMPILHA TOPO.
int empilharRemovido(int rrnRemovido, RegCab * registroCabecalho){
    //RESGATA O TOPO NO REGISTRO DE CABEÇALHO.
    int topoAntigo = registroCabecalho->topo;

    //ATUALIZA O TOPO COM O RRN 'rrnRemovido' DO NOVO REMOVIDO.
    registroCabecalho->topo = rrnRemovido;

    return topoAntigo;
}

//  PREENCHE LIXO NO REGISTRO DE 'nBytesLidos' até 'tamRegistro'.
void insereLixo(int nBytesLidos, int tamRegistro, FILE * out){
    char lixo = '$';
    for(int a = nBytesLidos; a < tamRegistro; a++)
        fwrite(&lixo, sizeof(char), 1, out);
}

//  MUDA INTEIRO DO CAMPO NO OFFSET 'offset'.
void mudarCampoInteiro(int offset, int campo, FILE * arquivo){
    fseek(arquivo, offset, SEEK_SET);
    fwrite(&campo, sizeof(int), 1, arquivo);
}

//  MUDA STRING DO CAMPO NO OFFSET 'offset'.
void mudarCampoString(int offset, char * campo, int tamCampo, FILE * arquivo){
    fseek(arquivo, offset, SEEK_SET);
    fwrite(campo, sizeof(char), tamCampo, arquivo);

}


/********************************************
    INSERÇÃO E REMOÇÃO DE REGISTROS EM DISCO.
*********************************************/

//CRIA REGISTRO DE CABEÇALHO.
void alocarRegistroCabecalho(RegCab registroCabecalho, FILE * out){

    //ESCREVE OS CAMPOS NO ARQUIVO BINARIO.
    fwrite(&registroCabecalho.status, sizeof(char), 1, out);
    fwrite(&registroCabecalho.topo, sizeof(int), 1, out);
    fwrite(&registroCabecalho.proxRRN, sizeof(int), 1, out);
    fwrite(&registroCabecalho.nRegRem, sizeof(int),1,out);
    fwrite(&registroCabecalho.nPagDisco, sizeof(int),1,out);
    fwrite(&registroCabecalho.qtdCompacta, sizeof(int),1,out);
    insereLixo(TAM_REG_CAB, TAM_PAG, out);          
}

//INSERE UM NOVO REGISTRO DE DADOS.
void insereRegistroDados(int offset, RegDados registroDados, FILE * arquivo){
                            
    //VAI AO OFFSET 'posCursor' DO RRN A SE INSERIR.
    fseek(arquivo, offset, SEEK_SET);

    //ESCREVE OS CAMPOS FIXOS NO ARQUIVO BINARIO.
    fwrite(&registroDados.removido, sizeof(char), 1, arquivo);
    fwrite(&registroDados.encadeamento, sizeof(int), 1, arquivo);
    fwrite(&registroDados.idConecta, sizeof(int), 1, arquivo);
    
    if(registroDados.siglaPais != NULL)
        fwrite(registroDados.siglaPais, sizeof(char), 2, arquivo);
    else
        insereLixo(0, 2, arquivo);

    fwrite(&registroDados.idPoPsConectado, sizeof(int), 1, arquivo);

    if(registroDados.unidadeMedida != NULL)
        fwrite(registroDados.unidadeMedida, sizeof(char), 1, arquivo);
    else
        insereLixo(0, 1, arquivo);
    
    fwrite(&registroDados.velocidade, sizeof(int),1,arquivo);

    //ESCREVE O CAMPO VARIÁVEL 'nomePoPs' NO ARQUIVO BINARIO.
    int tamNomePoPs = 0;
    if(registroDados.nomePoPs != NULL){
        tamNomePoPs = strlen(registroDados.nomePoPs);
        fwrite(registroDados.nomePoPs, sizeof(char),tamNomePoPs, arquivo);
    }
    fwrite(&delimitador, sizeof(char), 1, arquivo);

    //ESCREVE O CAMPO VARIÁVEL 'nomePais' NO ARQUIVO BINARIO.
    int tamNomePais = 0;
    if(registroDados.nomePais != NULL){
        tamNomePais = strlen(registroDados.nomePais);
        fwrite(registroDados.nomePais, sizeof(char),tamNomePais, arquivo);
    }
    fwrite(&delimitador, sizeof(char), 1, arquivo);

    //PREENCHE ESPAÇOS VAZIOS COM LIXO.
    insereLixo(TAM_REG_DADOS_FIX + tamNomePoPs + tamNomePais + 2, TAM_REG_DADOS, arquivo);
}

//REMOVE LOGICAMENTE UM REGISTRO DE DADOS DE RRN 'rrnRegistro'.
void removeRegistroDados(int rrnRegistro, RegCab * registroCabecalho, FILE * arquivo){

    int offset = TAM_PAG + rrnRegistro*TAM_REG_DADOS; //calcula offset do registro.
    char * removido = retornaCampoFixoString(offset, 1, arquivo); //obtem o campo 'removido' do registro de dados.
    
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
