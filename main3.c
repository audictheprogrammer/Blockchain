#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "exo5.h"
#include "exo6.h"


int main(){
    // Constantes
    const int TEST_CELL_KEY = 1; // valgrind passed
    const int TEST_READ_PUBLIC_KEYS1 = 1 ; // valgrind passed
    const int TEST_READ_PUBLIC_KEYS2 = 1; // valgrind passed
    const int TEST_CELL_PROTECTED = 1; // valgrind passed
    const int TEST_READ_PROTECTED = 1; // valgrind passed

    const int TEST_FILTER1 = 1; // valgrind passed
    const int TEST_FILTER2 = 1; // valgrind passed
    const int TEST_HASHTABLE_BASICS = 1; // valgrind passed
    const int TEST_FIND_POSITION = 1; // valgrind passed
    // Initialisation
    srand(time(NULL));

    // Tests pour exo5
    if (TEST_CELL_KEY){
        /* Test l'ensemble des fonctions suivantes:
           cell_en_tete
           delete_list_keys
           print_list_keys
        */
        Key* pKey = (Key*) malloc(sizeof(Key));
        Key* sKey = (Key*) malloc(sizeof(Key));
        init_pair_keys(pKey, sKey, 3, 7);
        CellKey* LCK = create_cell_key(pKey);

        printf("Avant cell_en_tete\n");
        print_list_keys(LCK);
        cell_en_tete(&LCK, sKey);
        printf("Apres cell_en_tete\n");

        printf("Avant delete_list_keys\n");
        print_list_keys(LCK);
        delete_list_keys(LCK);
        printf("Apres delete_list_keys\n");
    }

    if (TEST_READ_PUBLIC_KEYS1){
        generate_random_data(100, 10);
        FILE* f = fopen("keys.txt", "r");

        printf("Lit le fichier keys.txt\n");
        CellKey* cKey = read_public_keys(f);
        printf("Apres avoir lu le fichier keys.txt\n");

        print_list_keys(cKey);
        delete_list_keys(cKey);
        fclose(f);
    }

        if (TEST_READ_PUBLIC_KEYS2){
        FILE* f = fopen("candidates.txt", "r");

        printf("Lit le fichier condidates.txt\n");
        CellKey* cKey = read_public_keys(f);
        printf("Apres avoir lu le fichier candidates.txt\n");
        
        print_list_keys(cKey);
        delete_list_keys(cKey);
        fclose(f);
    }
    
    if (TEST_CELL_PROTECTED){
        /* Test l'ensemble des fonctions suivantes:
           ajout_protected
           delete_list_cell_protected
           print_cell_protected
        */
        Key* pKey = malloc(sizeof(Key));
        Key* sKey = malloc(sizeof(Key));
        init_pair_keys(pKey, sKey, 3, 7);
        char* mess1 = key_to_str(pKey);
        char* mess2 = key_to_str(sKey);
        Signature* sgn1 = sign(mess1, pKey);
        Signature* sgn2 = sign(mess2, sKey);
        Protected* pr1 = init_protected(pKey, mess1, sgn1);
        Protected* pr2 = init_protected(sKey, mess2, sgn2);
        free(mess1);
        free(mess2);
        
        CellProtected* LCP = create_cell_protected(pr1);
        printf("Avant ajout_protected\n");
        print_cellProtected(LCP);
        ajout_protected(&LCP, pr2);
        printf("Apres ajout_protected\n");
        print_cellProtected(LCP);

        delete_list_cell_protected(LCP);
    }

    if (TEST_READ_PROTECTED){
        FILE* f = fopen("declarations.txt", "r");
        
        printf("Lit le fichier declarations.txt\n");
        CellProtected* LCP = read_protected(f);
        printf("Apres avoir lu le fichier declarations.txt\n");

        print_cellProtected(LCP);
        delete_list_cell_protected(LCP);
        fclose(f);
    }

    // Tests pour exo6
    if (TEST_FILTER1){
        /* Test la fonction filter avec seulement des 
           signatures toutes correctes
        */
        char* nomfichier = "declarations_filter_correct.txt";
        FILE* f = fopen(nomfichier, "r");
        
        CellProtected* LCP = read_protected(f);
        printf("Avant filter\n");
        print_cellProtected(LCP);
        filter(&LCP);
        printf("Apres filter\n");
        print_cellProtected(LCP);
        
        fclose(f);  
        delete_list_cell_protected(LCP);
    }

    if (TEST_FILTER2){
        /* Test la fonction filter avec des 3 signatures
           incorrectes dont:
           - 2 en tete de liste
           - 1 en fin de liste.
        */
        char* nomfichier = "declarations_filter_incorrect.txt";
        FILE* f = fopen(nomfichier, "r");

        CellProtected* LCP = read_protected(f);
        printf("Avant filter\n");
        print_cellProtected(LCP);
        filter(&LCP);
        printf("Apres filter\n");

        print_cellProtected(LCP);
        delete_list_cell_protected(LCP);
        fclose(f); 
    }

    if (TEST_HASHTABLE_BASICS){
        /* Test l'ensemble des fonctions suivantes:
           create_hashcell
           hash_function (Peut-etre)
           delete_hashcell
           create_hashtable
           delete_hashtable
        */
        // Initialisation en supposant deja valide
        generate_random_data(9, 1);
        FILE* f = fopen("keys.txt", "r");
        CellKey* cKey = read_public_keys(f);

        // Debut du test
        printf("Debut test\n");
        HashTable* hTable = create_hashtable(cKey, 9);
        delete_hashtable(hTable);
        printf("Fin test\n");
        
        // Nettoyage
        while(cKey){
            CellKey* tmp = cKey;
            cKey = cKey->next;
            free(tmp);
        }
        fclose(f);
    
    }

    if (TEST_FIND_POSITION){
        // Test find position ainsi que compute winner
        generate_random_data(50, 3);
        FILE* f = fopen("keys.txt", "r");
        FILE* f1 = fopen("candidates.txt", "r");
        FILE* f2 = fopen("declarations.txt", "r");
        CellKey* cKey = read_public_keys(f);
        CellKey* candidat = read_public_keys(f1);
        CellProtected* declaration = read_protected(f2);
        CellKey* supp = cKey; 
        HashTable* hTable = create_hashtable(cKey, 50);

        //Debut du test
        printf("pos1 = %d\n", hash_function(cKey->data,50));
        int pos1 = find_position(hTable, cKey->data);
        cKey = cKey->next;
        int pos2 = find_position(hTable, cKey->data);
        printf("pos2 = %d\n", hash_function(cKey->data,50));
        cKey = cKey->next;
        int pos3 = find_position(hTable, cKey->data);
        printf("pos3 = %d\n", hash_function(cKey->data,50));
        printf("Find pos1 = %d\n", pos1);
        printf("Find pos2 = %d\n", pos2);
        printf("Find pos3 = %d\n", pos3);

        // Nettoyage
        Key* gagnant = compute_winner(declaration,candidat,supp,3,50);
        char* str_gagnant = key_to_str(gagnant);
        printf("Le gagnant est %s\n", str_gagnant);
        free(gagnant);
        free(str_gagnant);

        delete_list_cell_protected(declaration);
        delete_hashtable_vide(hTable);
        delete_list_keys_vide(candidat);
        delete_list_keys_vide(supp);
        fclose(f);
        fclose(f1);
        fclose(f2);

    }

    return 0;
}