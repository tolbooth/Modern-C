#include <tgmath.h>
#include <stdlib.h>
#include <assert.h>
#include "rat.h"

 /* Internal Definitions */

size_t gcd(size_t p, size_t q);
Rational rat_get_normal(Rational x);
Rational rat_get_extended(Rational x, size_t a);

/* Implementation */

Rational rat_get_sum(Rational p, Rational q) {
	size_t c = gcd(p.rat_denom, q.rat_denom);
	size_t p_factor = q.rat_denom / c;
	size_t q_factor = p.rat_denom / c;

	p = rat_get_extended(p, p_factor);
	q = rat_get_extended(q, q_factor);

	assert(p.rat_denom == q.rat_denom);

	bool sign = 0;
	size_t num = 0;
	size_t denom = p.rat_denom;

	if (p.rat_sign == q.rat_sign) {
		sign = p.rat_sign;
		num = p.rat_num + q.rat_num;
	} else if (p.rat_num > q.rat_num) {
		sign = p.rat_sign;
		num = p.rat_num - q.rat_num;
	} else {
		sign = q.rat_sign;
		num = q.rat_num - p.rat_num;
	}

	Rational rat = {
		.rat_sign = sign,
		.rat_num = num,
		.rat_denom = denom,
	};
	return rat_get_normal(rat);
}

Rational rat_get_prod(Rational p, Rational q) {
	Rational rat =  {
		.rat_sign = p.rat_sign ^ q.rat_sign,
		.rat_num = p.rat_num * q.rat_num,
		.rat_denom = p.rat_denom * q.rat_denom,
	};
	return rat_get_normal(rat);
}

Rational rat_get(long long p, long long q) {
	size_t num = abs(p), denom = abs(q);
	bool sign = ((p > 0) - (p < 0)) ^ ((q > 0) - (q < 0));
	Rational rat = {
		.rat_sign = sign,
		.rat_num = num,
		.rat_denom = denom,
	};
	return rat;
}

long double rat_get_float(Rational p) {
	return (p.rat_num / p.rat_denom) * (-1 + (2 * p.rat_sign));
}

Rational rat_get_normal(Rational x) {
	size_t c = gcd(x.rat_num, x.rat_denom);
	Rational rat = {
		.rat_sign = x.rat_sign,
		.rat_num = x.rat_num / c,
		.rat_denom = x.rat_denom / c,
	};
	return rat;
}

Rational rat_get_extended(Rational x, size_t a) {
	Rational rat = {
		.rat_sign = x.rat_sign,
		.rat_num = x.rat_num * a,
		.rat_denom = x.rat_denom * a,
	};
	return rat;
}

/* https://en.wikipedia.org/wiki/Greatest_common_divisor#Binary_GCD_algorithm */
size_t gcd(size_t p, size_t q) {
	size_t d = 0;
	while (p % 2 == 0 && q % 2 == 0) {
		p /= 2;
		q /= 2;
		++d;
	}

	while(p % 2 == 0) p /= 2;
	while(q % 2 == 0) q /= 2;

	while (p != q) {
		if (p > q) {
			p -= q;
			while (p % 2 == 0) p /= 2;
		} else {
			q -= p;
			while (q % 2 == 0) q /= 2;
		}
	}
	return (1u << d) * p;
}