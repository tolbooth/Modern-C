#include <stdlib.h>
#include <stdio.h>
#include "quicksort.h"

void quick_sort(double A[], size_t len) {
	quick_sort_recursion(A, 0, len - 1);
}

void quick_sort_recursion(double A[], size_t lo, size_t hi) {
	if (lo < hi) {
		size_t pivot = partiton(A, lo, hi);
		quick_sort_recursion(A, lo, pivot - 1);
		quick_sort_recursion(A, pivot + 1, hi);
	}
}

size_t partiton(double A[], size_t lo, size_t hi) {
	size_t pivot = median_of_three(A, lo, hi);
	size_t i = lo, j = hi;

	if (pivot != hi) swap_dbl(A, pivot, hi);

	while(i < j) {
		while(A[i] <= A[hi] && i < hi) i++; 
		while(A[j] > A[hi] && j > lo) j--;
		if (i < j) swap_dbl(A, i, j);
	}

	swap_dbl(A, i, hi);
	return i;
}

size_t median_of_three(double A[], size_t lo, size_t hi) {
	size_t three[3] = { lo,  lo + (hi - lo) / 2, hi };
	if (A[three[0]] > A[three[1]]) swap_int(three, 0, 1);
	if (A[three[1]] > A[three[2]]) swap_int(three, 1, 2);
	if (A[three[0]] > A[three[1]]) swap_int(three, 0, 1);
	return three[1];
}

void swap_dbl(double A[], size_t a, size_t b) {
	if (A[a] != A[b]) {
		double temp = A[a];
		A[a] = A[b];
		A[b] = temp;
	}
}

void swap_int(size_t A[], size_t a, size_t b) {
	if (A[a] != A[b]) {
		A[a] ^= A[b];
		A[b] ^= A[a];
		A[a] ^= A[b];
	}
}
