#ifndef FINDERASPECT_H
#define FINDERASPECT_H
#include "Basic.h"
#include "Aspect.h"
class FinderAspect: public Aspect{
	char getAspectChar();
	int getBgColor();
	int getFgColor();
	static Logger log;
};
#endif
