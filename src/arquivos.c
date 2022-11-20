#include "arquivos.h"
#include "mensagensErro.h"
#include "insertInto.h"
#include "limparBuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


/************************
    BUSCA DE REGISTROS.
*************************/

void buscaApagaRegistros(char * nomeCampo, char * valorCampo, RegCab * registroCabecalho, FILE * arquivo){
    int idCampo = retornaCampoID(nomeCampo);
    if(idCampo == -1)
        return;
    
    //SE O CAMPO FOR 'idConecta' OU 'idPoPsConectado' OU 'velocidade'.
    if(idCampo == 0 || idCampo == 2 || idCampo == 4){
        int valorCampoInteiro = atoi(valorCampo);
        int registrosEncontrados = 0;
        int rrnCampo = -1;

        while(!registrosEncontrados){
            rrnCampo = buscaCampoFixoInteiro(rrnCampo+1, idCampo, valorCampoInteiro, registroCabecalho, arquivo);
            if(rrnCampo != -1)
                removeRegistroDados(rrnCampo, registroCabecalho, arquivo);
            else
                registrosEncontrados = !registrosEncontrados;
        }
    }

    //SE O CAMPO FOR 'siglaPais' OU 'unidadeMedida'.
    else if(idCampo == 1 || idCampo == 3){
        int registrosEncontrados = 0;
        int rrnCampo = -1;

        while(!registrosEncontrados){
            rrnCampo = buscaCampoFixoString(rrnCampo+1, idCampo, valorCampo, registroCabecalho, arquivo);
            if(rrnCampo != -1)
                removeRegistroDados(rrnCampo, registroCabecalho, arquivo);
            else
                registrosEncontrados = !registrosEncontrados;
        }
    }
    //SE O CAMPO FOR 'nomePoPs' OU 'nomePais'.
    else{
        int registrosEncontrados = 0;
        int rrnCampo = -1;

        while(!registrosEncontrados){
            rrnCampo = buscaCampoVariavel(rrnCampo+1, idCampo, valorCampo, registroCabecalho, arquivo);
            if(rrnCampo != -1)
                removeRegistroDados(rrnCampo, registroCabecalho, arquivo);
            else
                registrosEncontrados = !registrosEncontrados;
        }
    }
}

void buscaImprimeRegistros(char * nomeCampo, char * valorCampo, RegCab * registroCabecalho, FILE * arquivo){
    int idCampo = retornaCampoID(nomeCampo);
    if(idCampo == -1)
        return;

    int qtdRegistros = 0;

    //SE O CAMPO FOR 'idConecta' OU 'idPoPsConectado' OU 'velocidade'.
    if(idCampo == 0 || idCampo == 2 || idCampo == 4){
        int valorCampoInteiro = atoi(valorCampo);
        int registrosEncontrados = 0;
        int rrnCampo = -1;

        while(!registrosEncontrados){
            rrnCampo = buscaCampoFixoInteiro(rrnCampo+1, idCampo, valorCampoInteiro, registroCabecalho, arquivo);
            if(rrnCampo != -1){
                RegDados registroDados = retornaRegistroDados(rrnCampo, arquivo);
                imprimeRegistro(registroDados, arquivo);
                liberaRegistroDados(registroDados);
                qtdRegistros++;
            }
            else{
                if(qtdRegistros == 0){
                    registroNaoAlocado();
                    printf("\n");
                }
                registrosEncontrados = !registrosEncontrados;
            }
        }
    }

    //SE O CAMPO FOR 'siglaPais' OU 'unidadeMedida'.
    else if(idCampo == 1 || idCampo == 3){
        int registrosEncontrados = 0;
        int rrnCampo = -1;

        while(!registrosEncontrados){
            rrnCampo = buscaCampoFixoString(rrnCampo+1, idCampo, valorCampo, registroCabecalho, arquivo);
            if(rrnCampo != -1){
                RegDados registroDados = retornaRegistroDados(rrnCampo, arquivo);
                imprimeRegistro(registroDados, arquivo);
                liberaRegistroDados(registroDados);
                qtdRegistros++;
            }
            else{
                if(qtdRegistros == 0){
                    registroNaoAlocado();
                    printf("\n");
                }
                registrosEncontrados = !registrosEncontrados;
            }
        }
    }
    //SE O CAMPO FOR 'nomePoPs' OU 'nomePais'.
    else{
        int registrosEncontrados = 0;
        int rrnCampo = -1;

        while(!registrosEncontrados){
            rrnCampo = buscaCampoVariavel(rrnCampo+1, idCampo, valorCampo, registroCabecalho, arquivo);
            if(rrnCampo != -1){
                RegDados registroDados = retornaRegistroDados(rrnCampo, arquivo);
                imprimeRegistro(registroDados, arquivo);
                liberaRegistroDados(registroDados);
                qtdRegistros++;
            }
            else{
                if(qtdRegistros == 0){
                    registroNaoAlocado();
                    printf("\n");
                }
                registrosEncontrados = !registrosEncontrados;
            }
        }
    }
}

