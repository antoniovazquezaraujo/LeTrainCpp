#ifndef ASPECT_H
#define ASPECT_H
#include "Basic.h"
class Aspect{
public:
	virtual int getAspectChar();
	virtual int getAspectChar(int value);
	virtual int getBgColor()=0;
	virtual int getFgColor()=0;
	virtual void updateAspect(Dir d);
};

#endif
