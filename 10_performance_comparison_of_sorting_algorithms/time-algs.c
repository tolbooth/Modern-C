#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <float.h>
#include "mergesort.h"
#include "quicksort.h"

void populate_random_arrays(size_t len, double A[len], double B[len]);

void print_results(double quick_sort_times[3], double merge_sort_time[3], size_t sizes[3], size_t runs);

double get_sort_time(double arr[], void (*sort)(double A[], size_t len), size_t len);

double random_double(void);

/* 	First argument specifies size of arrays for testing, 				*/
/*	Second argument specifies number of runs (in hundreds) to test.	*/
int main(int argc, char* argv[argc+1]) {
	if (argc < 2) return EXIT_FAILURE;

	/* Seed srand with time in miliseconds */
	struct timespec seed_time;
	timespec_get(&seed_time, TIME_UTC);
	srand(seed_time.tv_sec * 1000 + seed_time.tv_nsec / 1000);

	/* Get number of runs from command line */
	size_t runs = strtol(argv[1], 0, 0);

	if (!runs) return EXIT_FAILURE;

	double A_small[100] = {0}, B_small[100] 	= {0},
		A_medium[10000] = {0}, B_medium[10000] 	= {0},
		A_large[100000]	= {0}, B_large[100000] 	= {0};

	size_t array_sizes[3] = {100, 10000, 100000};
	double* quick_sort_arrays[3] = {A_small, A_medium, A_large};
	double* merge_sort_arrays[3] = {B_small, B_medium, B_large};

	double quick_sort_times[3] = {0};
	double merge_sort_times[3] = {0};

	/* Make the runs. Populate arrays first, then time each sort. */
	for (size_t i = 0; i < runs; ++i) {
		for (size_t j = 0; j < 3; ++j) {
			populate_random_arrays(array_sizes[j], quick_sort_arrays[j], merge_sort_arrays[j]);
			quick_sort_times[j] += get_sort_time(quick_sort_arrays[j], quick_sort, array_sizes[j]);
			merge_sort_times[j] += get_sort_time(merge_sort_arrays[j], merge_sort, array_sizes[j]);
		}
	}

	print_results(quick_sort_times, merge_sort_times, array_sizes, runs);

	return EXIT_SUCCESS;
}

void populate_random_arrays(size_t len, double A[len], double B[len]) {
	for (size_t i = 0; i < len; ++i) {
		A[i] = random_double();
		B[i] = A[i];
	}
}

void print_results(double quick_sort_times[3], double merge_sort_times[3], size_t sizes[3], size_t runs) {
	printf("\t\tQuicksort: \tMergesort:\n");
	for (size_t i = 0; i < 3; ++i) {
		printf("%d items: \t%f  \t%f\n", sizes[i],
			quick_sort_times[i] / runs,
			merge_sort_times[i] / runs
		);
	}	
}

double get_sort_time(double arr[], void (*sort)(double A[], size_t len), size_t len) {
	clock_t start_time = clock();
	sort(arr, len);
	clock_t finish_time = clock();

	return (finish_time - start_time) / (double)CLOCKS_PER_SEC;
}

/* Generates 53 bits of randomness for mantissa, */
double random_double(void) {
    uint64_t r53 = ((uint64_t)(rand()) << 21) ^ (rand() >> 2);
    return ((double)r53 / 9007199254740991.0 ) * DBL_MAX; // 2^53 - 1
}