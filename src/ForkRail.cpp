#include "Basic.h"
#include "ForkRail.h"
#include "ForkAspect.h"
#include "ForkDirSelector.h"
MAKE_LOGGER(ForkRail);

ForkRail::ForkRail(DirEnv * env)
	:Rail(env, new ForkAspect(env)), selector(new ForkDirSelector(env)){
	forkAspect = (ForkAspect*)this->aspect;	
	selector->setNextDir();
	aspect->updateAspect(selector->getSelectedDir());
	LOG_DEBUG(log,"creando fork");
}
ForkRail::~ForkRail(){
//	delete aspect;
//	delete selector;
}
Dir ForkRail::getPath(Dir dir){
	LOG_DEBUG(log,"getPath de dir:" << dir.getValue() << ":" << selector->getPath(dir).getValue());
	return selector->getPath(dir);
}
void ForkRail::selectDir(Dir dir){
	selector->selectDir(dir);
	aspect->updateAspect(dir);
}
Dir ForkRail::getSelectedDir(){
	LOG_DEBUG(log,"selectedDir:" << selector->getSelectedDir().getValue());
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
