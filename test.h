#ifndef test_h
#define test_h
#include <stdarg.h>
#include <stdio.h>

/* Fail the test case unless expr is true */
#define fail_unless(expr, ...)\
        _fail_unless(expr, __FILE__, __LINE__,\
        "Assertion '"#expr"' failed" , ## __VA_ARGS__, NULL)

/* Fail the test case if expr is true */
#define fail_if(expr, ...)\
        _fail_unless(!(expr), __FILE__, __LINE__,\
        "Failure '"#expr"' occured", ## __VA_ARGS__, NULL)

/* Always fail */
#define fail(...) _fail_unless(0, __FILE__, __LINE__, "Failed" , ## __VA_ARGS__, NULL)

void test();
int dotest(void (*test)(void));
int dotestset(void (*tests[])());
void setup(void);
void cleanup(void);
void _fail_if(int expr, char *file, int line);
void _fail_unless (int result, const char *file, int line, const char *expr, ...);
#endif
