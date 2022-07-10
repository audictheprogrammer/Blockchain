#ifndef EXO6_H
#define EXO6_H
#include "exo5.h"

typedef struct hashcell{
    Key* key;
    int val;
} HashCell;

typedef struct hashtable{
    HashCell** tab;
    int size;
} HashTable;

void filter(CellProtected** LCP);
// Manipulation de HashCell
HashCell* create_hashcell(Key* key);
int hash_function(Key* key, int size);
int find_position(HashTable* t, Key* key);
void delete_hashcell(HashCell* t);

// Manipulation de HashTable
HashTable* create_hashtable(CellKey* keys, int size);
void delete_hashtable(HashTable* t);
void delete_hashtable_vide(HashTable* t);

Key* compute_winner(CellProtected* decl, CellKey* candidates, 
                    CellKey* voters, int sizeC, int sizeV);

#endif
