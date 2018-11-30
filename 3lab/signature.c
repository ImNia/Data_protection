#include "signature.h"

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

int sha256_file(int message, unsigned char *outputBuffer)
{
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, &message, sizeof(message));
    SHA256_Final(outputBuffer, &sha256);

    return 0;
}

void sign_rsa_coder(unsigned char *hash, long long *signature_bank)
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

    for (int i = 0; i < 65; i++){
        signature_bank[i] = module_power(hash[i], c, n);
    }

    fclose(sign_rsa_key);
}

int sign_rsa_decoder(unsigned char *hash, long long *signature_bank)
{
    FILE *sign_rsa_key = fopen("sign_rsa/sign_rsa_key.txt", "rb");

    long long n, d;
    fread(&n, sizeof(long long), 1, sign_rsa_key);
    fread(&d, sizeof(long long), 1, sign_rsa_key);

    long long w[65];
    for (int i = 0; i < 65; i++){
        w[i] = module_power(signature_bank[i], d, n);
        if(hash[i] != w[i]){
            printf("Error\n");
            fclose(sign_rsa_key);
            return 1;
        }
    }

    fclose(sign_rsa_key);
    return 0;
}
