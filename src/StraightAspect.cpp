#include "Basic.h"
#include "StraightAspect.h"
#include "Dir.h"
#include "DirEnv.h"
MAKE_LOGGER(StraightAspect);

StraightAspect::StraightAspect(DirEnv * env)
	: env(env){
	aspectChar = selectAspectChar(getDir(env));
}
char StraightAspect::getAspectChar(){
	return aspectChar;
}
int StraightAspect::getBgColor(){
	return 0;
}
int StraightAspect::getFgColor(){
	return 1;
}

Dir StraightAspect::getDir(DirEnv * env){
	for (Dir dir; dir<SE;dir++){
		Dir found = env->getPath(dir);
		if(found.getValue() != NO_DIR){
			return found;
		}
	}
	return NO_DIR;
}
char StraightAspect::selectAspectChar(Dir dir){
	char ret = HORIZONTAL_LINE;
	switch(dir.getValue()){
		case E : 
			ret = HORIZONTAL_LINE;
			break;
		case NE : 
			ret = GO_UP_LINE;
			break;
		case N  : 
			ret = VERTICAL_LINE;
			break;
		case NW : 
			ret = GO_DOWN_LINE;
			break;
		case W  : 
			ret = HORIZONTAL_LINE;
			break;
		case SW : 
			ret = GO_UP_LINE;
			break;
		case S  :
			ret = VERTICAL_LINE;
			break;
		case SE : 
			ret = GO_DOWN_LINE;
			break;
	}
	return ret;
}
