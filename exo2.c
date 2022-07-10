#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "exo1.h"

long extended_gcd(long s, long t, long *u, long *v){
    /* Fontion recursive de l'algorithme d'Euclide etendu.
       Determine les entier u et v verifiant l'equation de Bezout : s*u+t*v = PGCD(s,t)
       puis les stock dans les pointeurs en question.
       Renvoie PGCD(s,t).
    */
    if (s == 0){
        *u = 0;
        *v = 1;
        return t;
    }
    long uPrim, vPrim;
    long gcd = extended_gcd(t%s, s, &uPrim, &vPrim);
    *u = vPrim - (t/s)*uPrim;
    *v = uPrim;

    return gcd;
}   


void generate_key_values(long p, long q, long *n, long *s, long *u){
    /* Genere les cles publiques et secretes pkey = (s,n), skey = (u,n)
       en suivant le protocole RSA à l'aide des nombres premiers p et q.
    */
    *n = p * q;
    long v = 0;
    long t = (p-1) * (q-1);
    *s = rand_long(2, t); 

    while (extended_gcd(*s, t, u, &v) != 1){
        *s = rand_long(2, t);
    }
}

long* encrypt(char* chaine, long s, long n){
    /* Chiffre la chaine de caractere chaine a l'aide
       de la cle publique (s, n).
       Renvoie un tableau de long correspondant au message
       crypte.
    */
    long* crypted_msg = (long *)malloc(sizeof(long)*strlen(chaine));
    if (crypted_msg == NULL){
        printf("Erreur d'allocation ! \n");
        return NULL;
    }

    int i = 0;
    int m;

    while (chaine[i] != '\0'){
        m = (int)chaine[i];
        crypted_msg[i] = (long)modpow(m, s, n);
        i++;
    }

    return crypted_msg;
}

char* decrypt(long* crypted, int size, long u, long n){
    /* Dechiffre le tableau crypted de taille size à 
       l'aide de la cle secrete (u ,n).
       Renvoie la chaine de caractere decryptee.
    */
    int m;
    char* res = malloc(sizeof(char)*(size + 1));
    if (res == NULL){
        printf("Erreur d'allocation\n");
    }
    
    for (int i = 0; i < size; i++){
        m = modpow(crypted[i], u, n);
        res[i] = (char) m;
    }
    res[size] = '\0';

    return res;

}