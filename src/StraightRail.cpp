#include "Basic.h"
#include "StraightRail.h"
#include "StraightAspect.h"
MAKE_LOGGER(StraightRail);

StraightRail::StraightRail(DirEnv * env)
	:Rail(env, new StraightAspect(env)){

	}
StraightRail::~StraightRail(){
	delete aspect;
}
