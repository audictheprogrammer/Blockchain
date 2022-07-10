#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "exo3.h"


void init_key(Key* key, long val, long n){
    /* Initialise une cle key deja allouee en lui
       associant les valeurs val et n.
    */
    key->val = val;
    key->n = n;
}

void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size){
    /* Initialise une cle publique pKey et une cle secrete sKey
       deja allouee en suivant le protocole RSA.
       Les nombres premiers p et q auront une taille entre low_size bits
       et up_size bits. 
    */
    long p, q;
    p = random_prime_number(low_size, up_size, K);
    q = random_prime_number(low_size, up_size, K);
    long n, s, u;
    generate_key_values(p, q, &n, &s, &u);

    if (u < 0){
        long t = (p-1) * (q-1);
        u = u + t;
    }
    init_key(pKey, s, n);
    init_key(sKey, u, n);
    
}

char* key_to_str(Key* key){
    /* Permet de passer d'une cle key à sa representation 
       sous forme de chaine de caracteres.
       Renvoie la chaine de caractere correspondante.
    */
   if(key == NULL){
       return NULL;
   }
    char *res = (char*) malloc(sizeof(char)*25);
    if(res == NULL){
        printf("Erreur d'allocation\n");
        return NULL;
    }
    sprintf(res, "(%lx,%lx)", key->val, key->n);
    return res;
}

Key* str_to_key(char* str){
    /* Permet de passer d'une chaine de caracteres str de la forme (x,y)
       a une variable de type Key.
       Renvoie la cle associee a la chaine str.
    */
    char x[256];
    char y[256];
    int interrupteur = 0;
    int i = 1;
    int indiceY = 0;
    
    while (str[i] != ')'){
        if (str[i] == ','){
            interrupteur = 1;
            indiceY = 0;
            x[i-1] = '\0';
        }else if (interrupteur == 0){
            x[i-1] = str[i];
        }else{
            y[indiceY] = str[i];
            indiceY++;
        }
        i++;
    }
    y[indiceY] = '\0';

    long longX = (long) strtol(x, NULL, 16);
    long longY = (long) strtol(y, NULL, 16);
    Key* key = (Key*) malloc(sizeof(Key));
    if (key == NULL){
        printf("Erreur d'allocation\n");
        return NULL;
    }
    init_key(key, longX, longY);
    
    return key;
}

int sameKey(Key* k1, Key* k2){
    /* Test l'egalite entre 2 cles
       Renvoie 0 si les cles sont differentes.
       Renvoie un chiffre different de 0 sinon.
    */
    if(k1 == NULL || k2 == NULL){
        return 0;
    }
    return k1->n == k2->n && k1->val == k2->val;
}

Signature* init_signature(long* cont, int size){
    /* Alloue et remplit une signature avec un tableau
       de long cont deja alloue de taille size.
       Renvoie la signature obtenu.
    */
    Signature* sign = (Signature*) malloc(sizeof(Signature));
    if (sign == NULL){
        printf("Erreur d'allocation\n");
    }
    sign->size = size;
    sign->content = cont;

    return sign;
}

Signature* sign(char* mess, Key* sKey){
    /* Cree une signature a partir du message mess et
       de la cle secrete sKey.
       Renvoie la signature obtenu.
    */
    long* tab = encrypt(mess, sKey->val, sKey->n);
    return init_signature(tab, strlen(mess));
}

char* signature_to_str(Signature* sgn){
    /* Permet de passer d'une signature sgn a sa 
       representation sous forme de chaine de caracteres.
       Renvoie la chaine de caractere correspondante.
    */
    char* result = (char*) malloc (10 * sgn->size * sizeof(char));
    result [0]= '#';
    int pos = 1;
    char buffer[156];
    for (int i = 0; i < sgn->size; i++){
        sprintf(buffer, "%lx", sgn->content[i]);
        for (int j = 0; j < strlen(buffer); j++){
            result[pos] = buffer[j];
            pos = pos+1;
        }
        result[pos] = '#';
        pos = pos + 1;
    }
    result[pos] = '\0';
    result = realloc(result, (pos+1) * sizeof(char));
    
    return result;
}

Signature* str_to_signature(char* str){
    /* Permet de passer d'une chaine de caracteres str 
       de la forme #a#b#...#z a une variable de type Signature.
       Renvoie la signature correspondante.
    */
    int len = strlen(str);
    long *content = (long*) malloc(sizeof(long)*len);
    int num = 0;
    char buffer[256];
    int pos = 0;

    for (int i=0; i<len; i++){
        if (str[i] != '#'){
            buffer[pos] = str[i];
            pos = pos+1;
        }
        else{
            if (pos != 0){
                buffer[pos] = '\0';
                sscanf(buffer, "%lx", &(content[num]));
                num = num + 1;
                pos = 0;
            }
        }
    }

    content = realloc(content, num * sizeof(long));
    return init_signature(content, num);
}

Protected* init_protected(Key* pKey, char* mess, Signature* sgn){
    /* Alloue et initialise une declaration signee a l'aide des arguments:
       -pKey
       -mess
       -sgn
       Renvoie la declaration obtenu.
    */
    Protected* prot = (Protected *) malloc(sizeof(Protected));
    if (prot == NULL){
        printf("Erreur d'allocation\n");
        return NULL;
    }
    prot->pKey = pKey;
    prot->mess = strdup(mess);
    prot->sign = sgn;
    return prot;
}

int verify(Protected *pr){
    /* Verifie que la signature contenue dans pr correspond bien
       au message et a la personne contenus dans pr.
       Retourne 0 si la signature ne correspond pas au message 
       et a la personne, 1 sinon.
    */
    Signature *s = sign(pr->mess, pr->pKey);

    for(int i = 0; i < strlen(pr->mess); i++){
        if(s->content[i] != pr->sign->content[i]){
            free(s->content);
            free(s);
            return 0;
        }
    }
    free(s->content);
    free(s);
    return 1;
}

char* protected_to_str(Protected* p){
    /* Permet de passer d'un Protected p a sa representation 
       sous forme de chaine de caracteres.
       Revoie la chaine de caractere correspondante.
    */
    char* res = malloc(sizeof(char)*256);
    char* tmp1 = key_to_str(p->pKey);
    char* tmp2 = signature_to_str(p->sign);
    char* tmp3 = " ";
    res[0]='\0';
    strcat(res, tmp1);
    strcat(res, tmp3);
    strcat(res, p->mess);
    strcat(res, tmp3);
    strcat(res, tmp2);
    free(tmp1);
    free(tmp2);

    return res;
    
}

Protected *str_to_protected(char* str) {
    /* Permet de passer d'une chaine de caractere str a une variable de type Protected.
       La chaine doit contenir dans l'ordre (separe d'un espace): 
       -la cle publique de l'emetteur
       -son message
       -sa signature.
       Renvoie la declaration ainsi obtenu.
    */
    Protected* pro = malloc(sizeof(Protected));
    char ch_pKey[256];
    char mess[256];
    char ch_sign[256];

    if (sscanf(str, "%s %s %s", ch_pKey,mess, ch_sign) != 3){
        printf("Erreur de sscanf dans str_to_p\n");
        return NULL;
    }
    Key* pKey = str_to_key(ch_pKey);
    Signature* sign = str_to_signature(ch_sign);
    pro->mess = strdup(mess);
    pro->pKey = pKey;
    pro->sign = sign;
    
    return pro;
}

