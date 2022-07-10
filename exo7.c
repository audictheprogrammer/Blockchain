#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "exo7.h"

void ecrireBlock(Block* b, FILE* f){
    /* Ecrit dans un fichier f ouvert en ecriture 
       le bloc b donne en parametre.
    */
    if (b == NULL){
        printf("Bloc nul dont ecriture impossible!\n");
        return;
    }
    // Author
    char* str_author = key_to_str(b->author);
    fprintf(f, "%s\n", str_author);
    free(str_author);
    // hash
    fprintf(f,"%s\n", b->hash);
    // previous_hash
    fprintf(f, "%s\n", b->previous_hash);
    // nonce
    fprintf(f, "%d\n", b->nonce);
    // votes
    CellProtected* LCP = b->votes;
    while (LCP){
        if (LCP->data == NULL){
            return;
        }
        char* protected_Str = protected_to_str(LCP->data);
        fprintf(f, "%s\n", protected_Str);
        LCP = LCP->next;
        free(protected_Str);
    }

}

Block* lireBlock(FILE* f){
    /* Lit un fichier f ouvert en lecture
       puis renvoie le block lu.
       Le fichier est de la forme:
       - author
       - hash
       - previous_hash
       - nonce
       - votes.
    */
    Block* res = (Block*) malloc(sizeof(Block));
    if (res == NULL){
        printf("Erreur d'allocation\n");
        return NULL;
    }
    char buffer[256];
    char tmp[256];
    int nonce;

    // Recupere le champ author
    if (fgets(buffer, 256, f) == NULL){
        free(res);
        return NULL;
    }
    if (sscanf(buffer,"%s\n", tmp) != 1){
        free(res);
        return NULL;
    }
    res->author = str_to_key(tmp);

    // Recupere le champ hash
    if (fgets(buffer, 256, f) == NULL){
        free(res->author);
        free(res);
        return NULL;
    }
    if (sscanf(buffer,"%s\n", tmp) != 1){
        free(res->author);
        free(res);
        return NULL;
    }
    res->hash = (unsigned char *) strdup(tmp);

    // Recupere le champ previous->hash
    if (fgets(buffer, 256, f) == NULL){
        free(res->author);
        free(res->hash);
        free(res);
        return NULL;
    }
    if (sscanf(buffer,"%s\n", tmp) != 1){
        free(res->author);
        free(res->hash);
        free(res);
        return NULL;
    }
    res->previous_hash = (unsigned char *)strdup(tmp);

    // Recupere le champ nonce
    if (fgets(buffer, 256, f) == NULL){
        free(res->author);
        free(res->hash);
        free(res->previous_hash);
        free(res);
        return NULL;
    }
    if (sscanf(buffer, "%d\n", &nonce) != 1){
        free(res->author);
        free(res->hash);
        free(res->previous_hash);
        free(res);
        return NULL;
    }
    res->nonce = nonce;

    CellProtected* votes,* prec,* tmp1;
    prec = NULL;
    while (fgets(buffer, 256, f) != NULL){
        tmp1 = (CellProtected*) malloc(sizeof(CellProtected));
        tmp1->data = str_to_protected(buffer);
        if(prec == NULL){
            prec = tmp1;
            votes = tmp1; 
        }
        else{
            prec->next = tmp1;
            prec = prec->next; 
        }
        tmp1 = tmp1->next; 

    }
    prec->next = NULL;
    res-> votes = votes;
    return res;
}

char* block_to_str(Block* b){
    /* Genere une chaine de caracteres representant
       le bloc b.
       Renvoie la chaine obtenue.
    */
    if (b == NULL){
        printf("Bloc nul!");
        return NULL;
    }
    char* str = (char*) malloc(sizeof(char) * 10000);
    if(str == NULL){
        printf("Erreur d'allocation\n");
        return NULL;
    }
    char* str_author = key_to_str(b->author);
    sprintf(str, "%s %s ", str_author, b->previous_hash);
    free(str_author);

    CellProtected* LCP = b->votes;
    while (LCP){
        if (LCP->data == NULL){
            printf("Liste chainee de protected sans data\n");
            return NULL;
        }
        char* str_protected = protected_to_str(LCP->data);
        strcat(str, str_protected);
        LCP = LCP->next;
        free(str_protected);
    }

    char nonce[10000];
    sprintf(nonce, "%d", b->nonce);
    strcat(str, nonce);
    return str;
}

unsigned char* hash_SHA256(char* c){
    /* Fonction de hachage utilisant le protocole
       SHA256 de la librairie SHA.
       Rennvoie la valeur hachee de c.
    */
    unsigned char *d = SHA256((const unsigned char *)c, strlen(c), 0);
    unsigned char *res = malloc(sizeof(unsigned char) * 2048);
    char buffer[1024];
    
    if (d == NULL){
        printf("Erreur SHA256, erreur d'allocation\n");
        return NULL;
    }
    if(res == NULL){
        printf("Erreur d'allocation\n");
        free(d);
        return NULL;
    }

    res[0] = '\0';
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++){
        sprintf(buffer,"%02x",d[i]);
        strcat((char*) res,buffer);    
    }
    return res;
}

void compute_proof_of_work(Block* b, int d){
    /* Realise la preuve de travail du bloc b.
       La valeur hacher du bloc b doit commencer par d zeros.
    */
    unsigned char* hash_value;
    b->nonce = 0;
    char* str_b;
    int valid = 0;

    while (valid == 0){
        str_b = block_to_str(b);  
        hash_value = hash_SHA256(str_b);
        valid = 1;
        for (int i = 0; i < d; i++){
            if (hash_value[i] != '0'){
                valid = 0;
                break;
            }
        }
        if (valid == 1){
            break;
        }
        free(hash_value);
        free(str_b);
        b->nonce++;
    }
    if (b->hash != NULL){
        free(b->hash);
    }

    b->hash = (unsigned char*) strdup((const char*)hash_value);
    free(hash_value);
    free(str_b);
}

int verify_block(Block* b, int d){
    /* Verifie que le bloc b a bien d zeros dans son champ hash.
    */
    char* str_b = block_to_str(b);
    unsigned char* hash_value = hash_SHA256(str_b);

    for (int i = 0; i < d; i++){
        if (hash_value[i] != '0'){
            free(str_b);
            return 0;
        }
    }

    if ((strcmp((const char*)b->hash, (const char*)hash_value)) != 0) {
        free(str_b);
        return 0;
    }

    free(str_b);
    free(hash_value);

    return 1;
}

void delete_block(Block* b){
    /* Desalloue un bloc b, libere tous les champs sauf :
        -le champ Author.
        -le contenu du champ votes (Protected)
    */
    if (b == NULL){
        return ;
    }
    delete_list_cell_protected_elem(b->votes);
    if(b->hash != NULL){
        free(b->hash);
    }
    if(b->previous_hash != NULL){
        free(b->previous_hash);
    }
    free(b);
}





