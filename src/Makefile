all: programaTrab

main.o: main.c
	gcc -o main.o -c main.c

createTable.o: createTable.c
	gcc -o createTable.o -c createTable.c

mensagensErro.o: mensagensErro.c
	gcc -o mensagensErro.o -c mensagensErro.c

programaTrab: main.o createTable.o
	gcc -o programaTrab main.o createTable.o mensagensErro.o

run:
	./programaTrab

clean:
	rm -f *.o
	rm -f exec