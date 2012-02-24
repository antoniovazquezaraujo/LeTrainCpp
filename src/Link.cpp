#include "Basic.h"
#include "Link.h"
#include "RailVehicle.h"
MAKE_LOGGER(Link);

Link::Link() {
}
bool Link::link(RailVehicle * r){
	if(isLinked(r)){
		return true;
	}
	if(!links[0] ){
		links[0]=r;
		return true;
	}
	if(!links[1]){
		links[1]=r;
		return true;
	}
	return false;
}
RailVehicle * Link::unlink(RailVehicle * r){
	if(links[0] == r){
		links[0]=0;
		return r;
	}
	if(links[1] == r){
		links[1]=0;
		return r;
	}
	return 0;
}
bool Link::isLinked(RailVehicle * r){
	return (links[0] == r || links[1] == r);
}
bool Link::isEmpty(){
	return (!links[0] && !links[1]);
}

bool Link::isFull(){
	return (links[0] && links[1]);
}
RailVehicle * Link::getVehicleLinkedWith(RailVehicle * one, RailVehicle * other){
	if(one->getVehicleLinkedAt(0) == other){
		return one->getVehicleLinkedAt(1);
	}
	if(one->getVehicleLinkedAt(1) == other){
		return one->getVehicleLinkedAt(0);
	}
	return 0;
}
RailVehicle * Link::getVehicleLinkedAt(int numLink){
	return links[numLink];
}
