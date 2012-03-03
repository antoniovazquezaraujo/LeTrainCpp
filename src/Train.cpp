#include "Basic.h"
#include "Train.h"
MAKE_LOGGER(Train);
//////////////////////////////////////////////////////
Train::Train()
	:vehicleSelector(&vehicles){
	totalImpulse=0;
	totalMass = 0;
	trainDir = Dir::Dir::NO_DIR;
	moved = false;
	reversed = false;
	selected = false;
	reversedSelector = false;
}
void Train::paint(Window * g){
	for(auto vehicle:vehicles){
		vehicle->paint(g);
	}
}
//Manejo manual
void Train::setSelected(bool selected){
	this->selected = selected;
}
bool Train::isSelected(){
	return selected;
}
vector <RailVehicle*> & Train::getVehicles(){
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
	for(auto locomotive: locomotives){
		locomotive->incImpulseGenerated();
	}
}
void Train::decImpulseGenerated(){
	for(auto locomotive: locomotives){
		locomotive->decImpulseGenerated();
	}
}
void Train::setSpeed(int speed){

}
int Train::getSpeed(){
	return 10;
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
void Train::addVehicle(RailVehicle* v){
	v->addToTrain(this);
	totalMass = getTotalMass();
	//setForwardDir();
}
void Train::addWagon(Wagon* w){
	vehicles.push_back(w);
	w->setTrain(this);
}
void Train::addLocomotive(Locomotive* l){
	vehicles.push_back(l);
	locomotives.push_back(l);
	l->setTrain(this);
}
// Estas devuelven un puntero al tren que desaparece y que debemos borrar
// o bien al nuevo que se crea automáticamente y que debemos agregar
Train * Train::link(){
	if(!vehicleSelector.atFirst()
			&& !vehicleSelector.atLast()){
		return nullptr;
	}
	Dir actualDir;
	if(vehicleSelector.atFirst()){
		actualDir = getDirFromFirst();
	}else{
		actualDir = getDirFromLast();
	}
	Rail * selectedRail = (*vehicleSelector.getSelected())->getRail();
	Dir reverseDir = selectedRail->getPath(actualDir);
	Rail * nextRail = selectedRail->getLinkedRailAt(reverseDir);
	RailVehicle * linked = nextRail->getRailVehicle();
	Train * linkedTrain=0;
	if(linked){
		linkedTrain = linked->getTrain();
		for(RailVehicle * v : linkedTrain->getVehicles()){
			addVehicle(v);
		}
		linkedTrain->clear();
		totalMass = getTotalMass();
	}
	return linkedTrain;
}

Train * Train::unlink(){
	if(vehicleSelector.atFirst()
			|| vehicleSelector.atLast()){
		return nullptr;
	}
	Train * unlinkedTrain = new Train;
	if(reversedSelector){
		for_each(vehicles.begin(), vehicleSelector.getSelected(), 
				[&unlinkedTrain](RailVehicle * v){
				unlinkedTrain->addVehicle(v);
				});
		vehicles.erase(vehicles.begin(), vehicleSelector.getSelected());
	}else{
		for_each(vehicleSelector.getSelected(), vehicles.end(), 
				[&unlinkedTrain](RailVehicle * v){
				unlinkedTrain->addVehicle(v);
				});
		vehicles.erase(vehicleSelector.getSelected(),vehicles.begin());
	}
	return unlinkedTrain;
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

//Gestión de elementos
//Si hay via libre agregamos al final del tren en la dirección en la que
//va el selector de vehículos
void Train::addWagon(){

}
void Train::addLocomotive(){

}
void Train::clear(){
	vehicles.clear();
}
//Borra el último vehículo en la dirección del selector de vehículos
//Si se quiere eliminar uno del medio, dividir primero el tren en dos,
//borrar el último y luego volver a unirlos.
void Train::removeLastVehicle(){

}
////////////////////////////////////////////////////
Dir Train::getDirFromFirst(){
	if(reversed) return getDirFromLast();
	vector<RailVehicle*>::iterator i = vehicles.begin();
	//LOG_DEBUG(log," Vehiculo inicial: " << **i); 
	Rail * r = (*i)->getRail();
	//LOG_DEBUG(log," Su rail : " << *r); 
	Dir d = (*i)->getDir();
	//LOG_DEBUG(log," Su direccion: " << d); 
	//LOG_DEBUG(log," Su path: " << r->getPath(d)); 
	return r->getPath(d);
}
Dir Train::getDirFromLast(){
	if(reversed) return getDirFromFirst();
	vector<RailVehicle*>::reverse_iterator i = vehicles.rbegin();
	Rail * r = (*i)->getRail();
	Dir d = (*i)->getDir();
	//return r->getPath(d);
	return d;
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
	vector<RailVehicle*>::reverse_iterator i;
	for(i=vehicles.rbegin();i!=vehicles.rend();i++){
		(*i)->getRail()->reverseVehicle();
	}
}
/*
void Train::setForwardDir(){
	vector<RailVehicle*>::reverse_iterator i = vehicles.rbegin();
	Dir d;
	if(i != vehicles.rend()){
		d = (*i)->getDir();
		Rail * r = (*i)->getRail();
		Rail * next = r->getLinkedRailAt(d);
		if(next){
			i++;
			if(i != vehicles.rend()){
				RailVehicle * v = next->getRailVehicle();
				if (v != *i){
					//dirección contraria;
					d = next->getPath(d);
				}else{
				}
			}
		}
		i = vehicles.rbegin();
		while(i != vehicles.rend()){
			RailVehicle* next = *i;
			Rail * r = next->getRail();
			next->setDir(d);
			d = r->getPath(d);
			i++;
		}
	}
	reversed = false;
}
void Train::setBackwardDir(){
	vector<RailVehicle*>::iterator i = vehicles.begin();
	Dir d;
	if(i != vehicles.end()){
		d = (*i)->getDir();
		Rail * r = (*i)->getRail();
		Rail * next = r->getLinkedRailAt(d);
		if(next){
			i++;
			if(i != vehicles.end()){
				RailVehicle * v = next->getRailVehicle();
				if (v != *i){
					//dirección contraria;
					d = next->getPath(d);
				}
			}
		}
		i = vehicles.begin();
		while(i != vehicles.end()){
			RailVehicle* next = *i;
			Rail * r = next->getRail();
			next->setDir(d);
			d = r->getPath(d);
			i++;
		}
	}
	reversed = true;
}
*/
int Train::sumImpulse(){
	int total=0;
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
	//chocamos con algo
	//le pasamos todo el impulso y esperamos resto
	crashed->receiveImpulse(impulse, d);
	Train * crashedTrain = crashed->getTrain();
	int consumed= crashedTrain->move();
	return consumed;
}
int Train::move(){
	if(moved) return totalMass; 
	moved = true;
	int consumed = 0;
	totalImpulse += sumImpulse();
	if(totalImpulse < 0){
		if(!reversed){
			invert();
			reversed=true;
		}
	}else if(totalImpulse > 0){
		if(reversed){
			invert();
			reversed=false;
		}
	}

	if(abs(totalImpulse) >= totalMass){
		//el tren intentará moverse, porque su impulso puede con su masa
		RailVehicle * crashedVehicle= nullptr;
		RailVehicle * topVehicle    = nullptr;
		Rail        * topRail = nullptr;
		Rail        * nextRail   = nullptr;
		Dir           trainDir;
		if(totalImpulse > 0){
			vector<RailVehicle*>::iterator         i = vehicles. begin();
			topVehicle = *i;
		}else{
			vector<RailVehicle*>::reverse_iterator i = vehicles.rbegin();
			topVehicle = *i;
		}
		trainDir = topVehicle->getDir();
		topRail = topVehicle->getRail();
		nextRail = topRail->getLinkedRailAt(trainDir);
		if(nextRail){
			crashedVehicle = nextRail->getRailVehicle();
			if(crashedVehicle){
				consumed = crash(crashedVehicle, totalImpulse,trainDir );
				totalImpulse-=consumed;
				return consumed;
			}else{
				if(reversed){
					shiftBackward();
				}else{
					shiftForward();
				}
				consumed= totalMass;
				totalImpulse-=consumed;
				return consumed;
			}
		}else{
			// agregar aqui descarrilamientos!!!
			return 9999999;
		}
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
	o<< endl;
	o << "Locomotives:" ;
	for(auto l: t.locomotives){
		o << *l;
	}
	o<< endl;
	o << " Impulso total: "  << t.totalImpulse;
	o << " Masa total:"      << t.totalMass;
	o << " Dir: "            << t.trainDir;
	o << " Moved: "          << t.moved;
	o << " Reversed:"        << t.reversed;
	o << " Selected:"        << t.selected;
	o << " ReversedSelector:"<< t.reversedSelector;
	o<< endl;
	return o;
}
