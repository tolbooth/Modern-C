#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "./include/parser.h"
#include "./include/string_manip_utils.h"
/** 
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

int main(void) {
    const char* first = "aabaabaac";
    const char* first_key = "aac";
    const char* second = "christmas";
    const char* second_key = "istm";
  	
    // string comparison tests	
   	assert(tp_find_string_in_word(first, first_key) == 6);
	assert(tp_find_string_in_word(second, second_key) == 3);

	char* replaced_str = tp_replace_word_in_string(first, first_key, second);
	assert(!strcmp(replaced_str, "aabaabchristmas"));
	free(replaced_str);

	// regex element basics tests
	RegexElement* test_elem_a = re_init(nodeElement, zeroOrOne) ; 
	RegexElement* test_elem_b = re_init(leafElement, zeroOrMore) ; 

	assert(test_elem_a != 0);
	assert(test_elem_b != 0);
	
	re_destroy(test_elem_a);
	re_destroy(test_elem_b);	
	// no asserts yet as no implementation	
	re_parse(0, 0);
	re_parse(2, "to");
}
