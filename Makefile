all: programaTrab

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

buscarRegistros.o: ./src/buscarRegistros.c
	gcc -o ./src/buscarRegistros.o -c ./src/buscarRegistros.c

programaTrab: main.o createTable.o mensagensErro.o arquivos.o insertInto.o remocaoLogica.o compactacao.o limparBuffer.o recuperaDados.o buscarRegistros.o
	gcc -o programaTrab ./src/main.o ./src/createTable.o ./src/mensagensErro.o ./src/arquivos.o ./src/insertInto.o ./src/remocaoLogica.o ./src/compactacao.o ./src/limparBuffer.o ./src/recuperaDados.o ./src/buscarRegistros.o

run:
	./programaTrab

clean:
	rm -f ./src/*.o
	rm -f programaTrab