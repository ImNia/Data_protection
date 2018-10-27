CC = gcc -Wall

.PHONY : clean

all: dir kript 

dir: 
	mkdir -p sign_rsa

kript: kript.c cipher.c
	$(CC) -g -o test kript.c hash.c cipher.c signature.c -lm -lcrypto

clean:
	rm test
	rm -rf sign_rsa
