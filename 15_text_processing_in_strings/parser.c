#include <assert.h>
#include <limits.h>
#include "./include/parser.h"
#include "./include/regex.h"
#include "./include/errors.h"

/**
 * Generates an AST based on the provided regex string.
 * 
 * @param regex		the out param, a reference to generated AST root. 
 * @param str_size	size of string to parse.
 * @param rgx_str	string representing a valid regular expression.
 *
 * @return 			number of characters consumed, should match str_size. may
 *					return error codes -EFAULT for invalid input, and -ENOMEM
 *					if execution encounters an allocation error.
 *
 */
int parse(RegexAtom** regex, size_t str_size, char rgx_str[str_size]) {
	if (!regex)
		return -EFAULT;
	if (*regex)
		return -EFAULT;
	if (!rgx_str)
		return -EFAULT;
	if (str_size == 0) 
		return -EFAULT;
	
	size_t index = 0;
	int err = errno;
	int return_code = 0;

	Stack* processing_stack = s_init(s_destroy);
	if (!processing_stack)
		return error_cleanup(ENOMEM, err); 

	Stack* first_group = s_init(re_destroy);
	if (!first_group) {
		return_code = error_cleanup(ENOMEM, err); 
		goto CLEANUP;
	}
	
	s_push(processing_stack, first_group);
	
	bool negated_flag = 0;
	while (index < str_size) {
		RegexAtom* next_elem = 0;
		RegexAtom* prev_elem = 0;
		Stack* new_group = 0;
		char next_char = rgx_str[index]; 
		switch (next_char) {
	 		case '*': 
				// god this is ugly... i should fix my stack
				prev_elem =	(RegexAtom*) 
					s_peek((Stack*) s_peek(processing_stack));
				// only one quantifier should be allowed at once. this can be
				// changed to allow for lazy quantifiers.
				if (!prev_elem || prev_elem->re_quantifier != exactlyOne) {
					return_code = error_cleanup(EFAULT, err);
					goto CLEANUP;		
				}
				prev_elem->re_quantifier = zeroOrMore;
				++index;
	 			break;
	 		case '?': 
				prev_elem = (RegexAtom*)
					s_peek((Stack*) s_peek(processing_stack));
				// only one quantifier should be allowed at once. this can be
				// changed to allow for lazy quantifiers.
				if (!prev_elem || prev_elem->re_quantifier != exactlyOne) {
					return_code = error_cleanup(EFAULT, err);
					goto CLEANUP;		
				}
				prev_elem->re_quantifier = zeroOrOne;
				++index;
	 			break;
			case '(':
				new_group = s_init(re_destroy);
				if (!new_group) {
					return_code = error_cleanup(ENOMEM, err);
					goto CLEANUP;		
				}
				s_push(processing_stack, s_init(re_destroy));
				++index;
				break;
			case ')':
				// Make sure we do actually have an open group!
				if (processing_stack->s_size < 2) {
					return_code = error_cleanup(ENOMEM, err);
					goto CLEANUP;		
				}

				new_group = s_pop(processing_stack);

				next_elem = re_init(conjunctionGroup, exactlyOne, negated_flag);

				if (!next_elem) {
					return_code = error_cleanup(ENOMEM, err);
					goto CLEANUP;		
				}

				if (negated_flag)
				   	negated_flag = 0;

				next_elem->group_exp = new_group;
				s_push((Stack*) s_peek(processing_stack), next_elem);
				++index;
	 			break; 
				++index;
				break;
	 		case '^': // make sure to consume this when we push new element on!
				// this approach doesn't work with groups... how could we
				// address this?
				if (rgx_str[index + 1] == '^' || rgx_str[index + 1] == ')' ||
						rgx_str[index + 1] == '*' || rgx_str[index + 1] == '?') {
					return_code = error_cleanup(ENOMEM, err);
					goto CLEANUP;		
				}
				
				// needs a stack >:( with ability to track context, i.e. where
				// was this negation invoked, to handle ^(bc)

				negated_flag = 1;
				++index;
	 			break; 
	 		case '[': // pattern matching! for now, only ranges [a-Z] 
				char range_start = rgx_str[index + 1];
				char range_end = rgx_str[index + 3];
				if (rgx_str[index + 2] != '-' || rgx_str[index + 4] != ']') {
					return_code = error_cleanup(EFAULT, err);
					goto CLEANUP;		
				}
				
				next_elem = re_init(rangeType, exactlyOne, negated_flag);
				if (!next_elem) {
					return_code = error_cleanup(ENOMEM, err);
					goto CLEANUP;		
				}

				next_elem->range_exp.r_start = range_start;
				next_elem->range_exp.r_end = range_end;

				if (negated_flag)
				   	negated_flag = 0;

				s_push((Stack*) s_peek(processing_stack), next_elem);
						
				index += 5;
	 			break;
	 		case ']': // error on mismatched brackets? 
				return_code = error_cleanup(EFAULT, err);
				goto CLEANUP;		
	 			break;
	 		case '.': 
				next_elem = re_init(rangeType, exactlyOne, negated_flag);
				if (!next_elem) {
					return_code = error_cleanup(ENOMEM, err);
					goto CLEANUP;		
				}

				next_elem->range_exp.r_start = CHAR_MIN;
				next_elem->range_exp.r_end = CHAR_MAX;

				if (negated_flag)
				   	negated_flag = 0;

				s_push((Stack*) s_peek(processing_stack), next_elem);
				++index;
	 			break; 
			// alphas and numerics first, defined in a range and separated by a
			// dash.  
	 		case '\\': 
	 			// push the character in the next index, escaped. returns val
				// -EFAULT if this is the last character in the regex string. 
				if (index + 1 > str_size) {
					return_code = error_cleanup(EFAULT, err);
					goto CLEANUP;
				}

				next_elem = re_init(symbolType, exactlyOne, negated_flag);
				if (!next_elem) {
					return_code = error_cleanup(ENOMEM, err);
					goto CLEANUP;		
				}

				if (negated_flag)
				   	negated_flag = 0;
				
				next_elem->symbol_exp = rgx_str[index + 1];
				s_push((Stack*) s_peek(processing_stack), next_elem);
				index += 2;
	 			break;
	 		default:
	 			// just directly push the character we get
				next_elem = re_init(symbolType, exactlyOne, negated_flag);

				if (!next_elem) {
					return_code = error_cleanup(ENOMEM, err);
					goto CLEANUP;		
				}

				if (negated_flag)
				   	negated_flag = 0;
				
				next_elem->symbol_exp = next_char;
				s_push((Stack*) s_peek(processing_stack), next_elem);
				++index;
	 			break; 
	 	}
	}

	return_code = index;	
	assert(index == str_size);
	assert(processing_stack->s_size == 1);

	RegexAtom* final_group = re_init(conjunctionGroup, exactlyOne, 0);
	final_group->group_exp = (Stack*) s_pop(processing_stack); 
	*regex = final_group; 
 CLEANUP:	 
	s_destroy(processing_stack);
	return return_code;
}
