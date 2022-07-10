#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exo8.h"


CellTree * create_node(Block* b){
    /* Alloue un noeud a partir d'un bloc b.
       Renvoie le noeud cree.
    */
    CellTree* res = (CellTree *) malloc(sizeof(CellTree));
    if (res == NULL){
        printf("Erreur d'allocation\n");
        return NULL;
    }
    res->block = b;
    res->father = NULL;
    res->firstChild = NULL;
    res->nextBro = NULL;
    res->height= 0;

    return res;

}

int update_height(CellTree * father, CellTree * child){
    /* Met a jour la hauteur du pere father quand son fils child a ete modifie.
       Retourne 1 si on a modifie la taille du pere.
       Retourne 0 sinon.
    */
    if (father->height > child->height+1){
        return 0;
    }
    father->height = child->height+1;
    return 1;
}

void add_child(CellTree* father, CellTree* child){
    /* Ajoute un fils child a father en mettant a jour la
       hauteur de tous les ascendants.
    */
    if (father->firstChild == NULL){
        father->firstChild = child;
        child->father = father;    
    }
    else{
        child->father = father;
        child->nextBro = father->firstChild;
        father->firstChild = child;   
    }

    while(father != NULL && update_height(father, child) == 1){
        child = father;
        father = father->father;
    }
}

void print_tree(CellTree * tree){
    /* Affiche l'arbre tree : 
       pour chaque noeud, la hauteur du noeud et
       la valeur hachee du bloc sont affichees.
       Le parcours se fait etage par etage.
    */

    if (tree == NULL){
        return;
    }
    CellTree* courant = tree;
    while (courant) {
        printf("Hauteur = %d , Hash = %s\n", courant->height, courant->block->hash);
        courant = courant->nextBro;
    }
    printf("\n");

    print_tree(tree->firstChild);
}

void delete_node(CellTree* node){
    /* Supprime le noeud node en supprimant totalement le block
       present dans le noeud.
    */
    free(node->block->author);
    if (node->block->votes != NULL){
        delete_list_cell_protected_contenu(node->block->votes);
    }
    delete_block(node->block);
    free(node);
}

void delete_tree(CellTree* tree){
    /* Supprime l'arbre tree en entier. 
    */
    if (tree == NULL){
        return;
    }
    delete_tree(tree->firstChild);
    delete_tree(tree->nextBro);

    delete_node(tree);
}

CellTree* highest_child(CellTree* cell){
    /* Renvoie le noeud fils de cell avec la plus grande hauteur.
    */
    if (cell == NULL){
        return NULL;
    }
    CellTree* filsCourant = cell->firstChild;
    while (filsCourant != NULL && filsCourant->height != cell->height-1 ){
        filsCourant = filsCourant->nextBro;
    }
    return filsCourant;
}

CellTree* last_node(CellTree* cell){
    /* Renvoie le dernier noeud de la plus longue chaine de cell.
    */
    CellTree* filsCourant = highest_child(cell);
    if (filsCourant == NULL){
        return cell;
    }
    
    while (filsCourant->firstChild != NULL) {
        filsCourant = highest_child(filsCourant);
    }
    return filsCourant;
}

CellProtected* fusion_declaration(CellProtected* cell1, CellProtected* cell2){
    /*  Fusionne les liste cell1 et cell2 en ajoutant cell2 a la fin de cell1.
        Si cell1 est vide, renvoie cell2.
        Sinon, retourne un pointeur sur la tete de liste de cell1.

    */
    if (cell1 == NULL){
        return cell2;
    }
    CellProtected* tmp = cell1;
    while(tmp->next){
        tmp = tmp->next;
    }
    tmp->next = cell2;
    return cell1;
}

CellProtected* regroup_cellP(CellTree* cell){
    /* Fusionne les listes chainees de declarations 
       de la plus longue chaine de bloc de cell.
       Retourne la fusion des declaration ainsi obtenu.
    */
    CellProtected* cell1 = cell->block->votes;
    CellTree* courant = highest_child(cell);
    while (courant != NULL){
        CellProtected* cell2 = courant->block->votes;
        cell1 = fusion_declaration(cell1, cell2);
        courant = highest_child(courant);
    }
    return cell1;
}

void set_tree_block_votes_null(CellTree* t){
    /*  Met le champ vote des fils de la plus longue chaine de t a NULL 
    */
    CellTree* child = highest_child(t);
    if(child == NULL){
        return;
    }
    set_tree_block_votes_null(child);
    child->block->votes = NULL;
}