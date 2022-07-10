#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "exo3.h"
#include "exo4.h"

void print_long_vector(long *result, int size){ 
    
    printf("Vector: [");
    for (int i = 0; i < size; i++){
        printf("%lx \t", result[i]);
    }
    printf("]\n");
}

int main(void) {
    srand(time(NULL));
    //Testing Init Keys
    Key* pKey = malloc(sizeof(Key));
    Key* sKey = malloc(sizeof(Key));
    init_pair_keys(pKey, sKey, 3, 7);
    printf("pKey : %lx , %lx \n", pKey->val, pKey->n);
    printf("sKey : %lx , %lx \n", sKey->val, sKey->n);
    
    //Testing Key Serialization
    char* chaine = key_to_str(pKey);
    printf("key to str : %s \n", chaine);
    Key* k = str_to_key(chaine);
    printf("str to key : %lx , %lx \n", k->val, k->n);
    
    //Testing signature
    //Candidate keys:
    Key* pKeyC = malloc(sizeof(Key));
    Key* sKeyC = malloc(sizeof(Key));
    init_pair_keys(pKeyC, sKeyC, 3, 7);
    
    //Declaration:
    char* mess = key_to_str(pKeyC);
    char* chaine1=key_to_str(pKey);
    printf("%s vote pour %s\n", chaine1, mess);
    Signature* sgn = sign(mess, pKey);
    
    printf("signature : ");
    print_long_vector(sgn->content, sgn->size);
    free(chaine);
    chaine = signature_to_str(sgn);
    printf("signature to str : %s \n", chaine);
    free(sgn->content);
    free(sgn);
    sgn = str_to_signature(chaine);
    printf("str to signature : ");
    print_long_vector(sgn->content, sgn->size);
    
    //Testing protected:
    Protected* pr = init_protected(pKey, mess, sgn);

    //Verification:
    if (verify(pr)){ 
        printf("Signature valide\n");
    }
    else{
        printf("Signature non valide\n");
    }
    free(chaine);
    chaine = protected_to_str(pr);
    printf("protected to str : %s\n", chaine);
    free(pr->mess);
    free(pr);
    pr = str_to_protected(chaine);
    free(chaine1);
    chaine1 = key_to_str(pr->pKey);
    char * chaine2 = signature_to_str(pr->sign);
    printf("str_to_protected : %s %s %s \n",
            chaine1, pr->mess, chaine2);
    
    free(pKey);
    free(sKey);   
    free(chaine);
    free(k);
    free(pKeyC);
    free(sKeyC);
    free(mess);
    free(chaine1);
    free(sgn->content);
    free(sgn);
    free(pr->mess);
    free(pr->sign->content);
    free(pr->sign);
    free(pr->pKey);
    free(pr);
    free(chaine2);

    generate_random_data(100, 10);
    return 0;
    }

