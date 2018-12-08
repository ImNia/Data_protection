CC = gcc -Wall

.PHONY : clean

all: main

main: gamilton.c kript.c hash.c
	$(CC) -g -o test gamilton.c kript.c hash.c -lm

clean:
	rm test
