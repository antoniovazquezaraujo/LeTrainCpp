#include "Basic.h"
#include "FinderAspect.h"
MAKE_LOGGER(FinderAspect);

int FinderAspect::getAspectChar(){
	return '@';
}
int FinderAspect::getBgColor(){
	return 0;
}
int FinderAspect::getFgColor(){
	return 12;
}
