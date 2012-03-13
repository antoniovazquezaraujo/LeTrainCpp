#include "Basic.h"
#include "StraightAspect.h"
#include "Dir.h"
#include "DirEnv.h"
MAKE_LOGGER(StraightAspect);

StraightAspect::StraightAspect(DirEnv * env)
	: env(env){
	aspectChar = selectAspectChar(getDir(env));
}
int StraightAspect::getAspectChar(){
	return aspectChar;
}
int StraightAspect::getBgColor(){
	return 0;
}
int StraightAspect::getFgColor(){
	return 7;
}

Dir StraightAspect::getDir(DirEnv * env){
	for (Dir dir; dir<Dir::SE;dir++){
		Dir found = env->getPath(dir);
		if(found.getValue() != Dir::NO_DIR){
			return found;
		}
	}
	return Dir::NO_DIR;
}
int StraightAspect::selectAspectChar(Dir dir){
	int ret = HORIZONTAL_LINE;
	switch(dir.getValue()){
		case Dir::E : 
			ret = HORIZONTAL_LINE;
			break;
		case Dir::NE : 
			ret = GO_UP_LINE;
			break;
		case Dir::N  : 
			ret = VERTICAL_LINE;
			break;
		case Dir::NW : 
			ret = GO_DOWN_LINE;
			break;
		case Dir::W  : 
			ret = HORIZONTAL_LINE;
			break;
		case Dir::SW : 
			ret = GO_UP_LINE;
			break;
		case Dir::S  :
			ret = VERTICAL_LINE;
			break;
		case Dir::SE : 
			ret = GO_DOWN_LINE;
			break;
	}
	return ret;
}
