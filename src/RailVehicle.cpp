#include "Basic.h"
#include "RailVehicle.h"
#include "Rail.h"
#include "Finder.h"
#include "TrainIterator.h"
MAKE_LOGGER(RailVehicle);

ostream & operator << (ostream & o, RailVehicle & v){
	o << "RailVehicle{" << endl ;
	Vehicle * v1 = v.linker.getVehicleLinkedAt(0);
	if(v1) o << "Link 1: " << *v1 ;
	Vehicle * v2 = v.linker.getVehicleLinkedAt(1);
	if(v2) o << "Link 2: " << *v2 ;
	o << endl << "Dir: " << v.dir.getName() << endl;
	o <<endl << "}"<<endl ;
	return o;
}

RailVehicle::RailVehicle(Aspect * aspect)
	:Vehicle(aspect),
	imp (1000),
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
// TODO
}
void RailVehicle::setUnlink(Dir d){
// TODO

}
bool RailVehicle::gotoRail(Rail * r){
	if(r->getRailVehicle()!= 0){
		return false;
	}
	// si ya tengo un rail lo vacio
	if(rail != 0){
		rail->setRailVehicle(0);
	}
	if(dir.getValue() != NO_DIR){
		dir = r->getPath(-dir);
	}
	pos = r->getPos();
	rail = r;
	r->setRailVehicle(this);
	return true;
}
bool RailVehicle::pushedCanMove(){
	bool ret=true;
	Finder* f= new Finder(this);
	RailVehicle * next = 0;
	f->setDir(this->dir);
	f->forward();
	next = f->getRail()->getRailVehicle();
	while(next != 0){
		next->dir = f->getDir();
		if(f->forward()){
			next = f->getRail()->getRailVehicle();
		}else{
			next = 0;
		}
	}
	f->reverse();
	f->forward();
	next = f->getRail()->getRailVehicle();
	while(next != 0 && next != this){ //move all yast to BEFORE v !
		if(! next->canMove()){
			ret = false;
			break;
		}
		f->forward();
		next = f->getRail()->getRailVehicle();
	}
	delete f;
	return ret;
}
bool RailVehicle::canMove(){
	if(imp >= (mass+brakes)){
		Rail * r = rail->getLinkedRailAt(dir);
		if( r == 0){
			return false;
		}
	}else{
		return false;
	}
	return true;
}
bool RailVehicle::linkedCanMove(){
	// probamos de momento a no hacer esto
	//return true;
	Dir antidir = rail->getPath(dir);
	bool ret = true;
	RailVehicle * next = 0;
	TrainIterator * t = new TrainIterator(this, antidir);
	while(t->hasNext()){
		next = t->next();
		if(! next->canMove()){
			ret = false;
			break;
		}else{

		}
	}
	delete t;
	return ret;
}
bool RailVehicle::forward(int distance){
	int ret = 0;
	Rail * next = rail->getLinkedRailAt(dir);
	if( next != 0){
		if (gotoRail(next)){
			ret = 1;
		}else{
			ret = 0;
		}
	}else{
		ret = 0;
	}
	return ret;
}
bool RailVehicle::backwards(int distance){
	dir = rail->getPath(dir);
	return true; // ??
}
int RailVehicle::linkedMass(Dir dir){
	RailVehicle * next = 0;
	int totalmass = 0;
	TrainIterator * t = new TrainIterator(this, dir);
	while(t->hasNext()){
		next = t->next();
		totalmass += next->getMass()+ next->getBrakes();
	}
	delete t;
	return totalmass;
}
int RailVehicle::pushedMass(){
	RailVehicle * next = 0;
	int totalmass = 0;
	Finder* f= new Finder(this);
	f->forward();
	next = f->getRail()->getRailVehicle();
	while(next != 0){
		totalmass += next->getMass()+ next->getBrakes();
		if( f->forward()){
			next = f->getRail()->getRailVehicle();
		}else{
			next = 0;
		}
	}
	delete f;
	return totalmass;
}
void RailVehicle::setSpeed(int speed){
	this->speed = speed;
}
int RailVehicle::getSpeed(){
	return speed;
}
void RailVehicle::brakeTrain(){
	RailVehicle * next = 0;
	Finder* f= new Finder(this);
	TrainIterator * t = new TrainIterator(this, this->dir);
	while(t->hasNext()){
		next = t->next();
		next->brakes = brakes; 
		f->forward();
	}
	delete t;
	delete f;
}
float RailVehicle::distLinked(float imp){
	if(!rail) return imp;
	int cons = 0;
	RailVehicle * next = 0;
	if(dir.getValue() == NO_DIR){
		dir = rail->getAnyPath();
	}
	Dir antidir = rail->getPath(dir);
	TrainIterator * t = new TrainIterator(this, antidir);
	while(t->hasNext()){
		next = t->next();
		cons = next->getMass() + next->getBrakes();
		imp -= cons;
		if(imp < 0){
			next->incImp(imp * -1);
			imp = 0;
			break;
		}else{
			next->incImp(cons);
		}
	}
	delete t;
	return imp;
}
bool RailVehicle::moveLinked(){
	LOG_DEBUG(log, " movelinked");
	Dir antidir = rail->getPath(rail->getPath(-dir));
	orientLinked(antidir);
	bool ret = true;
	RailVehicle * next = 0;
	TrainIterator * t = new TrainIterator(this, antidir);
	LOG_DEBUG(log, " antes de iterar");
	while(t->hasNext()){
		LOG_DEBUG(log, " iterando");
		next = t->next();
		LOG_DEBUG(log, " tengo el next" << *next);
		if(! next->forward()){
			LOG_DEBUG(log, " next no puede avanzar");
			ret = false;
			break;
		}else{
			LOG_DEBUG(log, " next puede avanzar");
			next->setMoved(true);
		}
	}
	LOG_DEBUG(log, " fin iteracion");
	delete t;
	return ret;
}
void RailVehicle::orientLinked(Dir dir){
	// esto no funciona, de momento queda en 40ena 
	return;

	LOG_DEBUG(log, " orientLinked");
	RailVehicle * next = 0;
	Finder* f= new Finder(this);
	f->reverse();
	LOG_DEBUG(log, " atras en orient linked");
	TrainIterator * t = new TrainIterator(this, dir);
	while(t->hasNext()){
		LOG_DEBUG(log, " en bucle de orientlinked");
		next = t->next();
		f->reverse();
		next->setDir(f->getDir());
		f->reverse();
		f->forward();
	}
	delete t;
	delete f;
}
float RailVehicle::distPushed(float imp){
	int cons =0;
	Finder* f= new Finder(this);
	RailVehicle * next = 0;
	if(f->forward()){
		next = f->getRail()->getRailVehicle();
		while(next != 0){
			cons = next->getMass() + next->getBrakes();
			imp -= cons;
			if(imp < 0){
				next->incImp(imp * -1);
				imp = 0;
				break;
			}else{
				next->incImp(cons);
			}
			// no se debería llamar aqui
			// a algo como orientPushed()???
			next->setDir(f->getDir());
			/////////////////////////

			f->forward();
			next = f->getRail()->getRailVehicle();
		}
	}
	delete f;
	return imp;
}

