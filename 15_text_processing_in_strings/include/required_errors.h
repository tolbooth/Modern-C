#ifndef REQUIRED_ERRORS_H
#define REQUIRED_ERRORS_H

#include <errno.h>

#ifndef EFAULT
# define EFAULT EDOM
#endif
#ifndef ENOMEM
# define ENOMEM (EFAULT - EOF)
# if ENOMEM > INT_MAX
#  error ENOMEM constant is too large
# endif
#endif

int error_cleanup(int err, int prev);

#endif
