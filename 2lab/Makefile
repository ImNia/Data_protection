CC = gcc -Wall

.PHONY : clean

all: dir kript 

dir:
	mkdir -p poker

kript: kript.c
	$(CC) -g -o test kript.c hash.c poker.c -lm

clean:
	rm test
	rm -rf poker
