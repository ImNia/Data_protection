#include <stdio.h>
#include <stdlib.h>

#include "kript.h"
#define COUNT 5

void prin_pack(long long *card)
{
    for(int i = 0; i < COUNT; i++)
    {
        printf("%lld\t", card[i]);
    }
    printf("\n\n");
}

/*  Перемешивание колоды.
 * */
void mix(long long *card)
{
    long long pack[COUNT];
    for(int i = 0; i < COUNT; i++){
        pack[i] = card[i];
    }

    int edge_pack = COUNT;
    int rand_num;
    for(int i = 0; i < COUNT; i++){
        rand_num = 0 + rand() % edge_pack;
        card[i] = pack[rand_num];
        for(int j = rand_num; j < edge_pack - 1; j++){
            pack[j] = pack[j + 1];
        }
        edge_pack -= 1;
    }
}

void a_coder(long long *card, long long *a_key, long long p)
{
    long long help[COUNT];
    for(int i = 0; i < COUNT; i++){
        help[i] = card[i];
    }

    for(int i = 0; i < COUNT; i++){
        card[i] = module_power(help[i], a_key[0], p);
    }
    mix(card);
    prin_pack(card);
}

void b_coder(long long *card, long long *b_key, long long p)
{
    long long help[COUNT];
    for(int i = 0; i < COUNT; i++){
        help[i] = card[i];
    }
    
    for(int i = 0; i < COUNT; i++){
        card[i] = module_power(help[i], b_key[0], p);
    }
    
    mix(card);
    prin_pack(card);
}

void a_decoder(long long *card, long long *a_key, long long p)
{
    long long help[2];
    for(int i = 0; i < 2; i++){
        help[i] = card[i];
    }

    for(int i = 0; i < 2; i++){
        card[i] = module_power(help[i], a_key[1], p);
    }
}

void b_decoder(long long *card, long long *b_key, long long p)
{
    long long help[2];
    for(int i = 0; i < 2; i++){
        help[i] = card[i];
    }
    
    for(int i = 0; i < 2; i++){
        card[i] = module_power(help[i], b_key[1], p);
    }

}

void mental_poker()
{
    long long *card = malloc(sizeof(long long) * (COUNT + 1));

    for(int i = 0; i <= COUNT; i++){
        card[i] = 1 + rand() % 1000;
    }
    
    long long p = p_generation();
    long long *evk = (long long*)malloc(4);
    evk[0] = 0;
    long long ca, da;
    while(evk[0] != 1){
        ca = 1 + rand() % (p - 1);
        evklid(ca, (p - 1), evk);
        da = evk[2];
        if(da < 0)
            da += (p - 1);
        if(evk[0] == 1){
            evk[0] = 0;
            if(((ca * da) % (p - 1)) == 1)
                evk[0] = 1;
        }
    }
    long long *a_key = malloc(sizeof(long long) * 2);
    a_key[0] = ca;
    a_key[1] = da;

    evk[0] = 0;
    long long cb, db;
    while(evk[0] != 1){
        cb = 1 + rand() % (p - 1);
        evklid(cb, (p - 1), evk);
        db = evk[2];
        if(db < 0)
            db += (p - 1);
        if(evk[0] == 1){
            evk[0] = 0;
            if(((cb * db) % (p - 1)) == 1)
                evk[0] = 1;
        }
    }
    long long *b_key = malloc(sizeof(long long) * 2);
    b_key[0] = cb;
    b_key[1] = db;

    prin_pack(card);
    a_coder(card, a_key, p);
    b_coder(card, b_key, p);

    long long a_c[2];
    a_c[0] = card[0];
    a_c[1] = card[1];
    b_decoder(a_c, b_key, p);
    a_decoder(a_c, a_key, p);  

    printf("alica: %lld\t%lld\n", a_c[0], a_c[1]);
    
    long long b_c[2];
    b_c[0] = card[2];
    b_c[1] = card[3];
    a_decoder(b_c, a_key, p);
    b_decoder(b_c, b_key, p);  

    printf("bob: %lld\t%lld\n", b_c[0], b_c[1]);

    free(card);
}

int main()
{
    srand(time(NULL));
    mental_poker();
    return 0;
}
