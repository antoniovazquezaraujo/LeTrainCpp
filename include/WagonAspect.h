#ifndef WAGONASPECT_H
#define WAGONASPECT_H
#include "Basic.h"
#include "Aspect.h"
#include "Dir.h"

class WagonAspect : public Aspect{
	int getAspectChar();
	int getAspectChar(int dirValue);
	int getBgColor();
	int getFgColor();
	static Logger log;
};
#endif
