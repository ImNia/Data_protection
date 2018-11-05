#include <sdtio.h>
#include "kript.h"
#define COUNT 3

void a_distr(long long p)
{
    FILE *key = fopen("poker/a_key.txt", "rb");
    FILE *cards = fopen("poker/cards.txt", "wb");
    
    long long ca, da;
    fread(&ca, sizeof(ca), 1, key);
    fread(&da, sizeof(da), 1, key);



    fclose(key);
    fclose(cards);
}

void mental_poker()
{
    FILE *a_key = fopen("poker/a_key.txt", "wb");
    FILE *b_key = fopen("poker/b_key.txt", "wb");
    FILE *cards = fopen("poker/cards.txt", "wb");

    long long p = p_generation();
    
    long long *evk = (long long*)malloc(4);
    evk[0] = 0;
    long long ca, da;
    while(evk[0] != 1){
        ca = 1 + rand() % (p - 1);
        evklid(ca, (p - 1), evk);
    }
    da = evk[2];
    fwrite(&ca, sizeof(ca), 1, a_key);
    fwrite(&da, sizeof(da), 1, a_key);
    
    evk[0] = 0;
    long long cb, db;
    while(evk[0] != 1){
        cb = 1 + rand() % (p - 1);
        evklid(cb, (p - 1), evk);
    }
    db = evk[2];
    fwrite(&cb, sizeof(cb), 1, b_key);
    fwrite(&db, sizeof(db), 1, b_key);

    //Нужно ли генерить карты??
    long long random_num[COUNT];
    for(int i = 0; i < COUNT; i++){
        random_num[i] = 1 + rand() % (p - 1);
        fwrite(&random_num[i], sizeof(random_num), 1, cards);
    }

    fclose(a_key);
    fclose(b_key);
    fclose(cards);
}

int main()
{
    mental_poker();
    return 0;
}
