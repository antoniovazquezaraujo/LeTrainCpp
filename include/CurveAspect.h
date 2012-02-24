#ifndef CURVEASPECT_H
#define CURVEASPECT_H
#include "Basic.h"
#include "Aspect.h"
class CurveAspect : public Aspect{
	char getAspectChar();
	int getBgColor();
	int getFgColor();
	static Logger log;
};
#endif
