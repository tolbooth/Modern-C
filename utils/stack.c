#include "stack.h"

static int error_cleanup(int err, int prev) {
	errno = prev;
	return -err;	
}

/**
 * Push
 *
 */
int s_push(Stack* s_stack, void* sn_data) {
	if (!s_stack || !sn_data) 
		return -EFAULT;
	
	int err = errno;
		
	StackNode* sn_new_node = malloc(sizeof(*sn_new_node));
	if (!sn_new_node) {
		return error_cleanup(ENOMEM, err);
	}
	
	sn_new_node->sn_data = sn_data;
	sn_new_node->sn_prev = (StackNode*) s_stack->s_tail;
	s_stack->s_tail = sn_new_node;	
	++s_stack->s_size;
		
	return s_stack->s_size;
}


/**
 * Pop
 * is the problem here? after a pop, data is incorrect.
 */
void* s_pop(Stack* s_stack) {
	if (!s_stack) 
		return 0;
	if (!s_stack->s_tail) 
		return 0;

	StackNode* sn_popped = (StackNode*) s_stack->s_tail;	
	s_stack->s_tail = sn_popped->sn_prev;	
	void* sn_popped_data = sn_popped->sn_data;
	
	--s_stack->s_size;
	free(sn_popped);
	return sn_popped_data;
}
/**
 * Peek
 *
 */
void* s_peek(Stack* s_stack) {
	if (!s_stack) 
		return 0;
	if (!s_stack->s_tail) 
		return 0;
	StackNode* sn_peeked_data = (StackNode*) s_stack->s_tail;
	return sn_peeked_data->sn_data;
}

/**
 * Create
 *
 */
Stack* s_init(void (*s_destroy_data)(void* data)) {
	int err = errno;

	Stack* s_stack = malloc(sizeof(*s_stack));
	if (!s_stack) {
		errno = err;
		return 0; 
	}

	s_stack->s_tail = 0;
	s_stack->s_size = 0;
	s_stack->s_destroy_data = s_destroy_data;

	return s_stack;
}

/**
 * Destroy
 *
 */
void s_destroy(void* s_stack) {
	if (!s_stack)
		return;
	
	Stack* s_stack_cast = (Stack*)s_stack;	

	while(s_stack_cast->s_tail) {
		void* scrap_data = s_pop(s_stack_cast);
		if (s_stack_cast->s_destroy_data)
			s_stack_cast->s_destroy_data(scrap_data);
	}

	free(s_stack_cast);
}
