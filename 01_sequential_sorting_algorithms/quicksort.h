#ifndef QUICKSORT_H
#define QUICKSORT_H

void quick_sort(double A[], size_t len);

void quick_sort_recursion(double A[], size_t lo, size_t hi);

size_t partiton(double A[], size_t lo, size_t hi);

size_t median_of_three(double A[], size_t lo, size_t hi);

void swap_dbl(double A[], size_t a, size_t b);

void swap_int(size_t A[], size_t a, size_t b);

#endif