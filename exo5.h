#ifndef EXO5_H
#define EXO5_H
#include "exo4.h"

typedef struct cellKey{
    Key* data;
    struct cellKey* next;
} CellKey;

typedef struct cellProtected {
    Protected* data;
    struct cellProtected* next;
} CellProtected;

// Manipulation de liste de cles
CellKey* create_cell_key(Key* key);
void cell_en_tete(CellKey** cKey, Key* key);
CellKey* read_public_keys(FILE* f);
void print_list_keys(CellKey* LCK);
void delete_cell_key(CellKey* c);
void delete_list_keys(CellKey* LCK);
void delete_list_keys_vide(CellKey *LCK);


// De liste de declarations signees
CellProtected* create_cell_protected(Protected* pr);
void ajout_protected(CellProtected** LCP, Protected* p);
CellProtected* read_protected (FILE* f);
void print_cellProtected(CellProtected* LCP);
void delete_cell_protected(CellProtected* c);
void delete_list_cell_protected(CellProtected *LCP);
void delete_list_cell_protected_contenu(CellProtected *LCP);
void delete_list_cell_protected_elem(CellProtected *LCP);


#endif