# **TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS III**

Desenvolvido por:
* Hugo Hiroyuki Nakamura
* Mateus Santos Messias

---
# **Funcionalidades**
## **[1] CREATE TABLE:** 
*_por: Hugo Nakamura._*

Comando para criar uma nova tabela. Recebe como entradas o nome dos arquivos CSV e binário. Após a criação da tabela (de cabeçalho e de dados), o código imprime o conteúdo binário do arquivo de saída.
  * **1.1. Entrada:**
    * Arquivo CSV dos dados;
    * Arquivo binário de dados;
  * **1.2. Saída:**
    * Os bits do arquivo binário.
  * **1.3. Erro:**
    * "Falha no processamento do arquivo."

## **[2] IMPRESSÃO DOS REGISTROS:**
*_por: Mateus Messias._*

Comando que imprime todos os registros de dados do arquivo binário.
  * **2.1. Entrada:**
    * Diretório do arquivo binário dos registros.
  * **2.2. Saída:**
    * Todos os registros do arquivo de dados.
  * **2.3. Erro:**
    * "Falha no processamento do arquivo."

## **[3] IMPRESSÃO DE REGISTROS SELECIONADOS:**
*_por: Mateus Messias._*

Comando que imprime apenas os registros de campos específicos, indicados na entrada da funcionalidade.

  * **3.1. Entrada:**
    * Diretório do arquivo binário dos registros;
    * Número de seleções desejadas;
    * Os respectivos campos e seus valores;
  * **3.2. Saída:**
    * Os registros de dados selecionados.
  * **3.3. Erro:**
    * "Falha no processamento do arquivo."

## **[4] REMOÇÃO LÓGICA:** 
*_por: Hugo Nakamura._*

Comando que faz a remoção lógica de n campos no arquivo binário. Através dos comandos "Select" e "Where", seleciona-se o registro a se remover.
  * **4.1. Entrada:**
    * Diretório do arquivo binário dos registros;
    * Número de campos a se remover;
    * Os respectivos campos e seus valores;
  * **4.2. Saída:**
    * Os bits do arquivo binário.
  * **4.3. Erro:**
    * "Falha no processamento do arquivo."

## **[5] INSERT INTO:**
*_por: Hugo Nakamura._*

Comando que insere n registros no arquivo binário. A inserção ocorre primeiro em registros lógicamente removidos. Se todos os registros estiverem no arquivo, então aloca-se o novo registro no fim. Todos os campos devem ser designados na inserção e, aqueles que forem nulos, deve-se indicar, na entrada, a palavra "NULO".
  * **5.1. Entrada:**
    * Diretório do arquivo binário dos registros;
    * Número 'n' que indica a quantidade de inserções.
    * Linha contendo todos os campos do registro de dados.
  * **5.2. Saída:**
    * Os bits do arquivo binário.
  * **5.3. Erro:**
    * "Falha no processamento do arquivo."

## **[6] COMPACTAÇÃO:**
*_por: Hugo Nakamura._*

Comando que retira todos os registros removidos de um arquivo. Ele funciona a partir de uma cópia de um arquivo para outro, mas apenas dos registros marcados com '0' no campo "removido".
  * **6.1. Entrada:**
    * Diretório do arquivo binário dos registros a se compactar;
  * **6.2. Saída:**
    * Os bits do arquivo binário compactado.
  * **6.3. Erro:**
    * "Falha no processamento do arquivo."

## **[7] COMPACTAÇÃO:**
*_por: Hugo Nakamura._*

Comando que retira todos os registros removidos de um arquivo. Ele funciona a partir de uma cópia de um arquivo para outro, mas apenas dos registros marcados com '0' no campo "removido".
  * **6.1. Entrada:**
    * Diretório do arquivo binário dos registros a se compactar;
  * **6.2. Saída:**
    * Os bits do arquivo binário compactado.
  * **6.3. Erro:**
    * "Falha no processamento do arquivo."

## **[8] COMPACTAÇÃO:**
*_por: Hugo Nakamura._*

Comando que retira todos os registros removidos de um arquivo. Ele funciona a partir de uma cópia de um arquivo para outro, mas apenas dos registros marcados com '0' no campo "removido".
  * **6.1. Entrada:**
    * Diretório do arquivo binário dos registros a se compactar;
  * **6.2. Saída:**
    * Os bits do arquivo binário compactado.
  * **6.3. Erro:**
    * "Falha no processamento do arquivo."

## **[9] COMPACTAÇÃO:**
*_por: Hugo Nakamura._*

Comando que retira todos os registros removidos de um arquivo. Ele funciona a partir de uma cópia de um arquivo para outro, mas apenas dos registros marcados com '0' no campo "removido".
  * **6.1. Entrada:**
    * Diretório do arquivo binário dos registros a se compactar;
  * **6.2. Saída:**
    * Os bits do arquivo binário compactado.
  * **6.3. Erro:**
    * "Falha no processamento do arquivo."

## **[10] COMPACTAÇÃO:**
*_por: Hugo Nakamura._*

Comando que retira todos os registros removidos de um arquivo. Ele funciona a partir de uma cópia de um arquivo para outro, mas apenas dos registros marcados com '0' no campo "removido".
  * **6.1. Entrada:**
    * Diretório do arquivo binário dos registros a se compactar;
  * **6.2. Saída:**
    * Os bits do arquivo binário compactado.
  * **6.3. Erro:**
    * "Falha no processamento do arquivo."


---
# **ARQUIVOS.**
>## **in**
* arquivos **binários** de entrada;
* arquivos **csv** de entrada;

>## **out**
* arquivos **binários** de saída;

> ## **src**
* "**arquivos.c**" e "**arquivos.h**": 
Arquivos com todas as funcionalidades relacionadas a manipulação de registros (ex: remoção, inserção, impressão, leitura, posicionamento de cursor etc.).

* "**mensagensErro.c**" e "**mensagensErro.h**":
Arquivo com funções de impressão de erro.

* "**limparBuffer.c**" e "**limparBuffer.h**":
Arquivo com uma função de limpar o buffer da entrada padrão do sistema.

* "**createTable.c**" e "**createTable.h**":
Arquivos com a funcionalidade [1] Create Table.

* "**recuperaDados.c**" e "**recuperaDados.h**":
Arquivos com a funcionalidade [2] Select All.

* "**impressaoRegistros.c**" e "**impressaoRegistros.h**":
Arquivos com a funcionalidade [3] Select From.

* "**remocaoLogica.c**" e "**remocaoLogica.h**":
Arquivos com a funcionalidade [4] Remoção Lógica.

* "**insertInto.c**" e "**insertInto.h**":
Arquivos com a funcionalidade [5] Insert Into.

* "**compactacao.c**" e "**compactacao.h**":
Arquivos com a funcionalidade [6] Compactação.

> ## **Makefile**
Arquivo Makefile para compilação dos códigos.
