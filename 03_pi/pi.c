#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

int factorial(int k);

int main(int argc, char* argv[argc+1]) {
	if (argc < 2) {
		return EXIT_SUCCESS;
	}

	int term = atol(argv[1]);

	if (term < 1) {
		return EXIT_FAILURE;
	}

	long k = 1;
    double a_k = term, a_sum = term, b_sum = 0, C = 640320;
    double c_cubed_div_24 = pow(C, 3) / 24;
    for (;;) {
        a_k 	*= -(6*k-5) * (2*k-1) * (6*k-1);
        a_k 	/= k*k*k * c_cubed_div_24;
        a_sum 	+= a_k;
        b_sum 	+= k * a_k;
        k 		+= 1;
        if (a_k == 0) {
            break;
        }
    }

    printf("pi: %.42f\n", (426880*sqrt(10005)*term) / (13591409*a_sum + 545140134*b_sum));
    return EXIT_SUCCESS;
}