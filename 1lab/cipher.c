#include <stdio.h>
#include <stdlib.h>

#include "kript.h"

int d_generation(int c_number, int p)
{
    int d_number = 1;
    while(((c_number * d_number) % (p - 1)) != 1){
        d_number = 1 + rand() % 1000; 
    }
    return d_number;
}
/* Шифр Шамиля.
 * */
void shamil(int message)
{
    int p = p_generation();
    printf("p: %d\n", p);

    int ca = 1 + rand() % 1000;
    int da = d_generation(ca, p);

    int cb = 1 + rand() % 1000;
    int db = d_generation(cb, p);

    int x1, x2, x3, x4;
    x1 = module_power(message, ca, p);
    printf("x1: %d\n", x1);
    x2 = module_power(x1, cb, p);
    printf("x2: %d\n", x2);
    x3 = module_power(x2, da, p);
    printf("x3: %d\n", x3);
    x4 = module_power(x3, db, p);
    printf("x4: %d\n", x4);
}

void el_gamal(int message)
{
    int p = p_generation();
    int g = p_generation();

    int ca = 1 + rand() % (p - 1);
    int da = module_power(g, ca, p);
    int cb = 1 + rand() % (p - 1);
    int db = module_power(g, cb, p);

    int k = 1 + rand() % (p - 2);
    int r = module_power(g, k, p);
    int e = message * module_power(db, k, p);

    int message_b = e * module_power(r, p - 1 - cb, p);
    printf("%d\n", message_b);
}

int main()
{
    srand(time(NULL));
//    shamil(10);
    el_gamal(10);
    return 0;
}
