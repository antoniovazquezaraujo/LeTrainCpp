#include "Basic.h"
#include "TrainIterator.h"
#include "Rail.h"
#include "RailVehicle.h"
MAKE_LOGGER(TrainIterator);

TrainIterator::TrainIterator(RailVehicle*  v, Dir dir){
	                                        LOG_DEBUG(log, " construyendo iterator");
	finder = new Finder(v);
	//finder->gotoRail(v->getRail());
	this->dir = dir;
	start = v;
	reset();
	                                        LOG_DEBUG(log, " fin de construyendo iterator");
}
TrainIterator::~TrainIterator(){
	delete finder;
}
int TrainIterator::hasNext(){
	return actual != 0;
}
RailVehicle*  TrainIterator::next(){
	RailVehicle*  oldActual = actual;
	                                        LOG_DEBUG(log," En TrainIterator.next actual vale:" << *actual);
	prev = actual;
	if(nextVehicle && actual->isLinked(nextVehicle)){ 
		                                        LOG_DEBUG(log, " obteniendo next vehiculo");
		actual = nextVehicle;
		                                        LOG_DEBUG(log, " En actual metemos " << *actual);
		nextVehicle = nextVehicle->getVehicleLinkedWith(oldActual);
		                                        LOG_DEBUG(log, " En nextVehicle metemos " << *nextVehicle);
	}else{
		                                        LOG_DEBUG(log, " no está enlazado con el siguiente o no hay siguiente");
		actual = 0; 
		nextVehicle = 0; 
	}
	return prev;
}
void TrainIterator::reset(){
	actual = start;
	prev = start;
	                                        LOG_DEBUG(log, " Reseteando " << *actual);
	finder->gotoRail(actual->getRail()); 
	                                        LOG_DEBUG(log, " Despues de ir al rail " );
//	finder->setDir(dir);
	                                        LOG_DEBUG(log," despues de ponerle la dir " << dir.getName());
	finder->forward();
	                                        LOG_DEBUG(log, " antes de reset finder->getRail");
	Rail * r = finder->getRail();
	                                        LOG_DEBUG(log, " obtenido el rail en reset " << *r);
	if(r){
		                                        LOG_DEBUG(log, " antes de pedir getRailVehicle");
		nextVehicle = r->getRailVehicle();
		if(!nextVehicle){
			                                        LOG_DEBUG(log, " no hay un siguiente vehiculo!!!!!!!!");
		}else{
			                                        LOG_DEBUG(log, " reseteado. tenemos nextVehicle: " << nextVehicle);
		}
	}else{
		                                        LOG_DEBUG(log, " reseteado y no hay siguiente vehiculo");
		nextVehicle=0;
	}
}
