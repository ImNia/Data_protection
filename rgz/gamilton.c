#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "kript.h"

#define SIZE 4

void print_graf(int *graf_p)
{
    for(int i = 0; i < SIZE * SIZE; i++){
        printf("%d\t", graf_p[i]);
        if((i + 1) % SIZE == 0)
            printf("\n");
    }
    printf("\n");
}

void shake(int *graf, int *graf_shake, int ver_file, int *versh)
{
    int help_versh[ver_file];
    for(int i = 0; i < ver_file; i++){
        help_versh[i] = i;
    }

    int edge_graf = ver_file;
    int rand_num;
    for(int i = 0; i < ver_file; i++){
        rand_num = 0 + rand() % edge_graf;
        versh[i] = help_versh[rand_num];
        for(int j = rand_num; j < edge_graf; j++){
            help_versh[j] = help_versh[j + 1];
        }
        edge_graf -= 1;
        printf("%d\t", versh[i]);
    }
    printf("\n\n");

    for(int i = 0; i < ver_file; i++){
        for(int j = 0; j < ver_file; j++){
            graf_shake[versh[i] + ver_file * versh[j]] = graf[i + ver_file * j];
        }
    }
}

void shake_graf(int *graf, int *graf_shake, int *versh, int ver_file)
{
    for(int i = 0; i < ver_file; i++){
        for(int j = 0; j < ver_file; j++){
            graf_shake[i + ver_file * j] = graf[versh[i] + ver_file * versh[j]];
        }
    }
}

void make_big(int *graf_shake, int *secret_graf, int *coder_graf, int ver_file, long long N, long long d)
{
    for(int i = 0; i < ver_file * ver_file; i++){
        secret_graf[i] = 100 + rand() % 1000;
    }

    for(int i = 0; i < ver_file * ver_file; i++){
        if(graf_shake[i] == 1)
            secret_graf[i] = secret_graf[i] | 1;
        else{
            secret_graf[i] = secret_graf[i] >> 1;
            secret_graf[i] = secret_graf[i] << 1;
            secret_graf[i] = secret_graf[i] | 0;
        }
    }
    
    for(int i = 0; i < ver_file * ver_file; i++){
        coder_graf[i] = module_power(secret_graf[i], d, N);
    }
}

void Bob_first_question(int *graf, int *graf_shake, int *secret_graf, int *coder_graf, int ver_file, int *versh, long long N, long long d)
{
    int *check_graf = malloc((ver_file * ver_file) * sizeof(int));
    for(int i = 0; i < ver_file * ver_file; i++){
        check_graf[i] = secret_graf[i];
        check_graf[i] = module_power(check_graf[i], d, N);
        if(coder_graf[i] != check_graf[i]){
            printf("%d\t%d\t", coder_graf[i], check_graf[i]);
            printf("Error.Not one.\n");
        }
    }

    int *decoder_graf = malloc((ver_file * ver_file) * sizeof(int));
    for(int i = 0; i < ver_file * ver_file; i++){
        decoder_graf[i] = secret_graf[i] & 1;
        printf("%d\t", decoder_graf[i]);
        if((i + 1) % ver_file == 0)
            printf("\n");
    }

    int *help_graf = malloc((ver_file * ver_file) * sizeof(int));
    shake_graf(decoder_graf, help_graf, versh, ver_file);
    printf("\n\n");
    print_graf(help_graf);

    for(int i = 0; i < ver_file; i++){
        for(int j = 0; j < ver_file; j++){
            if(graf[i] != help_graf[i]){
                printf("Error.Not first graf!\n");
            }
        }
    }
}

int main()
{
    srand(time(NULL));
    FILE *file = fopen("file.txt", "r");
    int ver_file;

    fscanf(file, "%d", &ver_file);

    int *graf = malloc((ver_file * ver_file) * sizeof(int));
    int *graf_shake = malloc((ver_file * ver_file) * sizeof(int));

    int index;
    for(int i = 0; i < ver_file * ver_file; i++){
        fscanf(file, "%d", &index);
        if(index == 1)
            graf[i] = 1;
        else
            graf[i] = 0;
    }

    int *coder_graf = malloc((ver_file * ver_file) * sizeof(int));
    int *versh = malloc(ver_file * sizeof(int));
    print_graf(graf);
    shake(graf, graf_shake, ver_file, versh);
    print_graf(graf_shake);
    
    int *secret_graf = malloc((ver_file * ver_file) * sizeof(int));
    //for RSA
    long long p = p_generation();
    long long q = p_generation(p);
    long long N = p * q;
    long long moduli = (p - 1) * (q - 1);
    long long *evk = (long long*)malloc(4);
    long long d, c;
    evk[0] = 0;
    while(evk[0] != 1){
        c = 1 + rand() % (moduli - 1);
        evklid(c, moduli, evk);
        d = evk[2];
        if(d < 0)
            d += moduli;
        if(evk[0] == 1){
            evk[0] = 0;
            if((d * c) % moduli == 1){
                evk[0] = 1;
            }
        }
    }
    //End

//    printf("p %lld, q %lld\n", p, q);
//    printf("N %lld, c %lld\n", N, c);
    make_big(graf_shake, secret_graf, coder_graf, ver_file, N, d);
    print_graf(secret_graf);

    Bob_first_question(graf, graf_shake, secret_graf, coder_graf, ver_file, versh, N, d);

    free(graf);
    free(graf_shake);
    fclose(file);
    return 0;
}
