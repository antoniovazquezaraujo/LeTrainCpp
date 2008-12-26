#include "Basic.h"
#include "Train.h"
#include "RailVehicle.h"
#include "Locomotive.h"
MAKE_LOGGER(Train);

Train::Train(Locomotive* locomotive){
	locomotives.push_back(locomotive);
	allVehicles.push_back(locomotive);
}
void Train::setSpeed(int speed){
	vector<Locomotive *>::iterator itLocomotives; 
	for(itLocomotives = locomotives.begin();itLocomotives != locomotives.end();itLocomotives++){
		(*itLocomotives)->setSpeed(speed);
	}
}

void Train::paint(Window * g){
	vector<RailVehicle*>::iterator itVehicles;
	for(itVehicles = allVehicles.begin();itVehicles!= allVehicles.end(); itVehicles++){
		RailVehicle* next = (*itVehicles);
		next->paint(g);
	}
}
void Train::addLocomotive(Locomotive * locomotive){
	locomotives.push_back(locomotive);
	//prueba
	allVehicles.push_back(locomotive);
}
void Train::addWagon(RailVehicle * wagon){
	wagons.push_back(wagon);
	allVehicles.push_back(wagon);
}
void Train::moveTrain(){
	vector<Locomotive *>::iterator itLocomotives; 
	vector<RailVehicle*>::iterator itVehicles;
	//aceleramos todas las locos con su power
	for(itLocomotives = locomotives.begin();itLocomotives != locomotives.end();itLocomotives++){
		Locomotive * next = (*itLocomotives);
		next->incImp(next->getPower());
		LOG_DEBUG(log," incremento impulso a locomotoras");
	}

	//marcamos a todos los vehiculos como no movidos
	for(itVehicles = allVehicles.begin();itVehicles!= allVehicles.end(); itVehicles++){
		RailVehicle* next = (*itVehicles);
		next->setMoved(false);
		LOG_DEBUG(log," marcando como no movidos");
	}

	for(itVehicles = allVehicles.begin();itVehicles != allVehicles.end();itVehicles++){
		RailVehicle * next= *itVehicles;
		if(next->getImp() > 0){
			LOG_DEBUG(log," El tren tiene impulso");
			next->setSpeed(next->getImp() / next->getMass());
			if(next->turns > 0 ){
				next->turns--;
				LOG_DEBUG(log," Decremento turnos del tren a " << next->turns);
			}else{
				LOG_DEBUG(log," Ya no le quedan turnos");
				float remainingImp = next->getImp();
				if(remainingImp > 0){
					LOG_DEBUG(log," queda energia para linkeds");
					remainingImp = next->distLinked(remainingImp);
				}
				if(remainingImp > 0){
					LOG_DEBUG(log," queda para pusheds");
					remainingImp = next->distPushed(remainingImp);
				}
				if(remainingImp > 0){
					LOG_DEBUG(log," queda para mi");
					next->setImp(remainingImp);
				}
				if(next->getImp() > 0){
					LOG_DEBUG(log," si aun queda algo");
					if(next->pushedCanMove() && next->linkedCanMove()){
						LOG_DEBUG(log," si se pueden mover todos los muevo");
						next->movePushed();
						next->moveLinked();
					}
				}
				if(next->getSpeed() > 0){
					LOG_DEBUG(log," si le queda velocidad");
					next->turns = RailVehicle::MAX_TURNS* 1/next->getSpeed();
				}else{
					LOG_DEBUG(log," no le queda velocidad");
					next->turns = RailVehicle::MAX_TURNS;
				}
			}
		}
	}
}

