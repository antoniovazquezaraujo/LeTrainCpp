#include "Basic.h"
#include "LocomotiveAspect.h"


int LocomotiveAspect::getAspectChar(){
	return '?';
}
int LocomotiveAspect::getAspectChar(int value){
	int ret = 0; 
	switch(value){
	case Dir::N:
		ret =  ACS_UARROW;
		break;
	case Dir::S:
		ret =  ACS_DARROW;
		break;
	case Dir::E:
		ret =  ACS_RARROW;
		break;
	case Dir::W:
		ret =  ACS_LARROW;
		break;
	case Dir::NE:
		ret =  ACS_URCORNER;
		break;
	case Dir::SW:
		ret =  ACS_LLCORNER;
		break;
	case Dir::NW:
		ret =  ACS_ULCORNER;
		break;
	case Dir::SE:
		ret =  ACS_LRCORNER;
		break;
	default:
		ret = '*';
		break;
	}
	return ret;
}
int LocomotiveAspect::getBgColor(){
	return 1;
}
int LocomotiveAspect::getFgColor(){
	return 8;
}
