#ifndef RAT_H
#define RAT_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct Rational Rational;
struct Rational {
	bool rat_sign;
	size_t rat_num;
	size_t rat_denom;
};

Rational rat_get_sum(Rational p, Rational q);
Rational rat_get_prod(Rational p, Rational q);
Rational rat_get(long long p, long long q);
long double rat_get_float(Rational p);

#endif