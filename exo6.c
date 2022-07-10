#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exo6.h"


void filter(CellProtected** LCP){
    /* Supprime les declarations de LCP dont la signature
       n'est pas valide.
    */
    CellProtected* courant = *LCP;

    // Filtre les premiers jusqu'a une signature valide
    while (*LCP != NULL && verify((*LCP)->data) == 0){
        courant = *LCP;

        if ((*LCP)->next == NULL){
            printf("Aucun\n");
            delete_cell_protected(*LCP);
            *LCP = NULL;
            printf("Aucune declarations signees valides\n");
            return ;
        }
        
        *LCP = (*LCP)->next;
        delete_cell_protected(courant);
    }


    // Filtre les prochains
    if(*LCP == NULL){
        *LCP = NULL;
        printf("Aucune declarations signees valides\n");
        return ;
    }
    CellProtected* precedent = *LCP;
    courant = precedent->next;
    while(courant != NULL){
        if (courant->data != NULL && 
            verify(courant->data) == 0){
            precedent->next = courant->next;
            delete_cell_protected(courant);
        }else{
            precedent = courant;
        }
        courant = precedent->next;
    }
}

HashCell* create_hashcell(Key* key){
    /* Alloue et initialise une celulle de table de hachage a l'aide
       de la cle key.
       Renvoie la cellule obtenu.
    */
    HashCell* cHash = (HashCell*) malloc(sizeof(HashCell));
    if (cHash == NULL){
        printf("Erreur d'allocation\n");
        return NULL;
    }
    cHash->key = key;
    cHash->val = 0;
    return cHash;
}


int hash_function(Key * key, int size){
    /* Fonction de hachage qui renvoie la position de 
       l'element key dans la table de hachage de taille size.
       La methode de hachage choisie est arbitraire.
    */
    return (key->val + key->n) % size;
}

int find_position(HashTable* t, Key* key){
    /* Cherche dans la table de hachage t s'il existe un
       element dont la cle public est key.
       Les collisions sont gerees par probing lineaire.
       Renvoie la position de key dans la table si 
       key a ete trouve.
       Renvoie la position ou il aurait du etre sinon.
    */
    int position = hash_function(key, t->size);
    int initial_position = position;
    int ind_tour = 0;

    while (t->tab[position] != NULL){
        if (sameKey(t->tab[position]->key, key) != 0){
            return position;
        }
        if (position != initial_position || ind_tour == 0){
            position = (position + 1) % (t->size);
            ind_tour++;
        }
        else{
            // Deja parcouru toutes les cellules
            return initial_position;
        }
    }

    return initial_position;
}

void delete_hashcell(HashCell* cHash){
    /* Supprime la cellule cHash.
    */
    if (cHash != NULL){
        free(cHash->key);
        cHash->key = NULL;
        free(cHash);
    }
}


HashTable* create_hashtable(CellKey* keys, int size){
    /* Alloue et initialise une table de hachage de
       taille size a l'aide de keys.
    */

    HashTable* res = (HashTable*) malloc(sizeof(HashTable));
    if(res == NULL){
        printf("Erreur d'allocation\n");
        return NULL;
    }
    res->size = size;
    res->tab = (HashCell**) malloc(sizeof(HashCell*)*size);
    if(res->tab == NULL){
        printf("Erreur d'allocation\n");
        free(res);
        return NULL;
    }
    for(int i = 0 ; i < size; i++){
        res->tab[i] = NULL;
    }

    while(keys){
        int position = hash_function(keys->data, size);
        if(res->tab[position] == NULL){
            res->tab[position] = create_hashcell(keys->data);
        }
        else{
            while(res->tab[position] != NULL){
                position = (position + 1) % size;
            }
            res->tab[position] = create_hashcell(keys->data);
        }
        keys = keys->next;
    }
    
    return res;
}


void delete_hashtable(HashTable* t){
    /* Supprime la table de hachage t.
    */
    for (int i = 0; i < t->size; i++){
        delete_hashcell(t->tab[i]);
    }
    free(t->tab);
    free(t);
}

void delete_hashtable_vide(HashTable* t){
    /* Supprime la table de hachage t dont 
       les cles ont deja ete supprimees.
    */
    for (int i = 0; i < t->size; i++){
        free(t->tab[i]);
    }
    free(t->tab);
    free(t);
}


Key* compute_winner(CellProtected* decl, CellKey* candidates, 
                    CellKey* voters, int sizeC, int sizeV){
    /* Calcule le vainqueur de l'election.

        Representation des parametres:
        CellProtected* decl -> liste declarations valides
        CellKey* candidates -> liste de candidats
        CellKey* voters -> liste des personnes autorisees a voter
        int sizeC -> taille liste Candidats
        int sizeV -> taille liste Votants
    */
    HashTable* Hc = create_hashtable(candidates, sizeC);
    HashTable* Hv = create_hashtable(voters, sizeV);
    
    while(decl){
        int position = find_position(Hv, decl->data->pKey);
        if (sameKey(Hv->tab[position]->key, decl->data->pKey) != 0){
            // A bien le droit de voter
            if (Hv->tab[position]->val == 0){
                // N'a pas deja voter
                Key* tmp = str_to_key(decl->data->mess);
                int position2 = find_position(Hc, tmp);
                if (sameKey(Hc->tab[position2]->key, tmp) != 0){
                    // Le candidat existe bien
                    Hc->tab[position2]->val++;
                    Hv->tab[position]->val = 1;
                }
                free(tmp);
            }
        }
        decl = decl->next;
    }

    int max = -1;
    Key* gagnant = NULL;
    
    for (int i = 0; i < sizeC; i++){
        if (Hc->tab[i]->val > max){
            max = Hc->tab[i]->val;
            gagnant = Hc->tab[i]->key; 
        }
    }

    printf("Le gagnant a obtenu %d voix\n", max);

    Key* res = (Key*) malloc(sizeof(Key));
    if (res == NULL){
        printf("Erreur d'allocation\n");
        delete_hashtable(Hc);
        delete_hashtable(Hv);
        return NULL;
    }
    init_key(res, gagnant->val, gagnant->n);
    delete_hashtable(Hc);
    delete_hashtable(Hv);
    return res;
    
}





