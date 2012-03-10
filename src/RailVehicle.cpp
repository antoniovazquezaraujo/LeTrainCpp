#include "Basic.h"
#include "RailVehicle.h"
#include "Rail.h"
#include "Finder.h"
#include "TrainIterator.h"
MAKE_LOGGER(RailVehicle);

ostream & operator << (ostream & o, RailVehicle & v){
	o << "RailVehicle{" ;
		o << " Dir:" << v.dir.getName();

		if(v.rail){
			o << " Rail:" << *(v.rail) ;
		}else{
			o << "Sin rail!";
		}
	o << "}"<<endl ;
	return o;
}

RailVehicle::RailVehicle(Aspect * aspect)
	:Vehicle(aspect),
	impulse(0),
	impulseGenerated (0),
	brakes(0),
	mass(10),
	speed(0),
	friction(0),
	rail(0),
	moved(false),
	selectedLink(0),
	turns(1){

}
RailVehicle::~RailVehicle(){

}
bool RailVehicle::isSelected(){
	return selected;
}
void RailVehicle::setSelected(bool selected){
	this->selected = selected;
}
void RailVehicle::forward(){
	gotoRail(rail->getLinkedRailAt(dir));
}
void RailVehicle::backward(){
	goBackToRail(rail->getLinkedRailAt(rail->getPath(dir)));
}
bool RailVehicle::goBackToRail(Rail * r){
	if(r->getRailVehicle()!= 0){
		return false;
	}
	// si ya tengo un rail lo vacio
	if(rail != 0){
		rail->setRailVehicle(0);
	}
	if(dir.getValue() != Dir::NO_DIR){
		dir = r->getPath(dir);
		dir = -dir;
	}else{
		dir = r->getAnyPath();
	}
 	pos = r->getPos();
 	rail = r;
 	r->setRailVehicle(this);
	return true;
}
bool RailVehicle::gotoRail(Rail * r){
	if(r->getRailVehicle()!= 0){
		return false;
	}
	// si ya tengo un rail lo vacio
	if(rail != 0){
		rail->setRailVehicle(0);
	}
	if(dir.getValue() != Dir::NO_DIR){
		dir = r->getPath(-dir);
	}else{
		dir = r->getAnyPath();
	}
 	pos = r->getPos();
 	rail = r;
 	r->setRailVehicle(this);
	return true;
}
void RailVehicle::setSpeed(int speed){
	this->speed = speed;
}
int RailVehicle::getSpeed(){
	return speed;
}
int RailVehicle::getImpulse(){
	return impulse;
}
void RailVehicle::setTrain(Train * t){
	this->train = t;
}
Train * RailVehicle::getTrain(){
	return train;
}
int RailVehicle::receiveImpulse(int impulseReceived, Dir dir){
	int consumed = 0;
	if(getRail()->getPath(-dir) == this->dir){
		consumed = mass - this->impulse;
		if(consumed < 0) consumed = 0;
	}else{
		consumed = mass + this->impulse;
	}
	if(impulseReceived >= consumed){
		this->impulse += consumed;
		return impulseReceived - consumed;
	}else{
		this->impulse += impulseReceived;
		return 0;
	}
}
void RailVehicle::reverseImpulse(){
	impulse*= -1;
}
void RailVehicle::incImpulseGenerated(int n){
	impulseGenerated+=n;
}
void RailVehicle::decImpulseGenerated(int n){
	impulseGenerated-=n;
}
void RailVehicle::generateImpulse(){
	impulse+= impulseGenerated;
}
void RailVehicle::consumeImpulse(){
	impulse =0;
}
int RailVehicle::getBrakes(){
	return brakes;
}
void RailVehicle::setBrakes(int brakes){
	this->brakes = brakes;
}
int RailVehicle::getMass(){
	return mass;
}
Rail* RailVehicle::getRail(){
	return rail;
}
void RailVehicle::setRail(Rail * r){
	this->rail = r;
}
bool RailVehicle::isMoved(){
	return moved;
}
void RailVehicle::setMoved(bool moved){
	this->moved = moved;
}
