#include "Basic.h"
#include "CurveRail.h"
#include "CurveAspect.h"
MAKE_LOGGER(CurveRail);

CurveRail::CurveRail(DirEnv * env)
	:Rail(env, new CurveAspect()){

}
CurveRail::~CurveRail(){
	delete aspect;
}
