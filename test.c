#include "test.h"
static int numfails= 0;
void _fail_unless (int result, const char *file, int line, const char *expr, ...) {
	const char *msg;
	if (!result) {
		va_list ap;

		printf("%s:%d:", file, line);
		va_start(ap,expr);
		msg = (const char*)va_arg(ap, char *);
		if (msg == NULL){
			msg = expr;
		}
		vprintf(msg, ap);
		printf("\n");
		va_end(ap);
		numfails++;	
	}
}
int dotest(void (*test)(void)){
	extern int numfails;
	numfails =0;	
	setup();
	test();
	cleanup();
	int ret = numfails;
	numfails = 0;
	return ret;
}
int dotestset(void (*tests[])()){
	int ret = 0;
	int ntest;
	void (*f)() = NULL;
	for(ntest = 0;(f = tests[ntest])!= NULL; ntest++){
		ret += dotest(f);
	}
	return ret;
}
