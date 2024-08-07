#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "./include/parser.h"
#include "./include/string_search.h"
#include "./include/errors.h"

/* 
 *  Can you search for a given word in a string?
 *  Can you replace a word in a string and return a copy with the new contents?
 *  Can you implement regex-matching functions for strings? E.g. find a
 *  character class such as [A-Q] or [^0-9], match with * (anything) or match
 *  with? (any char)
 *  Or can you implement a regex-matching function for POSIX character classes
 *  such as [[:alpha:]], [[:digit:]], and so on?
 *  Can you stitch these all together to search for a regexp in a string?
 *  Do query replace with regexp against a single word?
 *  Extend a regexp with grouping?
 */

/**
 * Helper function for comparing regex objects. Currently broken!
 *
 */
static int compare_regex(void* item_a, void* item_b) {
	RegexAtom* reg_a = (RegexAtom*) item_a;
	RegexAtom* reg_b = (RegexAtom*) item_b;

	if (!reg_a || !reg_b)
		return 0;
	if (reg_a->re_type != reg_b->re_type)
		return 0;
	if (reg_a->re_quantifier != reg_b->re_quantifier)
		return 0;
	if (reg_a->re_negated != reg_b->re_negated)
		return 0;
	
	switch(reg_a->re_type) {
		case symbolType:
			return reg_a->symbol_exp == reg_b->symbol_exp;
		case wildCard:
			return 1;
		default:
			return s_compare(reg_a->group_exp, reg_b->group_exp, compare_regex);
	}
}

int main(void) {
    const char* first = "aabaabaac";
    const char* first_key = "aac";
    const char* second = "christmas";
    const char* second_key = "istm";
  	
    // string comparison tests	
   	assert(tp_find_string_in_word(first, first_key) == 6);
	assert(tp_find_string_in_word(second, second_key) == 3);
	
	// string replacement tests
	char* replaced_str = 0;
    int check = tp_replace_word_in_string(&replaced_str, 
			first, first_key, second);
	assert(check != -ENOMEM && check != -EFAULT);
	assert(!strcmp(replaced_str, "aabaabchristmas"));
	free(replaced_str);

	check = tp_replace_word_in_string(0, first, first_key, second);
	assert(check == -EFAULT);
	
	check = tp_replace_word_in_string(&replaced_str, first, first_key, second);
	assert(check == -EFAULT);

	// regex element basics tests
	{
		RegexAtom* test_elem_a = re_init(conjunctionGroup, zeroOrOne); 
		RegexAtom* test_elem_b = re_init(symbolType, zeroOrMore); 

		assert(test_elem_a != 0);
		assert(test_elem_b != 0);
		
		assert(parse(0, 0, 0) == -EFAULT);
		assert(parse(&test_elem_a, 2, "to") == -EFAULT);

		re_destroy(test_elem_a);
		re_destroy(test_elem_b);	
		test_elem_a = 0;
		test_elem_b = 0;

		assert(parse(&test_elem_a, 0, 0) == -EFAULT);
	}
	// valid regex. check if this is parsed correctly
	{
		RegexAtom* test_elem_b = 0;
		assert(parse(&test_elem_b, 2, "to") == 2);

		assert(test_elem_b->re_type == conjunctionGroup);
		assert(test_elem_b->re_quantifier == exactlyOne);
		
		Stack* expected_stack = s_init(re_destroy);	
		RegexAtom* t_regex = re_init(symbolType, exactlyOne); 
		RegexAtom* o_regex = re_init(symbolType, exactlyOne); 
		t_regex->symbol_exp = 't';
		o_regex->symbol_exp = 'o';
		s_push(expected_stack, t_regex);
		s_push(expected_stack, o_regex);
		
		RegexAtom* expected_regex = re_init(conjunctionGroup, exactlyOne);
		expected_regex->group_exp = expected_stack;
		assert(compare_regex(test_elem_b, expected_regex));
		
		re_destroy(expected_regex);
		re_destroy(test_elem_b);
	}	
	// valid wildcard regex	
	{
		RegexAtom* test_elem_c = 0; 
		assert(parse(&test_elem_c, 1, ".") == 1);

		Stack* expected_stack = s_init(re_destroy);	
		RegexAtom* wild_regex = re_init(wildCard, exactlyOne); 
		s_push(expected_stack, wild_regex);

		RegexAtom* expected_regex = re_init(conjunctionGroup, exactlyOne);
		expected_regex->group_exp = expected_stack;
		assert(compare_regex(test_elem_c, expected_regex));
		
		re_destroy(expected_regex);
		re_destroy(test_elem_c);
	}
}
