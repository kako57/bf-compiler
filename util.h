#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>

#define report_error(msg)                                               \
	do {                                                            \
		fprintf(stderr, "%s:%d in %s(): Error: %s\n", __FILE__, \
			__LINE__, __func__,  msg);                                 \
		exit(1);                                                \
	} while (0)

#endif // ifndef UTIL_H
