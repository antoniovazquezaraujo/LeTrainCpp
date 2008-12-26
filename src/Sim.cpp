#include "Basic.h"
#include "Sim.h"
#include "SimView.h"
#include "RailMap.h"
#include "RailPen.h"
#include "Finder.h"
#include "Train.h"
#include "Wagon.h"
#include "Sensor.h"
#include "Event.h"
#include "ForkRail.h"
#include "Message.h"
#include "Semaphore.h"
#include "EventProgram.h"
#include "Rail.h"
#include "RailVehicle.h"
#include "Locomotive.h"

MAKE_LOGGER(Sim);

Sim::Sim (Game * game)
	:game(game), railMap(new RailMap()), railPen(new RailPen()), finder(new Finder(0)){ 
	int screenRows = Window::getScreenRows();
	int screenCols = Window::getScreenCols();
	int commandCols = screenCols * 20/100;
	int commandRows = screenRows * 20/100;
	int mapCols = screenCols - commandCols;
	int mapRows = screenRows - commandRows;

	Window * window = new Window(0, 0, mapCols, mapRows);
	window->setTitle("Map");
	view = new SimView( this); 
}
void Sim::paint(){
	view->paint();
}
void Sim::erase(){
	view->erase();
}
void Sim::followVehicle(Vehicle * v){
	view->followVehicle(v);
}

Sim::~Sim(){
	delete railMap;
	delete railPen;
}
RailMap * Sim::getRailMap(){
	return railMap;
}
RailPen * Sim::getRailPen(){
	return railPen;
}
Finder * Sim::getFinder(){
	return finder;
}
void Sim::addEventProgram(EventProgram * p, int id){
	if(id == -1){
		id = eventPrograms.size();
	}
	eventPrograms[id] = p;
}
Sim::CSemaphores * Sim::getSemaphores(){
	return &semaphores;
}
/*
void Sim::removeEventProgram(int id){
	eventPrograms.erase(remove(eventPrograms.begin(), eventPrograms.end(), eventPrograms[id]), eventPrograms.end());
}
EventProgram * Sim::getEventProgram(int id){
	return eventPrograms[id];
}
void Sim::setEventProgram(int id, EventProgram * p){
	eventPrograms[id] = p;
}
*/
Sim::CPrograms * Sim::getEventPrograms(){
	return &eventPrograms;
}
void Sim::addSensor(Sensor * s, int id){
	if(id== -1){
		id= sensors.size();
	}
	s->setId(id);
	sensors[id] = s;
}
Sensor * Sim::getSensor(int id){
	return sensors[id];
}
Sim::CSensors * Sim::getSensors(){
	return & sensors;
}
void Sim::addWagon(Wagon * wagon, int id){
	if(id == -1){
		id = wagons.size();
	}
	LOG_DEBUG(log," Cantidad de vagones actual: " << id);
	wagons[id] = wagon;
}
void Sim::removeWagon(int n){
	wagons.erase(n);
}
void Sim::removeLocomotive(int n){
	locomotives.erase(n);
}
void Sim::addLocomotive(Locomotive* locomotive, int id){
	if(id == -1){
		id = locomotives.size()+1;
	}
	locomotive->setId(id);
	locomotives[id] = locomotive;
	LOG_DEBUG(log," Cantidad de locomotoras actual: " << id);
}
Sim::CLocomotives * Sim::getLocomotives(){
	return &locomotives;
}
Locomotive * Sim::getLocomotive(int n){
	map<int, Locomotive*>::iterator i = locomotives.find(n);
	if(i != locomotives.end()){
		return i->second;
	}
	return 0;
}
Wagon * Sim::getWagon(int id){
	return wagons[id];
}
Sim::CWagons * Sim::getWagons(){
	return &wagons;
}
void Sim::addFork(ForkRail * r, int id){
	if(id == -1){
		id = forks.size();
	}
	forks[id] = r;
}
Semaphore * Sim::getSemaphore(int id){
	return semaphores[id];
}
void Sim::addSemaphore(Semaphore * s, int id){
	if(id == -1){
		id = semaphores.size();
		s->setId(id);
	}
	semaphores[id] = s;
}
ForkRail * Sim::getFork(int id){
	return forks[id];
}
void Sim::moveTrains(){
	moveLocomotives();
	moveWagons();
}
void Sim::moveLocomotives(){
	Sim::CLocomotives::iterator it;
	for(it=locomotives.begin(); it!= locomotives.end(); it++){
		Locomotive* next = it->second;
		next->move();
	}
}
void Sim::moveWagons(){
	map<int, Wagon *>::iterator it;
	for(it=wagons.begin(); it!= wagons.end(); it++){
		Wagon * next = (*it).second;
		next->movePushed();
	}
}
void Sim::checkSensors(){
	for(Sim::CSensors::iterator it=sensors.begin(); it!= sensors.end(); it++){
		Sensor * next = (*it).second;
		Event *event = next->check();
		if(event){
			LOG_DEBUG(log," Evento creado checkeando en Sim: " << event->str());
			for(Sim::CPrograms::iterator it2=eventPrograms.begin(); it2!= eventPrograms.end(); it2++){
				EventProgram * eventProgram = (*it2).second;
				acceptAndRun(event, eventProgram);
			}
			delete event;
		}
	}

}
void Sim::acceptAndRun(Event * event, EventProgram * eventProgram){

	if(!eventProgram->accept(event)){
		LOG_DEBUG(log," el evento no se acepta en Sim");
		return;	
	}
	LOG_DEBUG(log," el evento SI se acepta en Sim");

	EventProgram::CMessages * messages = eventProgram->getMessages();
	for(EventProgram::CMessages::iterator i = messages->begin();i!= messages->end(); i++){
		Message * message = (*i);
		Locomotive * loco=0;
		ForkRail * forkRail=0;
		Semaphore * semaphore=0;
		Sensor * sensor=0;
		Rail * rail=0;
		RailVehicle * vehicle = 0;
		LOG_DEBUG(log," evento de tipo:" << message->getCommand());
		switch(message->getCommand()){
		case Message::FORK_DIR:
			forkRail = getFork(message->getTarget()); 
			forkRail->selectDir(Dir(message->getValue()));
			LOG_DEBUG(log," Cambiando fork:" << message->getTarget() << " a: " << message->getValue());
			break;
		case Message::SEMAPHORE:
			LOG_DEBUG(log," Cambiando semaforo:" << message->getTarget() << " a: " << message->getValue());
			semaphore = getSemaphore(message->getTarget());
			if(message->getValue() == Semaphore::OPEN){ 
				semaphore->open();
			}else{
				semaphore->close();
			}
			break;
		case Message::TRAIN_SPEED:
			LOG_DEBUG(log," Cambiando velocidad de:" << message->getTarget() << " a: " << message->getValue());
			loco = getLocomotive(message->getTarget());
			loco->setSpeed(message->getValue());
			break;
		case Message::TRAIN_AT_SENSOR_SPEED:
			sensor = getSensor(message->getTarget());
			rail = sensor->getRail();
			vehicle = rail->getRailVehicle();
			if(vehicle){
				Locomotive * locomotive = dynamic_cast<Locomotive *>(vehicle);
				if(locomotive){
					//Ojo, debe hacerse a todo el tren, no solo a esa loco!!!
					locomotive->setSpeed(message->getValue());
				}
			}
			break;
		case Message::EVENT_TRAIN_SPEED:
			loco = getLocomotive(event->getLocomotiveId());
			loco->setSpeed(message->getValue());
			break;

		}

	}
}
