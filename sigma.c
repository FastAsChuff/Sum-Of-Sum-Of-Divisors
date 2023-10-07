#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// gcc sigma.c -o sigma.bin -lm -O3  -Wall -ffast-math -march=native -std=c11 -fstrict-aliasing -fsanitize=undefined 

// using gcc (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0

uint64_t sigma(uint64_t n) {
  // Return sum of all positive divisors of n
  // Note: If (p,q) = 1, sigma(pq) = sigma(p)*sigma(q)
  uint64_t sum = 0, i = 1;
  while (i*i < n) {
    if ((n % i) == 0) {
      sum += i + n/i;
    }
    i++;
  }
  if (i*i == n) sum += i;
  return sum;
}

uint64_t sum_sigma_naive(uint64_t n, uint64_t modulus) {
  uint64_t i, sigmasum = 0;
  for (i=1; i<=n; i++) {
    sigmasum = (sigmasum + sigma(i)) % modulus;
  }
  return sigmasum;
}

uint64_t sum_sigma(uint64_t n, uint64_t modulus) {
  uint64_t i, sigmasum, sigmasum2;
  sigmasum = n % modulus;
  sigmasum = (sigmasum * sigmasum) % modulus;
  sigmasum2 = 0;
  for (i=1; i<=n; i++) {
    sigmasum2 = (sigmasum2 + (n % i)) % modulus;
  }
  return (sigmasum + (modulus - sigmasum2)) % modulus;
}

uint64_t sum_sigma_fast(uint64_t n, uint64_t modulus) {
  uint64_t sigmasum;
  __int128 q, i, j, k, sigmasum2;
  sigmasum = n % modulus;
  sigmasum = (sigmasum * sigmasum) % modulus;
  sigmasum2 = 0;
  for (i=1; i*i<=n; i++) {
    sigmasum2 = (sigmasum2 + (n % i)) % modulus;
  }
  for (; i<=n; ) {
    q = n/i;
    // Find j s.t. (n/j = q) && (n/(j+1) < q)
    j = n/q;
    if (j > n) j = n;
    k = j-i+1;
    sigmasum2 = (sigmasum2 + k*n - (q*(i+j)*k)/2) % modulus;
    i = j + 1;
  }
  return (sigmasum + (modulus - sigmasum2)) % modulus;
}

int main(int argc, char **argv)
{  
  int64_t sigmasum, arg = 12, modulus = 1000000007;
  if (argc > 1) {
    arg = atol(argv[1]);
    if ((arg < 1) || (arg > 1000000000000UL)) {
      printf("Only numbers between 1 and 10^12 are supported.\n");
      exit(1);
    }
  } else {
    printf("For the sum of positive divisors function \nsigma(n) = sum_{d|n} d\nthis program calculates the sum of all such sigma up to n mod 1000000007.\nUsage %s 321\nOutput: Sum_{i = 1 to 321} sigma(i) = 84835 mod 1000000007\n", argv[0]);
    exit(0);
  }
  if (arg < 1) arg = 1;
  //sigmasum = sum_sigma(arg, modulus);
  //printf("Sum_{i = 1 to %lu} sigma(i) = %lu mod %lu\n", arg, sigmasum, modulus);
  sigmasum = sum_sigma_fast(arg, modulus);
  printf("Sum_{i = 1 to %lu} sigma(i) = %lu mod %lu\n", arg, sigmasum, modulus);
  exit(0);
}
