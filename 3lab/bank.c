#include <string.h>

#include "kript.h"
#include "signature.h"

#define BASE_BANK 10
#define SIZE_HASH 65
int bank_data[BASE_BANK][SIZE_HASH];
int score;

void client(int count_purchase);
void bank_from_client(unsigned char *hash, int count_purchase, long long *signature);
void store(int face_value, int currency_number, long long *signature_bank);
int bank_for_store(int face_value, int currency_number, long long *signature_bank);

void bank_from_client(unsigned char *hash, int count_purchase, long long *signature_bank)
{
    //хеш номер купюры
    for(int i = 0; i < SIZE_HASH; i++){
        bank_data[count_purchase][i] = hash[i];
    }

    sign_rsa_coder(hash, signature_bank);
}

void client(int count_purchase)
{
    int face_value;
    printf("Enter face value:");
    scanf("%d", &face_value);
    printf("\n");
    int currency_number = 1 + rand() % 100000;
    unsigned char *hash = malloc(sizeof(int) * SIZE_HASH);
    sha256_sign(currency_number, hash);
    long long *signature_bank = malloc(sizeof(long long) * SIZE_HASH);
    bank_from_client(hash, count_purchase, signature_bank);
    
    store(face_value, currency_number, signature_bank);
}

int bank_for_store(int face_value, int currency_number, long long *signature_bank)
{
    unsigned char *hash = malloc(sizeof(int) * SIZE_HASH);
    sha256_sign(currency_number, hash);
    //Сейчас будет убого, простите
    for(int i = 0; i < BASE_BANK; i++){
        for(int j = 0; j < SIZE_HASH; j++){
            if(hash[j] != bank_data[i][j])
                break;
            if((j - 1) == sizeof(hash))
                printf("Correct currence_number!!\n");
        }
/*        if(strcmp(hash, bank_data[i]) == 0){
            printf("Currency_number is correct\n");
        }
*/
    }

    int test_signature = sign_rsa_decoder(hash, signature_bank);
    if(test_signature == 0){
        if(score < face_value){
            printf("Insufficient funds!\n");
            exit(EXIT_FAILURE);
        }else{
            score -= face_value;
            printf("Customer account: %d\n", score);
        }
        return 0;
    }else{
        printf("Signature uncorrect!\n");
    }
    return 1;
}

void store(int face_value, int currency_number, long long *signature_bank)
{
    int test = bank_for_store(face_value, currency_number, signature_bank);
    if(test == 0){
        printf("Product your!\n\n");
    }else{
        printf("Signature not correct!\n\n");
    }
}

int main()
{
    printf("Enter client initial balance:");
    scanf("%d", &score);
    int count_purchase = 0;
    while(1){
        client(count_purchase);
        count_purchase++;
    }
    return 0;
}
