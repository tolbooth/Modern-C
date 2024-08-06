#ifndef REGEX_H
#define REGEX_H

#include <stdbool.h>

#include "stack.h"

#define MAX_CLASS_SIZE 63
#define MAX_NUM_GROUPS (MAX_CLASS_SIZE / 2)

typedef enum RegexQuantifier RegexQuantifier;
enum RegexQuantifier {
	zeroOrOne,
	exactlyOne,
	zeroOrMore,
};

typedef enum RegexElementType RegexElementType;
enum RegexElementType {
	conjunctionGroup,
	disjunctionGroup,
	symbolType,
};

typedef struct RegexAtom RegexAtom;
struct RegexAtom {
	RegexElementType re_type;
	RegexQuantifier re_quantifier;
	bool re_negated;
	union {
		Stack* group_exp;
		char symbol_exp;
	};
};

RegexAtom* re_init(RegexElementType	re_type, RegexQuantifier re_quantifier);

void re_destroy(void* rexp);

#endif
