#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* For a text processor, can you use a doubly linked list to store text? The idea is to represent a “blob”
of text through a struct that contains a string (for the text) and pointers to preceding and following blobs.

	- Can you build a function that splits a text blob in two at a given point?
	- One that joins two consecutive text blobs?
	- One that runs through the entire text and puts it in the form of one blob per line?
	- Can you create a function that prints the entire text or prints until the text is cut off due to the screen size? 
*/

typedef struct TextBlob TextBlob;
struct TextBlob {
	TextBlob* tb_next;
	TextBlob* tb_prev;
	char tb_text[];
};

typedef struct TextBlobDLL TextBlobDLL;
struct TextBlobDLL {
	TextBlob* head;
	TextBlob* tail;
};

TextBlob* tb_get_new_blob(char tb_text[], TextBlob* tb_previous, TextBlob* tb_next) {
	return malloc(2 * sizeof(TextBlob*) + strlen(tb_text));
}

TextBlob* split_text_blob(TextBlob* tb_to_split, size_t i) {
	if (!tb_to_split) return 0;

	return 0;
}

TextBlob* add_text_blob_after(TextBlob* tb_to_add, TextBlob* tb_prior) {
	if (!tb_to_add || !tb_prior) return 0;

	tb_to_add->tb_prev = tb_prior;
	tb_to_add->tb_next = tb_prior->tb_next;
	tb_prior->tb_prev = tb_to_add;

	return tb_to_add;
}

int main(int argc, char* argv[argc+1]) {

	return EXIT_SUCCESS;
}