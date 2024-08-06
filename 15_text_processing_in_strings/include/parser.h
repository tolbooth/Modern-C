#ifndef PARSER_H
#define PARSER_H

#include "regex.h"

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
 * one. Before implementing grouping, this stack should always have one 
 * element.
 */
int parse(RegexAtom** regex, size_t str_size, char rgx_str[str_size]);

#endif
