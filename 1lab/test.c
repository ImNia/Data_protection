#include "kript.h"
#include "cipher.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main()
{
    srand(time(NULL));

    rsa();

    return 0;
}
