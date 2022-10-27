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

programaTrab: main.o createTable.o mensagensErro.o arquivos.o insertInto.o remocaoLogica.o compactacao.o
	gcc -o programaTrab ./src/main.o ./src/createTable.o ./src/mensagensErro.o ./src/arquivos.o ./src/insertInto.o ./src/remocaoLogica.o ./src/compactacao.o

run:
	./programaTrab

clean:
	rm -f ./src/*.o
	rm -f programaTrab