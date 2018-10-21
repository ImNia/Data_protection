#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "kript.h"

/* Шифр Шамира.
 * */
void shamir(int message)
{
    long long p = p_generation();
    long long ca = 0;
    long long da = 0;
    while(((ca * da) % (p - 1)) != 1){
        ca = 10 + rand() % 100000;
        da = 10 + rand() % 100000;
    }

    long long cb = 0;
    long long db = 0;
    while(((cb * db) % (p - 1)) != 1){
        cb = 10 + rand() % 100000;
        db = 10 + rand() % 100000;
    }
    
    long long x1 = module_power(message, ca, p);
    long long x2 = module_power(x1, cb, p);
    long long x3 = module_power(x2, da, p);
    long long x4 = module_power(x3, db, p);
    if (message != x4)
        printf("halol\n");
}

int el_gamal_coder(int message)
{
    FILE *el_gamal_coder = fopen("el_gamal_coder.txt", "ab");
    FILE *el_gamal_key = fopen("el_gamal_key.txt", "ab");

    int p = p_generation();
    int g = g_generation(p);

    long long cb = 1 + rand() % (p - 1);
    long long db = module_power(g, cb, p);

    long long k = 1 + rand() % (p - 2);
    long long r = module_power(g, k, p);
    long long e = ((message % p) * module_power(db, k, p)) % p;

    fwrite(&r, sizeof(long long), 1, el_gamal_coder);
    fwrite(&e, sizeof(long long), 1, el_gamal_coder);
    fwrite(&cb, sizeof(long long), 1, el_gamal_key);
    
    return p;
}

void el_gamal_decoder(int p)
{
    FILE *el_gamal_coder = fopen("el_gamal_coder.txt", "rb");
    FILE *el_gamal_key = fopen("el_gamal_key.txt", "rb");
    
    FILE *el_gamal_decoder = fopen("el_gamal_decoder.txt", "wb");
    
    long long cb;
    long long r, e;
    long long message_b;

    int help = 1;
    fread(&cb, sizeof(long long), 1, el_gamal_key);
    while(help > 0){
        fread(&r, sizeof(long long), 1, el_gamal_coder);
        help = fread(&e, sizeof(long long), 1, el_gamal_coder);
        
        message_b = ((e % p) * module_power(r, p - 1 - cb, p)) % p;
        printf("Msg\t%lld\n", message_b);
        fwrite(&message_b, sizeof(long long), 1, el_gamal_decoder);
    }
}

int main()
{
    srand(time(NULL));
    char str;

    FILE *read_file = fopen("read_file.txt", "rb");
    FILE *key = fopen("key.txt", "rb");
    FILE *write_file = fopen("write_file.txt", "w");
    
    //fread(&p, sizeof(int), 1, key);

    int p;
    if(read_file != NULL){
        while(fread(&str, sizeof(char), 1, read_file) != 0){
            p = el_gamal_coder((int)str);
        }
    }else{
        printf("File can't open");
    }    
    
    el_gamal_decoder(p);

    fclose(read_file);
    fclose(key);
    fclose(write_file);
    return 0;
}
