#include "./include/required_errors.h"

int error_cleanup(int err, int prev) {
	errno = prev;
	return -err;
}
