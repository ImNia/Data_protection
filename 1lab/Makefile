CC = gcc -Wall

all: dir kript

dir: ./el_gamal_file ./vernam_file

./el_gamal_file:
	mkdir el_gamal_file

./vernam_file:
	mkdir vernam_file

kript: kript.c cipher.c
	$(CC) -g -o test kript.c hash.c cipher.c -lm

clean:
	rm test
	rm -rf el_gamal_file
	rm -rf vernam_file
