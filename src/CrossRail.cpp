#include "Basic.h"
#include "CrossRail.h"
#include "Rail.h"
#include "CrossAspect.h"
MAKE_LOGGER(CrossRail);

CrossRail::CrossRail(DirEnv * env)
	:Rail(env, new CrossAspect()){

}
CrossRail::~CrossRail(){
	delete aspect;
}
