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

Sim::Sim ()
	:
	finder(new Finder(0)){ 
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
	for(auto t : trains){
		delete t.second;
	}
	for(auto t : wagons){
		delete t.second;
	}
	for(auto t : locomotives){
		delete t.second;
	}
	for(auto t : sensors){
		delete t.second;
	}
	for(auto t : eventPrograms){
		delete t.second;
	}
	for(auto t : forks){
		delete t.second;
	}
}
Finder * Sim::getFinder(){
	return finder;
}
void Sim::addEventProgram(EventProgram * p, int id){
	if(id == -1){
		eventPrograms+=p;
	}
	eventPrograms[id] = p;
}
Sim::CSemaphores * Sim::getSemaphores(){
	return &semaphores;
}
void Sim::removeEventProgram(int id){
	eventPrograms.erase(id);
}
EventProgram * Sim::getEventProgram(int id){
	return eventPrograms.get(id);
}
Sim::CPrograms * Sim::getEventPrograms(){
	return &eventPrograms;
}
void Sim::addSensor(Sensor * s, int id){
	if(id== -1){
		sensors+=s;
	}
	s->setId(id);
	sensors[id] = s;
}
Sensor * Sim::getSensor(int id){
	return sensors.get(id);
}
Sim::CSensors * Sim::getSensors(){
	return & sensors;
}
void Sim::addTrain(Train* train, int id){
	if(id == -1){
		trains+=train;
	}
	trains[id] = train;
}
void Sim::addWagon(Wagon * wagon, int id){
	if(id == -1){
		wagons+=wagon;
	}
	wagons[id] = wagon;
}
void Sim::removeTrain(int n){
	trains.erase(n);
	trains.selectPrev();
}
void Sim::removeWagon(int n){
	wagons.erase(n);
}
void Sim::removeLocomotive(int n){
	locomotives.erase(n);
}
void Sim::addLocomotive(Locomotive* locomotive, int id){
	if(id == -1){
		locomotives+=locomotive;
	}
	locomotive->setId(id);
	locomotives[id] = locomotive;
}
Sim::CLocomotives * Sim::getLocomotives(){
	return &locomotives;
}
Locomotive * Sim::getLocomotive(int n){
	return locomotives.get(n); 
}
Train * Sim::getTrain(int id){
	return trains.get(id);
}
Wagon * Sim::getWagon(int id){
	return wagons.get(id);
}
Sim::CTrains* Sim::getTrains(){
	return &trains;
}
Sim::CWagons * Sim::getWagons(){
	return &wagons;
}
void Sim::addFork(ForkRail * r, int id){
	if(id == -1){
		forks+=r;
	}
	forks[id] = r;
}
Semaphore * Sim::getSemaphore(int id){
	return semaphores.get(id);
}
void Sim::addSemaphore(Semaphore * s, int id){
	if(id == -1){
		semaphores+=s;
		s->setId(id);
	}
	semaphores[id] = s;
}
ForkRail * Sim::getFork(int id){
	return forks.get(id);
}
void Sim::moveTrains(){
	for(auto t : trains){
		t.second->setMoved(false);
	};
	for(auto t : trains){
		t.second->move();
	}
}
Train * Sim::getSelectedTrain(){
	return trains.getSelected(); 
}
void Sim::selectPrevTrain(){
	trains.getSelected()->setSelected(false);
	trains.selectPrev();
	trains.getSelected()->setSelected(true);
}
void Sim::selectNextTrain(){
	trains.getSelected()->setSelected(false);
	trains.selectNext();
	trains.getSelected()->setSelected(true);
}
void Sim::selectNextVehicle(){
	trains.getSelected()->selectNextVehicle();
}
void Sim::selectPrevVehicle(){
	trains.getSelected()->selectPrevVehicle();
}

void Sim::checkSensors(){
	for(auto s : sensors){
		Sensor * next = s.second;
		Event *event = next->check();
		if(event){
			for(auto program: eventPrograms){
				acceptAndRun(event, program.second);
			}
		}
	}
}
void Sim::acceptAndRun(Event * event, EventProgram * eventProgram){

	if(!eventProgram->accept(event)){
		return;	
	}

	for(auto message : *eventProgram->getMessages()){
		Locomotive  * loco=0;
		ForkRail    * forkRail=0;
		Semaphore   * semaphore=0;
		Sensor      * sensor=0;
		Rail        * rail=0;
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
			//Ahora debería hacerse a todo el tren!!!!!!
			//loco->setSpeed(message->getValue());
			break;
		}
	}
}
