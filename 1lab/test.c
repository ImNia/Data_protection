#include "kript.h"
#include "cipher.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main()
{
    srand(time(NULL));

//    shamir();
//    el_gamal();
//    vernam();
    rsa();

    return 0;
}
