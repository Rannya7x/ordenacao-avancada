CC=gcc
CFLAGS=-O2 -std=c11 -Wall -Wextra -Iinclude
LDFLAGS=
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)
BIN=bin/tp02

all: $(BIN)

bin:
	mkdir -p bin

$(BIN): bin $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -f src/*.o $(BIN)
