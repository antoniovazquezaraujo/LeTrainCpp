#include "debug.h"

void startlog(char * logname){
	logfile = fopen(logname, "a");
}

void stoplog(){
	fclose(logfile);
}

void xlog(char * name, int level, char *fmt, ...) {
	va_list arg;
	fprintf(logfile, "%s:%d:", name, level);
	va_start(arg, fmt);
	vfprintf(logfile, fmt, arg);
	va_end(arg);
	fprintf(logfile, "\n");
}

