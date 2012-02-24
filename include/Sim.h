#ifndef SIM_H
#define SIM_H

#include "Basic.h"
#include "RailMap.h"
#include "RailPen.h"
template<typename T>
class IntMap : public map<int , T>{
public:
	IntMap()
		:selected(0){
	}
	void operator +=(T t){
		int i = this->size();
		insert({i,t});
	}
	void selectNext(){
		if(this->find(++selected) != this->end()){
			return;
		}
		selected=0;
	}
	void selectPrev(){
		if((--selected) >= 0){
			return;
		}
		selected=0;
	}
	void select(int i){
		auto f = this->find(i);
		assert(f != this->end());
		selected=i;
	}
	T get(int i){
		auto f = this->find(i);
		if(f != this->end()){
			return f->second;
		}else{
			return nullptr;
		}
	}
	T getSelected(){
		auto f = this->find(selected);
		if(f != this->end()){
			return f->second;
		}
		return nullptr;
	}
private:
	int selected;
};
class Sim{
public:
	typedef IntMap<Train        *> CTrains; 
	typedef IntMap<Locomotive   *> CLocomotives; 
	typedef IntMap<Wagon        *> CWagons; 
	typedef IntMap<EventProgram *> CPrograms; 
	typedef IntMap<Sensor       *> CSensors; 
	typedef IntMap<Semaphore    *> CSemaphores; 
	typedef IntMap<ForkRail     *> CForks; 
	Sim();
	~Sim();

	CTrains      * getTrains();
	CWagons      * getWagons();
	CLocomotives * getLocomotives();
	CSensors     * getSensors();
	CPrograms    * getEventPrograms();
	CSemaphores  * getSemaphores();
	CForks       * getForks();

	void addTrain         (Train*,        int key=-1);
	void addWagon         (Wagon*,        int key=-1);
	void addLocomotive    (Locomotive*,   int key=-1);
	void addSensor        (Sensor*,       int key=-1);
	void addEventProgram  (EventProgram*, int key=-1);
	void addFork          (ForkRail*,     int key=-1);
	void addSemaphore     (Semaphore*,    int key=-1);

	Train * getSelectedTrain();
	void selectPrevTrain();
	void selectNextTrain();
	void selectNextVehicle();
	void selectPrevVehicle();
	Train        * getTrain(        int key);
	Wagon        * getWagon(        int key);
	Locomotive   * getLocomotive(   int key);
	Sensor       * getSensor(       int key);
	EventProgram * getEventProgram( int key);
	ForkRail     * getFork(         int key);
	Semaphore    * getSemaphore(    int key);

	void removeTrain(        int key);
	void removeWagon(        int key);
	void removeLocomotive(   int key);
	void removeSensor(       int key);
	void removeEventProgram( int key);
	void removeFork(         int key);
	void removeSemaphore(    int key);

	void moveTrains();
	//void moveWagons();
	//void moveLocomotives();
	void checkSensors();

	void followVehicle(Vehicle * v);

	void acceptAndRun(Event * event, EventProgram * eventProgram);
	Finder * getFinder();
	void paint();
	void erase();
	RailMap railMap;
	RailPen railPen;
private:
	Finder  * finder;
	SimView * view;
	CTrains      trains;
	CWagons      wagons;
	CLocomotives locomotives;
	CSensors     sensors;
	CPrograms    eventPrograms;
	CForks       forks;
	CSemaphores semaphores;

	CTrains::iterator selectedTrain;
	static Logger log;
};
#endif
