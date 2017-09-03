// Based on minunit with usability modifications
#ifndef SUNIT_H
#define SUNIT_H

#include <stdio.h>

#define su_assert(_message, test) \
do {\
	if (!(test)) {\
		message = _message;\
		return;\
	}\
} while (0)
#define su_run_test(test) \
do {\
	fprintf(stderr, "Running "#test"...");\
	message = NULL;\
	test();\
	tests_run++;\
	if (message) {\
		fprintf(stderr, " FAILED\n");\
		fprintf(stderr, "\tError: %s\n\n", message);\
		tests_failed++;\
	} else {\
		fprintf(stderr, " PASSED\n");\
	}\
} while (0)
#define su_test_summary() \
do {\
	fprintf(stderr, "\nRan %d test(s), %d failure(s), %d success(es).\n",\
			tests_run, tests_failed, tests_run - tests_failed);\
} while (0)

extern int tests_run;
extern int tests_failed;
extern char * message;

#endif
