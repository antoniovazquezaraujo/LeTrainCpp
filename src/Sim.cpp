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
	finder(new Finder(0)),
	trainSelector(&trains){ 
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
		delete t;
	}
	for(auto t : wagons){
		delete t;
	}
	for(auto t : locomotives){
		delete t;
	}
	for(auto t : sensors){
		delete t;
	}
	for(auto t : eventPrograms){
		delete t;
	}
	for(auto t : forks){
		delete t;
	}
}
Selector<Sim::TTrains> * Sim::getTrainSelector(){
	return &trainSelector;
}
Finder * Sim::getFinder(){
	return finder;
}
void Sim::addEventProgram(EventProgram * p){
	eventPrograms.push_back(p);
}
vector<Semaphore *> & Sim::getSemaphores(){
	return semaphores;
}
void Sim::removeEventProgram(int id){
	eventPrograms.erase(eventPrograms.begin()+id);
}
EventProgram * Sim::getEventProgram(int id){
	return eventPrograms.at(id);
}
vector<EventProgram *> & Sim::getEventPrograms(){
	return eventPrograms;
}
void Sim::addSensor(Sensor * s){
	sensors.push_back(s);
}
Sensor * Sim::getSensor(int id){
	return sensors.at(id);
}
vector<Sensor *>& Sim::getSensors(){
	return sensors;
}
void Sim::addTrain(Train* train){
	trains.push_back(train);
	//agregar aqui las loco a locomotives y los wagons a wagons???
}
void Sim::addWagon(Wagon * wagon){
	wagons.push_back(wagon);
}
void Sim::removeVehicle (RailVehicle * v){
	if(!wagons.empty()) wagons.erase(find(wagons.begin(), wagons.end(), v));
	if(!locomotives.empty()) locomotives.erase(find(locomotives.begin(), locomotives.end(), v));
	v->getRail()->setRailVehicle(nullptr);
	delete v;
}
void Sim::removeTrain(int n){
	trains.erase(trains.begin()+n);
}
void Sim::removeWagon(int n){
	wagons.erase(wagons.begin()+n);
}
void Sim::removeLocomotive(int n){
	locomotives.erase(locomotives.begin()+n);
}
void Sim::addLocomotive(Locomotive* locomotive){
	locomotives.push_back(locomotive);
}
vector<Locomotive *> & Sim::getLocomotives(){
	return locomotives;
}
Locomotive * Sim::getLocomotive(int n){
	return locomotives.at(n); 
}
Train * Sim::getTrain(int id){
	return trains.at(id);
}
Wagon * Sim::getWagon(int id){
	return wagons.at(id);
}
Sim::TTrains & Sim::getTrains(){
	return trains;
}
vector<Wagon *> & Sim::getWagons(){
	return wagons;
}
void Sim::addFork(ForkRail * r){
	forks.push_back(r);
}
Semaphore * Sim::getSemaphore(int id){
	return semaphores.at(id);
}
void Sim::addSemaphore(Semaphore * s){
	semaphores.push_back(s);
}
ForkRail * Sim::getFork(int id){
	return forks.at(id);
}
void Sim::moveTrains(){
	for(auto t : trains){
		t->setMoved(false);
	};
	for(auto t : trains){
		if(t->getVehicles().empty()){
			t->markAsDeleted();
		}
	}
	for(auto t : trains){
		if(t->isMarkedAsDeleted()){
			for(auto v:t->getVehicles()){
				removeVehicle(v);
			}
			t->getVehicles().clear();
		}
	}
	trains.erase(remove_if(trains.begin(), trains.end(),
			[](Train * t){return t->isMarkedAsDeleted();}),
			trains.end());

	for(auto t : trains){
		t->move();
	}
}
Sim::TTrains::iterator Sim::getSelectedTrain(){
	return trainSelector.getSelected(); 
}
void Sim::selectPrevTrain(){
	if(trainSelector.isSelected()){
		(*trainSelector.getSelected())->setSelected(false);
	}
	trainSelector.selectPrev();
	if(trainSelector.isSelected()){
		(*trainSelector.getSelected())->setSelected(true);
	}
}
void Sim::selectNextTrain(){
	if(trainSelector.isSelected()){
		(*trainSelector.getSelected())->setSelected(false);
	}
	trainSelector.selectNext();
	if(trainSelector.isSelected()){
		(*trainSelector.getSelected())->setSelected(true);
	}
}
void Sim::selectNextVehicle(){
	if(trainSelector.isSelected()){
		(*trainSelector.getSelected())->selectNextVehicle();
	}
}
void Sim::selectPrevVehicle(){
	if(trainSelector.isSelected()){
		(*trainSelector.getSelected())->selectPrevVehicle();
	}
}

void Sim::link(){
	if(trainSelector.isSelected()){
		Train * t = *(trainSelector.getSelected());
		t->link();
	}
}
Train * Sim::unlink(){
	if(trainSelector.isSelected()){
		Train * t = *(trainSelector.getSelected());
		return t->unlink();
	}
}
void Sim::linkTrain(){
}
void Sim::checkSensors(){
	for(auto s : sensors){
		Event *event = s->check();
		if(event){
			for(auto program: eventPrograms){
				acceptAndRun(event, program);
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
