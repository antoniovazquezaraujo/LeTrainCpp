#include "Train.h"
MAKE_LOGGER(Train);
//////////////////////////////////////////////////////
Train::Train()
	:
	vehicleSelector   (&vehicles),
	totalImpulse      (0),
	numStoppedTurns   (0),
	speed             (0),
	totalMass         (0),
	trainDir          (Dir::NO_DIR),
	moved             (false),
	reversed          (false),
	selected          (false),
	deleted           (false),
	reversedSelector  (false){
		
}
void Train::paint(Window * g){
	for(auto vehicle:vehicles){
		vehicle->paint(g);
	}
}
bool Train::isMarkedAsDeleted(){
	return deleted;
}
void Train::markAsDeleted(){
	deleted = true;
}
//Manejo manual
void Train::setSelected(bool selected){
	this->selected = selected;
}
bool Train::isSelected(){
	return selected;
}
Train::TVehicles & Train::getVehicles(){
	return vehicles;
}
void Train::push(){
// Aquí, lo mejor sería agregar tanto impulso como 
// la masa total del tren y moverlo normalmente.
}
void Train::pull(){
// Aquí, lo mejor sería agregar tanto impulso como 
// la masa total del tren y moverlo normalmente.
}
void Train::selectNextForkDir(){

}
void Train::selectPrevForkDir(){

}

void Train::setMoved(bool moved){
	this->moved = moved;
}
//Motores
void Train::incImpulseGenerated(){
	for(auto v : vehicles){
		v->incImpulseGenerated();
	}
}
void Train::decImpulseGenerated(){
	for(auto v: vehicles){
		v->decImpulseGenerated();
	}
}
void Train::setSpeed(float speed){
	this->speed = speed;
}
float Train::getSpeed(){
	return speed;
}
int Train::getTotalMass(){
	int total = 0;
	for_each(
		vehicles.begin(), 
		vehicles.end(),
		[&total](RailVehicle * vehicle){
			total+= vehicle->getMass();
		}
	);
	return total;
}

Selector<Train::TVehicles>  Train::getVehicleSelector(){
	return vehicleSelector;
}
//Manejo de vehículos
void Train::selectNextVehicle(){
	if(vehicleSelector.isSelected()){
		(*vehicleSelector.getSelected())->setSelected(false);
	}
	vehicleSelector.selectNext();
	if(vehicleSelector.isSelected()){
		(*vehicleSelector.getSelected())->setSelected(true);
	}
}
void Train::selectPrevVehicle(){
	if(vehicleSelector.isSelected()){
		(*vehicleSelector.getSelected())->setSelected(false);
	}
	vehicleSelector.selectPrev();
	if(vehicleSelector.isSelected()){
		(*vehicleSelector.getSelected())->setSelected(true);
	}
}
void Train::addVehicle(int p , RailVehicle* v){
	RailVehicle * linker=nullptr;
	if(p == FRONT){
		linker = vehicles.empty()?nullptr:*(vehicles.begin());
		vehicles.push_front(v);
	}else{
		linker = vehicles.empty()?nullptr:*(vehicles.rbegin());
		vehicles.push_back(v);
	}
	if(linker ){
		Rail * linkedRail = v->getRail();
		v->setDir(linkedRail->getPath(-(linker->getDir())));
	}
	v->setTrain(this);
	totalMass = getTotalMass();
}
void Train::link(){
		RailVehicle * topVehicle = nullptr;
		Rail        * topRail    = nullptr;
		Rail        * nextRail   = nullptr;
		if(!reversed){
			Train::TVehicles::iterator         i = vehicles. begin();
			topVehicle = *i;
		}else{
			Train::TVehicles::reverse_iterator i = vehicles.rbegin();
			topVehicle = *i;
		}
		trainDir = topVehicle->getDir();
		topRail = topVehicle->getRail();
		nextRail = topRail->getLinkedRailAt(trainDir);
		if(nextRail){
			RailVehicle* linked = nextRail->getRailVehicle();
			if(linked){
				Train *t = linked->getTrain();
				if(*(t->vehicles.begin()) == linked){
					//enganchamos con el primero
					for_each(t->vehicles.begin(), t->vehicles.end(),
						[&](RailVehicle* v){
							if(!reversed){
								addVehicle(FRONT, v);
							}else{
								addVehicle(BACK, v);
							}
						}
					);
				}else{ //enganchamos con el último
					for_each(t->vehicles.rbegin(), t->vehicles.rend(),
						[&](RailVehicle* v){
							if(!reversed){
								addVehicle(FRONT, v);
							}else{
								addVehicle(BACK, v);
							}
						}
					);

				}
				t->vehicles.clear();

			}else{
				//nadie para enganchar.
			}

	}else{
		//No hay rail en esa direccion!!
	}
}
Train * Train::unlink(){
	Train * t = nullptr;
	if(vehicles.size()>1){
		t = new Train();
		if(reversed){
			t->addVehicle(FRONT, vehicles.front());
			vehicles.pop_front();
		}else{
			t->addVehicle(FRONT, vehicles.back());
			vehicles.pop_back();
		}
	}
	/*
			v->generateImpulse();
			total += v->getImpulse(); 
			v->consumeImpulse();
	*/
	totalMass = getTotalMass();
	return t;
}
void Train::reverseSelector(){
	reversedSelector = !reversedSelector;
}
void Train::advanceSelector(){
	if(reversedSelector){
		selectPrevVehicle();
	}else{
		selectNextVehicle();
	}
}

