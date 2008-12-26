#include "Basic.h"
#include "Rail.h"
#include "DirEnv.h"
#include "RailEnv.h"
#include "Window.h"
#include "Aspect.h"
MAKE_LOGGER(Rail);

Rail::Rail(DirEnv * dirEnv, Aspect * aspect)
	:aspect(aspect), vehicle(0){
	this->dirEnv = new DirEnv(*dirEnv);
	this->railEnv = new RailEnv();
}

Rail::~Rail(){
	delete dirEnv;
	delete railEnv;
}
DirEnv Rail::getEnv(){
	return *dirEnv;
}
Aspect * Rail::getAspect(){
	return aspect; 
}
void Rail::paint(Window * g){
	g->setBg(getAspect()->getBgColor());
	g->setFg(getAspect()->getFgColor());
	g->putC(pos.row, pos.col, getAspect()->getAspectChar());  
}
void Rail::erase(Window * g){
	g->setBg(getAspect()->getBgColor());
	g->setFg(getAspect()->getFgColor());
	g->putC(pos.row, pos.col, ' ');  
}
Dir Rail::getDir(Dir dir){
	return dirEnv->getDir(dir);
}
Dir Rail::getPath(Dir dir){
	return dirEnv->getPath(dir);
}
Dir Rail::getAnyPath(){
	return dirEnv->getAnyPath();
}
void Rail::setRailVehicle(RailVehicle * vehicle){
	this->vehicle = vehicle;
}
RailVehicle * Rail::getRailVehicle(){
	return vehicle;
}
Point Rail::getPos(){
	return pos;
}
void Rail::setPos(Point pos){
	this->pos = pos;
}
Rail * Rail::getLinkedRailAt(Dir dir){
	return railEnv->getRail(dir);
}
void Rail::linkRailAt(Dir dir, Rail * rail){
	railEnv->addPath(dir, rail);
	if(rail){
		if(rail->getLinkedRailAt(-dir) != this){
			rail->linkRailAt(-dir, this);
		}
	}
}
ostream & operator << (ostream & o, Rail & rail){
	/*
	o << endl << "{" ;
	o << *(rail.railEnv);
	o << "}" << endl;
	*/
	DirEnv * env = rail.dirEnv;
	int n ;
	o << endl; 
	//first row
	n= NW;
	o << "[" << setw(2) << (env->getPath(n).getValue() != NO_DIR? env->getPath(n).getName():" ") << "]";
	n= N;
	o << "[" <<setw(2) <<  (env->getPath(n).getValue() != NO_DIR? env->getPath(n).getName():" ") << "]";
	n= NE;
	o << "[" << setw(2) << (env->getPath(n).getValue() != NO_DIR? env->getPath(n).getName():" ") << "]";
	o << endl; 

	// central row
	n= W;
	o << "[" << setw(2) << (env->getPath(n).getValue() != NO_DIR? env->getPath(n).getName():" ") << "]";
	// central point
	o << "( " << rail.getAspect()->getAspectChar() << ")" ; 
	n= E;
	o << "[" << setw(2) << (env->getPath(n).getValue() != NO_DIR? env->getPath(n).getName():" ") << "]";
	o << endl; 

	// third row
	n= SW;
	o << "[" << setw(2) << (env->getPath(n).getValue() != NO_DIR? env->getPath(n).getName():" ") << "]";
	n= S;
	o << "[" << setw(2) << (env->getPath(n).getValue() != NO_DIR? env->getPath(n).getName():" ") << "]";
	n= SE;
	o << "[" << setw(2) << (env->getPath(n).getValue() != NO_DIR? env->getPath(n).getName():" ") << "]";
	o << endl; 
	return o;
}
