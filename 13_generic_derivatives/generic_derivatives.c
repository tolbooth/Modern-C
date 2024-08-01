#if __STDC_NO_COMPLEX__
#error ”This code needs complex numbers”
#endif

#include <tgmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <complex.h>
#include <stdbool.h>
#include "rat.h"

const size_t ITER_CAP = 18;

typedef struct Polynomial Polynomial;
struct Polynomial {
	size_t degree;
	double* coefficients;
};

typedef double fn_real(double);

typedef double complex fn_cplx(double complex);

double newtons_method(const fn_real F, double x);

double dfdx(const fn_real F, const double x);

double complex dfdz(const fn_cplx F, const double complex z);

bool fuzzy_equals_r(const double a, const double b);

bool fuzzy_equals_c(const double complex a, const double complex b);

double complex evaluate_polynomial(const Polynomial P, const double complex x);

void make_monic(const Polynomial* P);

int main(int argc, char* argv[argc+1]) {

	printf("%f\n", newtons_method(sin, 2));
	
	return EXIT_SUCCESS;
}

double newtons_method(const fn_real F, double x) {
	for (size_t i = 0; i < ITER_CAP; ++i) {
		double x_i = x - F(x) / dfdx(F, x);
		if (!fuzzy_equals_r(F(x_i), 0)) {
			return x_i;
		} else x = x_i;
	}
	return INFINITY;
}

double dfdx(const fn_real F, const double x) {
	return (F(x + ((sqrt(DBL_EPSILON) * x))) - F(x - (sqrt(DBL_EPSILON) * x))) / (2 * sqrt(DBL_EPSILON) * x);
}

double complex dfdz(const fn_cplx F, const double complex z) {
	return (F(z + ((sqrt(DBL_EPSILON) * z))) - F(z - (sqrt(DBL_EPSILON) * z))) / (2 * sqrt(DBL_EPSILON) * z);
}

bool fuzzy_equals_r(const double a, const double b) {
	return (abs(a - b) < (DBL_EPSILON * (abs(a) + abs(b)) / 2) );
}

bool fuzzy_equals_c(const double complex a, const double complex b) {
	return (abs(a - b) < (DBL_EPSILON * (abs(a) + abs(b)) / 2) );
}

double complex evaluate_polynomial(const Polynomial P, const double complex x) {
	double complex Px = 0;
	for (size_t i = 0; i < P.degree; ++i) {
		Px += P.coefficients[i] * pow(x, i); 
	}
	return Px;
}

void print_roots(const Polynomial P, double complex roots[P.degree]) {
	printf("The roots of the polynomial ");
	for (size_t i = 0; i < P.degree; ++i) {
		if (P.coefficients[i] != 0) {
			printf("%+f x^%ld", P.coefficients[i], i);
		}
	}
	printf(": ");
	for (size_t i = 0; i < P.degree; ++i) {
		if (!fuzzy_equals_c(cimag(roots[i]), 0) && !fuzzy_equals_c(creal(roots[i]), 0)) {
			printf("(%+f %+f)", creal(roots[i]), cimag(roots[i]));
		} else if (!fuzzy_equals_c(creal(roots[i]), 0)) {
			printf("%+f ", creal(roots[i]));
		} else {
			printf("%+f ", cimag(roots[i]));
		}
	}
	puts(".");
}

void make_monic(const Polynomial* P) {
	double scale_factor = 1 / P->coefficients[P->degree];
	for (size_t i = 0; i < P->degree - 1; ++i) {
		P->coefficients[i] /= scale_factor;
	}
	P->coefficients[P->degree] = 1;
}