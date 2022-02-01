CC = g++
CFLAGS = -Wall -g

all: build

build: sync convert

sync: tema.cpp
	$(CC) $(CFLAGS) -o $@ $^

convert: bonus.cpp
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

run: build
	./sync $(problem)

labyrinth: build
	./convert

clean:
	rm -f *.o *~ sync convert