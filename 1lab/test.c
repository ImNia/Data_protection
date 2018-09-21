#include "kript.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));

    int x = child_giant(2, 23, 9);
    printf("x = %d\n", x);

    return 0;
}
