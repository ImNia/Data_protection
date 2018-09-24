#ifndef __HASH__H
#define __HASH__H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHTAB_SIZE 71
#define HASHTAB_MUL 31

struct listnode {
	int key;
    int value;
    struct listnode *next;
};

unsigned int hashtab_hash(int key);
void hashtab_init(struct listnode **hashtab);
void hashtab_add(struct listnode **hashtab, int key, int value);
struct listnode *hashtab_lookup(struct listnode **hashtab, int key, int value);
void hashtab_print(struct listnode **hashtab);

#endif
