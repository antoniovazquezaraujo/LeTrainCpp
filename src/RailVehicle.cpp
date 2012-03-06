#include "Basic.h"
#include "RailVehicle.h"
#include "Rail.h"
#include "Finder.h"
#include "TrainIterator.h"
MAKE_LOGGER(RailVehicle);

ostream & operator << (ostream & o, RailVehicle & v){
	o << "RailVehicle{" ;
		o << " Dir:" << v.dir.getName();

		Vehicle * v1 = v.linker.getVehicleLinkedAt(0);
		if(v1) o << " Link 1:" << *v1 ;

		Vehicle * v2 = v.linker.getVehicleLinkedAt(1);
		if(v2) o << " Link 2:" << *v2 ;

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
RailVehicle * RailVehicle::getVehicleLinkedWith(RailVehicle * r){
	return linker.getVehicleLinkedWith(this, r);
}
RailVehicle * RailVehicle::getVehicleLinkedAt(int numLink){
	return linker.getVehicleLinkedAt(numLink);
}
bool RailVehicle::link(RailVehicle * b){
	return linker.link(b);
}
bool RailVehicle::interLink(RailVehicle * b){
	if(b->link(this)){
		if(linker.link(b)){
			return true;
		}else{
			b->unlink(this);
			return false;
		}
	}else{
		return false;
	}
}
void RailVehicle::unlink(RailVehicle * b){
	linker.unlink(b);
}
int RailVehicle::isLinked(RailVehicle *b){
	return linker.isLinked(b); 
}
void RailVehicle::setLink(Dir d){
	assert(false);

// TODO
}
void RailVehicle::setUnlink(Dir d){
	assert(false);
// TODO

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
/*
void RailVehicle::receiveImpulse(int impulse, Dir d ){
						LOG_DEBUG(log," Tenia: " << this->impulse << " recibe" << impulse << " hacia " << d); 
	if(getRail()->getPath(-d) == dir){
		if(this->impulse >= 0){
			this->impulse+=impulse;
		}else{
			this->impulse-=impulse;
		}
	}else{
		if(this->impulse >= 0){
			this->impulse-=impulse;
		}else{
			this->impulse+=impulse;
		}
	}
						LOG_DEBUG(log," queda: " << this->impulse); 
}
*/
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
