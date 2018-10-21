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


/*  Кодирование файла шифром Эль Гамаля, на вход получает сообщение
 *  и сгеренерированые p и g
 * */
void el_gamal_coder(int message, int p, int g)
{
    FILE *el_gamal_coder = fopen("el_gamal_file/el_gamal_coder.txt", "ab");
    FILE *el_gamal_key = fopen("el_gamal_file/el_gamal_key.txt", "ab");

    long long cb = 1 + rand() % (p - 1);
    long long db = module_power(g, cb, p);

    long long k = 1 + rand() % (p - 2);
    long long r = module_power(g, k, p);
    long long e = ((message % p) * module_power(db, k, p)) % p;

    fwrite(&r, sizeof(long long), 1, el_gamal_coder);
    fwrite(&e, sizeof(long long), 1, el_gamal_coder);
    fwrite(&cb, sizeof(long long), 1, el_gamal_key);
    
    fclose(el_gamal_coder);
    fclose(el_gamal_key);
}

/*  Декодирование файла шифром Эль Гамаля, на вход получает
 *  простое число p
 * */
void el_gamal_decoder(int p)
{
    FILE *el_gamal_coder = fopen("el_gamal_file/el_gamal_coder.txt", "rb");
    FILE *el_gamal_key = fopen("el_gamal_file/el_gamal_key.txt", "rb");
    
    FILE *el_gamal_decoder = fopen("el_gamal_file/el_gamal_decoder.txt", "wb");
    
    long long cb;
    long long r, e;
    long long message_b;

    int help = 1;
    while(help > 0){
        fread(&r, sizeof(long long), 1, el_gamal_coder);
        help = fread(&e, sizeof(long long), 1, el_gamal_coder);
        fread(&cb, sizeof(long long), 1, el_gamal_key);
        
        message_b = ((e % p) * module_power(r, p - 1 - cb, p)) % p;
        fwrite(&message_b, sizeof(char), 1, el_gamal_decoder);
    }

    fclose(el_gamal_coder);
    fclose(el_gamal_key);
    fclose(el_gamal_decoder);
}

/* Шифр Эль Гамаля. Шифрует и дешифрует.
 *
 * */
void el_gamal()
{
    char str;
    int p = p_generation();
    int g = g_generation(p);

    FILE *read_file = fopen("read_file.txt", "rb");
    
    if(read_file != NULL){
        while(fread(&str, sizeof(char), 1, read_file) != 0){
            el_gamal_coder((int)str, p, g);
        }
    }else{
        printf("File can't open");
    }

    el_gamal_decoder(p);
    
    fclose(read_file);
}

int main()
{
    srand(time(NULL));
    
    el_gamal();

    return 0;
}
