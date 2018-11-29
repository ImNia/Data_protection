#include "kript.h"
#include "signature.h"

#define BASE_BANK 10
#define SIZE_HASH 65
int bank_data[BASE_BANK][SIZE_HASH];

void client(int count_purchase);
void bank_from_client(unsigned char *hash, int count_purchase);

void bank_from_client(unsigned char *hash, int count_purchase)
{
    //хеш номер купюы
    for(int i = 0; i < SIZE_HASH; i++){
        bank_data[count_purchase][i] = hash[i];
    }

    long long *signature_bank = malloc(sizeof(long long) * SIZE_HASH);
    sign_rsa_coder(hash, signature_bank);
    for(int i = 0; i < sizeof(signature_bank); i++)
        printf("%lld\t", signature_bank[i]);
    printf("\n\n");
    int gg = sign_rsa_decoder(hash, signature_bank);
    if(gg == 0)
        printf("\n\nCorrect\n\n");
}

void client(int count_purchase)
{
    int currency_number = 1 + rand() % 100000;

    unsigned char *hash = malloc(sizeof(unsigned char) * SIZE_HASH);
    sha256_file(currency_number, hash);
    for(int i = 0; i < 65; i++)
        printf("%c\t", hash[i]);
    printf("\n\n");
    bank_from_client(hash, count_purchase);
}

int main()
{
    int count_purchase = 0;
    client(count_purchase);
    for(int i = 0; i < SIZE_HASH; i++)
        printf("%c\t", bank_data[count_purchase][i]);
    count_purchase++;
    return 0;
}
