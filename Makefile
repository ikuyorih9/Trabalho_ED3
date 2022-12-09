all:
	gcc -o programaTrab ./src/*.c

main.o: ./src/main.c
	gcc -o ./src/main.o -c ./src/main.c

createTable.o: ./src/createTable.c
	gcc -o ./src/createTable.o -c ./src/createTable.c

insertInto.o: ./src/insertInto.c
	gcc -o ./src/insertInto.o -c ./src/insertInto.c

mensagensErro.o: ./src/mensagensErro.c
	gcc -o ./src/mensagensErro.o -c ./src/mensagensErro.c

arquivos.o: ./src/arquivos.c
	gcc -o ./src/arquivos.o -c ./src/arquivos.c

remocaoLogica.o: ./src/remocaoLogica.c
	gcc -o ./src/remocaoLogica.o -c ./src/remocaoLogica.c

compactacao.o: ./src/compactacao.c
	gcc -o ./src/compactacao.o -c ./src/compactacao.c

limparBuffer.o: ./src/limparBuffer.c
	gcc -o ./src/limparBuffer.o -c ./src/limparBuffer.c

recuperaDados.o: ./src/recuperaDados.c
	gcc -o ./src/recuperaDados.o -c ./src/recuperaDados.c

impressaoRegistros.o: ./src/impressaoRegistros.c
	gcc -o ./src/impressaoRegistros.o -c ./src/impressaoRegistros.c

arvoreB.o: ./src/arvoreB.c
	gcc -o ./src/arvoreB.o -c ./src/arvoreB.c

createIndex.o: ./src/createIndex.c
	gcc -o ./src/createIndex.o -c ./src/createIndex.c

recuperaArvore.o: ./src/recuperaArvore.c
	gcc -o ./src/recuperaArvore.o -c ./src/recuperaArvore.c

insercaoArvore.o: ./src/insercaoArvore.c
	gcc -o ./src/insercaoArvore.o -c ./src/insercaoArvore.c

recuperaConectados.o: ./src/recuperaConectados.c
	gcc -o ./src/recuperaConectados.o -c ./src/recuperaConectados.c

run:
	./programaTrab

clean:
	rm -f ./src/*.o
	rm -f programaTrab