bool RailVehicle::movePushed(){
	bool ret=true;
	Finder* f= new Finder(this);
	RailVehicle * next = 0;
	LOG_DEBUG(log, " pongo dir a finder" << this->dir.getValue());
	f->setDir(this->dir);
	LOG_DEBUG(log, " Avanzo finder");
	f->forward();
	LOG_DEBUG(log, " tomo el vehiculo del finder");
	next = f->getRail()->getRailVehicle();
	if(next){
		while(next != 0){
			LOG_DEBUG(log, " tomo la dir de next" << f->getDir().getValue());
			next->dir = f->getDir();
			if(f->forward()){
				LOG_DEBUG(log, " Consigue avanzar");
				next = f->getRail()->getRailVehicle();
			}else{
				LOG_DEBUG(log, " no consigue avanzar");
				next = 0;
			}
		}
		LOG_DEBUG(log, " voy atras con el finder");
		f->reverse();
		LOG_DEBUG(log, " voy adelante con el finder");
		f->forward();
	}
	LOG_DEBUG(log, " tomo el vehiculo del finder otra vez");
	LOG_DEBUG(log, " nuevo next" << *f );
	LOG_DEBUG(log, " Nuevo rail de next" << *(f->getRail()));
	next = f->getRail()->getRailVehicle();
	if(next){
		while((next != 0) && (next != this)){ //move all but this !
			LOG_DEBUG(log, " tengo el siguiete vehiculo");
			if( ! next->forward()){
				LOG_DEBUG(log, " si avanza ");
				ret = false;
				break;
			}else{
				LOG_DEBUG(log, " no avanza");
				next->setMoved(true);
			}
			LOG_DEBUG(log, " avanzo de nuevo finder");
			f->forward();
			LOG_DEBUG(log, " tomo el siguiente vehiculo del finder al final");
			next = f->getRail()->getRailVehicle();
		}
	}
	LOG_DEBUG(log, " fin de movepushed");
	delete f;
	return ret;
}
float RailVehicle::getImp(){
	return imp;
}
void RailVehicle::incImp(float imp){
	if(imp >=0){
		this->imp +=imp;
	}
}
void RailVehicle::setImp(float imp){
	this->imp = imp;
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