int buscaCampoFixoInteiro(int inicio, int idCampo, int valorCampo, RegCab * registroCabecalho, FILE * arquivo){
    int rrnEncontrado = -1;

    int campoOffset;
    switch(idCampo){
        case 0:
            campoOffset = 5;
            break;
        case 2:
            campoOffset = 11;
            break;
        case 4:
            campoOffset = 16;
            break;
        default:
            printf("CAMPO NÃO É INTEIRO!");
            return -1;
    }

    for(int i = inicio; i <registroCabecalho->proxRRN && rrnEncontrado == -1; i++){
        int offset = TAM_PAG + i*TAM_REG_DADOS;
        char * removido = retornaCampoFixoString(offset, 1,arquivo);
        if(*removido == '1'){
            free(removido);
            continue;
        }
        int campoRegistro = retornaCampoFixoInteiro(offset + campoOffset, arquivo);
        if(campoRegistro == valorCampo)
            rrnEncontrado = i;
    }
    
    return rrnEncontrado;
}

int buscaCampoFixoString(int inicio, int idCampo, char * valorCampo, RegCab * registroCabecalho, FILE * arquivo){
    int rrnEncontrado = -1;
    int campoOffset;
    int numBytes;
    switch(idCampo){
        case 1:
            campoOffset = 9;
            numBytes = 2;
            break;
        case 3:
            campoOffset = 15;
            numBytes = 1;
            break;
        default:
            printf("CAMPO NÃO É STRING!");
            return -1;
    }

    for(int i = inicio; i <registroCabecalho->proxRRN && rrnEncontrado == -1; i++){
        int offset = TAM_PAG + i*TAM_REG_DADOS;
        char * removido = retornaCampoFixoString(offset, 1,arquivo);
        if(*removido == '1'){
            free(removido);
            continue;
        }
        char * campoRegistro = retornaCampoFixoString(offset + campoOffset, numBytes, arquivo);
        if(strcmp(campoRegistro, valorCampo) == 0)
            rrnEncontrado = i;
    }
    return rrnEncontrado;
}

int buscaCampoVariavel(int inicio, int idCampo, char * valorCampo, RegCab * registroCabecalho, FILE * arquivo){
    int rrnEncontrado = -1;
    //PERCORRE TODOS OS REGISTROS DO ARQUIVO.
    for(int i = inicio; i < registroCabecalho->proxRRN && rrnEncontrado == -1; i++){
        int offset = TAM_PAG + i*TAM_REG_DADOS; //calcula o offset do registro.
        char * removido = retornaCampoFixoString(offset, 1,arquivo);
        if(*removido == '1'){
            free(removido);
            continue;
        }
        char * nomePoPs = retornaCampoVariavel(offset + 20, arquivo);
        if(idCampo == 5){
            if(nomePoPs[0] == '\0'){
                free(nomePoPs);
                continue;
            }
            if(strcmp(nomePoPs, valorCampo) == 0){
                rrnEncontrado = i;
            }
        }
        else{
            char * nomePais = retornaCampoVariavel(offset + 20 + strlen(nomePoPs) + 1, arquivo);
            if(strcmp(nomePais, valorCampo) == 0)
                rrnEncontrado = i;
            free(nomePais);
        }
        free(removido);
        free(nomePoPs);
    }
    return rrnEncontrado;
}


