#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "./include/parser.h"
#include "./include/string_search.h"

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
	RegexElement* reg_a = (RegexElement*) item_a;
	RegexElement* reg_b = (RegexElement*) item_b;

	if (!reg_a || !reg_b)
		return 0;
	if (reg_a->re_type != reg_b->re_type)
		return 0;
	if (reg_a->re_quantifier != reg_b->re_quantifier)
		return 0;

	if (reg_a->re_type == leafElement)
		return reg_a->re_value == reg_b->re_value;
	else
		return s_compare(reg_a->re_stack, reg_b->re_stack, compare_regex);
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
	RegexElement* test_elem_a = re_init(nodeElement, zeroOrOne); 
	RegexElement* test_elem_b = re_init(leafElement, zeroOrMore); 

	assert(test_elem_a != 0);
	assert(test_elem_b != 0);
	
	assert(re_parse(0, 0, 0) == -EFAULT);
	assert(re_parse(&test_elem_a, 2, "to") == -EFAULT);

	re_destroy(test_elem_a);
	re_destroy(test_elem_b);	
	test_elem_a = 0;
	test_elem_b = 0;

	assert(re_parse(&test_elem_a, 0, 0) == -EFAULT);

	// valid regex. check if this is parsed correctly
	assert(re_parse(&test_elem_b, 2, "to") == 2);

	assert(test_elem_b->re_type == nodeElement);
	assert(test_elem_b->re_quantifier == exactlyOne);
	
	Stack* expected_stack = s_init(re_destroy);	
	RegexElement* t_regex = re_init(leafElement, exactlyOne); 
	RegexElement* o_regex = re_init(leafElement, exactlyOne); 
	t_regex->re_value = 't';
	o_regex->re_value = 'o';
	s_push(expected_stack, t_regex);
	s_push(expected_stack, o_regex);
	
	RegexElement* expected_regex = re_init(nodeElement, exactlyOne);
	expected_regex->re_stack = expected_stack;
	assert(compare_regex(test_elem_b, expected_regex));
	
	re_destroy(expected_regex);
	re_destroy(test_elem_b);	
}
