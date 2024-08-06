#include <stdlib.h>

#include "./include/errors.h"
#include "./include/regex.h"

RegexAtom* re_init(RegexElementType re_type, RegexQuantifier re_quantifier) { 
	int err = errno;

	RegexAtom* new_re = malloc(sizeof(*new_re));
	if (!new_re) {
	 	error_cleanup(ENOMEM, err);	
		return 0; 
	} 
	
	new_re->re_type = re_type;
	new_re->re_quantifier = re_quantifier;	
	new_re->re_negated = 0;

	if (re_type != symbolType) {
		Stack* group = s_init(re_destroy);	// add destructor here
		new_re->group_exp = group;	
	} else {
		new_re->symbol_exp = 0;
	}

	return new_re;
}

// Potential for issue here, though works fine as is. Revisit.
void re_destroy(void* re_slated) {
	if (!re_slated) return;
	
	RegexAtom* re_slated_cast = (RegexAtom*) re_slated;

	if (re_slated_cast->re_type != symbolType) {
		s_destroy(re_slated_cast->group_exp);	
	}

	free(re_slated_cast);
}
