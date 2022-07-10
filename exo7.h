#ifndef EXO7_H
#define EXO7_H
#include <openssl/sha.h>
#include "exo6.h"

typedef struct block{
    Key* author;
    CellProtected* votes;
    unsigned char* hash;
    unsigned char* previous_hash;
    int nonce;
}Block;

void ecrireBlock(Block* b, FILE* f);
Block* lireBlock(FILE* f);
char* block_to_str(Block* b);
unsigned char* hash_SHA256(char * c);
void compute_proof_of_work(Block* b, int d);
int verify_block(Block* b, int d);
void delete_block(Block* b);

#endif