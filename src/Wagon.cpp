#include "Basic.h"
#include "Wagon.h"
#include "Train.h"
#include "WagonAspect.h"


Wagon::Wagon()
	:RailVehicle(new WagonAspect()){
}
Wagon::~Wagon(){

}
void Wagon::incImpulseGenerated(float n){
}
void Wagon::decImpulseGenerated(float n){
}
void Wagon::paint(Window * g){
	Train * t = getTrain();
	if(t && t->isSelected()){
		g->setFg(getAspect()->getFgColor());
		g->setBg(getAspect()->getBgColor());
	}else {
		g->setFg(getAspect()->getBgColor());
		g->setBg(getAspect()->getFgColor());
	}
	g->putC(pos.row, pos.col, getAspect()->getAspectChar(dir.getValue()));  
}
void Wagon::erase(Window * g){
	g->setBg(getAspect()->getBgColor());
	g->setFg(getAspect()->getFgColor());
	g->putC(pos.row, pos.col, ' ');  
}
