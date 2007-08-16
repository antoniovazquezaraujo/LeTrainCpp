/*
  sim.cpp
  The real simulator
  -------------------------------------
*/

#include "sim.h"

static int running = 0;
static int visormoved = 0;

Sim::Sim (){
	railmap = 0;
	vehicle=0;
	cross = 0;
	rail = 0;
	locomotives = new List();
	i_locomotives = locomotives->iterator();
	vehicles = new List();
	i_crosses = 0;
	maprow = 0;
	mapcol=0;
	maprows = LINES;
	mapcols=  COLS - (COLS/4);
	statusrow = 0;
	statuscol= COLS -(COLS/4);
	statusrows = LINES;
	statuscols=  COLS/4;
	commander = new Commander();;
}
Sim::~Sim(){
	assert(locomotives != 0);
	delete locomotives;
	Iterator * i = vehicles->iterator();
	while(i->hasNext()){
		Vehicle*  v = (Vehicle* ) i->next();
		delete v;
	}
	delete i;
	delete vehicles;
	delete i_locomotives;
	assert(i_crosses!= 0);
	delete i_crosses;
	delete commander;
	delete railmap;
}
void Sim::setup(){
	i_locomotives = locomotives->iterator();
	i_crosses = railmap->crosses->iterator();
	commander->c_setsim(this);
	linkvehicles();
	nextCross();
	selectMapCol(0);
	selectMapRow(0);
	g_sset(statusrow, statuscol, statusrows, statuscols);
}
int Sim::save(){
	return 0;
}
void Sim::start(){
	running = 1;
	cycle();
}
void Sim::stop(){
	running = 0;
}
Vehicle*  Sim::vehiclebyid(int id){
	Vehicle*  next = 0;
	Vehicle*  ret = 0;
	Iterator * i = vehicles->iterator();
	while(i->hasNext()){
		next = (Vehicle* )i->next();
		if(next->id == id){
			ret = next;
			break;
		}
	}
	delete i;
	return ret;
}
void Sim::linkvehicles(){
	Vehicle*  next = 0;

	//provisional
	if(vehicles == 0)return;

	Iterator * i = vehicles->iterator();
	while(i->hasNext()){
		next = (Vehicle* )i->next();
		if(next->link0id > 0){
			next->links[0] = vehiclebyid(next->link0id);
		}else {
			next->links[0] = 0;
		}
		if(next->link1id > 0){
			next->links[1] = vehiclebyid(next->link1id);
		}else {
			next->links[1] = 0;
		}

	}
	delete i;

}
Vehicle*  Sim::selVehicle(int vehicleIndex){
	vehicle = (Vehicle* )vehicles->get(vehicleIndex);
	return vehicle;
}
Vehicle*  Sim::getVehicle(){
	return vehicle;
}
void Sim::incpower(){
	if(vehicle) 
		vehicle->power++;
}
void Sim::decpower(){
	if(vehicle) {
		if(vehicle->power >0){
			vehicle->power--;
		}
	}
}
int Sim::incBrakes(){
	int brakes = vehicle->getBrakes();
	brakes++;
	vehicle->setBrakes(brakes);
	vehicle->braketrain();
	return brakes;
}
int Sim::decBrakes(){
	int brakes = vehicle->getBrakes();
	if(brakes > 0) brakes--;
	vehicle->setBrakes(brakes);
	vehicle->braketrain();
	return brakes;
}
int Sim::reverse(){
	if(vehicle) vehicle->reverse();
	return 0;
}
void Sim::del(){
	vehicles->del(vehicle);
	//Seleccionar el siguiente aqui
	return ;
} 
void Sim::addVehicle(Vehicle*  v){
	vehicles->add(v);
	if(v->type == LOCOMOTIVE){
		locomotives->add(v);
	}
	v->rail = railmap->rails[v->pos.row][v->pos.col];
	v->rail->vehicle = v;
} 
Vehicle* Sim::addloco(){
	Vehicle*  v = new Vehicle(LOCOMOTIVE);
	locomotives->add(v);
	vehicles->add(v);
	return v;
}
Vehicle*  Sim::addwagon(){
	Vehicle*  v = new Vehicle(WAGON);
	vehicles->add(v);
	return v;
}
Vehicle*  Sim::nextloco(){
	if(!i_locomotives->hasNext()){
		i_locomotives->reset();
	}
	if(i_locomotives->hasNext()){
		vehicle = (Vehicle* ) i_locomotives->next();
	}
	return vehicle;
}
Vehicle*  Sim::prevloco(){
	if(!i_locomotives->hasPrev()){
		i_locomotives->toEnd();
	}
	if(i_locomotives->hasPrev()){
		vehicle = (Vehicle* ) i_locomotives->prev();
	}
	return vehicle;
}
Rail * Sim::getRail(){
	return 0;
}
void Sim::updatevisor(){
	if(visormoved){
		g_vset(maprow, mapcol, maprows, mapcols);
		visormoved = 0;
	}
}

