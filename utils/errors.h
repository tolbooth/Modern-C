#ifndef ERRORS_H
#define ERRORS_H

#include <errno.h>
#ifndef EFAULT
# define EFAULT EDOM
#endif
#ifndef ENOMEM
# define ENOMEM (EFAULT - EOF)
# if ENOMEM > INT_MAX
#  error ENOMEM constant too large
# endif
#endif

#endif
