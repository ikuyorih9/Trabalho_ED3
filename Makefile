all: programaTrab

src/main.o: src/main.c
	gcc -o src/main.o -c src/main.c

src/createTable.o: src/createTable.c
	gcc -o src/createTable.o -c src/createTable.c

src/insertInto.o: src/insertInto.c
	gcc -o src/insertInto.o -c src/insertInto.c

src/mensagensErro.o: src/mensagensErro.c
	gcc -o src/mensagensErro.o -c src/mensagensErro.c

src/arquivos.o: src/arquivos.c
	gcc -o src/arquivos.o -c src/arquivos.c

src/remocaoLogica.o: src/remocaoLogica.c
	gcc -o src/remocaoLogica.o -c src/remocaoLogica.c

programaTrab: src/main.o src/createTable.o src/mensagensErro.o src/arquivos.o src/insertInto.o src/remocaoLogica.o
	gcc -o programaTrab src/main.o src/createTable.o src/mensagensErro.o src/arquivos.o src/insertInto.o src/remocaoLogica.o

run:
	./programaTrab

clean:
	rm -f src/*.o
	rm -f programaTrab