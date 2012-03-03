#include "Basic.h"
#include "Locomotive.h"
#include "Train.h"
#include "LocomotiveAspect.h"
#include "Window.h"

MAKE_LOGGER(Locomotive);

Locomotive::Locomotive()
	:RailVehicle(new LocomotiveAspect()){

}
Locomotive::~Locomotive(){

}
int Locomotive::getId(){
	return id;
}
void Locomotive::decPower(int dec){
	power-=dec;
}
void Locomotive::incPower(int inc){
	power+=inc;
}
void Locomotive::setId(int id){
	this->id = id;
}
int Locomotive::getPower(){
	return power;
}
void Locomotive::setPower(int power){
	this->power = power;
} 
void Locomotive::paint(Window * g){
	Train * t = getTrain();
	if(t && t->isSelected()){
		g->setFg(getAspect()->getBgColor());
		g->setBg(getAspect()->getFgColor());
	}else {
		g->setFg(getAspect()->getFgColor());
		g->setBg(getAspect()->getBgColor());

	}
	//g->putC(pos.row, pos.col, getAspect()->getAspectChar());  
	g->putC(pos.row, pos.col, dir.getAspectChar());  
}
void Locomotive::erase(Window * g){
	g->setBg(getAspect()->getBgColor());
	g->setFg(getAspect()->getFgColor());
	g->putC(pos.row, pos.col, ' ');  
}

void Locomotive::addToTrain(Train * t){
	t->addLocomotive(this);
}
