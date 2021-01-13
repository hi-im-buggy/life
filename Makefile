CC = gcc
CLIBS = -lncurses
CFLAGS = -Wall

build: src/main.c src/life.c src/life.h
	$(CC) $(CFLAGS) $? -o bin/life $(CLIBS)

install: build
	cp bin/life ~/.local/bin

run: build
	bin/life

clean:
	rm bin/*
