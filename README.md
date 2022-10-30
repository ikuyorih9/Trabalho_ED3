# TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS III

Desenvolvido por:
* Hugo Hiroyuki Nakamura
* Mateus Santos Messias
---

# Funcionalidades
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

  

