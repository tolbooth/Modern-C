#include "./include/parser.h"
// stack size is defined in terms of max uchar val
#define STATE_SIZE UCHAR_MAX + 1

static void* re_mem_err_cleanup(int prev_err) {
	errno = prev_err;
	return 0;
}

RegexElement* re_init(ElementType re_type, Quantifier re_quantifier) { 
	int err = errno;

	RegexElement* new_re = malloc(sizeof(*new_re));
	if (!new_re) 
		return re_mem_err_cleanup(err);
	
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
 *
 */
RegexElement* re_parse(size_t str_size, char rgx_str[str_size]) {
	if (!rgx_str || str_size == 0) 
		return 0;
	
	size_t index = 0;
	int err = errno;

	Stack* procesing_stack = s_init(s_destroy);
	if (!procesing_stack)
		return re_mem_err_cleanup(err);	
	
	while (index < str_size) {
		switch (rgx_str[index]) {
			case '.': 
				assert(0 && "TODO: implement this case");
				break;
			case '*': 
				assert(0 && "TODO: implement this case");
				break;
			case '^':
				assert(0 && "TODO: implement this case");
				break;
			case '?': 
				assert(0 && "TODO: implement this case");
				break;
			case '[': 
				assert(0 && "TODO: implement this case");
				break;
			case ']': 
				assert(0 && "TODO: implement this case");
				break;
			case '\\': 
				assert(0 && "TODO: implement this case");
				break;
			default:
				// just directly push the character we get
				assert(0 && "TODO: implement this case");
				break; 
		}
	}
	
	s_destroy(procesing_stack);

	return 0;
}
