#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <limits.h>
#include "required_errors.h"

typedef struct Stack Stack;
struct Stack {
	void* s_tail;
	void (*s_destroy_data)(void* data);
	size_t s_size;
};

typedef struct StackNode StackNode;
struct StackNode {
	StackNode* sn_prev;
	void* sn_data; 
};

/**
 * Push
 *
 */
int s_push(Stack* s_stack, void* sn_data);

/**
 * Pop
 *
 */
void* s_pop(Stack* s_stack);

/**
 * Peek
 *
 */
void* s_peek(Stack* s_stack);

/**
 * Create
 *
 */
Stack* s_init(void (*s_destroy_data)(void* data));

/**
 * Destroy
 *
 */
void s_destroy(void* s_stack);

/*
 * Compare
 *
 */
int s_compare(Stack* stack_a, Stack* stack_b,
		int (*compare_fn)(void* a, void* b));

#endif
