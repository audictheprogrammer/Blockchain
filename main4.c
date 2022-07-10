#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "exo7.h"


void perf_compute_proof_of_work(char* filename, Block* b, int d_start, int d_finish){
    FILE* f_perf = fopen(filename, "w");
    for (int i = d_start; i <= d_finish; i++){
        clock_t temps_initial = clock();
        compute_proof_of_work(b, i);
        clock_t temps_final = clock();
        double temps_cpu = ((double) ( temps_final - temps_initial) ) / CLOCKS_PER_SEC;
        printf("d=%d temps=%f\n", i, temps_cpu);
        fprintf(f_perf, "%d %f\n", i, temps_cpu);
    }
    fclose(f_perf);
}

int main(){

    FILE* f = fopen("BlockDecla1.txt", "r"); 

    Block* b = (Block*) malloc(sizeof(Block));
    b->author = str_to_key("(19f7,41b1)");
    b->votes = read_protected(f);
    b->previous_hash = (unsigned char*) strdup("test_previousHash");
    b->nonce = 0;
    b->hash = (unsigned char*) block_to_str(b);

    compute_proof_of_work(b, 1);
    // Mesure le temps pour compute_proof_of_work;
    // perf_compute_proof_of_work("Compute_proof_of_work.txt", b, 0, 7);
    
    FILE* f_block = fopen("ecrireBlock.txt", "w");
    ecrireBlock(b, f_block);

    char* str1 = key_to_str(b->author);
    printf("Vérifier que dans le fichier ecrireBlock.txt on a : \n");
    printf("%s \n", str1);
    printf("%s \n", b->hash);
    printf("%s \n", b->previous_hash);
    printf("%d \n", b->nonce);
    print_cellProtected(b->votes);

    fclose(f_block);
    free(str1);

    f_block = fopen("ecrireBlock.txt", "r");
    Block* b1 = lireBlock(f_block);
    char* str2 = key_to_str(b1->author);
    printf("\nInversement, le bloc ci dessous devrais contenir les mêmes données que précédemment : \n");
    printf("%s \n", str2);
    printf("%s \n", b1->hash);
    printf("%s \n", b1->previous_hash);
    printf("%d \n", b1->nonce);
    print_cellProtected(b1->votes);

    printf("\nVérification de block_to_str : \n");
    char* tmp = block_to_str(b1);
    printf("%s\n", tmp);
    free(tmp);
    free(str2);
    free(b1->author);
    delete_list_cell_protected_contenu(b1->votes);
    delete_block(b1);
    fclose(f_block);

    printf("\n Est ce que la valeur hachée de b commence bien par 1 zéro ? \n");
    printf("%s \n", b->hash);

    if (verify_block(b, 1) == 1 ) {
        printf("Bloc valide.\n");
    }
    else {
        printf("Block non valide.\n");
    }

    free(b->author);
    delete_list_cell_protected_contenu(b->votes);
    delete_block(b);
    fclose(f);

    return 0;

}