#include "Basic.h"
#include "ForkRail.h"
#include "ForkAspect.h"
#include "ForkDirSelector.h"


ForkRail::ForkRail(DirEnv * env)
	:Rail(env, new ForkAspect(env)), selector(new ForkDirSelector(env)){
	forkAspect = (ForkAspect*)this->aspect;	
	selector->setNextDir();
	aspect->updateAspect(selector->getSelectedDir());
	
}
ForkRail::~ForkRail(){
//	delete aspect;
//	delete selector;
}
Dir ForkRail::getPath(Dir dir){
	
	return selector->getPath(dir);
}
void ForkRail::selectDir(Dir dir){
	selector->selectDir(dir);
	aspect->updateAspect(dir);
}
Dir ForkRail::getSelectedDir(){
	
	return selector->getSelectedDir();
}
ForkRail & ForkRail::operator++(){
	selector->setNextDir();
	aspect->updateAspect(selector->getSelectedDir());
	return *this;
}
ForkRail & ForkRail::operator--(){
	selector->setPrevDir();
	aspect->updateAspect(selector->getSelectedDir());
	return *this;
}
ostream & operator << (ostream & o, ForkRail & rail){
	o << *rail.selector;
	return o;
}
