#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "kript.h"

struct gamil_loop{
    int left;
    int right;
    int key;
};

void print_graf(int *graf_p, int ver_file)
{
    for(int i = 0; i < ver_file * ver_file; i++){
        printf("%d\t", graf_p[i]);
        if((i + 1) % ver_file == 0)
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
    }

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

void Bob_first_question(int *graf, int *secret_graf, int *coder_graf, int ver_file, int *versh, long long N, long long d)
{
    int *check_graf = (int*)malloc((ver_file * ver_file) * sizeof(int));
    for(int i = 0; i < ver_file * ver_file; i++){
        check_graf[i] = secret_graf[i];
        check_graf[i] = module_power(check_graf[i], d, N);
        if(coder_graf[i] != check_graf[i]){
            printf("%d\t%d\t", coder_graf[i], check_graf[i]);
            printf("Error.Not one.\n");
            return;
        }
    }
    printf("Coded graph is correct!\n");

    int *decoder_graf = (int*)malloc((ver_file * ver_file) * sizeof(int));
    for(int i = 0; i < ver_file * ver_file; i++){
        decoder_graf[i] = secret_graf[i] & 1;
    }

    int *help_graf = (int*)malloc((ver_file * ver_file) * sizeof(int));
    shake_graf(decoder_graf, help_graf, versh, ver_file);

    for(int i = 0; i < ver_file; i++){
        for(int j = 0; j < ver_file; j++){
            if(graf[i] != help_graf[i]){
                printf("Error.Not first graf!\n");
                return;
            }
        }
    }
    printf("The original graph and mixed graph is the same!\n\n");
}

void Bob_second_question(int *coder_graf, struct gamil_loop *gamil, int ver_file, long long N, long long c)
{
    int *check_graf = (int*)malloc((ver_file * ver_file) * sizeof(int));
    int pas_peaks[ver_file];
    for(int i = 0; i < ver_file; i++){
        pas_peaks[i] = 0;
    }

    for(int i = 0; i < ver_file; i++){
        check_graf[i] = coder_graf[gamil[i].right + ver_file * gamil[i].left];
        check_graf[i] = module_power(check_graf[i], c, N);
        if(gamil[i].key != check_graf[i]){
            printf("Alice passed the wrong data!\n");
            return;
        }
        pas_peaks[gamil[i].right] += 1; 
    }
    printf("Alice passed the correct data!\n");

    for(int i = 0; i < ver_file; i++){
        if(pas_peaks[i] != 1){
            printf("Hamilton cycle is not true: %d\n", pas_peaks[i]);
            return;
        }
    }
    printf("Hamilton cycle is true!\n\n");

}

int main()
{
    srand(time(NULL));
    FILE *file = fopen("file.txt", "r");
    int ver_file;

    fscanf(file, "%d", &ver_file);

    int *graf = (int*)malloc((ver_file * ver_file) * sizeof(int));
    int *graf_shake = (int*)malloc((ver_file * ver_file) * sizeof(int));

    int index;
    for(int i = 0; i < ver_file * ver_file; i++){
        fscanf(file, "%d", &index);
        if(index == 1)
            graf[i] = 1;
        else
            graf[i] = 0;
    }

    int *coder_graf = (int*)malloc((ver_file * ver_file) * sizeof(int));
    int *versh = (int*)malloc(ver_file * sizeof(int));
//    print_graf(graf, ver_file);
    shake(graf, graf_shake, ver_file, versh);
//    print_graf(graf_shake, ver_file);

    int *secret_graf = (int*)malloc((ver_file * ver_file) * sizeof(int));
    //for RSA
    long long p = p_generation();
    long long q = p_generation();
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

    make_big(graf_shake, secret_graf, coder_graf, ver_file, N, d);
    print_graf(coder_graf, ver_file);

    //Question from Bob
    int question;
    while(1){
        printf("Enter number question 1 or 2:\t");
        scanf("%d", &question);
        printf("\n");
        if(question == 1){
            Bob_first_question(graf, secret_graf, coder_graf, ver_file, versh, N, d);
        }else if(question == 2){
            FILE *file_second = fopen("zikl.txt", "r");

            int loop[ver_file];
            for(int i = 0; i < ver_file; i++){
                fscanf(file_second, "%d", &loop[i]);
                loop[i] -= 1;
            }
            struct gamil_loop gamil[ver_file];

            for(int i = 0; i < ver_file; i++){
                int key_pos = loop[(i + 1) % ver_file] + ver_file * loop[i];
                struct gamil_loop tmp = {
                    loop[i], 
                    loop[(i + 1) % ver_file], 
                    secret_graf[key_pos] 
                };
                gamil[i] = tmp;
            }

            Bob_second_question(coder_graf, gamil, ver_file, N, c);
            fclose(file_second);
        }
    }
    printf("\n");

    free(graf);
    free(graf_shake);
    free(secret_graf);
    free(coder_graf);
    fclose(file);
    return 0;
}
