CC = gcc -Wall

all: kript test

kript: kript.c cipher.c
	$(CC) -g -o test kript.c hash.c cipher.c -lm

clean:
	rm test