void Train::clear(){
	for(auto v : vehicles){
		v->getRail()->setRailVehicle(nullptr);		
	}
	vehicles.clear();
}
//Borra el último vehículo en la dirección del selector de vehículos
//Si se quiere eliminar uno del medio, dividir primero el tren en dos,
//borrar el último y luego volver a unirlos.
void Train::removeLastVehicle(){

}
////////////////////////////////////////////////////
Dir Train::getDirFromFirst(){
	Rail * r =(*(vehicles.begin()))->getRail();
	Dir d    =(*(vehicles.begin()))->getDir();
	return r->getPath(d);
}
Dir Train::getDirFromLast(){
	Rail * r =(*(vehicles.rbegin()))->getRail();
	Dir d    =(*(vehicles.rbegin()))->getDir();
	return r->getPath(d);
}
void Train::shiftForward(){
	for_each(vehicles.begin(), vehicles.end(),
		[](RailVehicle* v){
			v->getRail()->exitVehicle();
		}
	);
}
void Train::shiftBackward(){
	for_each(vehicles.rbegin(), vehicles.rend(),
		[](RailVehicle* v){
			v->getRail()->exitVehicle();
		}
	);
}
void Train::invert(){
	Train::TVehicles::iterator i;
	for(i=vehicles.begin();i!=vehicles.end();i++){
		Dir d = (*i)->getDir();
		(*i)->getRail()->reverseVehicle();
	}
}
float Train::sumImpulse(){
	float total=0;
	for_each(vehicles.begin(), vehicles.end(),
		[&total](RailVehicle* v){
			v->generateImpulse();
			total += v->getImpulse(); 
			v->consumeImpulse();
		}
	);
	return total;
}
int Train::crash(RailVehicle * crashed, int impulse, Dir d){
	return crashed->receiveImpulse(impulse, d);
}
void Train::move(){
	if(moved) return ; 
	moved = true;
	totalImpulse += sumImpulse();

	if(reversed && totalImpulse > 0){
		invert();
		reversed=false;
	}else if(!reversed && totalImpulse < 0){
		invert();
		reversed=true;
	}
	if(abs(totalImpulse) >= totalMass){
		//el tren intentará moverse, porque su impulso puede con su masa
		RailVehicle * crashedVehicle= nullptr;
		RailVehicle * topVehicle    = nullptr;
		Rail        * topRail = nullptr;
		Rail        * nextRail   = nullptr;
		//Dir           trainDir;
		if(!reversed){
			Train::TVehicles::iterator         i = vehicles. begin();
			topVehicle = *i;
		}else{
			Train::TVehicles::reverse_iterator i = vehicles.rbegin();
			topVehicle = *i;
		}
		trainDir = topVehicle->getDir();
		topRail = topVehicle->getRail();
		nextRail = topRail->getLinkedRailAt(trainDir);
		if(nextRail){
			crashedVehicle = nextRail->getRailVehicle();
			float consumed = 0;
			if(crashedVehicle){
				consumed = crash(crashedVehicle, abs(totalImpulse),trainDir );
				if(totalImpulse >=0){
					totalImpulse -=abs(consumed);
				}else{
					totalImpulse +=abs(consumed);
				}
			}else{
				if(reversed){
					shiftBackward();
				}else{
					shiftForward();
				}
				consumed= totalMass;
				if(totalImpulse >=0){
					totalImpulse -=consumed;
				}else{
					totalImpulse +=consumed;
				}
			}
		}else{
			// agregar aqui descarrilamientos!!!
			assert(false);
		}
		if(numStoppedTurns > 0){
			if(numStoppedTurns == 1){
				setSpeed(400);
			}else{
				setSpeed(1.0/std::log(numStoppedTurns)*150);
			}
			//assert(speed < 1000);
			//assert(abs(totalImpulse)< 10000);
		}else{
		}
		numStoppedTurns=0;	
	}else{
		numStoppedTurns++;
	}
}
ostream & operator << (ostream & o, Train t){
	o << "Vehicles:" ;
	for(auto v: t.vehicles){
		o << *v;
	}
	o<< endl;
	if(t.vehicleSelector.isSelected()){
		o << "Vehicle selected" << *(t.vehicleSelector.getSelected());
	}
	o<< endl
	 << " Speed: "     << left << setw(12) << t.speed
	 << " Impulse: "   << left << setw(12) << t.totalImpulse
	 << " Total mass:" << left << setw(12) << t.totalMass
	<< endl;
	return o;
}
