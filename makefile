CC = gcc
CFLAGS = -Wall
LDFLAGS =
OBJFILES = circuit.o functions.o radiation_reduction_functions.o
TARGET = circuit

all:	$(TARGET)

$(TARGET): $(OBJFILES) circuit.c functions.c radiation_reduction_functions.c
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

run:	circuit
	./circuit
