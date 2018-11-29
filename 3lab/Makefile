CC = gcc -Wall

.PHONY : clean

all: kript dir

dir:
	mkdir -p sign_rsa

kript: kript.c bank.c
	$(CC) -g -o test kript.c hash.c signature.c bank.c -lm -lcrypto

clean:
	rm test
	rm -rf sign_rsa
