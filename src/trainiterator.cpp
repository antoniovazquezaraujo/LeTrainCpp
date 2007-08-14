/*
  TrainIterator.cpp
  The train iterator 
  -------------------------------------
*/
#include "trainiterator.h"
#include "vehicle.h"
#include "finder.h"
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
	return actual != 0;
}
Vehicle*  TrainIterator::next(){
	Vehicle*  oldactual = actual;
	prev = actual;
	if(nextVehicle != 0 && actual->linked(nextVehicle)){
		if(nextVehicle->links[0] != 0 && nextVehicle->links[0] != oldactual){
			actual = nextVehicle;
			nextVehicle = nextVehicle->links[0];
		}else if(nextVehicle->links[1] != 0 && nextVehicle->links[1] != oldactual){
			actual = nextVehicle;
			nextVehicle = nextVehicle->links[1];
		}else{
			actual = nextVehicle; 
			nextVehicle = 0; 
		}
	}else{
		actual = 0; 
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
		nextVehicle = 0;
	}
}
