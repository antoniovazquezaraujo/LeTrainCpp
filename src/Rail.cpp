#include "Basic.h"
#include "Rail.h"
#include "DirEnv.h"
#include "RailEnv.h"
#include "RailVehicle.h"
#include "Window.h"
#include "Aspect.h"
MAKE_LOGGER(Rail);

Rail::Rail(DirEnv * dirEnv, Aspect * aspect)
	:aspect(aspect), vehicle(0),dirEnv(*dirEnv){
}

Rail::~Rail(){
}
DirEnv Rail::getEnv(){
	return dirEnv;
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
	return dirEnv.getDir(dir);
}
Dir Rail::getPath(Dir dir){
	return dirEnv.getPath(dir);
}
Dir Rail::getAnyPath(){
	return dirEnv.getAnyPath();
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
	return railEnv.getRail(dir);
}
void Rail::linkRailAt(Dir dir, Rail * rail){
	railEnv.addPath(dir, rail);
	if(rail){
		if(rail->getLinkedRailAt(-dir) != this){
			rail->linkRailAt(-dir, this);
		}
	}
}
void Rail::enter(RailVehicle* v){
	setRailVehicle(v);
	Dir dir = v->getDir();
	if(dir.getValue() != Dir::NO_DIR){
		v->setDir(getPath(-dir));
	}else{
		v->setDir(getAnyPath());
	}
	v->setPos(getPos());
	v->setRail(this);
}
ostream & operator << (ostream & o, Rail & rail){
	/*
	o << endl << "{" ;
	o << *(rail.railEnv);
	o << "}" << endl;
	*/
	DirEnv * env = &(rail.dirEnv);
	int n ;
	o << endl; 
	//first row
	n= Dir::NW;
	o << "[" << setw(2) << (env->getPath(n).getValue() != Dir::NO_DIR? env->getPath(n).getName():" ") << "]";
	n= Dir::N;
	o << "[" <<setw(2) <<  (env->getPath(n).getValue() != Dir::NO_DIR? env->getPath(n).getName():" ") << "]";
	n= Dir::NE;
	o << "[" << setw(2) << (env->getPath(n).getValue() != Dir::NO_DIR? env->getPath(n).getName():" ") << "]";
	o << endl; 

	// central row
	n= Dir::W;
	o << "[" << setw(2) << (env->getPath(n).getValue() != Dir::NO_DIR? env->getPath(n).getName():" ") << "]";
	// central point
	o << "( " << rail.getAspect()->getAspectChar() << ")" ; 
	n= Dir::E;
	o << "[" << setw(2) << (env->getPath(n).getValue() != Dir::NO_DIR? env->getPath(n).getName():" ") << "]";
	o << endl; 

	// third row
	n= Dir::SW;
	o << "[" << setw(2) << (env->getPath(n).getValue() != Dir::NO_DIR? env->getPath(n).getName():" ") << "]";
	n= Dir::S;
	o << "[" << setw(2) << (env->getPath(n).getValue() != Dir::NO_DIR? env->getPath(n).getName():" ") << "]";
	n= Dir::SE;
	o << "[" << setw(2) << (env->getPath(n).getValue() != Dir::NO_DIR? env->getPath(n).getName():" ") << "]";
	o << endl; 
	return o;
}
