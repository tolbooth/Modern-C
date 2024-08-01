#include <stdlib.h>
#include <tgmath.h>
#include <float.h>

int fuzzy_equals(double a, double b);

double vec_vec_prod(size_t n, double u[n], double v[n]);

void matrix_vec_prod(size_t m, size_t n, double A[m][n], double x[n], double b[m]);

void gauss_elim_partial_pivot(size_t m, size_t n, double A[m][n], double x[n], double b[m]);

void LU_factorize(size_t m, size_t n, double A[m][n], double L[m][m], double U[m][n], double P[m][m]);

void copy_matrix(size_t m, size_t n, double source[m][n], double target[m][n]);

size_t max_index(size_t len, double vec[len]);

void swap_row_LU_upper(size_t i, size_t k, size_t m, size_t n, double U[m][n]);

void swap_row_LU_lower(size_t i, size_t k, size_t m, size_t n, double L[m][n]);

void swap_row_LU_permutation(size_t i, size_t k, size_t m, size_t n, double P[m][n]);

void initialize_identity_matrix(size_t n, double A[n][n]);

void forward_sub(size_t m, size_t n, double A[m][n], double x[n], double b[m]);

void back_sub(size_t m, size_t n, double A[m][n], double x[n], double b[m]);

void initialize_vector(size_t n, double x[n]);

int main(void) {
	return EXIT_SUCCESS;
}

int fuzzy_equals(double a, double b) {
	return (fabs(a - b) < (DBL_EPSILON * fmax(fabs(a), fabs(b))));
}

double vec_vec_prod(size_t n, double u[n], double v[n]) {
	double u_dot_v = 0;
	for(size_t i = 0; i < n; ++i) {
		u_dot_v += u[i] * v[i];
	}
	return u_dot_v;
}

void matrix_vec_prod(size_t m, size_t n, double A[m][n], double x[n], double b[m]) {
	for(size_t i = 0; i < m; ++i) {
		b[i] = vec_vec_prod(n, A[i], x);
	}
}

void gauss_elim_partial_pivot(size_t m, size_t n, double A[m][n], double x[n], double b[m]) {
	double L[m][m];
	initialize_identity_matrix(m, L);

	double U[m][n];
	copy_matrix(m, n, A, U);

	double P[m][m];
	initialize_identity_matrix(m, P);
	
	LU_factorize(m, n, A, L, U, P);

	double Pb[m];
	initialize_vector(m, Pb);

	double y[m];
	initialize_vector(m, y);

	matrix_vec_prod(m, n, P, b, Pb);
	forward_sub(m, n, L, y, Pb);
	back_sub(m, n, U, x, y);
}

void LU_factorize(size_t m, size_t n, double A[m][n], double L[m][m], double U[m][n], double P[m][m]) {
	for (size_t k = 0; k < n; ++k) {
		size_t i = max_index(m, U[k]);
		swap_row_LU_upper(i, k, m, n, U);
		swap_row_LU_lower(i, k, m, n, L);
		swap_row_LU_permutation(i, k, m, n, P);
		for (size_t j = k + 1; j < n; ++j) {
			L[j][k] = U[j][k] / U[k][k];
			for(size_t i = k; i < n; ++i) {
				U[j][i] -= L[j][k]*U[k][i];
			}
		}
	}
}

void copy_matrix(size_t m, size_t n, double source[m][n], double target[m][n]) {
	for (size_t i = 0; i < m; ++i) {
		for (size_t j = 0; j < n; ++j) {
			target[i][j] = source[i][j];
		}
	}
}

size_t max_index(size_t len, double vec[len]) {
	size_t max = 0;
	for (size_t i = 0; i < len; ++i) {
		if (vec[i] > vec[max]) max = i;
	}
	return max;
}

void swap_row_LU_upper(size_t i, size_t k, size_t m, size_t n, double U[m][n]) {
	for (size_t j = k; j < n; ++j) {
		double temp = U[k][j];
		U[k][j] = U[i][j];
		U[i][j] = temp;
	}
}

void swap_row_LU_lower(size_t i, size_t k, size_t m, size_t n, double L[m][n]) {
	for (size_t j = 0; j < k; ++j) {
		double temp = L[k][j];
		L[k][j] = L[i][j];
		L[i][j] = temp;
	}
}

void swap_row_LU_permutation(size_t i, size_t k, size_t m, size_t n, double P[m][n]) {
	for (size_t j = 0; j < n; ++j) {
		double temp = P[k][j];
		P[k][j] = P[i][j];
		P[i][j] = temp;
	}
}

void initialize_identity_matrix(size_t n, double A[n][n]) {
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			if (i == j) A[i][j] = 1;
			else A[i][j] = 0;
		}
	}
}

void initialize_vector(size_t n, double x[n]) {
	for (size_t i = 0; i < n; ++i) x[i] = 0;
}

void forward_sub(size_t m, size_t n, double A[m][n], double x[n], double b[m]) {
	return;
}

void back_sub(size_t m, size_t n, double A[m][n], double x[n], double b[m]) {
	return;
}