#include "cipher.h"

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

void vernam_coder(char message)
{
    FILE *vernam_coder = fopen("vernam_file/vernam_coder.txt", "ab");
    FILE *vernam_key = fopen("vernam_file/verman_key.txt", "ab");
    
    char coder_key = rand();
    
    fwrite(&coder_key, sizeof(char), 1, vernam_key);

    char coder_word = message ^ coder_key;
    fwrite(&coder_word, sizeof(char), 1, vernam_coder);

    fclose(vernam_coder);
    fclose(vernam_key);
}

void vernam_decoder()
{
    FILE *vernam_coder = fopen("vernam_file/vernam_coder.txt", "rb");
    FILE *vernam_key = fopen("vernam_file/verman_key.txt", "rb");
    FILE *vernam_decoder = fopen("vernam_file/verman_decoder.txt", "ab");

    char coder_word, coder_key, message;
    int help = 1;
    while(help > 0){
        help = fread(&coder_word, sizeof(char), 1, vernam_coder);
        fread(&coder_key, sizeof(char), 1, vernam_key);

        message = coder_word ^ coder_key;

        fwrite(&message, sizeof(char), 1, vernam_decoder);
    }  

    fclose(vernam_coder);
    fclose(vernam_key);
    fclose(vernam_decoder);
}

void vernam()
{
    char str;
    FILE *read_file = fopen("read_file.txt", "rb");
    
    if(read_file != NULL){
        while(fread(&str, sizeof(char), 1, read_file) != 0){
            vernam_coder(str);
        }
    }else{
        printf("File can't open");
    }

    vernam_decoder();

    fclose(read_file);
}

/*  Кодирование шифром RSA. На вход получает сообщение
 *  и простые числа p и q.
 * */
void rsa_coder(char message, int p, int q)
{
    FILE *rsa_coder = fopen("rsa_file/rsa_coder.txt", "ab");

    long long n_abonent = p * q;
    long long moduli = (p - 1) * (q - 1);
    long long d = moduli - 1;

    long long e = module_power((long long)message, d, n_abonent);
    fwrite(&e, sizeof(long long), 1, rsa_coder);
    fclose(rsa_coder);
}

/*  Декодирование шифром RSA. 
 *  На вход получает простые числа p и q.
 * */
void rsa_decoder(int p, int q)
{
    FILE *rsa_coder = fopen("rsa_file/rsa_coder.txt", "rb");
    FILE *rsa_decoder = fopen("rsa_file/rsa_decoder.txt", "ab");
    long long n_abonent = p * q;

    long long e;

    long long moduli = (p - 1) * (q - 1);
    long long d = moduli - 1;
    long long c = 0;
    while(((c * d) % moduli) != 1){
        c = 1 + rand() % moduli;
    }

    long long message_decoder;
    int help = 1;
    while(help > 0){
        help = fread(&e, sizeof(long long), 1, rsa_coder);
        message_decoder = module_power(e, c, n_abonent);
        fwrite(&message_decoder, sizeof(char), 1, rsa_decoder);
    }

    fclose(rsa_coder);
    fclose(rsa_decoder);
}

/*  Шифр RSA.
 * */
void rsa()
{
    char str;
    int p = p_generation();
    int q = p_generation();

    FILE *read_file = fopen("read_file.txt", "rb");

    if(read_file != NULL){
        while(fread(&str, sizeof(char), 1, read_file) != 0){
            rsa_coder(str, p, q);
        }
    }else{
        printf("File can't open");
    }

    rsa_decoder(p, q);

    fclose(read_file);
}
