#include "kript.h"
#include "cipher.h"
#include "signature.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main()
{
    srand(time(NULL));

    sign_rsa();
    sign_el_gamal();
    sign_dsa();

    return 0;
}
