#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "exo9.h"

void submit_vote(Protected* p){
    /* Permet a un citoyen de soummetre un vote p
       en l'inscrivant dans le fichier Pending_votes.txt .
    */
    FILE* f = fopen("Pending_votes.txt", "a+");
    char* str_p = protected_to_str(p);
    fprintf(f,"%s\n", str_p);
    free(str_p);
    fclose(f);
}

void create_block(CellTree* tree, Key* author, int d){
    /* Cree un bloc valide en 6 etapes:
       - recupere les votes en attente dans Pending_votes.txt
       - Supprime le fichier Pending_votes.txt apres 
         avoir cree le bloc.
       - Initialise l'autheur du bloc a author.
       - Recherche du bloc precedent dans l'abre.
       - Hash le bloc jusqu'a avoir d zero successif.
       - Ecrit le bloc obtenu dans un fichier appelle 
         Pending_Block
    */
    Block * b = (Block*) malloc(sizeof(Block));
    if (b == NULL){
        printf("Erreur d'allocation\n");
        return ;
    }

    b->author = author;
    b->hash = NULL;
    FILE* f = fopen("Pending_votes.txt", "r");
    b->votes = read_protected(f);
    CellTree* last = last_node(tree);

    if (last == NULL){
        b->previous_hash = NULL;
    }
    else {
        b->previous_hash = (unsigned char*) strdup((const char*)last->block->hash);
    }

    compute_proof_of_work(b, d);
    fclose(f);
    remove("Pending_votes.txt");
    FILE* f2 = fopen("Pending_block", "w");
    ecrireBlock(b, f2);
    delete_list_cell_protected_contenu(b->votes);
    delete_block(b);
    fclose(f2);
}

void add_block(int d, char* name){
    /* Verifie que le bloc dans Pending_block est valide grace a d.
       Si c'est le cas,cree un fichier name representant le bloc 
       puis l'ajoute dans Blockchain.
    */
    FILE* f = fopen("Pending_block", "r");
    Block* b = lireBlock(f);
    if (verify_block(b, d) == 0){
        // Bloc non valide
        delete_list_cell_protected_contenu(b->votes);
        free(b->author);
        delete_block(b);
    }else{
        // Bloc valide
        char nom_fichier[256];
        sprintf(nom_fichier, "./Blockchain/%s", name);
        FILE *f2 = fopen(nom_fichier, "w");
        ecrireBlock(b, f2);
        free(b->author);
        delete_list_cell_protected_contenu(b->votes);
        delete_block(b);
        fclose(f2);
    }
    fclose(f);
    remove("Pending_block");
}

CellTree* read_tree(){
    /* Construit l'arbre correspondant aux blocs contenus
       dans le repertoire Blockchain en trois etapes:
        - Cree un noeud pour chaque bloc et le stock dans une CellTree**.
        - Trouve et etablie les liens de parentes.
        - Trouve la racine de l'arbre.
       Renvoie la racine de l'arbre ainsi cree.
    */
    CellTree** T = (CellTree**) malloc(sizeof(CellTree*));
    if(T == NULL){
        printf("Erreur d'allocation'\n");
        return NULL;
    }
    
    DIR* rep = opendir("./Blockchain/");
    
    // Etape 1
    int k = 1;
    if (rep != NULL){
        struct dirent* dir;
        while ((dir = readdir(rep))){
            if(strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0){
                char nom_fichier[2048];
                sprintf(nom_fichier, "./Blockchain/%s", dir->d_name);
                FILE* f = fopen(nom_fichier, "r");
                Block* b = lireBlock(f);
                if (k == 1){
                    T[k-1] = create_node(b);
                }
                else{
                    T = realloc(T, k * sizeof(CellTree*));
                    if(T == NULL){
                        printf("Erreur de realloc\n");
                        return NULL;
                    }
                    T[k-1] = create_node(b);
                }
                fclose(f);
                k++;
            }
        }
    }
    closedir(rep);
    k--;

    // Etape 2
    for (int i = 0; i < k; i++){
        CellTree* father = T[i];
        for (int j = 0; j < k; j++){
            CellTree* child = T[j];
            if(child->block->previous_hash != NULL){
                if (strcmp((const char*) father->block->hash, (const char*) child->block->previous_hash) == 0){
                    child->father = father;
                    add_child(father,child);
                }
            }
        }
    }

    // Etape 3
    CellTree* racine;
    for (int i = 0; i < k; i++){
        if (T[i]->father == NULL){
            racine = T[i];
            break;
        }
    }

    free(T);
    return racine;
}

Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
    /* Determine le gagnant de l'election en se basant sur la 
       plus longue chaine de l'arbre en 3 etapes:
       - Extraction de la liste des declarations de vote de la
         plus longue chaine de l'abre tree.
       - Suppression des declarations de vote non valides
       - Calcule le vainqueur de l'election.
       Renvoie la cle du vainqueur de l'election.
    */
    CellProtected* declaration = regroup_cellP(tree);
    filter(&declaration);
    return compute_winner(declaration, candidates, voters, sizeC, sizeV);

}











