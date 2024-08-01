#include <stdlib.h>
#include "mergesort.h"

void merge_sort(double A[], size_t len) {
	merge_sort_recursion(A, 0, len - 1);
}

void merge_sort_recursion(double A[], size_t l, size_t r) {
	if (l < r) {
		size_t m = l + (r - l) / 2;

		merge_sort_recursion(A, l, m);
		merge_sort_recursion(A, m + 1, r);

		merge(A, l, m, r);
	}
}

void merge(double A[], size_t l, size_t m, size_t r) {
	size_t l_len = m - l + 1;
	size_t r_len = r - m;

	double left[l_len];
	double right[r_len];

	copy_array(left, A, l_len, l);
	copy_array(right, A, r_len, m+1);

	for (size_t i = 0, j = 0, k = l; k <= r; ++k) {
		if ((i < l_len) && ((j >= r_len) || left[i] <= right[j])) {
			A[k] = left[i++];
		} else {
			A[k] = right[j++];
		}
	}
}

void copy_array(double target[], double source[], size_t len, size_t offset) {
	for (size_t i = 0; i < len; ++i) {
		target[i] = source[i + offset];
	}
}