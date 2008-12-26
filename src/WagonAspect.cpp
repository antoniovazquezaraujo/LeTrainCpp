#include "Basic.h"
#include "WagonAspect.h"

MAKE_LOGGER(WagonAspect);

char WagonAspect::getAspectChar(){
	return '#';
}
int WagonAspect::getBgColor(){
	return 2;
}
int WagonAspect::getFgColor(){
	return 12;
}
