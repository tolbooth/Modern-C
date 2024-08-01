#include <stdio.h>
#include <stdlib.h>

void sieve(size_t n, size_t primes[n], size_t i) {
	if (i > n) return;

	if (primes[i] != 0) {
		for (size_t j = 2*i; j <= n; j+=i) primes[j] = 0;
		if (!(n % i)) printf("%ld ", i);
	}
	sieve(n, primes, i+1);
}

int main(int argc, char *argv[argc+1]) {
	if (argc < 2) return EXIT_FAILURE;
	size_t n = strtoul(argv[1], 0, 0);

	if (n < 0) return EXIT_FAILURE;

	size_t primes[n];
	for (size_t i = 2; i < n; ++i) {
		primes[i] = i;
	}

	printf("N: ");
	sieve(n, primes, 2);
	puts("");
	
	return EXIT_SUCCESS;
}

