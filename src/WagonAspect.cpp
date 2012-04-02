#include "Basic.h"
#include "WagonAspect.h"



int WagonAspect::getAspectChar(){
	return '#';
}
int WagonAspect::getAspectChar(int value){
	int ret = 0; 
	switch(value){
	case Dir::N:
		ret =  '^';
		break;
	case Dir::S:
		ret =  'v';
		break;
	case Dir::E:
		ret =  '>';
		break;
	case Dir::W:
		ret =  '<';
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
int WagonAspect::getBgColor(){
	return 2;
}
int WagonAspect::getFgColor(){
	return 8;
}
