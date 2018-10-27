#include "cipher.h"

/* Шифр Шамира.
 * */
void shamir()
{
    FILE *read_file = fopen("read_file.txt", "rb");
    FILE *shamir_coder = fopen("shamir_file/shamir_coder.txt", "wb");
    FILE *shamir_decoder = fopen("shamir_file/shamir_decoder.txt", "wb");
    FILE *shamir_key = fopen("shamir_file/shamir_key.txt", "wb");

    unsigned char message;
    while(fread(&message, sizeof(message), 1, read_file) != 0){
        long long p = p_generation();
        long long ca = 0;
        long long da = 0;
        while(((ca * da) % (p - 1)) != 1){
            ca = 10 + rand() % 100000;
            da = 10 + rand() % 100000;
        }
        fwrite(&ca, sizeof(long long), 1, shamir_key);
        fwrite(&da, sizeof(long long), 1, shamir_key);

        long long cb = 0;
        long long db = 0;
        while(((cb * db) % (p - 1)) != 1){
            cb = 10 + rand() % 100000;
            db = 10 + rand() % 100000;
        }
        fwrite(&cb, sizeof(long long), 1, shamir_key);
        fwrite(&db, sizeof(long long), 1, shamir_key);
    
        long long x1 = module_power((long long)message, ca, p);
        long long x2 = module_power(x1, cb, p);
        fwrite(&x2, sizeof(x2), 1, shamir_coder);
        long long x3 = module_power(x2, da, p);
        long long x4 = module_power(x3, db, p);
        fwrite(&x4, sizeof(message), 1, shamir_decoder);
    
//        if (message != x4)
//            printf("error\n");
    }

    fclose(read_file);
    fclose(shamir_coder);
    fclose(shamir_decoder);
    fclose(shamir_key);
}

/*  Кодирование файла шифром Эль Гамаля, на вход получает сообщение
 *  и сгеренерированые p и g
 * */
void el_gamal_coder(int p, int g)
{
    FILE *el_gamal_coder = fopen("el_gamal_file/el_gamal_coder.txt", "wb");
    FILE *el_gamal_key = fopen("el_gamal_file/el_gamal_key.txt", "wb");
    FILE *read_file = fopen("6.png", "rb");
    
    char message;
    if(read_file != NULL){
        while(fread(&message, sizeof(char), 1, read_file) != 0){
            long long cb = 1 + rand() % (p - 1);
            long long db = module_power(g, cb, p);

            long long k = 1 + rand() % (p - 2);
            long long r = module_power(g, k, p);
            long long e = ((message % p) * module_power(db, k, p)) % p;

            fwrite(&r, sizeof(long long), 1, el_gamal_coder);
            fwrite(&e, sizeof(long long), 1, el_gamal_coder);
            fwrite(&cb, sizeof(long long), 1, el_gamal_key);
        }
    }else{
        printf("File can't open");
    }
    
    fclose(read_file);
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
    int p = p_generation();
    int g = g_generation(p);

    el_gamal_coder(p, g);

    el_gamal_decoder(p);
}

void vernam_coder()
{
    FILE *vernam_coder = fopen("vernam_file/vernam_coder.txt", "ab");
    FILE *vernam_key = fopen("vernam_file/verman_key.txt", "ab");
    FILE *read_file = fopen("6.png", "rb");
    
    char message;
    
    if(read_file != NULL){
        while(fread(&message, sizeof(char), 1, read_file) != 0){
            char coder_key = rand();
            fwrite(&coder_key, sizeof(char), 1, vernam_key);

            char coder_word = message ^ coder_key;
            fwrite(&coder_word, sizeof(char), 1, vernam_coder);
        }
    }else{
        printf("File can't open");
    }

    fclose(read_file);
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
    vernam_coder();

    vernam_decoder();
}

/*  Кодирование шифром RSA. На вход получает сообщение
 *  и простые числа p и q.
 * */
void rsa_coder(long long p, long long q)
{
    FILE *rsa_coder = fopen("rsa_file/rsa_coder.txt", "wb");
    FILE *rsa_key = fopen("rsa_file/rsa_key.txt", "wb");
    FILE *read_file = fopen("6.png", "rb");

    long long n_abonent = p * q;
    long long moduli = (p - 1) * (q - 1);
    long long d;
    
    long long *evk = (long long*)malloc(4);
    long long c;
    evk[0] = 0;
    while(evk[0] != 1){
        d = 1 + rand() % (moduli - 1);
        evklid(d, moduli, evk);
        c = evk[2];
        if(c < 0)
            c += moduli;
        if(evk[0] == 1){
            evk[0] = 0;
            if((c * d) % moduli == 1){
                evk[0] = 1;
            }
        }
    }
    fwrite(&c, sizeof(long long), 1, rsa_key);

    char str;
    long long e;
    if(read_file != NULL){
        while(fread(&str, sizeof(char), 1, read_file) != 0){
            e = module_power((long long)str, d, n_abonent);
            fwrite(&e, sizeof(long long), 1, rsa_coder);
        }
    }else{
        printf("File can't open");
    }
    free(evk);
    fclose(read_file);
    fclose(rsa_coder);
    fclose(rsa_key);
}

/*  Декодирование шифром RSA. 
 *  На вход получает простые числа p и q.
 * */
void rsa_decoder(long long p, long long q)
{
    FILE *rsa_coder = fopen("rsa_file/rsa_coder.txt", "rb");
    FILE *rsa_decoder = fopen("rsa_file/rsa_decoder.txt", "wb");
    FILE *rsa_key = fopen("rsa_file/rsa_key.txt", "rb");
    long long n_abonent = p * q;

    long long c;
    fread(&c, sizeof(long long), 1, rsa_key);

    long long message_decoder;
    long long e;
    int help = 1;
    while(help > 0){
        help = fread(&e, sizeof(long long), 1, rsa_coder);
        message_decoder = module_power(e, c, n_abonent);
        fwrite(&message_decoder, sizeof(char), 1, rsa_decoder);
    }

    fclose(rsa_coder);
    fclose(rsa_decoder);
    fclose(rsa_key);
}

/*  Шифр RSA.
 * */
void rsa()
{
    int p = p_generation();
    int q = p_generation();

    rsa_coder((long long)p, (long long)q);

    rsa_decoder((long long)p, (long long)q);

}
