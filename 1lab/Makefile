CC = gcc -Wall

.PHONY : clean

all: dir kript 

dir: 
	mkdir -p sign_rsa
	mkdir -p sign_el_gamal
	mkdir -p sign_dsa

kript: kript.c cipher.c
	$(CC) -g -o test kript.c hash.c cipher.c signature.c test.c -lm -lcrypto

clean:
	rm test
	rm -rf sign_rsa
	rm -rf sign_el_gamal
	rm -rf sign_dsa
