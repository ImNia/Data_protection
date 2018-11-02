#ifndef __SIGNATURE__H
#define __SIGNATURE__H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <openssl/sha.h>

#include "kript.h"

void sign_rsa();
void sign_el_gamal();
void sign_dsa();

#endif
