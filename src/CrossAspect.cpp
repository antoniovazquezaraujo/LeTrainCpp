#include "Basic.h"
#include "CrossAspect.h"
MAKE_LOGGER(CrossAspect);

char CrossAspect::getAspectChar(){
	return '+';
}
int CrossAspect::getBgColor(){
	return 0;
}
int CrossAspect::getFgColor(){
	return 1;
}
