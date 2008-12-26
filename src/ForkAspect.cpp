#include "Basic.h"
#include "ForkAspect.h"
#include "Dir.h"
MAKE_LOGGER(ForkAspect);

ForkAspect::ForkAspect(DirEnv * env)
	:StraightAspect(env){

}
void ForkAspect::updateAspect(Dir dir){
	aspectChar = selectAspectChar(dir);
}
int ForkAspect::getBgColor(){
	return 0;
}
int ForkAspect::getFgColor(){
	return 3;
}
