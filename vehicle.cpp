/*
  vehicle.c
  A wagon or locomotive. 
  -------------------------------------
*/
#include "letrain.h"
Vehicle::Vehicle(int type){
	this->id = newid();
	this->rail = NULL;
	this->type= type;
	this->pos.row=0;
	this->pos.col=0;
	switch(type){
	case LOCOMOTIVE:	
		this->aspect = 'L';
		this->fg = DEF_LOCOMOTIVE_FG;
		this->bg = DEF_LOCOMOTIVE_BG;
		break;
	case WAGON:	
		this->aspect =  'a'+ (char)(26.0 * rand()/(RAND_MAX)); 
		//this->aspect =  'u'; 
		//this->fg =  (int)(8.0 * rand()/(RAND_MAX)); 
		this->fg = DEF_WAGON_FG; 
		this->bg = DEF_WAGON_BG;
		break;
	}

	this->dir = NO_DIR;
	this->imp = 0;
	this->power = 0;
	this->brakes = 0;
	this->mass= 10; 
	this->links[0] = NULL;	
	this->links[1] = NULL;	
	this->moved = 0;
	this->speed = 0;
	this->friction= 0.5;
	this->turns = 0;
	this->maxTurns = 20;
}
int Vehicle::newid(){
	static int serialnumber;
	return ++serialnumber;
}
Vehicle::~Vehicle(){

}
Point * Vehicle::getPos(){
	return & pos;
}
void Vehicle::setPos(Point * pos){
	this->pos.row = pos->row;
	this->pos.col = pos->col;
}
void Vehicle::setDir(int dir){
	int oldDir = this->dir;
	this->dir = dir;
	if((speed > 10) && (oldDir == invert(dir))){
		g_alert("choque de trenes!!!!!!!");
	}
}
int Vehicle::getDir(){
	return dir;
}
float Vehicle::getImp(){
	return imp;
}
void Vehicle::incImp(float imp){
	if(imp >=0){
		this->imp +=imp;
	}
}
void Vehicle::setImp(float imp){
	if(imp >=0){
		this->imp = imp;
		/*
		if (this->imp > (mass + brakes) ) {
			this->imp = (mass + brakes);
		}
		*/
	}
}
int Vehicle::getBrakes(){
	return brakes;
}
void Vehicle::setBrakes(int brakes){
	assert(brakes >= 0 );
	this->brakes = brakes;
}
int Vehicle::getMass(){
	return mass;
}

