CC = gcc -Wall

.PHONY : clean

all: kript 

kript: kript.c
	$(CC) -g -o test kript.c hash.c poker.c -lm

clean:
	rm test
