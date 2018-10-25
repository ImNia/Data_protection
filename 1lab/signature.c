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

int sha256_file(char *path, char outputBuffer[65])
{
    FILE *file = fopen(path, "rb");
    if(!file) return -534;

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    const int bufSize = 32768;
    unsigned char *buffer = malloc(bufSize);
    int bytesRead = 0;
    if(!buffer) return -1;
    while((bytesRead = fread(buffer, 1, bufSize, file)))
    {
        SHA256_Update(&sha256, buffer, bytesRead);
    }
    SHA256_Final(hash, &sha256);

    sha256_hash_string(hash, outputBuffer);
    fclose(file);
    free(buffer);
    return 0;
}

void sign_rsa_coder()
{
    FILE *read_file = fopen("read_file.txt", "rb");
    FILE *sign_rsa_key = fopen("sign_rsa/sign_rsa_key", "wb");

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

    static unsigned char buffer[65];
    sha256_file("read_file.txt", buffer);
    printf("%s\n", buffer);

    fclose(sign_rsa_key);
    fclose(read_file);
}

void sign_rsa()
{
    sign_rsa();
}

int main()
{
    sign_rsa();
    return 0;
}
