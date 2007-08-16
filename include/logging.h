#ifndef LOGGING_H
#define LOGGING_H

#include <cstdio>
#include <cstdarg>
#include <cstdlib>

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
