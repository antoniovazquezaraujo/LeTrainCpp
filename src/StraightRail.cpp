#include "Basic.h"
#include "StraightRail.h"
#include "StraightAspect.h"


StraightRail::StraightRail(DirEnv * env)
	:Rail(env, new StraightAspect(env)){

	}
StraightRail::~StraightRail(){
	delete aspect;
}
