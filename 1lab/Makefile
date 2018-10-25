CC = gcc -Wall

.PHONY : clean

all: dir kript 

dir: 
	mkdir -p shamir_file
	mkdir -p el_gamal_file
	mkdir -p vernam_file
	mkdir -p rsa_file
	mkdir -p sign_rsa

kript: kript.c cipher.c
	$(CC) -g -o test kript.c hash.c cipher.c test.c -lm

clean:
	rm test
	rm -rf shamir_file
	rm -rf el_gamal_file
	rm -rf vernam_file
	rm -rf rsa_file
	rm -rf sign_rsa
