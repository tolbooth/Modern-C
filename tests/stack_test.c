#include "../utils/stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void test_simple_nested_stack() {
	Stack* test_stacked_stack = s_init(s_destroy);	
	assert(test_stacked_stack && "stack init");
	for (size_t i = 0; i < 10; ++i) {
		Stack* nested_stack = s_init(0);
		assert(nested_stack && "stack init");
		for (size_t j = 0; j < 1000; ++j) {
			int check_push = s_push(nested_stack, &j); 
			assert(check_push != -EFAULT && check_push != -ENOMEM);
		}
		int check_push = s_push(test_stacked_stack, nested_stack);	
		assert(check_push != -EFAULT && check_push != -ENOMEM);
	}
	s_destroy(test_stacked_stack);	
}

void test_complex_nested_stack() {
	Stack* test_stacked_stack = s_init(s_destroy);	
	assert(test_stacked_stack && "stack init");
	for (size_t i = 0; i < 10; ++i) {
		Stack* nested_stack = s_init(free);
		assert(nested_stack && "stack init");
		for (size_t j = 0; j < 1000; ++j) {
			size_t* arr = malloc(100 * sizeof arr);
			for (size_t k = 0; k < 100; ++k) {
				arr[k] = k;
			}
			int check_push = s_push(nested_stack, arr); 
			assert(check_push != -EFAULT && check_push != -ENOMEM);
		}
		int check_push = s_push(test_stacked_stack, nested_stack);	
		assert(check_push != -EFAULT && check_push != -ENOMEM);
	}
	s_destroy(test_stacked_stack);
}

void test_basic_functionality() {
	Stack* stack = s_init(0);
	assert(stack && "stack init");
	int test_arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8 , 9, 10 };
	for (size_t i = 0; i < 10; ++i) {
		s_push(stack, &test_arr[i]);
	}
	int popped_data = *(int*) s_pop(stack);
	assert(popped_data == 10 && "correct data popped");
	s_destroy(stack);
}

int main(void) {
	test_basic_functionality();
	test_simple_nested_stack();
	test_complex_nested_stack();
	
	return EXIT_SUCCESS;
}
