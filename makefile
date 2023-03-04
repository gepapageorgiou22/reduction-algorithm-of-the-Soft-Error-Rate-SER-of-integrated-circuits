all:	circuit

circuit: circuit.c functions.c readFileData.c
	gcc -Wall -g circuit.c functions.c readFileData.c -o circuit

run:	circuit
	./circuit
