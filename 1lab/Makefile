CC = gcc -Wall

all: kript test

kript: kript.c test.c
	$(CC) -g -o test kript.c hash.c test.c -lm

clean:
	rm test
