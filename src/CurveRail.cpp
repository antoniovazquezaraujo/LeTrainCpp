#include "Basic.h"
#include "CurveRail.h"
#include "CurveAspect.h"


CurveRail::CurveRail(DirEnv * env)
	:Rail(env, new CurveAspect()){

}
CurveRail::~CurveRail(){
	delete aspect;
}
