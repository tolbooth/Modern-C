#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

void differentiate(double (*F)(double), const double point);

int main(int argc, char* argv[argc+1]) {
	if (argc < 2) return EXIT_FAILURE;

	double val = strtod(argv[1], 0);
	double (*F)(double) = sin;

	differentiate(F, val);

	return EXIT_SUCCESS;
}

void differentiate(double (*F)(double), const double point) {
	double h = sqrt(DBL_EPSILON) * point;
	printf("x\t= %.6f\nF(x)\t= %.6f\nf(x)\t= %.6f\n",
		point,
		F(point),
		((F(point + h) - F(point - h))) / (2*h));
}