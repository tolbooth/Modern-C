#include "./include/parser.h"

RegexElement* re_init(ElementType re_type, Quantifier re_quantifier) { 
	int err = errno;

	RegexElement* new_re = malloc(sizeof(*new_re));
	if (!new_re) {
	 	error_cleanup(ENOMEM, err);	
		return 0; 
	} 
	
	new_re->re_type = re_type;
	new_re->re_quantifier = re_quantifier;	

	if (re_type == nodeElement) {
		Stack* re_stack = s_init(re_destroy);	// add destructor here
		new_re->re_stack = re_stack;	
	}

	return new_re;
}

// Potential for issue here, though works fine as is. Revisit.
void re_destroy(void* re_slated) {
	if (!re_slated) return;
	
	RegexElement* re_slated_cast = (RegexElement*) re_slated;

	if (re_slated_cast->re_type == nodeElement) {
		s_destroy(re_slated_cast->re_stack);	
	}

	free(re_slated_cast);
}

/**
 * mallocs space for node, must be handled with re_destroy
 * issues:
 *  1. handling character classes is messy
 *  2. negating a an element or group is not possible
 * 
 * in both of these cases, we need some sense of "any of these, but must be
 * at least one of them". do we need to have a new type of RegexElement?
 * need 4 characters to define boundaries of a range, to allow for negation
 */
int re_parse(RegexElement** regex, size_t str_size, char rgx_str[str_size]) {
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
		 RegexElement* next_elem = 0;
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
	 		case '.': 
	 			assert(0 && "TODO: implement this case");
				++index;
	 			break;
			// handle character classes. we need to match specific formats,
			// alphas and numerics first, defined in a range and separated by a
			// dash.  
	 		case '[': 
	 			assert(0 && "TODO: implement this case");
				++index;
	 			break;
	 		case ']': 
	 			assert(0 && "TODO: implement this case");
				++index;
	 			break;
	 		case '\\': 
	 			// push the character in the next index, escaped. returns val
				// -EFAULT if this is the last character in the regex string. 
				if (index + 1 > str_size) {
					return_code = error_cleanup(EFAULT, err);
					goto CLEANUP;
				}

				next_elem = re_init(leafElement, exactlyOne);
				if (!next_elem) {
					return_code = error_cleanup(ENOMEM, err);
					goto CLEANUP;		
				}

				next_elem->re_value = rgx_str[index + 1];
				s_push(first_group, next_elem);
				index += 2;
	 			break;
	 		default:
	 			// just directly push the character we get
				next_elem = re_init(leafElement, exactlyOne);

				if (!next_elem) {
					return_code = error_cleanup(ENOMEM, err);
					goto CLEANUP;		
				}

				next_elem->re_value = rgx_str[index];
				s_push(first_group, next_elem);
				++index;
	 			break; 
	 	}
	}

	return_code = index;	
	assert(index == str_size);
	assert(processing_stack->s_size == 1);

	RegexElement* final_group = re_init(nodeElement, exactlyOne);
	final_group->re_stack = (Stack*) s_pop(processing_stack); 
	*regex = final_group; 
 CLEANUP:	 
	s_destroy(processing_stack);
	return return_code;
}
