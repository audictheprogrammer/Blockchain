#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "exo1.h"

int is_prime_naive(long p){
    /* Test si p est premier.
       Renvoie 1 si p est premier, 0 sinon.
    */
    for (long i = 2; i < p; i++){
        if (p % i == 0){
            return 0;
        }
    }
    return 1;
}

long modpow_naive(long a, long m, long n){
    /* Renvoie a^m mod n de maniere naive. 
    */
    long res = 1;
    for (long i = 0; i < m; i++){
        res *= a;
        res = res % n   ;
    }
    return res;
}

int modpow(long a, long m, long n){
    /* Fonction qui renvoie a^m mod n de maniere non naive.
    */
    if (m == 0){
        return 1;
    }
    if (m == 1){
        return a % n;
    }

    int x = modpow(a, m/2, n);
    if (m%2 == 0){
        return x * x % n;

    }
    return a * x * x % n;
}

void comparer(long a, long m, long n, FILE *f){
    /* Compare les performances entre modpow et 
       modpow_naive. Ajoute les temps dans le fichier f.
    */    
    clock_t temps_initial_naif = clock();
    modpow_naive(a, m, n);
    clock_t temps_final_naif = clock();

    clock_t temps_initial = clock();
    modpow(a, m, n);
    clock_t temps_final = clock();

    // Traitement des donnees.    
    double temps_cpu_naif = ((double) ( temps_final_naif - temps_initial_naif) ) / CLOCKS_PER_SEC;
    double temps_cpu = ((double) ( temps_final - temps_initial) ) / CLOCKS_PER_SEC;

    fprintf(f, "%ld %f %f\n", m, temps_cpu_naif, temps_cpu);
        
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
int witness (long a, long b, long d, long p){
    /* Teste si a est un temoin de Millier pour p.
       Renvoie 1 si a est un temoin, 0 sinon.
    */
    long x = modpow(a, d, p);
    if (x == 1){
        return 0;
    }
    for (long i = 0; i < b; i++){
        if (x == p-1){
            return 0;
        }
        x = modpow(x, 2, p);
    }
    return 1;
}

long rand_long(long low, long up){
    /* Retourne un long genere aleatoirement entre
       low et up inclus.
    */
    return rand() % (up - low + 1) + low;
}


int is_prime_miller(long p, int k){
    /* Realise le test de Miller-Rabin pour l'entier p.
       Renvoie 0 si p n'est pas premier.
       Renvoie 1 si p est tres probablement premier.
    */
    if (p == 2){
        return 1;
    }
    if (!(p & 1) || p<=1){ //on verifie que p est impair et different de 1
        return 0;
    }
    // On determine b et d :
    long b = 0;
    long d = p - 1;
    while (!(d & 1) ) { //tant que d n’est pas impair
        d = d / 2;
        b = b + 1;
    }
    // On genere k valeurs pour a, et on teste si c’est un temoin :
    long a ;
    int i ;
    for (i=0; i < k; i++){
        a = rand_long(2, p-1);
        if (witness(a, b, d, p)){
            return 0;
        } 
    }
    return 1;
}

long random_prime_number(int low_size, int up_size, int k){
    /* Renvoie un nombre qui est tres probablement premier de 
       taille entre low_size et up_size.
       On considere n le nombre d'entier entre low_size et up_size.
       Si aucun nombre premier n'est trouve apres la n tentative, 
       la fonction renvoie 0.
    */
    long inf = pow(2, low_size - 1);
    long sup = pow(2, up_size) - 1;

    for (int i = inf; i < sup+1; i++){
        long nb = rand_long(inf, sup);
        if (is_prime_miller(nb, k)){
            return nb;
        }
    }

    return 0;
}
