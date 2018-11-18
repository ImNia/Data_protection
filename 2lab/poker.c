#include <stdio.h>
#include <stdlib.h>

#include "kript.h"
#define COUNT 52
#define PLAYER 3
#define CARD 2

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

void coder(long long *card, long long *key, long long p)
{
    long long help[COUNT];
    for(int i = 0; i < COUNT; i++){
        help[i] = card[i];
    }

    for(int i = 0; i < COUNT; i++){
        card[i] = module_power(help[i], key[0], p);
    }
    mix(card);
//    prin_pack(card);
}

void decoder(long long *card, long long *key, long long p)
{
    long long help[2];
    for(int i = 0; i < 2; i++){
        help[i] = card[i];
    }

    for(int i = 0; i < 2; i++){
        card[i] = module_power(help[i], key[1], p);
    }
}

void key_gen(long long *key, long long p)
{
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
    key[0] = ca;
    key[1] = da;

}

void mental_poker()
{
    long long original_card[4][15];
    original_card[0][0] = 1; //\xe2\x99\xa0; //♠
    original_card[1][0] = 2; //\xe2\x99\xa5; //♥
    original_card[2][0] = 3; //\xe2\x99\xa3; //♣
    original_card[3][0] = 4; //\xe2\x99\xa6; //♦
    
    long long *card = malloc(sizeof(long long) * (COUNT + 1));

    for(int i = 0; i <= COUNT; i++){
        card[i] = 1 + rand() % 1000;
        for(int j = 0; j < i; j++){
            if(card[i] == card[j])
                card[i] = 1 + rand() % 1000;

        }
    }
    printf("\n\n");
    for(int i = 0; i < 4; i++){
        for(int j = 1; j < 15; j++){
            original_card[i][j] = card[i * 13 + j - 1]; //j + 1;
//            printf("%lld\t", original_card[i][j]);
//Отрисовка карт
            {            
            if(i == 0)
                printf("Piki\t");
            if(i == 1)
                printf("Chervi\t");
            if(i == 2)
                printf("Kresti\t");
            if(i == 3)
                printf("Bubi\t");
            if(j <= 10){
                printf("%d\t%lld\n", j + 1, original_card[i][j]);
            }
            if(j == 11)
                printf("Balet\t%lld\n", original_card[i][j]);
            if(j == 12)
                printf("Dama\t%lld\n", original_card[i][j]);
            if(j == 13)
                printf("Korol\t%lld\n", original_card[i][j]);
            if(j == 14)
                printf("Tuz\t%lld\n", original_card[i][j]);
            }
        }
        printf("\n\n");
    }
    
    long long p = p_generation();
    

    long long **key = malloc(sizeof(**key) * PLAYER);
    for(int i = 0; i < PLAYER; i++){
        key[i] = malloc(sizeof(*key) * 2);
    }
    for(int i = 0; i < PLAYER; i++){
        key_gen(key[i], p);
    }

//    prin_pack(card);
    for(int i = 0; i < PLAYER; i++){
        coder(card, key[i], p);
    }

    long long card_player[PLAYER][2];
    for(int num_player = 0; num_player < PLAYER; num_player++){
        for(int k = 0; k <= CARD; k++){
            card_player[num_player][k] = card[(num_player * CARD) + k];
        }
        for(int j = 0; j < PLAYER; j++){
            if(j != num_player)
                decoder(card_player[num_player], key[j], p);
        }
        decoder(card_player[num_player], key[num_player], p);  
        
        long long help_card;
        printf("Player[%d]: \n", num_player);
        for(int l = 0; l < CARD; l++){
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 15; j++){
                    if(card_player[num_player][l] == original_card[i][j]){
                        help_card = card_player[num_player][l];
        {          
        if(i == 0)
            printf("Piki\t");
        if(i == 1)
            printf("Chervi\t");
        if(i == 2)
            printf("Kresti\t");
        if(i == 3)
            printf("Bubi\t");
        if(j <= 10)
            printf("%d\t%lld\n", j + 1, help_card);
        if(j == 11)
            printf("Balet\t%lld\n", help_card);
        if(j == 12)
            printf("Dama\t%lld\n", help_card);
        if(j == 13)
            printf("Korol\t%lld\n", help_card);
        if(j == 14)
            printf("Tuz\t%lld\n", help_card);
        }
                    }
                }
            }
        }
        printf("\n");
    }

    free(card);
    free(key);
}

int main()
{
    srand(time(NULL));
    mental_poker();
    return 0;
}
