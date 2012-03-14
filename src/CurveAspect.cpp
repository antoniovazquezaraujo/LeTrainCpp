#include "Basic.h"
#include "CurveAspect.h"
MAKE_LOGGER(CurveAspect);

int CurveAspect::getAspectChar(){
	return ACS_BULLET;
}
int CurveAspect::getBgColor(){
	return 0;
}
int CurveAspect::getFgColor(){
	return 7;
}
