#include "Basic.h"
#include "Wagon.h"
#include "WagonAspect.h"
MAKE_LOGGER(Wagon);

Wagon::Wagon()
	:RailVehicle(new WagonAspect()){
}
Wagon::~Wagon(){

}
void Wagon::paint(Window * g){
	g->setBg(getAspect()->getBgColor());
	g->setFg(getAspect()->getFgColor());
	g->putC(pos.row, pos.col, getAspect()->getAspectChar());  
}
void Wagon::erase(Window * g){
	g->setBg(getAspect()->getBgColor());
	g->setFg(getAspect()->getFgColor());
	g->putC(pos.row, pos.col, ' ');  
}
