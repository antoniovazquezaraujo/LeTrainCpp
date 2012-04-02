#include "Basic.h"
#include "CrossRail.h"
#include "Rail.h"
#include "CrossAspect.h"


CrossRail::CrossRail(DirEnv * env)
	:Rail(env, new CrossAspect()){

}
CrossRail::~CrossRail(){
	delete aspect;
}
