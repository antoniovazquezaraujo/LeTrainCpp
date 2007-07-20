/*
  TrainIterator.cpp
  The train iterator 
  -------------------------------------
*/
#include "letrain.h"
TrainIterator::TrainIterator(Vehicle*  v, int dir){
	f = new Finder(v);
	this->dir = dir;
	start = v;
	reset();
}
TrainIterator::~TrainIterator(){
	delete f;
}
int TrainIterator::hasNext(){
	return actual != NULL;
}
Vehicle*  TrainIterator::next(){
	Vehicle*  oldactual = actual;
	prev = actual;
	if(nextVehicle != NULL && actual->linked(nextVehicle)){
		if(nextVehicle->links[0] != NULL && nextVehicle->links[0] != oldactual){
			actual = nextVehicle;
			nextVehicle = nextVehicle->links[0];
		}else if(nextVehicle->links[1] != NULL && nextVehicle->links[1] != oldactual){
			actual = nextVehicle;
			nextVehicle = nextVehicle->links[1];
		}else{
			actual = nextVehicle; 
			nextVehicle = NULL; 
		}
	}else{
		actual = NULL; 
	}
	return prev;
}
void TrainIterator::reset(){
	actual = start;
	prev = start;
	f->gotoRail(actual->rail); 
	f->dir = dir;
	if(f->forward()){
		nextVehicle = f->rail->vehicle;
	}else{
		nextVehicle = NULL;
	}
}
