#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void shake(int *graf, int *graf_shake)
{
    int versh[SIZE];
    int help_versh[SIZE] = {0, 1, 2, 3};

    int edge_graf = SIZE;
    int rand_num;
    for(int i = 0; i < SIZE; i++){
        rand_num = 0 + rand() % edge_graf;
        versh[i] = help_versh[rand_num];
        for(int j = rand_num; j < edge_graf; j++){
            help_versh[j] = help_versh[j + 1];
        }
        edge_graf -= 1;
        printf("%d\t", versh[i]);
    }
    printf("\n\n");

    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            graf_shake[versh[i] + SIZE * versh[j]] = graf[i + SIZE * j];
        }
    }
}

void make_big(int *graf_shake, int *secret_graf, int ver_file)
{
    for(int i = 0; i < ver_file * ver_file; i++){
        secret_graf[i] = 1000 + rand() % 10000;
    }
    print_graf(secret_graf);

    for(int i = 0; i < ver_file * ver_file; i++){
        if(graf_shake[i] == 1)
            secret_graf[i] = secret_graf[i] | 1;
        else{
            secret_graf[i] = secret_graf[i] >> 1;
            secret_graf[i] = secret_graf[i] << 1;
            secret_graf[i] = secret_graf[i] | 0;
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

    print_graf(graf);
    shake(graf, graf_shake);
    print_graf(graf_shake);

    int *secret_graf = malloc((ver_file * ver_file) * sizeof(int));
    make_big(graf_shake, secret_graf, ver_file);
    print_graf(secret_graf);

    free(graf);
    free(graf_shake);
    fclose(file);
    return 0;
}
