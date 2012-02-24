#include "Basic.h"
#include "Train.h"
MAKE_LOGGER(Train);
//////////////////////////////////////////////////////
Train::Train(){
	vehicleIterator= train.begin();
	selectedVehicle = 0;
	selectVehicle(*vehicleIterator);
	totalImpulse=0;
	totalMass = 0;
	trainDir = Dir::Dir::NO_DIR;
	moved = false;
	reversed = false;
	selected = false;
	reversedSelector = false;
	selectorStatus = atBegin;
}
void Train::paint(Window * g){
	for(auto vehicle:train){
		vehicle->paint(g);
	}
	/*
	list<RailVehicle*>::iterator i = train.begin();
	while(i != train.end()){
		(*i)->paint(g);	
		i++;
	}
	*/
}
void Train::selectVehicle(RailVehicle * v){
	if(selectedVehicle){
		selectedVehicle->setSelected(false);
	}
	selectedVehicle = v;
}
//Manejo manual
void Train::setSelected(bool selected){
	this->selected = selected;
}
bool Train::isSelected(){
	return selected;
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
//invertimos el sentido de todo el tren
void Train::invert(){
	if(reversed){
		setForwardDir();
	}else{
		setBackwardDir();
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
		train.begin(), 
		train.end(),
		[&total](RailVehicle * vehicle){
			total+= vehicle->getMass();
		}
	);
	return total;
}

//Manejo de vehículos
void Train::selectNextVehicle(){
	vehicleIterator++;
	if(vehicleIterator!= train.end()){
		selectorStatus = atMiddle;
	}else{
		vehicleIterator--;
		selectorStatus = atEnd;
	}
	selectVehicle(*vehicleIterator); 
}
void Train::selectPrevVehicle(){
	if(vehicleIterator != train.begin()){
		vehicleIterator--;
	}
	if(vehicleIterator != train.begin()){
		selectorStatus = atMiddle;
	}else{
		selectorStatus = atBegin;
	}
	selectVehicle(*vehicleIterator); 
}
void Train::addVehicle(RailVehicle* v){
	v->addToTrain(this);
	totalMass = getTotalMass();
	selectNextVehicle();
	//setForwardDir();
}
void Train::addWagon(Wagon* w){
	train.push_back(w);
	w->setTrain(this);
}
void Train::addLocomotive(Locomotive* l){
	train.push_back(l);
	locomotives.push_back(l);
	l->setTrain(this);
}
// Estas devuelven un puntero al tren que desaparece y que debemos borrar
// o bien al nuevo que se crea automáticamente y que debemos agregar

Train * Train::link(){
	Train * linkedTrain = 0;
	RailVehicle * linked = 0;
	RailVehicle * linker = selectedVehicle;
	Rail * nextRail = 0;
	Dir actualDir=  Dir::NO_DIR;
	Dir reverseDir =  Dir::NO_DIR;
	Rail * selectedRail = 0;
	switch(selectorStatus){
	case atMiddle:
		//don't link at middle!
		break;
	case atBegin:
		if(reversedSelector){
			actualDir = getDirFromFirst();
			selectedRail = selectedVehicle->getRail();
			reverseDir = selectedRail->getPath(actualDir);
			nextRail = selectedRail->getLinkedRailAt(reverseDir);
			linked = nextRail->getRailVehicle();
			if(linked){
				linkedTrain = linked->getTrain();
				list<RailVehicle*>::iterator i = linkedTrain->train.begin();
				while(i != linkedTrain->train.end()){
					addVehicle(*i);
					linkedTrain->train.remove(*i);
					i++;
				}
				totalMass = getTotalMass();
			}
		}
		break;
	case atEnd:
		if(!reversedSelector){
			actualDir = getDirFromLast();
			selectedRail = selectedVehicle->getRail();
			reverseDir = selectedRail->getPath(actualDir);
			nextRail = selectedRail->getLinkedRailAt(reverseDir);
			linked = nextRail->getRailVehicle();
			if(linked){
				linkedTrain = linked->getTrain();
				list<RailVehicle*>::iterator i = linkedTrain->train.begin();
				while(i != linkedTrain->train.end()){
					addVehicle(*i);
					linkedTrain->train.remove(*i);
					i++;
				}
				totalMass = getTotalMass();
			}
		}
		break;
	}
	return linkedTrain;
}
Train * Train::unlink(){
	Train * unlinkedTrain = 0;
	RailVehicle * unlinkedVehicle = 0;
	RailVehicle * unlinkerVehicle = selectedVehicle;
	Rail * nextRail = 0;
	Dir actualDir=  Dir::NO_DIR;
	Dir reverseDir =  Dir::NO_DIR;
	Rail * selectedRail = 0;
	switch(selectorStatus){
	case atBegin:
		//nothing
		break;
	case atEnd:
		//nothing
		break;
	case atMiddle:
		unlinkedTrain = new Train;
		if(reversedSelector){
			selectPrevVehicle();
			unlinkedVehicle = selectedVehicle;
			list<RailVehicle*>::iterator i= vehicleIterator;
			if(i != train.begin()){
				do{
					i--;
					train.remove(*i);
					unlinkedTrain->addVehicle(*i);
				}while(i != train.begin());
			}
		}else{
			selectNextVehicle();
			unlinkedVehicle = selectedVehicle;
			list<RailVehicle*>::iterator i= vehicleIterator;
			while(i != train.end()){
				train.remove(*i);
				unlinkedTrain->addVehicle(*i);
				i++;
			}
		}
		break;
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

//Borra el último vehículo en la dirección del selector de vehículos
//Si se quiere eliminar uno del medio, dividir primero el tren en dos,
//borrar el último y luego volver a unirlos.
void Train::removeLastVehicle(){

}
////////////////////////////////////////////////////
Dir Train::getDirFromFirst(){
	/*
	list <RailVehicle*>::iterator i = train.begin();
	Rail * r = (*i)->getRail();
	Dir d = (*i)->getDir();
	return (d);
	*/
	list <RailVehicle*>::iterator i = train.begin();

}
Dir Train::getDirFromLast(){
	list <RailVehicle*>::reverse_iterator i = train.rbegin();
	Rail * r = (*i)->getRail();
	Dir d = (*i)->getDir();
	return r->getPath(d);
}

void Train::shiftForward(){
	list <RailVehicle*>::iterator i = train.begin();
	RailVehicle * next = *i;
	Rail * rail = next->getRail();
	trainDir= getDirFromFirst();
	Rail * nextRail =0; 
	Rail * prevRail = rail->getLinkedRailAt(trainDir);
	//next->gotoRail(prevRail);
	prevRail->enter(next);
	i++;
	while(i != train.end()){
		next = *i;
		nextRail=next->getRail();
		//next->gotoRail(prevRail);
		prevRail->enter(next);
		prevRail=nextRail;
		i++;
	}
}
void Train::shiftBackward(){
	list <RailVehicle*>::reverse_iterator i = train.rbegin();
	RailVehicle * next = *i;
	Rail * rail = next->getRail();
	trainDir= getDirFromLast(); 
	Rail * nextRail =0; 
	Rail * prevRail = rail->getLinkedRailAt(trainDir);
	next->setDir(-trainDir);
	//next->gotoRail(prevRail);
	prevRail->enter(next);
	next->setDir(-trainDir);
	i++;
	while(i != train.rend()){
		next = *i;
		nextRail=next->getRail();
		//next->gotoRail(prevRail);
		prevRail->enter(next);
		prevRail=nextRail;
		i++;
	}
}
void Train::setForwardDir(){
	list <RailVehicle*>::reverse_iterator i = train.rbegin();
	Dir d;
	if(i != train.rend()){
		d = (*i)->getDir();
		Rail * r = (*i)->getRail();
		Rail * next = r->getLinkedRailAt(d);
		if(next){
			i++;
			if(i != train.rend()){
				RailVehicle * v = next->getRailVehicle();
				if (v != *i){
					//dirección contraria;
					d = next->getPath(d);
				}else{
				}
			}
		}
		i = train.rbegin();
		while(i != train.rend()){
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
	list <RailVehicle*>::iterator i = train.begin();
	Dir d;
	if(i != train.end()){
		d = (*i)->getDir();
		Rail * r = (*i)->getRail();
		Rail * next = r->getLinkedRailAt(d);
		if(next){
			i++;
			if(i != train.end()){
				RailVehicle * v = next->getRailVehicle();
				if (v != *i){
					//dirección contraria;
					d = next->getPath(d);
				}
			}
		}
		i = train.begin();
		while(i != train.end()){
			RailVehicle* next = *i;
			Rail * r = next->getRail();
			next->setDir(d);
			d = r->getPath(d);
			i++;
		}
	}
	reversed = true;
}
int Train::sumImpulse(){
	int total=0;
	Dir d;
	list <RailVehicle*>::reverse_iterator i = train.rbegin();
	while(i != train.rend()){
		RailVehicle* next = *i;
		Rail * r = next->getRail();
		next->generateImpulse();
		int impulse = next->getImpulse();
		next->consumeImpulse();
		d= next->getDir();
		total += impulse;
		i++;
	}
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
			LOG_DEBUG(log,"Impulso total:" << totalImpulse);
	if(abs(totalImpulse) >= totalMass){
		//el tren intentará moverse, porque su impulso puede con su masa
		if(totalImpulse > 0){
			LOG_DEBUG(log,"Dir antes de from first: " << trainDir); 
			trainDir= getDirFromFirst();
			LOG_DEBUG(log,"Dir después de from first: " << trainDir); 
			list <RailVehicle*>::iterator i = train.begin();
			RailVehicle * first = *i;
			Rail * r = (*i)->getRail();
			Rail * next = r->getLinkedRailAt(trainDir);
			RailVehicle * crashed=0;
			if(next){
				if((crashed = next->getRailVehicle()) != 0){
					LOG_DEBUG(log,"Crash from first");
					consumed = crash(crashed, totalImpulse,trainDir );
					totalImpulse-=consumed;
					return consumed;
				}else{
					shiftForward();
					consumed= totalMass;
					totalImpulse-=consumed;
					return consumed;
				}
			}else{
				// agregar aqui descarrilamientos!!!
				LOG_DEBUG(log,"Descarrila hacia adelante");
				return 9999999;
			}
		}else if (totalImpulse < 0 ){
			trainDir= getDirFromLast();
			LOG_DEBUG(log,"Dir from last: " << trainDir); 
			list <RailVehicle*>::reverse_iterator i = train.rbegin();
			RailVehicle * last = *i;
			Rail * r = (*i)->getRail();
			Rail * next = r->getLinkedRailAt(trainDir);
			RailVehicle * crashed=0;
			if(next){
				if((crashed = next->getRailVehicle()) != 0){
			LOG_DEBUG(log,"Crash from last");
					consumed = crash(crashed, totalImpulse, trainDir);
					if(consumed < totalImpulse){
						totalImpulse-=consumed;
					}else{
						totalImpulse = 0;
					}
					return consumed;
				}else{
					//nos movemos hacia atrás 
			LOG_DEBUG(log,"Dir from last antes de shift: " << getDirFromLast()); 
					shiftBackward();
			LOG_DEBUG(log,"Dir from last despues de shift: " << getDirFromLast()); 
					consumed= totalMass;
					if(consumed < totalImpulse){
						totalImpulse-=consumed;
					}else{
						totalImpulse = 0;
					}
					return consumed;
				}
			}else{
				// agregar aqui descarrilamientos!!!
			LOG_DEBUG(log,"Descarrila hacia atras");
				return 9999999;
			}
		}else{
			return 0;
		}
	}
}
