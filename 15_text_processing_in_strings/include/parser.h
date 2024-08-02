#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include "stack.h"


typedef enum Quantifier Quantifier;
enum Quantifier {
	zeroOrOne = 0,
	exactlyOne = 1,
	zeroOrMore = 2,
};

typedef enum ElementType ElementType;
enum ElementType {
	leafElement,
	nodeElement,
}; 

typedef struct RegexElement RegexElement;
typedef struct RegexStack RegexStack;
struct RegexElement {
	ElementType re_type; 
	union {
		char re_value;
		Stack* re_stack;
	};
	Quantifier re_quantifier;
};

RegexElement* re_init(ElementType re_type, Quantifier re_quantifier);

void re_destroy(void* re_slated);

/*
 * Think of a string as a stream of characters, then each character is an event
 * that modifies the state.
 *
 */ 

/**
 * Parses the provided string as a regular expression and returns some data
 * structure of states that we can operate on.
 * 1. Support character classes, wildcards, etc.
 * 2. Support POSIX character classes
 * 3. Support grouping
 * 
 * Want we want is a stack of states arrays, and we want to end up with only
 * one. Before implementing grouping, this stack should always have one element.
 */
int re_parse(RegexElement** regex, size_t str_size, char rgx_str[str_size]);

#endif
