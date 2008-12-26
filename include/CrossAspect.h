#ifndef CROSSASPECT_H
#define CROSSASPECT_H
#include "Basic.h"
#include "Aspect.h"
class CrossAspect : public Aspect{
public:
	char getAspectChar();
	int getBgColor();
	int getFgColor();
	static Logger log;
};
#endif
