#ifndef WAGONASPECT_H
#define WAGONASPECT_H
#include "Basic.h"
#include "Aspect.h"

class WagonAspect : public Aspect{
	char getAspectChar();
	int getBgColor();
	int getFgColor();
	static Logger log;
};
#endif
