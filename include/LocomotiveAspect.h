#ifndef LOCOMOTIVEASPECT_H
#define LOCOMOTIVEASPECT_H
#include "Basic.h"
#include "Aspect.h"
#include "Dir.h"

class LocomotiveAspect : public Aspect{
	int getAspectChar(int dirValue);
	int getAspectChar();
	int getBgColor();
	int getFgColor();
	
};
#endif
