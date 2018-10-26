#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#include "kript.h"

void sha256(char *string, char outputBuffer[65])
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}

int sha256_file(char *path, unsigned char *outputBuffer)
{
    FILE *file = fopen(path, "rb");
    if(!file) return -534;

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    unsigned char buffer;
    int bytesRead = 0;
    while((bytesRead = fread(&buffer, 1, sizeof(buffer), file)))
    {
        SHA256_Update(&sha256, &buffer, bytesRead);
    }
    SHA256_Final(outputBuffer, &sha256);

    fclose(file);
    return 0;
}

void sign_rsa_coder()
{
    FILE *sign_rsa_key = fopen("sign_rsa/sign_rsa_key.txt", "wb");

    long long p = p_generation();
    long long q = p_generation();

    long long n = p * q;
    long long fi = (p - 1) * (q - 1);
    long long d;

    long long *evk = (long long*)malloc(4);
    long long c;
    evk[0] = 0;
    while(evk[0] != 1){
        d = 1 + rand() % (fi - 1);
        evklid(d, fi, evk);
        c = evk[2];
        if(c < 0)
            c += fi;
        if(evk[0] == 1){
            evk[0] = 0;
            if((c * d) % fi == 1){
                evk[0] = 1;
            }
        }
    }
    fwrite(&n, sizeof(long long), 1, sign_rsa_key);
    fwrite(&d, sizeof(long long), 1, sign_rsa_key);

    unsigned char *buffer = malloc(65);
    sha256_file("read_file.txt", buffer);
    printf("%s\n", buffer);

    fclose(sign_rsa_key);
}

void sign_rsa()
{
    sign_rsa_coder();
}

int main()
{
    sign_rsa();
    return 0;
}
