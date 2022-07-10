#ifndef EXO8_H
#define EXO8_H
#include "exo7.h"

typedef struct block_tree_cell{
    Block* block;
    struct block_tree_cell* father;
    struct block_tree_cell* firstChild;
    struct block_tree_cell* nextBro;
    int height;
}CellTree;

CellTree* create_node(Block* b);
int update_height(CellTree * father, CellTree * child);
void add_child(CellTree* father, CellTree* child);
void print_tree(CellTree * tree);
void delete_node(CellTree* node);
void delete_tree(CellTree* tree);
CellTree* highest_child(CellTree * cell);
CellTree* last_node(CellTree* cell);
CellProtected* fusion_declaration(CellProtected* cell1, CellProtected* cell2);
CellProtected* regroup_cellP(CellTree* cell);
void set_tree_block_votes_null(CellTree* t);
#endif