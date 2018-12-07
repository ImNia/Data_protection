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

int main()
{
    srand(time(NULL));
    FILE *file = fopen("file.txt", "r");
    char ver_file;

    fread(&ver_file, sizeof(char), 1, file);

    int *graf = malloc((ver_file * ver_file) * sizeof(int));
    int *graf_shake = malloc((ver_file * ver_file) * sizeof(int));

    for(int i = 0; i < ver_file; i++){
        for(int j = 0; j < ver_file; j++){
            graf[i + ver_file * j] = 0;
        }
    }
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

    free(graf_shake);
    return 0;
}
