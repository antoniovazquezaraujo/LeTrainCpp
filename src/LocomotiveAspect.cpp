#include "Basic.h"
#include "LocomotiveAspect.h"
MAKE_LOGGER(LocomotiveAspect);

char LocomotiveAspect::getAspectChar(){
	return 'L';
}
int LocomotiveAspect::getBgColor(){
	return 6;
}
int LocomotiveAspect::getFgColor(){
	return 8;
}
