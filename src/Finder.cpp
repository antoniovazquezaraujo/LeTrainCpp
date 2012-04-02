#include "Basic.h"
#include "Finder.h"
#include "FinderAspect.h"
#include "RailVehicle.h"
#include "Rail.h"
#include "Window.h"
#include "Aspect.h"



Finder::Finder(RailVehicle * r)
	:RailVehicle(new FinderAspect()){
	if(r){
		gotoRail(r->getRail());
	}else{
		pos.row=0;
		pos.col=0;
		rail = 0;
		dir = Dir::NO_DIR;
	}
	
}
Finder::~Finder(){

}
Rail * Finder::getRail(){
	return rail;
}
void Finder::incImpulseGenerated(float n){

}
void Finder::decImpulseGenerated(float n){

}
void Finder::generateImpulse(){

}
void Finder::consumeImpulse(){

}
float Finder::getImpulse(){
	return 0;
}
float Finder::receiveImpulse(float imp, Dir d){
	return 0;

}
Dir Finder::getDir(){
	return dir;
}
void Finder::setDir(Dir dir){
	this->dir = dir;
}
bool Finder::isEmpty(){
	return !(rail != 0 && rail->getRailVehicle() != 0);
}
void Finder::gotoRail(Rail * r){
	if(dir.getValue() != Dir::NO_DIR){
		dir = r->getPath(-dir);
	}else{
		dir = r->getAnyPath();
	}
	pos = r->getPos();
	rail = r;
}
void Finder::addToTrain(int, Train *){
	//dont!
}
bool Finder::forward(int distance){
	bool ret = false;
	if(dir.getValue() == Dir::NO_DIR){
		dir = rail->getAnyPath();
	}
	Rail * next = rail->getLinkedRailAt(dir);
	if( next ){
		gotoRail(next);
		ret = true;
	}else{
		ret = false;
	}

	return ret;
}
void Finder::reverse(){
	if(rail){
		dir = rail->getPath(rail->getPath(-dir));
	}
}
bool Finder::backwards(int distance){
	bool ret = false;
	
	if(dir.getValue() == Dir::NO_DIR){
		
		dir = rail->getPath(rail->getPath(-dir));
	}
	
	Rail * next = rail->getLinkedRailAt(dir);
	if( next ){
		
		gotoRail(next);
		ret = true;
	}else{
		
		ret = false;
	}
	
	return ret;
}
void Finder::paint(Window * g){
	g->setBg(getAspect()->getBgColor());
	g->setFg(getAspect()->getFgColor());
	//g->putC(pos.row, pos.col, getAspect()->getAspectChar()); 
	g->putC(pos.row, pos.col-1, '['); 
	g->putC(pos.row, pos.col+1, ']'); 
}
void Finder::erase(Window * g){
	g->setBg(getAspect()->getBgColor());
	g->setFg(getAspect()->getFgColor());
	//g->putC(pos.row, pos.col, ' ');  
	g->putC(pos.row, pos.col-1, ' '); 
	g->putC(pos.row, pos.col+1, ' '); 
}
