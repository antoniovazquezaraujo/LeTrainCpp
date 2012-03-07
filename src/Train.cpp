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
	Dir actualDir;
	RailVehicle * topVehicle;
	if(!reversed){
		actualDir = getDirFromFirst();
		topVehicle = *(vehicles.begin());
	}else{
		actualDir = getDirFromLast();
		topVehicle = *(vehicles.rbegin());
	}
	Rail * selectedRail = topVehicle->getRail();
	Rail * nextRail = selectedRail->getLinkedRailAt(actualDir);
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
			int consumed = 0;
			if(crashedVehicle){
				consumed = crash(crashedVehicle, totalImpulse,trainDir );
			}else{
				if(reversed){
					shiftBackward();
				}else{
					shiftForward();
				}
				consumed= totalMass;
			}
			if(totalImpulse > 0){
				totalImpulse -=consumed;
			}else{
				totalImpulse +=consumed;
			}
		}else{
			// agregar aqui descarrilamientos!!!
			assert(false);
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
