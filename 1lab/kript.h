#ifndef __KRIPT__H
#define __KRIPT__H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long module_power(long long base, long long power_exponent, long long moduli);
void evklid(long long _older, long long _junior, long long *older);
void diffie_hellman();
int child_giant(long long base, long long moduli, long long answer);

int p_generation();
int g_generation(int p);

#endif
