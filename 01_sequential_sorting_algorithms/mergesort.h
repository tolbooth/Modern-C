#ifndef MERGESORT_H
#define MERGESORT_H

void merge_sort(double A[], size_t len);

void merge_sort_recursion(double A[], size_t l, size_t r);

void merge(double A[], size_t l, size_t m, size_t r);

void copy_array(double target[], double source[], size_t len, size_t offset);

#endif