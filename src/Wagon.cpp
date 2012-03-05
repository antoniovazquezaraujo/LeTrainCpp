#include "Basic.h"
#include "Wagon.h"
#include "Train.h"
#include "WagonAspect.h"
MAKE_LOGGER(Wagon);

Wagon::Wagon()
	:RailVehicle(new WagonAspect()){
}
Wagon::~Wagon(){

}
void Wagon::paint(Window * g){
	Train * t = getTrain();
	if(t && t->isSelected()){
		g->setFg(getAspect()->getBgColor());
		g->setBg(getAspect()->getFgColor());
	}else {
		g->setFg(getAspect()->getFgColor());
		g->setBg(getAspect()->getBgColor());

	}
	g->putC(pos.row, pos.col, dir.getAspectChar());  
	/*
	if(isSelected()){
		g->putC(pos.row, pos.col, '*');  
	}else{
		g->putC(pos.row, pos.col, getAspect()->getAspectChar());  
	}
	*/
}
void Wagon::erase(Window * g){
	g->setBg(getAspect()->getBgColor());
	g->setFg(getAspect()->getFgColor());
	g->putC(pos.row, pos.col, ' ');  
}
void Wagon::addToTrain(Train * t){
	t->addWagon(this);	
}
