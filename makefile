all:	circuit

circuit: circuit.c functions.c
	gcc -Wall -g circuit.c functions.c -o circuit

run:	circuit
	./circuit
