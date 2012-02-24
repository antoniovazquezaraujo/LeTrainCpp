#ifndef ASPECT_H
#define ASPECT_H
#include "Basic.h"
class Aspect{
public:
	virtual char getAspectChar();
	virtual int getBgColor()=0;
	virtual int getFgColor()=0;
	virtual void updateAspect(Dir d);
};

#endif