/********************************************
    INSERÇÃO E REMOÇÃO DE REGISTROS EM DISCO.
*********************************************/

//CRIA REGISTRO DE CABEÇALHO.
void alocarRegistroCabecalho(RegCab registroCabecalho, FILE * arquivo){
    //APONTA PARA O INÍCIO DO ARQUIVO.
    moveCursor(0, arquivo);

    //ESCREVE OS CAMPOS NO ARQUIVO BINARIO.
    fwrite(&registroCabecalho.status, sizeof(char), 1, arquivo);
    fwrite(&registroCabecalho.topo, sizeof(int), 1, arquivo);
    fwrite(&registroCabecalho.proxRRN, sizeof(int), 1, arquivo);
    fwrite(&registroCabecalho.nRegRem, sizeof(int),1,arquivo);
    fwrite(&registroCabecalho.nPagDisco, sizeof(int),1,arquivo);
    fwrite(&registroCabecalho.qtdCompacta, sizeof(int),1,arquivo);
    insereLixo(TAM_REG_CAB, TAM_PAG, arquivo);          
}

//INSERE UM NOVO REGISTRO DE DADOS.
void insereRegistroDados(int offset, RegDados registroDados, FILE * arquivo){
    const char delimitador = '|';

    //VAI AO OFFSET 'posCursor' DO RRN A SE INSERIR.
    moveCursor(offset, arquivo);

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
    moveCursor(offset, arquivo);
    const char simboloRemovido = '1';
    fwrite(&simboloRemovido, sizeof(char), 1, arquivo); //escreve a remoção como '1'.
    fwrite(&encadeamento, sizeof(int), 1, arquivo); //escreve o encadeamento com 'encadeamento'.

    //SOBREPÕE O RESTO DO REGISTRO COM LIXO.
    insereLixo(5, TAM_REG_DADOS, arquivo);

    //INCREMENTA O NÚMERO DE REGISTROS REMOVIDOS NO CABEÇALHO.
    registroCabecalho->nRegRem++;

}


/***********************************
    LEITURA DE REGISTROS E AFINS.
 ***********************************/

//IMPRIME REGISTRO DA POSIÇÃO OFFSET
void imprimeRegistro(RegDados registroDados, FILE * arquivo){
    if (registroDados.removido == '1') 
        return;
    
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

}

//LIBERA AS VARIÁVEIS ALOCADAS DINAMICAMENTE DO REGISTRO DE DADOS.
void liberaRegistroDados(RegDados registroDados){
    if(registroDados.siglaPais != NULL)
        free(registroDados.siglaPais);
    if(registroDados.unidadeMedida != NULL)
        free(registroDados.unidadeMedida);
    if(registroDados.nomePoPs != NULL)
        free(registroDados.nomePoPs);
    if(registroDados.nomePais != NULL)
        free(registroDados.nomePais);
}

//MOVE O CURSOR PARA A POSIÇÃO DESEJADA E EVITA SEEKS DESNECESSÁRIOS.
void moveCursor(int offset, FILE * arquivo){
    int posCursor = ftell(arquivo);
    if(posCursor != offset)
        fseek(arquivo, offset, SEEK_SET);
}

//RETORNA VALOR DE UM CAMPO INTEIRO DA POSIÇÃO 'posCursor'.
int retornaCampoFixoInteiro(int offset, FILE * arquivo){
    int campo;
    moveCursor(offset, arquivo);
    fread(&campo, sizeof(int), 1, arquivo);
    return campo;
}

