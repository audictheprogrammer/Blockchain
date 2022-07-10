#include <stdio.h>
#include <stdlib.h>
#include "exo4.h"


void generate_random_data(int nv, int nc){
    /* Genere nv cle dans le fichier keys.txt.
       Genere nc candidats dans le fichier candidates.txt.
       Genere nv declaration dans le fichier declarations.txt.
       Les declarations sont faites par les nv votants du fichier keys.txt.
       Chaques declarations correspond a un vote pour un candidats du fichier
       candidates.txt

    */
    // Constantes
    const long low_size = 3;
    const long up_size = 9;

    // Ouverture de fichiers
    FILE *f = fopen("keys.txt", "w");
    FILE *f2 = fopen("candidates.txt", "w");
    FILE *f3 = fopen("declarations.txt", "w");
    
    // Initialisation
    int i = 0;
    int pareil = 0;
    Key** tab_pKey = (Key**)malloc(sizeof(Key*)* nv);
    Key** tab_sKey = (Key**)malloc(sizeof(Key*)* nv);

    if (tab_pKey == NULL){
        printf("Erreur d'allocation\n");
        return ;
    }
    if (tab_sKey == NULL){
        printf("Erreur d'allocation\n");
        free(tab_pKey);
        return ;
    }

    // Manipulation sur le fichier keys.txt
    while (i < nv) {
        Key* sKey = (Key*)malloc(sizeof(Key));
        Key* pKey = (Key*)malloc(sizeof(Key));

        if (sKey == NULL){
            printf("Erreur d'allocation\n");
            for (int j = 0; j < i; j++){
                free(tab_pKey[j]);
                free(tab_sKey[j]);
            }
            free(tab_pKey);
            free(tab_sKey);
            return ;
        }
        if (pKey == NULL){
            printf("Erreur d'allocation\n");
            for (int j = 0; j < i; j++){
                free(tab_pKey[j]);
                free(tab_sKey[j]);
            }
            free(tab_pKey);
            free(tab_sKey);
            free(sKey);
            return ;
        }

        init_pair_keys(pKey, sKey, low_size, up_size);
        pareil = 0;
        for (int j = 0; j < i; j++){
            if (sameKey(pKey, tab_pKey[j]) && sameKey(sKey, tab_sKey[j])){
                free(pKey);
                free(sKey);
                pareil = 1; 
                break;  
            }
        }
        if (!pareil){
            tab_pKey[i] = pKey;
            tab_sKey[i] = sKey;
            char* str_pKey = key_to_str(pKey);
            char* str_sKey = key_to_str(sKey);
            fprintf(f, "(%s,%s)\n", str_pKey, str_sKey); 
            free(str_pKey);
            free(str_sKey);
            i++; 
        }
    }

    // Manipulation sur le fichier candidates.txt
    int* tab_Cand = (int*) malloc(sizeof(int)*nc);
    int nb = 0;
    i = 0;

    if (tab_Cand == NULL){
        printf("Erreur d'allocation\n");
        for (int j = 0; j < nv; j++){
            free(tab_pKey[j]);
            free(tab_sKey[j]);
        }
        free(tab_pKey);
        free(tab_sKey);
        return ;
    }

    while (i < nc){
        int indiceCand = rand() % nv;
        int trouve = 0;
        for (int j = 0; j < nb; j++){
            if (indiceCand == tab_Cand[j]){
                trouve = 1;
                break;
            }
        }
        if (trouve != 1){
            char* str_pKey = key_to_str(tab_pKey[indiceCand]);
            fprintf(f2, "%s\n", str_pKey);
            tab_Cand[nb] = indiceCand;
            free(str_pKey);
            nb++;
            i++;
        }
    }

    // Manipulation sur le fichier declarations.txt
    for(i = 0; i < nv; i++){
        int indiceCand = rand() % nc;
        Key *elu = tab_pKey[tab_Cand[indiceCand]];
        char* eluStr = key_to_str(elu);
        Signature* signa = sign(eluStr, tab_pKey[i]);
        Protected* protec = init_protected(tab_pKey[i], eluStr, signa);
        char* protectedStr = protected_to_str(protec);
        fprintf(f3, "%s\n", protectedStr);
        free(eluStr);
        free(signa->content);
        free(signa);
        free(protec->mess);
        free(protec);
        free(protectedStr);
    }
    free(tab_Cand);

    for(int i = 0 ; i < nv; i++){
        free(tab_pKey[i]);
        free(tab_sKey[i]);
    }
    free(tab_pKey);
    free(tab_sKey);

    fclose(f);
    fclose(f2);
    fclose(f3);
}