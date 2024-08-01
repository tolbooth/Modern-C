#include <stdlib.h>
#include <stdint.h>

/* Identify elements of the base set with N in 0,1,...
	each representing a partition with a forest data
	structure, where each element has  a parent: another
	element in the same partition. Each set in these 
	partitions are identified by an element that forms 
	the root of the set. We need to allow for 2 operations:

	1.	Find: recieves one element of the ground set and
		returns the root of the corresponding set.
	2. 	Union: recieves two elements and merges their
		respective sets.
	*/

size_t Find(size_t parent[], size_t elem, size_t len);

void FindReplace(size_t parent[], size_t target, size_t next, size_t len);

size_t FindCompress(size_t parent[], size_t next, size_t len);

void Union(size_t parent[], size_t elem1, size_t elem2, size_t len);

void Init(size_t parent[], size_t len);

int main(void) {
	return EXIT_SUCCESS;
}

void Init(size_t parent[], size_t len) {
	if (!parent) {
		return;
	}

	for (int i = 0; i < len; ++i) {
		parent[i] = SIZE_MAX;
	}
}

size_t Find(size_t parent[], size_t elem, size_t len) {
	if (!parent || elem >= len) {
		return SIZE_MAX;
	} else if (parent[elem] == SIZE_MAX) {
		return elem;
	}
	return Find(parent, parent[elem], len);
}

void FindReplace(size_t parent[], size_t target, size_t next, size_t len) {
	if (!parent || next >= len) {
		return;
	}
	size_t temp = parent[next];
	parent[next] = target;
	FindReplace(parent, target, temp, len);
}

size_t FindCompress(size_t parent[], size_t next, size_t len) {
	if (!parent || next >= len) {
		return;
	}
	size_t root = Find(parent, next, len);
	FindReplace(parent, root, next, len);
	return root;
}

void Union(size_t parent[], size_t elem1, size_t elem2, size_t len) {
	size_t root = FindCompress(parent, elem1, len);
	FindReplace(parent, root, elem2, len);
}