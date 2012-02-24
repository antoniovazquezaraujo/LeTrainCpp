#ifndef LOCOMOTIVEASPECT_H
#define LOCOMOTIVEASPECT_H
#include "Basic.h"
#include "Aspect.h"

class LocomotiveAspect : public Aspect{
	char getAspectChar();
	int getBgColor();
	int getFgColor();
	static Logger log;
};
#endif
