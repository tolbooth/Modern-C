#ifndef STRING_MANIP_UTILS
#define STRING_MANIP_UTILS

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
		char table[n]);

size_t tp_find_string_in_word(const char tp_tstr[restrict static 1],
		const char tp_wstr[restrict static 1]);

/**
 * Replaces the provided word in given string with a new word.
 *
 * If old word is not found in this string, no changes are made. Additional
 * memory is allocated to store the new word, meaning the user is responsible
 * for freeing. If the user passes in a pointer to already allocated memory,
 * this function will return without further allocation.
 *
 * @param tp_new_tstr	Result populated by function, the changed string.
 * @param tp_tstr		Original string in which to search
 * @param tp_old_wstr	Original word to be replaced
 * @param tp_new_wstr	New word that replaces tp_old_wstr
 *
 * @return 				The size of the newly allocated string on success, or 
 * 						an error code on failure. EFAULT indicates that either
 * 						tp_old_wstr was not found, that the user passed a bad
 * 						value, or that tp_new_tstr points to already allocated
 * 						memory.
 */
int tp_replace_word_in_string(char** tp_new_tstr,
	   	const char tp_tstr[restrict static 1],
    	const char tp_old_wstr[restrict static 1],
		const char tp_new_wstr[restrict static 1]);

#endif
