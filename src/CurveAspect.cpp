#include "Basic.h"
#include "CurveAspect.h"
MAKE_LOGGER(CurveAspect);

char CurveAspect::getAspectChar(){
	return '.';
}
int CurveAspect::getBgColor(){
	return 0;
}
int CurveAspect::getFgColor(){
	return 7;
}
