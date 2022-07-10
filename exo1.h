#ifndef EXO1_H
#define EXO1_H

int is_prime_naive(long p);
long modpow_naive(long a, long m, long n);
int modpow(long a, long m, long n);
void comparer(long a, long m, long n, FILE *f);
int witness (long a, long b, long d, long p);
long rand_long(long low, long up);
int is_prime_miller(long p, int k);
long random_prime_number(int low_size, int up_size, int k);

#endif

