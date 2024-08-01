#if __STDC_NO_COMPLEX__
#error ”This code needs complex numbers”
#endif

#include <stdlib.h>
#include <stdio.h>
#include <tgmath.h>
#include <float.h>
#include <complex.h>

void differentiate(long double complex(*F)(long double complex), const long double complex point);

int main(int argc, char* argv[argc+1]) {
	long double real = strtod(argv[1], 0);
	long double imag = 0;

	if (argc > 2) {
		imag = strtod(argv[2], 0);
	}

	// using complex step for real differentiation for nice results.
	long double complex (*F)(long double complex) = csinl;
	differentiate(F, real + I*imag);
	
	return EXIT_SUCCESS;
}

void differentiate(long double complex (*F)(long double complex), const long double complex point) {
	long double h = sqrt(DBL_EPSILON) * point;
	long double complex Fz = F(point);
	long double complex fz = 0;

	if (cimag(point) == 0) {
		fz = cimag(F(point + I*h)) / h;
		printf("z\t= %.6Lf\nF(z)\t= %.6Lf\nf(z)\t= %.6Lf\n",
		creal(point),
		creal(Fz),
		creal(fz));
	} else {
		fz = (F(point + h) - F(point -h)) / (2*h);
		printf("z\t= %.6Lf %+Lf i\nF(z)\t= %.6Lf %+Lf i \nf(z)\t= %.6Lf %+Lf i \n",
		creal(point), cimag(point),
		creal(Fz), cimag(Fz),
		creal(fz), cimag(fz));
	}
}