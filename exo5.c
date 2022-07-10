#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exo5.h"

CellKey *create_cell_key(Key* key){
    /* Alloue et initialise une cellule de liste chainee de cle
       a partir de la cle key.
       Renvoie la cellule obtenu.
    */
    CellKey* cKey = (CellKey*) malloc(sizeof(CellKey));
    if (cKey == NULL){
        printf("Erreur d'allocation \n");
        return NULL;
    }
    cKey->data = key;
    cKey->next = NULL;
    return cKey;
}

void cell_en_tete(CellKey** cKey, Key* key){
    /* Ajoute une cle key en tete de la liste cKey.
    */
    CellKey* premier = create_cell_key(key);
    if (premier == NULL){
        return;
    }
    premier->next = *cKey;
    *cKey = premier;
}

CellKey* read_public_keys(FILE* f){
    /* Prend en entree keys.txt OU candidates.txt ouvert en lecture.
       Retourne une liste chainee contenant toutes les cles
       publiques du fichiers.
    */
    CellKey* LCK = NULL;
    char buffer[256];
    int premierEssaie = 1;
    Key* cle_courante;
    int boolean;
    
    while (fgets(buffer, 256, f) != NULL){
        if(premierEssaie == 1){
            if (buffer[0] == '\0'){
                // fichier vide
                printf("Fichier vide\n");
                free(LCK);
                return NULL;
            }
            boolean = 2;
            if (buffer[1] == '('){
                // keys.txt
                boolean = 1;
            }else{
                // candidates.txt
                boolean = 0;
            }
            premierEssaie = 0;
        }
        int index = boolean;
        char ch_cle[256]= "";
        char tmp[2] = "";
        while (buffer[index] != ')'){
            tmp[0] = buffer[index];
            strcat(ch_cle, tmp);
            index++;
        }
        tmp[0] = buffer[index];
        strcat(ch_cle, tmp);
        cle_courante = str_to_key(ch_cle);
        cell_en_tete(&LCK, cle_courante);
    }

    if(premierEssaie == 1){
        printf("Erreur de fgets\n");
        if (LCK != NULL){
            free(LCK);
        }
        return NULL;
    }
    return LCK;

}

void print_list_keys(CellKey* LCK){
    /* Affiche la liste chainee de cles LCK.
    */
    CellKey *tmp = LCK;
    while (tmp){
        if (tmp->data == NULL){
            return;
        }
        else {   
            char * key_Str = key_to_str(tmp->data);
            printf("%s\n", key_Str);
            free(key_Str);
        }
        tmp = tmp->next;
    }
}

void delete_cell_key(CellKey* c){
    /* Supprime une cellule c de liste chainee de cles.
    */
    if(c == NULL){
        return;
    }
    if(c->data !=NULL){
        free(c->data);
        c->data = NULL;
    }
    free(c);
}   

void delete_list_keys(CellKey *LCK){
    /* Supprime toute la liste chainee de cles LCK.
    */
    while(LCK){
        CellKey *tmp = LCK;
        LCK = LCK->next;
        delete_cell_key(tmp); 
    }
    free(LCK);
}

void delete_list_keys_vide(CellKey *LCK){
    /* Supprime toute la liste chainee de cles LCK, 
       sachant que les cles ont deja ete desallouees.
    */
    while(LCK){
        CellKey *tmp = LCK;
        LCK = LCK->next;
        free(tmp);
    }
    free(LCK);
}

CellProtected* create_cell_protected(Protected* pr){
    /* Alloue et initialise une cellule de liste chainee
       de declaration a partir de la declaration pr.
       Renvoie la cellule ainsi obtenu.
    */
    CellProtected * res = (CellProtected*) malloc(sizeof(CellProtected));
    if (res == NULL) {
        printf("Erreur d'allocation\n");
        return NULL;
    }
    res->data = pr;
    res->next = NULL;
    return res;
}

void ajout_protected(CellProtected** LCP,Protected* p){
    /* Ajoute une declaration signee p en tete de liste LCP.
    */
    CellProtected * premier = create_cell_protected(p);
    if ( premier == NULL){
        return ;
    }
    premier->next = *LCP;
    *LCP = premier;
}

CellProtected* read_protected(FILE* f){
    /* Lit le fichier declaration.txt ouvert en lecture et cree une liste
       chainee contenant toutes les declarations signees du fichier.
       Renvoie la liste obtenu.
    */
    char buffer[256];
    CellProtected* LCP = NULL;
    
    while (fgets(buffer, 256, f) != NULL){
        Protected* pr = str_to_protected(buffer);
        ajout_protected(&LCP, pr);
    }
    
    return LCP;
}

void print_cellProtected(CellProtected* LCP){
    /* Affiche la liste chainee LCP de declarations signees.
    */
    CellProtected *tmp = LCP;
    while (tmp){
        if(tmp->data == NULL){
            return;
        }
        char * protected_Str = protected_to_str(tmp->data);
        printf("%s\n", protected_Str);
        tmp = tmp->next;
        free(protected_Str);
    }
}

void delete_cell_protected(CellProtected *c){
    /* Supprime une cellule c de liste chainee de 
       declarations signees.
    */
    if(c->data == NULL){
        free(c);
        return;
    }
    free(c->data->pKey);
    free(c->data->mess);
    free(c->data->sign->content);
    free(c->data->sign);
    free(c->data);
    free(c);
}

void delete_list_cell_protected(CellProtected *LCP){
    /* Supprime toute la liste chainee LCP de 
       declarations signees.
    */
    while(LCP){
        CellProtected *tmp = LCP;
        LCP = LCP->next;
        delete_cell_protected(tmp);
    }
}

void delete_list_cell_protected_contenu(CellProtected *LCP){
    /* Supprime le contenu de la liste chaine LCP mais pas les 
       elements.
    */
    while(LCP){
        free(LCP->data->pKey);
        free(LCP->data->mess);
        free(LCP->data->sign->content);
        free(LCP->data->sign);
        free(LCP->data);
        LCP = LCP->next;
    }
}

void delete_list_cell_protected_elem(CellProtected *LCP){
    /* Supprime les cellules de la liste chaine LCP mais pas
       leur contenu.
    */
    while(LCP){
        CellProtected *tmp = LCP;
        LCP = LCP->next;
        free(tmp);    
    }
}