void Sim::selectMapRow(int row){
	maprow = row;
	visormoved=1;	
}
int Sim::getMapRow(){
	return maprow;
}
void Sim::selectMapCol(int col){
	mapcol = col;
	visormoved=1;	
}
int Sim::getMapCol(){
	return mapcol;
}
Cross * Sim::nextCross(){
	if(!i_crosses->hasNext()){
		i_crosses->reset();
	}
	if(i_crosses->hasNext()){
		cross= (Cross *) i_crosses->next();
	}
	return cross;
}
Cross * Sim::prevCross(){
	if(!i_crosses->hasPrev()){
		i_crosses->toEnd();
	}
	if(i_crosses->hasPrev()){
		cross= (Cross *) i_crosses->prev();
	}
	return cross;
}
Cross * Sim::getCross(){
	return cross;
}
void Sim::changeSelectedCross(){
	cross->nextDir();
} 
void Sim::movetrains(){
	Vehicle*  next = 0;
	Iterator * i;
	float remainingImp=0.0;
	i = vehicles->iterator();
	while(i->hasNext()){
		next = (Vehicle* )i->next();
		next->moved = 0;
	}
	delete i;
	i = locomotives->iterator();
	while(i->hasNext()){
		next = (Vehicle* )i->next();
		next->incImp(next->power);
	}
	delete i;

	i = locomotives->iterator();
	while(i->hasNext()){
		next = (Vehicle* )i->next();
		if(next->imp > 0){
			next->speed = next->imp/next->mass;
			if(next->turns > 0){
				next->turns--;
			}else{
				remainingImp = next->imp;
				if(remainingImp > 0 ){
					remainingImp = next->distlinked(remainingImp);
				}
				if(remainingImp > 0 ){
					remainingImp = next->distpushed(remainingImp);
				}
				if(remainingImp >= 0 ){
					next->imp = remainingImp;
				}
				if(next->imp > 0 ){
					if(next->pushedcanmove() && next->linkedcanmove()){
						next->movepushed();
						next->movelinked();
					}
				}
				if((int)next->speed >0){
					next->turns = (int)(next->maxTurns * (1.0/next->speed));
				}else{
					next->turns = next->maxTurns;
				}
			}
		}
	}
	delete i;
	i = vehicles->iterator();
	while(i->hasNext()){
		next = (Vehicle* )i->next();
		if(next->type == LOCOMOTIVE)
			continue;
		next->incImp(next->power);
		if(next->moved == 0){
			if(next->imp > 0){
				next->speed = next->imp/next->mass;
				if(next->turns > 0){
					next->turns--;
				}else{
					remainingImp = next->imp;
					if(remainingImp > 0 ){
						remainingImp = next->distlinked(remainingImp);
					}
					if(remainingImp > 0 ){
						remainingImp = next->distpushed(remainingImp);
					}
					if(remainingImp >= 0 ){
						next->imp = remainingImp;
					}	
					if(next->imp > 0 ){
						if(next->pushedcanmove() && next->linkedcanmove()){
							next->movepushed();
							next->movelinked();
						}
					}
					if((int)next->speed >0){
						next->turns = (int)(next->maxTurns * (1.0/next->speed));
					}else{
						next->turns = next->maxTurns;
					}
					next->moved = 1;
				}
			}
		}
	}
	delete i;
}
void Sim::cycle(){
	while(running){
		doactions();
		movetrains();
		draw();
	}
}
void Sim::doactions(){
	int key = g_getkey();
	if(key!= -1){
		commander->c_key(key);
	}
}
void Sim::draw(){
	clearrails();
	updatevisor();
	drawrails();
	drawcrosses();
	drawVehicles();
	drawfinder();
	drawstatus();
	drawhelp();
	g_repaint();
}
void Sim::drawhelp(){
	g_drawhelp(commander->mode);

}
void Sim::drawstatus(){
	Vehicle*  v;
	int link0id, link1id;
	v = vehicle;
	
	if(v != 0){
		link0id= v->links[0]?v->links[0]->id:0;
		link1id= v->links[1]?v->links[1]->id:0;
		g_drawVehicleStatus(v->id, v->pos.row, v->pos.col, v->dir, v->imp, v->power, v->brakes, link0id, link1id, v->aspect);
	}
}
void Sim::drawfinder(){
	if(commander->finder == 0){
		return;
	}
	if(commander->finder->rail == 0){
		return;
	}
	Rail * r = commander->finder->rail;
	if(r->vehicle != 0){
		g_drawfinder(
			r->pos.row, 
			r->pos.col, 
			r->vehicle->aspect,
			r->vehicle->fg,
			r->vehicle->bg);
	}else{
		g_drawfinder(
			r->pos.row, 
			r->pos.col, 
			r->aspect,
			DEF_FINDER_BG,
			DEF_FINDER_FG);
	}
}
void Sim::drawVehicles(){
	int selected;
	Iterator * i = vehicles->iterator();
	while(i->hasNext()){
		Vehicle*  next = (Vehicle* )i->next();
		if(next == getVehicle()){
			selected = 1;
		} else{
			selected = 0;
		}
		g_drawVehicle(
			next->pos.row, 
			next->pos.col, 
			next->aspect,
			next->bg,
			next->fg,
			selected);
	}
	assert(i != 0);
	delete i;
}
void Sim::drawrails(){
	Rail * r = 0;
	int n=0;
	int m=0;
	for(n = 0; n < RAIL_MAP_ROWS; n++){
		for(m = 0; m < RAIL_MAP_COLS; m++){
			r = railmap->rails[n][m];
			if(r != 0){
				g_drawRail(
					r->pos.row, 
					r->pos.col, 
					r->aspect,
					DEF_RAIL_FG,
					DEF_RAIL_BG);	
			}
		}
	}
}
void Sim::drawcrosses(){
	int selected;
	Rail * r = 0;
	Cross * next = 0;
	Iterator * i = railmap->crosses->iterator();
	while(i->hasNext()){
		next = (Cross *)i->next();
		r = next->rail;
		if(next == cross){
			selected = 1;
		} else{
			selected = 0;
		}
		g_drawCross(
			next->rail->pos.row, 
			next->rail->pos.col, 
			next->rail->aspect,
			DEF_CROSS_FG,
			DEF_CROSS_BG,
			selected);
	}
	delete i;
}
		
void Sim::clearrails(){
	Rail * r = 0;
	int n=0;
	int m=0;
	for(n = 0; n<RAIL_MAP_ROWS; n++){
		for(m = 0; m<RAIL_MAP_COLS; m++){
			r = railmap->rails[n][m];
			if(r != 0){
				g_clearRail(
					r->pos.row, 
					r->pos.col, 
					r->aspect, 
					DEF_RAIL_FG,
					DEF_RAIL_BG);
			}
		}
	}
}
