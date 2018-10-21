CC = gcc -Wall

all: kript test

kript: kript.c cipher.c
	make clean
	$(CC) -g -o test kript.c hash.c cipher.c -lm
	mkdir el_gamal_file

clean:
	rm test
	rm -rf el_gamal_file
