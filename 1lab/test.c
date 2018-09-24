#include "kript.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));

//    int x_module = module_power(1331, 723, 171);
//    printf("x_module = %d\n", x_module);

/*    long long *x_evklid = (long long*)malloc(4);
    x_evklid = evklid(100, 57);
    printf("Nod = %lld\nx = %lld\ny = %lld\n", x_evklid[0], x_evklid[1], x_evklid[2]);
*/

//    diffie_hellman();

    int x = child_giant(1331, 171, 134);
    printf("x = %d\n", x);

    return 0;
}
