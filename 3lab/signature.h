#ifndef __SIGNATURE__H
#define __SIGNATURE__H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <openssl/sha.h>

#include "kript.h"

int sha256_sign(int message, unsigned char *outputBuffer);
void sign_rsa_coder(unsigned char *hash, long long *signature_bank);
int sign_rsa_decoder(unsigned char *hash, long long *signature_bank);

#endif
