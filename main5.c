#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "exo8.h"
#include <dirent.h>
#include "exo9.h"


int main(int argc, char* argv[]){
    // Constantes
    const int TEST_CELLTREE_BASICS = 1; // valgrind passed
    const int TEST_CELLTREE_BASICS2 = 1; // valgrind passed
    const int TEST_GLOBAL = 1; // valgrind passed

    // Tests pour exo8
    if (TEST_CELLTREE_BASICS){
        /* Test l'ensemble des fonctions suivantes:
           create_node
           print_tree
           delete_node
        */
        // Fonctions supposees valides
        generate_random_data(10, 2);
        FILE* f_key = fopen("keys.txt", "r");
        FILE* f_sign = fopen("declarations.txt", "r");
        CellKey* list_key = read_public_keys(f_key); 
        CellProtected* list_prot = read_protected(f_sign);  

        Block* b = (Block*) malloc(sizeof(Block));
        b->author = list_key->data;
        b->votes = list_prot;
        b->hash = (unsigned char*)strdup("test_hash");
        b->previous_hash = (unsigned char*)strdup("test_previousHash");
        b->nonce = 0;

        // Debut test
        CellTree* node = create_node(b);
        print_tree(node);
        delete_node(node);
        // Fin du test
        printf("Fin test tree\n");

        // Nettoyage avec des fonctions suposees valides.
        list_key->data = NULL;
        delete_list_keys(list_key);
        fclose(f_key);
        fclose(f_sign);

    }

    if (TEST_CELLTREE_BASICS2){

        FILE* f1 = fopen("BlockDecla1.txt", "r");
        FILE* f2 = fopen("BlockDecla2.txt", "r");
        FILE* f3 = fopen("BlockDecla3.txt", "r");
        FILE* f4 = fopen("BlockDecla4.txt", "r");

        Block* b1 = (Block*) malloc(sizeof(Block));
        b1->author = str_to_key("(ef89,164bf)");
        b1->votes = read_protected(f1);
        b1->previous_hash = (unsigned char*) strdup("");
        b1->nonce = 0;
        b1->hash = (unsigned char*) block_to_str(b1);
        compute_proof_of_work(b1,1);

        Block* b2 = (Block*) malloc(sizeof(Block));
        b2->author = str_to_key("(ef89,164bf)");
        b2->votes = read_protected(f2);
        b2->previous_hash = (unsigned char*) strdup((const char*) b1->hash);
        b2->nonce = 0;
        b2->hash = (unsigned char*) block_to_str(b2);
        compute_proof_of_work(b2,1);

        Block* b3 = (Block*) malloc(sizeof(Block));
        b3->author = str_to_key("(ef89,164bf)");
        b3->votes = read_protected(f3);
        b3->previous_hash = (unsigned char*) strdup((const char*) b2->hash);
        b3->nonce = 0;
        b3->hash = (unsigned char*) block_to_str(b3);
        compute_proof_of_work(b3,1);

        Block* b4 = (Block*) malloc(sizeof(Block));
        b4->author = str_to_key("(ef89,164bf)");
        b4->votes = read_protected(f4);
        b4->previous_hash = (unsigned char*) strdup((const char*) b3->hash);
        b4->nonce = 0;
        b4->hash = (unsigned char*) block_to_str(b4);
        compute_proof_of_work(b4,1);

        // create_node
        CellTree* node1 = create_node(b1);
        CellTree* node2 = create_node(b2);
        CellTree* node3 = create_node(b3);
        CellTree* node4 = create_node(b4);

        // add_child
        add_child(node1, node3);
        add_child(node1, node2);
        add_child(node3, node4);

        // update_height
        printf("La hauteur de node1 devrait etre egale a 2, et elle est de : %d \n",
        node1->height);

        // print_tree
        print_tree(node1);

        // highest child
        printf("On devrait avoir %s, et on a : %s \n", 
        node3->block->hash, highest_child(node1)->block->hash);

        // last node
        printf("On devrait avoir %s, et on a : %s \n",
        node4->block->hash, last_node(node1)->block->hash);

        // regroup_cellP mais un peu long à print
        print_cellProtected(node1->block->votes);
        print_cellProtected(node3->block->votes);
        print_cellProtected(node4->block->votes);
        printf("\n");
        CellProtected* list_P = regroup_cellP(node1);
        print_cellProtected(list_P);

        // Nettoyage avec des fonctions suposees valides.
        set_tree_block_votes_null(node1);
        delete_tree(node1);

        fclose(f1);
        fclose(f2);
        fclose(f3);
        fclose(f4);

    }

    // Test global
    if (TEST_GLOBAL){
 
        system("rm -f Blockchain/*");
        if(argc != 3){
            printf("Nb parametre incorrect !\n");
            return -1;
        }
        if (atoi(argv[1]) < 10){
            printf("Premier parametre doit etre > 10 !\n");
            return -1;
        }
        if (atoi(argv[2]) <= 0){
            printf("Deuxieme parametre doit etre > 0 !\n");
            return -1;
        }
        
        generate_random_data(atoi(argv[1]), atoi(argv[2]));
        // Declarations
        FILE* f = fopen("declarations.txt","r");
        if (f == NULL){
            return -1;
        }
        CellProtected* declaration = read_protected(f);
        CellProtected* declaration_supp = declaration;
        fclose(f);

        // Cle citoyens
        f = fopen("keys.txt","r");
        if(f == NULL){
            delete_list_cell_protected(declaration);
            return -1;
        }
        CellKey* citoyens = read_public_keys(f);
        fclose(f);
        // Cle candidates
        f = fopen("candidates.txt","r");

        if(f == NULL){
            delete_list_cell_protected(declaration);
            delete_list_keys(citoyens);
            return -1;
        }
        CellKey* candidates = read_public_keys(f);
        fclose(f);

        int cpt = 0;
        Key* autheur;
        CellTree* tree = NULL;
        while(declaration){
            autheur = declaration->data->pKey;
            if(cpt == 10){
                cpt = 0;
                create_block(tree, autheur, 1);
                char* nom = key_to_str(autheur);
                add_block(1, nom);
                free(nom);
                if(tree != NULL){
                    delete_tree(tree);
                }
                tree = read_tree(); 
            }
            submit_vote(declaration->data);
            cpt++;
            declaration = declaration->next;
        }
        if(cpt == 10){
            cpt = 0;
            create_block(tree, autheur, 1);
            char* nom = key_to_str(autheur);
            add_block(1,nom);
            free(nom);
        }
        if(tree != NULL){
            delete_tree(tree);
        }
        tree = read_tree();
        print_tree(tree);
        Key* gagnant = compute_winner_BT(tree, candidates, 
        citoyens, atoi(argv[2]), atoi(argv[1]));

        char* str_gagnant = key_to_str(gagnant);
        printf("Le gagnant est %s\n", str_gagnant);
        free(str_gagnant);
        free(gagnant);

        set_tree_block_votes_null(tree);
        delete_list_cell_protected(declaration_supp);
        delete_list_keys_vide(candidates);
        delete_list_keys_vide(citoyens);
        delete_tree(tree);

    }

    return 0;
}