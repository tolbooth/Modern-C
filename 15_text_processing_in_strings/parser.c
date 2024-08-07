#include <assert.h>

#include "./include/parser.h"
#include "./include/regex.h"
#include "./include/errors.h"

/**
 * mallocs space for node, must be handled with re_destroy
 * issues:
 *  1. handling character classes is messy
 *  2. negating a an element or group is not possible
 * 
 * in both of these cases, we need some sense of "any of these, but must be
 * at least one of them". do we need to have a new type of RegexAtom?
 * need 4 characters to define boundaries of a range, to allow for negation
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
	if (!first_group)
		return error_cleanup(ENOMEM, err); 
	s_push(processing_stack, first_group);
	
	 while (index < str_size) {
		RegexAtom* next_elem = 0;
	 	switch (rgx_str[index]) {
	 		case '*': 
	 			assert(0 && "TODO: implement this case");
				++index;
	 			break;
	 		case '^':
	 			assert(0 && "TODO: implement this case");
				++index;
	 			break;
	 		case '?': 
	 			assert(0 && "TODO: implement this case");
				++index;
	 			break;
	 		case '[': 
	 			assert(0 && "TODO: implement this case");
				++index;
	 			break;
	 		case ']': // error on mismatched brackets? 
	 			assert(0 && "TODO: implement this case");
				++index;
	 			break;
	 		case '.': 
				next_elem = re_init(wildCard, exactlyOne);

				if (!next_elem) {
					return_code = error_cleanup(ENOMEM, err);
					goto CLEANUP;		
				}

				s_push(first_group, next_elem);
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

				next_elem = re_init(symbolType, exactlyOne);
				if (!next_elem) {
					return_code = error_cleanup(ENOMEM, err);
					goto CLEANUP;		
				}

				next_elem->symbol_exp = rgx_str[index + 1];
				s_push(first_group, next_elem);
				index += 2;
	 			break;
	 		default:
	 			// just directly push the character we get
				next_elem = re_init(symbolType, exactlyOne);

				if (!next_elem) {
					return_code = error_cleanup(ENOMEM, err);
					goto CLEANUP;		
				}

				next_elem->symbol_exp = rgx_str[index];
				s_push(first_group, next_elem);
				++index;
	 			break; 
	 	}
	}

	return_code = index;	
	assert(index == str_size);
	assert(processing_stack->s_size == 1);

	RegexAtom* final_group = re_init(conjunctionGroup, exactlyOne);
	final_group->group_exp = (Stack*) s_pop(processing_stack); 
	*regex = final_group; 
 CLEANUP:	 
	s_destroy(processing_stack);
	return return_code;
}
