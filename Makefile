SRC = $(wildcard src/*.c)
CC = gcc

all: $(SRC)
	$(CC) $(SRC) -o build/dry.exe