#ifndef logging_h
#define logging_h

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define DEBUG
#ifdef DEBUG
#	define lg xlog
#else
#	define lg if(0)xlog
#endif
void xlog(char * name, int level, char *fmt, ...);
void startlog(const char *);
void stoplog();
#endif
