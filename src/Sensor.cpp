#include "Basic.h"
#include "Sensor.h"
#include "Rail.h"
#include "Locomotive.h"
#include "Event.h"
#include "Window.h"
MAKE_LOGGER(Sensor);
Sensor::Sensor(int id)
	: id(id), armed(false){

	}
Event * Sensor::check(){
	Event * ret = 0;
	RailVehicle * v = rail->getRailVehicle();
	if(!v && !armed){
		armed=true;
	}
	if(armed & v!= 0){
		Locomotive * loco = dynamic_cast<Locomotive*>(v);
		if(loco){
			armed = false;
			time_t tiempo = time(&tiempo);
			struct tm* t = localtime(&tiempo);
			ret = new Event(id, t->tm_hour,t->tm_min,
					t->tm_sec,loco->getId(), loco->getSpeed(), 
					loco->getDir().getValue());
			LOG_DEBUG(log," Creado en Sensor evento: " << ret);
		}
	}
	return ret;
}

int Sensor::getId(){
	return id;
}
void Sensor::setId(int id){
	this->id = id;
}
bool Sensor::isArmed(){
	return armed;
}
void Sensor::setArmed(bool armed){
	this->armed = armed;
}
Rail * Sensor::getRail(){
	return rail;
}
void Sensor::setRail(Rail * rail){
	this->rail = rail;
}
Point Sensor::getPos(){
	return pos;
}
void Sensor::setRow(int row){
	pos.row = row;
}
void Sensor::setCol(int col){
	pos.col= col;
}
void Sensor::paint(Window * window){
	window->setBg(0);
	window->setFg(6);
	Point p = rail->getPos();
	//window->putC(p.row, p.col, '%');
	window->putC(p.row, p.col, ACS_NEQUAL);
}
void Sensor::erase(Window * window){
	window->setBg(0);
	window->setFg(6);
	Point p = rail->getPos();
	window->putC(p.row, p.col, ' ');
}
