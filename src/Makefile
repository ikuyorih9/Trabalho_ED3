all: programaTrab

main.o: main.c
	gcc -o main.o -c main.c

createTable.o: createTable.c
	gcc -o createTable.o -c createTable.c

insertInto.o: insertInto.c
	gcc -o insertInto.o -c insertInto.c

mensagensErro.o: mensagensErro.c
	gcc -o mensagensErro.o -c mensagensErro.c

arquivos.o:arquivos.c
	gcc -o arquivos.o -c arquivos.c

programaTrab: main.o createTable.o mensagensErro.o arquivos.o insertInto.o
	gcc -o programaTrab main.o createTable.o mensagensErro.o arquivos.o insertInto.o

run:
	./programaTrab

clean:
	rm -f *.o
	rm -f programaTrab