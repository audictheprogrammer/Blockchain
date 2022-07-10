#ifndef EXO3_H
#define EXO3_H
#include "exo2.h"
#define K 3000


typedef struct key{
    long val;
    long n;
}Key;

typedef struct signature{
    long* content;
    int size;
}Signature;

typedef struct protected{
    Key* pKey;
    char* mess;
    Signature* sign;
}Protected;

// Manipulation de cles
void init_key(Key* key, long val, long n);
void init_pair_keys(Key* pKey, Key *sKey, long low_size, long up_size);
char* key_to_str(Key* key);
Key* str_to_key(char* str);
int sameKey(Key* k1, Key* k2);

// Signature
Signature* init_signature(long* content, int size);
Signature* sign(char* mess, Key* sKey);
char* signature_to_str(Signature* sgn);
Signature* str_to_signature(char* str);

// Declarations signees
Protected* init_protected(Key* pKey, char* mess, Signature* sgn);
int verify(Protected* pr);
char* protected_to_str(Protected* p);
Protected* str_to_protected(char* str);

#endif