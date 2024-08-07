#ifndef REGEX_H
#define REGEX_H

#include <stdbool.h>

#include "stack.h"

#define MAX_CLASS_SIZE 63
#define MAX_NUM_GROUPS (MAX_CLASS_SIZE / 2)

typedef enum RegexQuantifier RegexQuantifier;
enum RegexQuantifier {
	zeroOrOne = 0,
	exactlyOne,
	zeroOrMore,
};

typedef enum RegexElementType RegexElementType;
enum RegexElementType {
	conjunctionGroup = 0,
	disjunctionGroup,
	symbolType,
	rangeType,	
};

typedef struct Range Range;
struct Range {
	char r_start;
	char r_end;
};

typedef struct RegexAtom RegexAtom;
struct RegexAtom {
	RegexElementType re_type;
	RegexQuantifier re_quantifier;
	bool re_negated;
	union {
		Stack* group_exp;
		Range range_exp;
		char symbol_exp;
	};
};

RegexAtom* re_init(RegexElementType	re_type, RegexQuantifier re_quantifier,
		bool re_negated);

void re_destroy(void* rexp);

#endif
