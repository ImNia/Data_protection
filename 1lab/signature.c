#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
    FILE *signature_rsa = fopen("sign_rsa/signature_rsa.txt", "wb");
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

    unsigned char *hash = malloc(65);
    sha256_file("read_file.txt", hash);

    long long s[65];
    for (int i = 0; i < 65; i++){
        s[i] = module_power(hash[i], c, n);
        fwrite(&s[i], sizeof(long long), 1, signature_rsa);
    }

    fclose(sign_rsa_key);
    fclose(signature_rsa);
}

void sign_rsa_decoder()
{
    FILE *sign_rsa_key = fopen("sign_rsa/sign_rsa_key.txt", "rb");
    FILE *signature_rsa = fopen("sign_rsa/signature_rsa.txt", "rb");

    long long n, d;
    fread(&n, sizeof(long long), 1, sign_rsa_key);
    fread(&d, sizeof(long long), 1, sign_rsa_key);

    long long s[65];
    for(int i = 0; i < 65; i++){
        fread(&s[i], sizeof(long long), 1, signature_rsa);
    }
    unsigned char *hash = malloc(65);
    sha256_file("read_file.txt", hash);

    long long w[65];
    for (int i = 0; i < 65; i++){
        w[i] = module_power(s[i], d, n);
        if(hash[i] != w[i]){
            printf("Error\n");
        }
    }

    fclose(sign_rsa_key);
    fclose(signature_rsa);
}

void sign_rsa()
{
    sign_rsa_coder();
    sign_rsa_decoder();
}

void sign_el_gamal_coder()
{
    FILE *sign_el_key = fopen("sign_el_gamal/sign_el_key.txt", "wb");
    FILE *signature_el_gamal = fopen("sign_el_gamal/signature_el_gamal.txt", "wb");

    long long p, g = -1;
    while(g == -1){
        p = p_generation();
        g = g_generation(p);
    }   

    long long x = 1 + rand() % (p - 1);
    long long y = module_power(g, x, p);
    fwrite(&p, sizeof(p), 1, sign_el_key);
    fwrite(&g, sizeof(g), 1, sign_el_key);
    fwrite(&y, sizeof(y), 1, sign_el_key);

    unsigned char *hash = malloc(65);
    sha256_file("read_file.txt", hash);
    for(int i = 0; i < sizeof(hash); i++){
        if(hash[i] >= p){
            printf("Error\n");
        }
    }

    long long k = 0, k_inverst;
    long long *evk = (long long*)malloc(4);
    evk[0] = 0;
    while(evk[0] != 1){
        k = 1 + rand() % (p - 1);
        evklid(k, (p - 1), evk);
        k_inverst = evk[2];
        if(k_inverst < 0)
            k_inverst += (p - 1);
    }
    if(((k * k_inverst) % (p - 1)) != 1){
        printf("Uncorrect k-1\n");
    }

    long long r = module_power(g, k, p);
    fwrite(&r, sizeof(long long), 1, signature_el_gamal);

    long long u, s;
    for(int i = 0; i <= sizeof(hash); i++){
        u = (hash[i] - x * r) % (p - 1);
        while(u < 0){
            u += (p - 1);
        }
        s = (k_inverst * u) % (p - 1);
//        printf("u = %lld\t%lld\n", u[i], s);
        fwrite(&s, sizeof(long long), 1, signature_el_gamal);
    }

    fclose(sign_el_key);
    fclose(signature_el_gamal);
}

void sign_el_gamal_decoder()
{
    FILE *sign_el_key = fopen("sign_el_gamal/sign_el_key.txt", "rb");
    FILE *signature_el_gamal = fopen("sign_el_gamal/signature_el_gamal.txt", "rb");

    unsigned char *hash = malloc(65);
    sha256_file("read_file.txt", hash);
   
    long long p, g, y;
    fread(&p, sizeof(p), 1, sign_el_key);
    fread(&g, sizeof(g), 1, sign_el_key);
    fread(&y, sizeof(y), 1, sign_el_key);
    long long r, s;
    fread(&r, sizeof(r), 1, signature_el_gamal);
    long long help, helpp;
    for(int i = 0; i <= sizeof(hash); i++){
        fread(&s, sizeof(long long), 1, signature_el_gamal);
        while(s < 0){
            s += p;
        }
        help = (module_power(y, r, p) * module_power(r, s, p)) % p;
        helpp = module_power(g, hash[i], p);
//      printf("%lld\t%lld\n", help, helpp);
        if(help != helpp)
            printf("Uncorrect check\n");
    }


    fclose(sign_el_key);
    fclose(signature_el_gamal);
}

void sign_el_gamal()
{
    sign_el_gamal_coder();
    sign_el_gamal_decoder();
}

int main()
{
    srand(time(NULL));
    sign_el_gamal();
    return 0;
}
