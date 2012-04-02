#ifndef FORKASPECT_H
#define FORKASPECT_H
#include "Basic.h"
#include "StraightAspect.h"
class ForkAspect : public StraightAspect{
public:
	ForkAspect(DirEnv * env);
	void updateAspect(Dir dir);
	int getBgColor();
	int getFgColor();
	
};
#endif