//RETORNA VALOR DE UM CAMPO INTEIRO DA POSIÇÃO 'posCursor'.
char * retornaCampoFixoString(int offset, int numBytes,FILE * arquivo){
    char * campo = malloc(sizeof(char)*(numBytes+1));

    moveCursor(offset, arquivo);
    int i;
    for(i = 0; i < numBytes; i++){
        fread(&(campo[i]), sizeof(char), 1, arquivo);
    }

    campo[i] = '\0';
    
    return campo;
}

char * retornaCampoVariavel(int offset, FILE * arquivo){
    moveCursor(offset, arquivo);
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

//RETORNA ID DO CAMPO.
int retornaCampoID(char * nomeCampo){    
     if(strcmp(nomeCampo, "idConecta") == 0)
        return 0;
    else if(strcmp(nomeCampo, "siglaPais") == 0)
        return 1;
    else if(strcmp(nomeCampo, "idPoPsConectado") == 0)
        return 2;
    else if(strcmp(nomeCampo, "unidadeMedida") == 0)
        return 3;
    else if(strcmp(nomeCampo, "velocidade") == 0)
        return 4;
    else if(strcmp(nomeCampo, "nomePoPs") == 0)
        return 5;
    else if(strcmp(nomeCampo, "nomePais") == 0)
        return 6;
    return -1;
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
    moveCursor(0, arquivo);
    fread(&(registroCabecalho.status), sizeof(char), 1, arquivo);
    fread(&(registroCabecalho.topo), sizeof(int), 1, arquivo);
    fread(&(registroCabecalho.proxRRN), sizeof(int), 1, arquivo);
    fread(&(registroCabecalho.nRegRem), sizeof(int), 1, arquivo);
    fread(&(registroCabecalho.nPagDisco), sizeof(int), 1, arquivo);
    fread(&(registroCabecalho.qtdCompacta), sizeof(int), 1, arquivo);

    return registroCabecalho;
}

RegDados retornaRegistroDados(int rrn, FILE * arquivo){
    int offset = TAM_PAG + rrn*TAM_REG_DADOS;
    moveCursor(offset, arquivo);

    RegDados registroDados;
    fread(&registroDados.removido, sizeof(char), 1, arquivo);

    if(registroDados.removido == '1'){
        registroDados.encadeamento = -1;
        registroDados.idConecta = -1;
        registroDados.siglaPais = NULL;
        registroDados.idPoPsConectado = -1;
        registroDados.unidadeMedida = NULL;
        registroDados.velocidade = -1;
        registroDados.nomePoPs = NULL;
        registroDados.nomePais = NULL;
        
        return registroDados;
    }
    
    registroDados.encadeamento = retornaCampoFixoInteiro(offset + 1, arquivo);
    registroDados.idConecta = retornaCampoFixoInteiro(offset + 5, arquivo);
    registroDados.siglaPais = retornaCampoFixoString(offset + 9, 2, arquivo);
    registroDados.idPoPsConectado = retornaCampoFixoInteiro(offset + 11, arquivo);
    registroDados.unidadeMedida = retornaCampoFixoString(offset + 15, 1, arquivo);
    registroDados.velocidade = retornaCampoFixoInteiro(offset + 16, arquivo);
    registroDados.nomePoPs = retornaCampoVariavel(offset + 20, arquivo);
    registroDados.nomePais = retornaCampoVariavel(offset + 20 + strlen(registroDados.nomePoPs) + 1, arquivo);

    return registroDados;
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
void insereLixo(int nBytesLidos, int tamRegistro, FILE * arquivo){
    char lixo = '$';
    for(int a = nBytesLidos; a < tamRegistro; a++)
        fwrite(&lixo, sizeof(char), 1, arquivo);
}

//  MUDA INTEIRO DO CAMPO NO OFFSET 'offset'.
void mudarCampoInteiro(int offset, int campo, FILE * arquivo){
    moveCursor(offset, arquivo);
    fwrite(&campo, sizeof(int), 1, arquivo);
}

//  MUDA STRING DO CAMPO NO OFFSET 'offset'.
void mudarCampoString(int offset, char * campo, int tamCampo, FILE * arquivo){
    moveCursor(offset, arquivo);
    fwrite(campo, sizeof(char), tamCampo, arquivo);

}