void Vehicle::link(Vehicle * b){
	unlink(b);
	if(links[0] == NULL){
		links[0] = b;
	}else if(links[1] == NULL){
		links[1] = b;
	}else{
		assert(0);
	}
	if(b->links[0] == NULL){
		b->links[0] = this;
	}else if(b->links[1] == NULL){
		b->links[1] = this;
	}else{
		assert(0);
	}
}
void Vehicle::unlink(Vehicle * b){
	if(links[0] == b){
		links[0] = NULL;
	}else if(links[1] == b){
		links[1] = NULL;
	}
	if(b->links[0] == this){
		b->links[0] = NULL;
	}else if(b->links[1] == this){
		b->links[1] = NULL;
	}
}
int Vehicle::linked(Vehicle *b){
	assert(this != NULL);
	assert(b != NULL);
	int atob= 0;
	int btoa= 0;
	if(links[0] == b){
		atob=1;	
	}else if(links[1] == b){
		atob=1;
	}
	if(b->links[0] == this){
		btoa=1;	
	}else if(b->links[1] == this){
		btoa=1;
	}
	return (atob && btoa);
}
int Vehicle::gotoRail(Rail * r){
	int inputDir = NO_DIR;
	if(r->vehicle != NULL){
		return 0;
	}
	if(rail != NULL){
		rail->vehicle = NULL;
	}
	if(dir != NO_DIR){
		inputDir = invert(dir);
	}else{
		inputDir = r->validDir();
	}
	dir = r->getDir(inputDir);
	this->pos.row = r->pos.row;
	this->pos.col = r->pos.col;
	this->rail = r;
	r->vehicle = this;
	return 1;
}
int Vehicle::pushedcanmove(){
	int ret=1;
	Finder* f= new Finder(this);
	Vehicle * next = NULL;
	f->dir = this->dir;
	f->forward();
	next = f->rail->vehicle;
	while(next != NULL){
		next->dir = f->dir;
		if(f->forward()){
			next = f->rail->vehicle;
		}else{
			next = NULL;
		}
	}
	f->reverse();
	f->forward();
	next = f->rail->vehicle;
	while(next != NULL && next != this){ //move all yast to BEFORE v !
		if(! next->canmove()){
			ret = 0;
			break;
		}
		f->forward();
		next = f->rail->vehicle;
	}
	delete f;
	return ret;
}
int Vehicle::canmove(){
	if(imp >= (mass+brakes)){
		Rail * r = rail->link(dir);
		if( r == NULL){
			return 0;
		}
	}else{
		return 0;
	}
	return 1;
}
int Vehicle::linkedcanmove(){
	int antidir = rail->getDir(dir);
	int ret = 1;
	Vehicle * next = NULL;
	TrainIterator * t = new TrainIterator(this, antidir);
	while(t->hasNext()){
		next = t->next();
		if(! next->canmove()){
			ret = 0;
			break;
		}else{

		}
	}
	delete t;
	return ret;
}
int Vehicle::forward(){
	assert (this != 0);
	int ret = 0;
	Rail * next = rail->link(dir);
	if( next != NULL){
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
void Vehicle::reverse(){
	dir = rail->getDir(dir);
}
int Vehicle::linkedMass(int dir){
	Vehicle * next = NULL;
	int totalmass = 0;
	TrainIterator * t = new TrainIterator(this, dir);
	while(t->hasNext()){
		next = t->next();
		totalmass += next->getMass()+ next->getBrakes();
	}
	delete t;
	return totalmass;
}
int Vehicle::pushedMass(){
	Vehicle * next = NULL;
	int totalmass = 0;
	Finder* f= new Finder(this);
	f->forward();
	next = f->rail->vehicle;
	while(next != NULL){
		totalmass += next->getMass()+ next->getBrakes();
		if( f->forward()){
			next = f->rail->vehicle;
		}else{
			next = NULL;
		}
	}
	delete f;
	return totalmass;
}
void Vehicle::braketrain(){
	Vehicle * next = NULL;
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
float Vehicle::distlinked(float imp){
	int cons = 0;
	Vehicle * next = NULL;
	int antidir = rail->getDir(dir);
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
int Vehicle::movelinked(){
	int antidir = rail->getDir(dir);
	orientlinked(antidir);
	int ret = 1;
	Vehicle * next = NULL;
	TrainIterator * t = new TrainIterator(this, antidir);
	while(t->hasNext()){
		next = t->next();
		if(! next->forward()){
			ret = 0;
			break;
		}else{
			next->moved = 1;
		}
	}
	delete t;
	return ret;
}
void Vehicle::orientlinked(int dir){
	Vehicle * next = NULL;
	Finder* f= new Finder(this);
	f->reverse();
	TrainIterator * t = new TrainIterator(this, dir);
	while(t->hasNext()){
		next = t->next();
		f->reverse();
		next->setDir(f->dir);
		f->reverse();
		f->forward();
	}
	delete t;
	delete f;
}
float Vehicle::distpushed(float imp){
	int cons =0;
	Finder* f= new Finder(this);
	Vehicle * next = NULL;
	if(f->forward()){
		next = f->rail->vehicle;
		while(next != NULL){
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
			next->setDir(f->dir);
			/////////////////////////

			f->forward();
			next = f->rail->vehicle;
		}
	}
	delete f;
	return imp;
}

int Vehicle::movepushed(){
	int ret=1;
	Finder* f= new Finder(this);
	Vehicle * next = NULL;
	f->dir = this->dir;
	f->forward();
	next = f->rail->vehicle;
	while(next != NULL){
		next->dir = f->dir;
		if(f->forward()){
			next = f->rail->vehicle;
		}else{
			next = NULL;
		}
	}
	f->reverse();
	f->forward();
	next = f->rail->vehicle;
	while((next != 0) && (next != this)){ //move all but this !
		if( ! next->forward()){
			ret = 0;
			break;
		}else{
			next->moved = 1;
		}
		f->forward();
		next = f->rail->vehicle;
	}
	delete f;
	return ret;
}

