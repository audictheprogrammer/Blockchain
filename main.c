#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "exo2.h"

void print_long_vector(long *result, int size){ 
    printf("Vector: [");
    for (int i = 0; i < size; i++){
        printf("%lx \t", result[i]);
    }
    printf("]\n");
}


int main(){
    // Constantes
    const int TEST1 = 1;
    const int TEST2 = 1;
    const int TEST3 = 1;
    const int TEST4 = 1;
    const int TEST5 = 1;
    
    // Initialisation
    srand(time(NULL));

    if (TEST1){
        // Test1: Lance 1 fois modpow et modpow_naive.
        int a = modpow(1, 5, 45);
        int b = modpow_naive(1, 5, 45);
        printf("a=%d, b=%d\n", a, b);
    }

    if (TEST2){
        // Test2: Teste la performance entre modpow et
        // modpow_naive.
        const char* filename = "comparaison_modpow.txt";
        const long max = 1024;
        FILE* f = fopen(filename, "w");
        for (long m = 1; m <= max; m = 2*m){
            comparer(15329813, m, 17211, f);
        }
        fclose(f);
    }

    if (TEST3){
        // Test3: Lance 1000 fois modpow et modpow_naive.
        FILE* f2 = fopen("verify_modpow_and_modpow_naive.txt","w");
        for(int i = 1; i<1000; i++){
            fprintf(f2, "%d %ld %d\n", i, modpow_naive(2, i, 17), modpow(2,i,17));    
        }
        fclose(f2);
    }

    if (TEST4){
        // Test4: Lance 1 fois random_prime_number
        FILE* f3 = fopen("random_prime_number.txt", "w");
        fprintf(f3, "%ld\n", random_prime_number(5, 8, 10));
        fclose(f3);
    }

    if (TEST5){
        // Test5: Test initial de l'enonce.
        // Generation de cle:
        long p = random_prime_number(3, 5, 50);
        long q = random_prime_number(3, 5, 50);
        while(p == q){
            q = random_prime_number(3, 5, 50);
        }
        long n, s, u;
        generate_key_values(p, q, &n, &s, &u);
        
        // Pour avoir des cles positives
        if (u < 0){
            long t = (p-1)*(q-1);
            u = u + t; // on aura toujours s*u mod t =1
        }

        // Afichage des cles en hexadecimal
        printf("cle publique = (%lx, %lx) \n", s , n);
        printf("cle privee = (%lx, %lx) \n", u , n);

        // Chiffrement:
        char mess[10] = "QQQQ";
        int len = strlen(mess);
        long* crypted = encrypt(mess, s, n);

        printf("Initial message: %s \n", mess);
        printf("Encoded representation : \n");
        print_long_vector(crypted, len);

        // Dechiffrement
        char* decoded = decrypt(crypted, len, u, n);
        printf("Decoded : %s \n", decoded);

        free(crypted);
        free(decoded);

    }


    return 0;
}