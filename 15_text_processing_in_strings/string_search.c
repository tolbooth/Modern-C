#include <stdlib.h>
#include <string.h>

#include "./include/string_search.h"
#include "./include/errors.h"

/**
 * Generates the lookup table for use in the KMP pattern matching algorithm.
 *
 * @param n         length of provided word and table to fill
 * @param tp_wstr   The word for which to generate prefix-suffix table
 * @param table     The resultant table containing at each index the longest
 * 					prefix of the substring s[0...i] that is also a suffix of
 * 					this substring.
 */
void get_kmp_lookup_table(const size_t n, const char tp_wstr[n],
		char table[n]) {

	memset(table, 0, n);

    for (size_t i = 1; i < n; ++i) {
        size_t j = table[i - 1];
        while(j > 0 && tp_wstr[i] != tp_wstr[j])
            j = table[j - 1];
        if (tp_wstr[i] == tp_wstr[j])
            table[i] = j + 1;
    }
}

size_t tp_find_string_in_word(const char tp_tstr[restrict static 1],
		const char tp_wstr[restrict static 1]) {

    const size_t tp_tlen = strlen(tp_tstr);
    const size_t tp_wlen = strlen(tp_wstr);

    char* table = malloc(sizeof(*table) * tp_wlen);
    get_kmp_lookup_table(tp_wlen, tp_wstr, table);

    size_t i = 0; // word counter
    size_t j = 1; // text counter
    while (j < tp_tlen) {
        if (tp_tstr[j] != tp_wstr[i]) {
            if (i == 0)
                j++;
            else
                i = table[i-1];
        } else {
            ++i, ++j;
            if (i == tp_wlen) {
                free(table);
                return j - i;
            }
        }
    }

    free(table);
    return tp_tlen + 1;
}

int tp_replace_word_in_string(char** tp_new_tstr,
	   	const char tp_tstr[restrict static 1],
    	const char tp_old_wstr[restrict static 1],
		const char tp_new_wstr[restrict static 1]) {
	
	if (!tp_new_tstr)
		return -EFAULT;
	if (!tp_tstr)
		return -EFAULT;
	if (!tp_old_wstr)
		return -EFAULT;
	if (!tp_new_wstr)
		return -EFAULT;
	if (*tp_new_tstr)
		return -EFAULT;
    size_t start_index = tp_find_string_in_word(tp_tstr, tp_old_wstr);
    if (start_index > strlen(tp_tstr))
		return -EFAULT; 

    size_t old_wstr_size = strlen(tp_old_wstr);
    size_t new_wstr_size = strlen(tp_new_wstr);
    size_t old_tstr_size = strlen(tp_tstr);
    size_t new_tstr_size = old_tstr_size 
		+ (new_wstr_size - old_wstr_size) + 1;
	
	int err = errno;
    char* temp_tstr = calloc(new_tstr_size, sizeof(*temp_tstr));
	
	if (!temp_tstr) 
		return error_cleanup(ENOMEM, err);

    // s+#+t -> s+#'+t
    memcpy(temp_tstr, tp_tstr, start_index);                        // s+0
    memcpy(&temp_tstr[start_index], tp_new_wstr, new_wstr_size);    // s+#'+0
    memcpy(&temp_tstr[start_index + new_wstr_size],
		   	&tp_tstr[start_index + old_wstr_size],
            old_tstr_size - (start_index + old_wstr_size));         // s+#'+t 
	temp_tstr[new_tstr_size - 1] = 0;

	// assign our newly allocated and set word to return field
	*tp_new_tstr = temp_tstr;

    return new_tstr_size;
}
